/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsBtDownloadImpl.h"
#include "vmsBtSessionImpl.h"
#include "vmsBtDownloadPeerInfoListImpl.h"
#include "vmsBtDownloadPeerInfoImpl.h"

vmsBtDownloadImpl::vmsBtDownloadImpl(void)
{
	m_iDownloadSpeedLimit = -1;
	m_pTorrent = NULL;
	m_peerList.m_dld = this;
	m_cRefs = 0;
	m_dwDob_status = 0;
}

vmsBtDownloadImpl::~vmsBtDownloadImpl(void)
{
	if (m_pTorrent)
		m_pTorrent->Release ();
}

vmsBtDownloadState vmsBtDownloadImpl::GetState ()
{
	if (m_handle.is_valid () == false)
		return BTDS_QUEUED;

	

	switch (m_handle.status ().state)
	{
	case torrent_status::queued_for_checking:
		return BTDS_QUEUED;

	case torrent_status::checking_files:
		return BTDS_CHECKING_FILES;

	case torrent_status::connecting_to_tracker:
		return BTDS_CONNECTING_TRACKER;

	case torrent_status::downloading:
		return BTDS_DOWNLOADING;

	case torrent_status::finished:
		return BTDS_FINISHED;

	case torrent_status::seeding:
		return BTDS_SEEDING;

	case torrent_status::allocating:
		return BTDS_ALLOCATING;
	}

	assert (FALSE);
	throw 0;
}

BOOL vmsBtDownloadImpl::IsPaused ()
{
	if (m_handle.is_valid () == false)
		return FALSE;
	
	return m_handle.status ().paused;
}

void vmsBtDownloadImpl::get_SavePath (LPSTR pszRes)
{
	if (m_handle.is_valid () == false) {
		*pszRes = 0;
		return;
	}
	
	strcpy (pszRes, m_handle.save_path ().string ().c_str ());
}

float vmsBtDownloadImpl::get_PercentDone ()
{
	if (m_handle.is_valid () == false)
		return 0;

	return (float)
		((double)get_TotalDownloadedBytesCount () / m_pTorrent->get_TotalFilesSize () * 100.0);
		
}

int vmsBtDownloadImpl::get_PiecesProgressMap (bool* pbPieces, int *pnCompletedPieces)
{
	if (m_handle.is_valid () == false) {
		if (pnCompletedPieces != NULL)
			*pnCompletedPieces = 0;
		return 0;
	}

	

	if (pnCompletedPieces != NULL)
		*pnCompletedPieces = status ().num_pieces;

	const std::vector<bool>* pieces = status ().pieces;

	if (pieces && pbPieces)
	{
		for (size_t i = 0; i < pieces->size (); i++)
			pbPieces [i] = pieces->at (i);
	}

	return pieces ? pieces->size () : 0;
}

int vmsBtDownloadImpl::get_DownloadConnectionCount ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	

	std::vector <peer_info> v;
	m_handle.get_peer_info (v);

	int n = 0;

	for (size_t i = 0; i < v.size (); i++)
	{
		if ((v [i].flags & peer_info::interesting) != 0 && 
				(v [i].flags & peer_info::queued) == 0)
			n++;
	}

	return n; 
}

UINT64 vmsBtDownloadImpl::get_TotalDownloadedBytesCount ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	return status ().total_done;
}    

UINT vmsBtDownloadImpl::GetDownloadSpeed ()
{
	if (m_handle.is_valid () == false)
		return 0;

	

	

	return (UINT)status ().download_payload_rate;
}

UINT vmsBtDownloadImpl::GetUploadSpeed ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	return (UINT)status ().upload_payload_rate;
}

void vmsBtDownloadImpl::Pause ()
{
	if (m_handle.is_valid () == false)
		return;
	
	m_handle.pause ();
}

void vmsBtDownloadImpl::Resume ()
{
	if (m_handle.is_valid () == false)
		return;
	
	m_handle.resume ();
}

void vmsBtDownloadImpl::SetDownloadLimit (int limit)
{
	if (m_handle.is_valid () == false)
		return;
	
	m_handle.set_download_limit (limit);
}

int  vmsBtDownloadImpl::GetDownloadLimit ()
{
	return m_iDownloadSpeedLimit;
}

vmsBtFile* vmsBtDownloadImpl::get_Torrent ()
{
	return m_pTorrent;
}

int vmsBtDownloadImpl::get_ConnectionCount ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	return status ().num_peers;
}

void vmsBtDownloadImpl::set_ConnectionLimit (int limit)
{
	if (m_handle.is_valid () == false)
		return;
	
	m_handle.set_max_connections (limit);
}

BOOL vmsBtDownloadImpl::get_FastResumeData (LPBYTE pbRes, DWORD dwSize, DWORD *pdwDataSize)
{
	if (m_handle.is_valid () == false)
		return FALSE;

	

try{

	entry e = m_handle.write_resume_data ();

	std::vector <char> v;
	bencode (std::back_inserter (v), e);

	*pdwDataSize = v.size ();

	if (pbRes == NULL)
		return TRUE;

	if (dwSize < v.size ())
		return FALSE;

	for (size_t i = 0; i < v.size (); i++)
		*pbRes++ = v [i];

	return TRUE;

}catch (...) {*pdwDataSize = 0; return FALSE;}
}

void vmsBtDownloadImpl::set_TrackerLogin (LPCSTR pszUser, LPCSTR pszPassword)
{
	if (m_handle.is_valid () == false)
		return;
	
	m_handle.set_tracker_login (pszUser, pszPassword);
}

BOOL vmsBtDownloadImpl::MoveToFolder (LPCSTR pszNewFolder)
{
	if (m_handle.is_valid () == false)
		return FALSE;
	

	char sz [MAX_PATH];
	strcpy (sz, pszNewFolder);
	LPSTR psz = sz;
	while (*++psz) if (*psz == '\\') *psz = '/';
	m_handle.move_storage (sz);

	return TRUE;
}

void vmsBtDownloadImpl::check_handle_is_valid ()
{
	if (m_handle.is_valid () == false)
		vmsBtSessionImpl::Instance ()->RestoreDownloadHandle (this);
	assert (m_handle.is_valid ());
}

int vmsBtDownloadImpl::get_NextAnnounceInterval ()
{
	if (m_handle.is_valid () == false)
		return -1;
	
	return m_handle.status ().next_announce.total_seconds ();
}

bool vmsBtDownloadImpl::is_PieceCompleted (int nIndex)
{
	if (m_handle.is_valid ())
		return status ().pieces ? m_status.pieces->at (nIndex) : false;
	return false;
}

void vmsBtDownloadImpl::get_CurrentTracker (LPSTR pszRes)
{
	if (m_handle.is_valid () == false) {
		*pszRes = 0;
		return;
	}
	
	torrent_status s = m_handle.status ();
	LPCSTR pszT = s.current_tracker.c_str ();
	if ((pszT == NULL || *pszT == 0) && m_handle.get_torrent_info ().trackers ().size ())
		pszT = m_handle.get_torrent_info ().trackers () [0].url.c_str ();
	if (pszT)
		strcpy (pszRes, pszT);
	else
		*pszRes = 0;
}

UINT64 vmsBtDownloadImpl::get_TotalUploadedByteCount ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	return status ().total_payload_upload;
}

void vmsBtDownloadImpl::get_PeersStat (int *pnPeersConnected, int *pnSeedsTotal, int *pnLeechersTotal, int *pnSeedsConnected)
{
	if (m_handle.is_valid () == false) {
		*pnPeersConnected = 0;
		*pnSeedsTotal = 0;
		*pnLeechersTotal = 0;
		*pnSeedsConnected = 0;
		return;
	}
	
	torrent_status s = m_handle.status ();
	*pnPeersConnected = s.num_peers;
	*pnSeedsTotal = s.num_complete;
	*pnLeechersTotal = s.num_incomplete;
	*pnSeedsConnected = s.num_seeds;
}

UINT64 vmsBtDownloadImpl::get_WastedByteCount ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	torrent_status s = status ();
	return s.total_failed_bytes + s.total_redundant_bytes;
}

double vmsBtDownloadImpl::get_ShareRating ()
{
	if (m_handle.is_valid () == false)
		return 0;
	
	torrent_status s = status ();
	if (s.total_payload_download == 0)
		return 0;
	return (double)s.total_payload_upload / s.total_payload_download;
}

vmsBtDownloadPeerInfoList* vmsBtDownloadImpl::get_PeerInfoList ()
{
	if (m_handle.is_valid () == false)
		return NULL;
	return &m_peerList;
}

void vmsBtDownloadImpl::get_FileProgress (float *p)
{
	if (m_handle.is_valid () == false)
		return;

	
	std::vector<float> v;
	m_handle.file_progress (v);
	for (size_t i = 0; i < v.size (); i++)
		p [i] = v [i];
}

BOOL vmsBtDownloadImpl::is_HandleValid ()
{
	return m_handle.is_valid ();
}

void vmsBtDownloadImpl::OnTrackerAlert (LPCSTR pszMsg)
{
	if (m_handle.trackers ().size () == 1 && m_pTorrent->m_torrent->trackers ().size () == 1)
		return;

	std::vector<announce_entry> const& vAllTrackers = m_pTorrent->m_torrent->trackers ();

	std::vector <announce_entry> v = m_handle.trackers ();
	int nTracker0 = -1, nTracker1 = -1;

	for (size_t i = 0; i < vAllTrackers.size (); i++)
	{
		if (strstr (pszMsg, vAllTrackers [i].url.c_str ()) != NULL)
		{
			nTracker0 = i;
			break;
		}
	}

	for (size_t i = 0; i < v.size (); i++)
	{
		if (strstr (pszMsg, v [i].url.c_str ()) != NULL)
		{
			nTracker1 = i;
			break;
		}
	}

	if (nTracker1 == -1)
		return;

	if (m_vTicksTrackersConnected.size () == 0)
	{
		
		for (size_t i = 0; i < vAllTrackers.size (); i++)
			m_vTicksTrackersConnected.push_back (DWORD (-1));
	}
	
	v.erase (v.begin () + nTracker1);

	const DWORD RECONNECT_MIN_TIME_INTERVAL	= 10*60*1000;

	
	for (size_t i = 0; i < v.size (); i++)
	{
		for (size_t j = 0; j < vAllTrackers.size (); j++)
		{
			if (v [i].url == vAllTrackers [j].url)
			{
				if (m_vTicksTrackersConnected [j] != DWORD (-1) &&
					(GetTickCount () - m_vTicksTrackersConnected [j] < RECONNECT_MIN_TIME_INTERVAL))
				{
					v.erase (v.begin () + i);
					i--;
					break;
				}
			}
		}
	}

	if (v.size () != 0)
	{
		m_vTicksTrackersConnected [nTracker0] = GetTickCount ();
		m_handle.replace_trackers (v);
		m_handle.force_reannounce ();
		return;
	}

	if (m_handle.trackers ().size () != vAllTrackers.size ())
		m_handle.replace_trackers (m_pTorrent->m_torrent->trackers ());
}

int vmsBtDownloadImpl::get_CurrentTaskProgress ()
{
	return (int)(m_handle.status ().progress * 100);
}

void vmsBtDownloadImpl::PrioritizeFiles (int* piPriorities)
{
	std::vector <int> v;
	int n = m_pTorrent->get_FileCount ();
	for (int i = 0; i < n; i++)
		v.push_back (*piPriorities++);
	m_handle.prioritize_files (v);
}

torrent_status& vmsBtDownloadImpl::status ()
{
	if (GetTickCount () - m_dwDob_status)
	{
		m_status = m_handle.status ();
		m_dwDob_status = GetTickCount ();
	}

	return m_status;
}