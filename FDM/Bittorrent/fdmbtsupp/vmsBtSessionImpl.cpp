/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsBtSessionImpl.h"
#include "vmsBtFileImpl.h"
#include <libtorrent/alert_types.hpp>
#include <libtorrent/extensions/ut_pex.hpp>

__declspec(dllexport) vmsBtSession* WINAPI vmsBt_getSession ()
{
	return vmsBtSessionImpl::Instance ();
}

__declspec(dllexport) void WINAPI vmsBt_Shutdown ()
{
	delete vmsBtSessionImpl::Instance ();
}  

vmsBtSessionImpl::vmsBtSessionImpl(void)
{
	m_bDHTstarted = FALSE;
	m_session.set_severity_level (alert::info);
	m_session.add_extension (libtorrent::create_ut_pex_plugin);
	m_session.set_max_half_open_connections (7);
	m_bThreadRunning = true;
	m_bNeedStop = false;
	m_pfnEvHandler = NULL;
	InitializeCriticalSection (&m_csRestoreTorrentHandle);
	InitializeCriticalSection (&m_csDeleteDownload);
	DWORD dw;
	CloseHandle (
		CreateThread (NULL, 0, _threadSession, this, 0, &dw));
}

vmsBtSessionImpl::~vmsBtSessionImpl(void)
{
	m_bNeedStop = true;
	while (m_bThreadRunning)
		Sleep (10);
	DeleteCriticalSection (&m_csRestoreTorrentHandle);
	DeleteCriticalSection (&m_csDeleteDownload);
}

time_t FILETIMEToUnixTime (FILETIME filetime)
{
	long long int t = filetime.dwHighDateTime;
    t <<= 32;
    t += (unsigned long)filetime.dwLowDateTime;
    t -= 116444736000000000LL;

	return t / 10000000;
}

vmsBtDownload* vmsBtSessionImpl::CreateDownload (vmsBtFile *torrent, LPCSTR pszOutputPath, LPBYTE pbFastResumeData, DWORD dwFRDataSize, vmsBtStorageMode enStorageMode)
{
	char szPath [10000];
	strcpy (szPath, pszOutputPath);
	LPSTR psz = szPath;
	while (*psz)
	{
		if (*psz == '\\')
			*psz = '/';
		psz++;
	}
	if (strlen (szPath) == 2 && szPath [1] == ':')
		strcat (szPath, "/");

	try 
	{
		torrent_handle th;
		vmsBtFileImpl *torrentimpl = (vmsBtFileImpl*) torrent;

		entry entryFastResume = pbFastResumeData ? bdecode (pbFastResumeData, pbFastResumeData + dwFRDataSize) : entry ();
		
		

		storage_mode_t sm;
		if (enStorageMode == BTSM_SPARSE)
			sm = storage_mode_sparse;
		else if (enStorageMode == BTSM_ALLOCATE)
			sm = storage_mode_allocate;
		else
			sm = storage_mode_compact;

		th = m_session.add_torrent (*torrentimpl->m_torrent, szPath, entryFastResume, sm);
		th.set_ratio (1);

		vmsBtDownloadImpl *pDld = new vmsBtDownloadImpl;
		pDld->AddRef ();
		pDld->m_handle = th;
		pDld->m_pTorrent = torrentimpl; torrentimpl->AddRef ();
		pDld->m_strOutputPath = szPath;
		
		m_vDownloads.push_back (pDld);
		return pDld;
	}
	catch (std::exception&) 
	{
		return NULL;
	}
}

void vmsBtSessionImpl::DeleteDownload (vmsBtDownload* pDld)
{
	EnterCriticalSection (&m_csDeleteDownload);

	vmsBtDownloadImpl *p = dynamic_cast <vmsBtDownloadImpl*> (pDld);
	if (p)
	{
		int nIndex = FindDownloadIndex (p->m_handle);
		if (nIndex != -1)
			m_vDownloads.erase (m_vDownloads.begin () + nIndex);
		try {
			m_session.remove_torrent (p->m_handle);
		}catch (...) {}
		p->Release ();
	}

	LeaveCriticalSection (&m_csDeleteDownload);
}

vmsBtSessionImpl* vmsBtSessionImpl::Instance ()
{
	static vmsBtSessionImpl* _p = NULL;
	if (_p == NULL)
	{
		try {
			_p = new vmsBtSessionImpl;
		}
		catch (...) {
			_p = NULL;
		}
	}
	return _p;
}

void vmsBtSessionImpl::SetDownloadLimit (int limit)
{
	m_session.set_download_rate_limit (limit);
}

void vmsBtSessionImpl::SetUploadLimit (int limit)
{
	m_session.set_upload_rate_limit (limit);
}

void vmsBtSessionImpl::ListenOn (int portFrom, int portTo)
{
	m_session.listen_on (std::make_pair (portFrom, portTo));
}

BOOL vmsBtSessionImpl::IsListening ()
{
	return m_session.is_listening ();
}

USHORT vmsBtSessionImpl::get_ListenPort ()
{
	return m_session.listen_port ();
}

void vmsBtSessionImpl::SetMaxUploads (int limit)
{
	m_session.set_max_uploads (limit);
}

void vmsBtSessionImpl::DHT_start (LPBYTE pbState, DWORD dwStateSize)
{
	if (m_bDHTstarted)
		return;
	m_bDHTstarted = TRUE;
	try {
		m_session.start_dht (pbState ? bdecode (pbState, pbState + dwStateSize) : entry ());
	}catch (...){}
}

void vmsBtSessionImpl::DHT_stop ()
{
	if (m_bDHTstarted == FALSE)
		return;
	m_session.stop_dht ();
	m_bDHTstarted = FALSE;
}

BOOL vmsBtSessionImpl::DHT_getState (LPBYTE pbBuffer, DWORD dwBufferSize, LPDWORD pdwDataSize)
{
	std::vector <char> v;
	bencode (std::back_inserter (v), m_session.dht_state ());

	*pdwDataSize = v.size ();

	if (pbBuffer == NULL)
		return TRUE;

	if (dwBufferSize < v.size ())	
		return FALSE;
	
	for (size_t i = 0; i < v.size (); i++)
		*pbBuffer++ = v [i];

	return TRUE;
}

BOOL vmsBtSessionImpl::DHT_isStarted ()
{
	return m_bDHTstarted;
}

DWORD vmsBtSessionImpl::_threadSession (LPVOID lp)
{
	vmsBtSessionImpl* pthis = (vmsBtSessionImpl*)lp;

	while (pthis->m_bNeedStop == false)
	{
		Sleep (100);

		
		std::auto_ptr <alert> alert = pthis->m_session.pop_alert ();
		if (alert.get () == NULL)
			continue;	

		if (pthis->m_pfnEvHandler == NULL)
			continue; 

		vmsBtSessionEvent ev;
		ev.pszMsg = alert->msg ().c_str ();
		
		
		torrent_handle hDownload;
		std::string strIp; 

		listen_failed_alert *lfa = dynamic_cast <listen_failed_alert*> (alert.get ());
		if (lfa != NULL)
		{
			ev.enType = BTSET_LISTEN_FAILED;
			goto _lRaiseEvent;
		}

		file_error_alert *fea = dynamic_cast <file_error_alert*> (alert.get ());
		if (fea != NULL)
		{
			ev.enType = BTSET_FILE_ERROR;
			hDownload = fea->handle;
			goto _lRaiseEvent;
		}

		tracker_announce_alert *taa = dynamic_cast <tracker_announce_alert*> (alert.get ());
		if (taa != NULL)
		{
			ev.enType = BTSET_TRACKER_ANNOUNCE;
			hDownload = taa->handle;
			goto _lRaiseEvent;
		}

		tracker_alert *ta = dynamic_cast <tracker_alert*> (alert.get ());
		if (ta != NULL)
		{
			ev.enType = BTSET_TRACKER;
			hDownload = ta->handle;
			ev.times_in_row = ta->times_in_row;
			ev.status_code = ta->status_code;
			goto _lRaiseEvent;
		}

		tracker_reply_alert *tra = dynamic_cast <tracker_reply_alert*> (alert.get ());
		if (tra != NULL)
		{
			ev.enType = BTSET_TRACKER_REPLY;
			hDownload = tra->handle;
			goto _lRaiseEvent;
		}

		tracker_warning_alert *twa = dynamic_cast <tracker_warning_alert*> (alert.get ());
		if (twa != NULL)
		{
			ev.enType = BTSET_TRACKER_WARNING;
			hDownload = twa->handle;
			goto _lRaiseEvent;
		}

		url_seed_alert *usa = dynamic_cast <url_seed_alert*> (alert.get ());
		if (usa != NULL)
		{
			ev.enType = BTSET_URL_SEED;
			ev.pszUrl = usa->url.c_str ();
			goto _lRaiseEvent;
		}

		hash_failed_alert *hfa = dynamic_cast <hash_failed_alert*> (alert.get ());
		if (hfa != NULL)
		{
			ev.enType = BTSET_HASH_FAILED;
			hDownload = hfa->handle;
			ev.piece_index = hfa->piece_index;
			goto _lRaiseEvent;
		}

		peer_ban_alert *pba = dynamic_cast <peer_ban_alert*> (alert.get ());
		if (pba != NULL)
		{
			ev.enType = BTSET_PEER_BAN;
			hDownload = pba->handle;
			strIp = pba->ip.address ().to_string ().c_str ();
			ev.pszIp = strIp.c_str ();
			goto _lRaiseEvent;
		}

		peer_error_alert *pea = dynamic_cast <peer_error_alert*> (alert.get ());
		if (pea != NULL)
		{
			ev.enType = BTSET_PEER_ERROR;
			strIp = pea->ip.address ().to_string ().c_str ();
			ev.pszIp = strIp.c_str ();
			goto _lRaiseEvent;
		}

		torrent_finished_alert *tfa = dynamic_cast <torrent_finished_alert*> (alert.get ());
		if (tfa != NULL)
		{
			ev.enType = BTSET_TORRENT_FINISHED;
			goto _lRaiseEvent;
		}

		continue;	

_lRaiseEvent:

		vmsBtDownloadImpl *pDld = NULL;
		if (hDownload.is_valid ())
		{
			EnterCriticalSection (&pthis->m_csDeleteDownload);
			int i = pthis->FindDownloadIndex (hDownload);
			if (i != -1)
			{
				pDld = pthis->m_vDownloads [i];
				pDld->AddRef ();
			}
			LeaveCriticalSection (&pthis->m_csDeleteDownload);
		}
		ev.pDownload = pDld;

		pthis->m_pfnEvHandler (pthis, &ev, pthis->m_pEvData);

		if (ev.pDownload != NULL && 
				(ev.enType == BTSET_TRACKER || ev.enType == BTSET_TRACKER_REPLY || ev.enType == BTSET_TRACKER_WARNING))
			((vmsBtDownloadImpl*)ev.pDownload)->OnTrackerAlert (ev.pszMsg);

		if (pDld)
			pDld->Release ();
	}

	pthis->m_bThreadRunning = false;
	return 0;
}

void vmsBtSessionImpl::set_EventsHandler (fntBtSessionEventsHandler pfn, LPVOID pData)
{
	m_pfnEvHandler = pfn;
	m_pEvData = pData;
}

int vmsBtSessionImpl::FindDownloadIndex (const torrent_handle &h)
{
	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		if (m_vDownloads [i]->m_handle == h)
			return i;
	}

	return -1;
}

void vmsBtSessionImpl::SetProxySettings (LPCSTR pszIp, int nPort, LPCSTR pszUser, LPCSTR pszPwd)
{
	proxy_settings ps;

	if (pszIp && *pszIp)
	{
		ps.hostname = pszIp;
		ps.port = nPort;
		ps.username = pszUser;
		ps.password = pszPwd;
		ps.type = pszUser && *pszUser ? proxy_settings::http_pw : proxy_settings::http;
	}
	else
	{
		ps.type = proxy_settings::none;
	}

	m_session.set_peer_proxy (ps);
	m_session.set_web_seed_proxy (ps);
	m_session.set_tracker_proxy (ps);
	m_session.set_dht_proxy (ps);
}

void vmsBtSessionImpl::RestoreDownloadHandle (vmsBtDownloadImpl* dld)
{
	EnterCriticalSection (&m_csRestoreTorrentHandle);

	if (dld->m_handle.is_valid ())
	{
		LeaveCriticalSection (&m_csRestoreTorrentHandle);
		return;
	}

	try {
		dld->m_handle = m_session.add_torrent (*dld->m_pTorrent->m_torrent, dld->m_strOutputPath);
		dld->m_handle.pause ();
	}
	catch (...) {
		LeaveCriticalSection (&m_csRestoreTorrentHandle);
		return;
	}

	LeaveCriticalSection (&m_csRestoreTorrentHandle);

	if (m_pfnEvHandler)
	{
		vmsBtSessionEvent ev;
		ev.enType = BTSET_BTDLD_WAS_RESET;
		ev.pDownload = dld;
		ev.pszMsg = NULL;
		m_pfnEvHandler (this, &ev, m_pEvData);
	}
}

int vmsBtSessionImpl::get_TotalDownloadConnectionCount ()
{
	int n = 0;
	for (size_t i = 0; i < m_vDownloads.size (); i++)
		n += m_vDownloads [i]->get_DownloadConnectionCount ();
	return n;
}

int vmsBtSessionImpl::get_DownloadCount ()
{
	return m_vDownloads.size ();
}

vmsBtDownload* vmsBtSessionImpl::get_Download (int nIndex)
{
	return m_vDownloads [nIndex];
}

int vmsBtSessionImpl::get_TotalDownloadSpeed ()
{
	return (int)m_session.status ().payload_download_rate;
}

int vmsBtSessionImpl::get_TotalUploadSpeed ()
{
	return (int)m_session.status ().payload_upload_rate;
}

UINT64 vmsBtSessionImpl::get_TotalDownloadedByteCount ()
{
	return m_session.status ().total_payload_download;
}

UINT64 vmsBtSessionImpl::get_TotalUploadedByteCount ()
{
	return m_session.status ().total_payload_upload;
}

void vmsBtSessionImpl::SetUserAgent (LPCSTR pszUA)
{
	session_settings s = m_session.settings ();
	if (stricmp (pszUA, "FDM 3.x") == 0 || stricmp (pszUA, "FDM 2.x") == 0)
		pszUA = "FDM/3.0(848)";
	s.user_agent = pszUA;
	m_session.set_settings (s);
}

void vmsBtSessionImpl::SetMaxHalfOpenConnections (int limit)
{
	if (limit > 0 && limit < 200)
		m_session.set_max_half_open_connections (limit);
}

void vmsBtSessionImpl::SetMaxConnections (int limit)
{
	if (limit < 0 && limit != -1)
		limit = -1; 
	m_session.set_max_connections (limit);
}