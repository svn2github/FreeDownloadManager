/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsBtSupport.h"
#include "mfchelp.h"
#include "vmsTrafficUsageModeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsBtSupport::vmsBtSupport()
{
	m_pbDHTstate = NULL;
	m_hBtDll = NULL;
	m_bWasShutdown = false;
	m_pSession = NULL;
}

vmsBtSupport::~vmsBtSupport()
{
}

BOOL vmsBtSupport::Initialize()
{
	if (false == LoadBtDll ())
		return FALSE;
	return LoadState ();
}

vmsBtSession* vmsBtSupport::get_Session()
{
	return m_pSession;
}

vmsBtSession* vmsBtSupport::create_Session()
{
	if (m_bWasShutdown)
		return NULL;

	vmsAUTOLOCKRW_WRITE(m_rwlSession);

	if (m_pSession)
		return m_pSession;

	vmsAUTOLOCKSECTION(m_cs1);

	if (!m_hBtDll)
		Initialize();

	typedef vmsBtSession* (WINAPI *FNS)();
	FNS pfnSession = (FNS)GetProcAddress(m_hBtDll, "vmsBt_createSession");

	if (pfnSession)
	{
		m_pSession = pfnSession();

		if (m_pSession)
		{

			CHAR szAgentName[10000] = { 0, };
#ifdef UNICODE
			
			DWORD dwBuffSize = 10000;
			vmsUnicodeToUtf8(vmsFdmAppMgr::getAppBtAgentName(), szAgentName, &dwBuffSize);
#else
			
#endif
			m_pSession->SetUserAgent(szAgentName);
			m_pSession->DisableOsCash();
			ApplyExtensions();
			ApplyListenPortSettings();
			ApplyDHTSettings();
			ApplyAdditionalTorrentSettings();
			m_pSession->SetDownloadLimit(-1);
			m_pSession->SetUploadLimit(-1);
			ApplyProxySettings();
			_DldsMgr.AttachToBtSession();
			_DldsMgr.setNeedApplyTrafficLimit();
		}
	}

	return m_pSession;
}

BOOL vmsBtSupport::is_Initialized()
{
	return m_hBtDll != NULL;
}

void vmsBtSupport::ApplyExtensions(){
	if (is_Initialized () == FALSE)
		return;

	LockSession(true);
	vmsBtSession *pBtSession = get_Session ();

	if (_App.Bittorrent_EnableUTPEX() && (pBtSession != NULL))
	{
		pBtSession->addExtensionUTPEX();
	}
	UnlockSession(true);
}

void vmsBtSupport::ApplyListenPortSettings()
{
	if (is_Initialized ())
	{
		LockSession(true);
		vmsBtSession *pBtSession = get_Session ();
		
		int portFrom = _App.Bittorrent_ListenPort_From (),
			portTo = _App.Bittorrent_ListenPort_To ();
		if ((pBtSession != NULL) && (pBtSession->IsListening() == FALSE ||
				pBtSession->get_ListenPort () > portTo ||
				pBtSession->get_ListenPort () < portFrom))
			pBtSession->ListenOn (portFrom, portTo);
		UnlockSession(true);
	}
}

void vmsBtSupport::ApplyDHTSettings()
{
	if (is_Initialized () == FALSE)
		return;

	LockSession(true);
	vmsBtSession *pBtSession = get_Session ();

	if (pBtSession){
		if (_App.Bittorrent_EnableDHT())
		{
			if (pBtSession->DHT_isStarted() == FALSE){
				pBtSession->DHT_start(m_pbDHTstate, m_dwDHTstateSize);
			}
		}
		else
		{
			if (pBtSession->DHT_isStarted())
				pBtSession->DHT_stop();
		}
	}
	UnlockSession(true);
}

void vmsBtSupport::ApplyAdditionalTorrentSettings(){
	if (is_Initialized () == FALSE)
		return;

	LockSession(true);
	vmsBtSession *pBtSession = get_Session ();

	if (pBtSession){

		if (_App.Bittorrent_EnableLocalPeerDiscovery())
		{
			pBtSession->LocalPeers_start();
		}
		else
		{
			pBtSession->LocalPeers_stop();
		}

		if (_App.Bittorrent_EnableUPnP())
		{
			pBtSession->UPNP_start();
		}
		else
		{
			pBtSession->UPNP_stop();
		}

		if (_App.Bittorrent_EnableNATPMP())
		{
			pBtSession->NATPMP_start();
		}
		else
		{
			pBtSession->NATPMP_stop();
		}

		pBtSession->setSequentialDownloads(_App.Bittorrent_EnableSequentialDownloading());
	}

	UnlockSession(true);
}

BOOL vmsBtSupport::SaveState()
{
	if (is_Initialized () == FALSE)
		return TRUE;

	vmsAUTOLOCKRW_READ(m_rwlSession);
	vmsBtSession *pBtSession = get_Session ();
	if (pBtSession == NULL)
		return TRUE;
	vmsBtPersistObject *pBtPO = NULL;
	pBtSession->getPersistObject (&pBtPO);
	assert (pBtPO != NULL);
	if (!pBtPO)
		return FALSE;

	if (!pBtPO->isDirty())
		return TRUE;

	if (pBtSession->DHT_isStarted ())
	{
		SAFE_DELETE_ARRAY (m_pbDHTstate);
	
		pBtPO->getStateBuffer(0, &m_dwDHTstateSize, false);

		m_pbDHTstate = new BYTE [m_dwDHTstateSize];

		pBtPO->getStateBuffer(m_pbDHTstate, &m_dwDHTstateSize, true);
	}

	if (m_pbDHTstate == NULL)
		return TRUE;

	HANDLE hFile = CreateFile (fsGetDataFilePath (_T("btdht.sav")), GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;
	WriteFile (hFile, m_pbDHTstate, m_dwDHTstateSize, &dw, NULL);

	CloseHandle (hFile);

	return TRUE;
}

BOOL vmsBtSupport::LoadState()
{
	if (GetFileAttributes (fsGetDataFilePath (_T("btdht.sav"))) == DWORD (-1))
		return TRUE;

	HANDLE hFile = CreateFile (fsGetDataFilePath (_T("btdht.sav")), GENERIC_READ,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	SAFE_DELETE_ARRAY (m_pbDHTstate);
	m_dwDHTstateSize = GetFileSize (hFile, NULL);
	m_pbDHTstate = new BYTE [m_dwDHTstateSize];

	DWORD dw;
	ReadFile (hFile, m_pbDHTstate, m_dwDHTstateSize, &dw, NULL);

	CloseHandle (hFile);

	return TRUE;
}

void vmsBtSupport::Shutdown()
{
	m_bWasShutdown = true;

	RemoveSession();
}

void vmsBtSupport::RemoveSession(){
	vmsAUTOLOCKRW_WRITE(m_rwlSession);

	if (is_Initialized() == FALSE)
		return;

	typedef void (WINAPI *FNS)();
	FNS pfn = (FNS)GetProcAddress(m_hBtDll, "vmsBt_RemoveSession");
	if (pfn)
		pfn();	
	m_pSession = NULL;
}

void vmsBtSupport::ApplyProxySettings()
{
	if (is_Initialized () == FALSE)
		return;
	vmsAUTOLOCKRW_READ(m_rwlSession);
	vmsBtSession *pBtSession = get_Session();
	if (pBtSession == NULL)
		return;
	fsString strIp, strUser, strPwd;
	int nPort = 0;

	switch (_App.InternetAccessType ())
	{
	case IATE_PRECONFIGPROXY:
		GetIeProxySettings (strIp, strUser, strPwd, nPort);
		break;

	case IATE_NOPROXY:
		break;

	case IATE_MANUALPROXY:
		strIp = _App.HttpProxy_Name ();
		strUser = _App.HttpProxy_UserName ();
		strPwd = _App.HttpProxy_UserName ();
		if (strIp.IsEmpty () == FALSE)
		{
			TCHAR tsz [1000];
			_tcscpy (tsz, strIp);
			LPTSTR pszPort = _tcsrchr (tsz, _T(':'));
			if (pszPort)
			{
				*pszPort++ = 0;
				nPort = _tstoi (pszPort);
				strIp = tsz;
			}
		}
		break;

	case IATE_FIREFOXPROXY:
		GetFirefoxProxySettings (strIp, strUser, strPwd, nPort);
		break;
	}

	get_Session ()->SetProxySettings (strIp ? stringFromTstring ((LPCTSTR)strIp).c_str() : nullptr, 
		nPort, strUser ? stringFromTstring ((LPCTSTR)strUser).c_str() : nullptr, 
		strPwd ? stringFromTstring ((LPCTSTR)strPwd).c_str() : nullptr);
}

void vmsBtSupport::GetIeProxySettings(fsString &strIp, fsString &strUser, fsString &strPwd, int &nPort)
{
	strIp = strUser = strPwd = _T(""); 
	nPort = 0;

	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 
			KEY_READ))
		return;

	DWORD dw;
	if (ERROR_SUCCESS != key.QueryValue (dw, _T("ProxyEnable")))
		return;
	if (dw == FALSE)
		return;

	TCHAR szProxy [1000];
	dw = sizeof (szProxy);
	if (ERROR_SUCCESS != key.QueryValue (szProxy, _T("ProxyServer"), &dw))
		return;

	LPTSTR pszPort = _tcsrchr (szProxy, _T(':'));
	if (pszPort)
	{
		*pszPort = 0;
		pszPort++;
		nPort = _tstoi (pszPort);
	}

	strIp = szProxy;
}

void vmsBtSupport::GetFirefoxProxySettings(fsString &strIp, fsString &strUser, fsString &strPwd, int &nPort)
{
	strIp = strUser = strPwd = _T(""); 
	nPort = 0;

	if (1 != _App.FirefoxSettings_Proxy_Type ())
		return;

	strIp = _App.FirefoxSettings_Proxy_Addr (_T("http"));
	nPort = _App.FirefoxSettings_Proxy_Port (_T("http"));
}

vmsBtFile* vmsBtSupport::CreateTorrentFileObject()
{
	if (m_hBtDll == NULL && false == LoadBtDll ())
		return NULL;
	
	typedef vmsBtFile* (WINAPI *FNBTF)();
	static FNBTF _pfnCreateTorrentFileObject = NULL;
	
	if (_pfnCreateTorrentFileObject == NULL)
	{
		_pfnCreateTorrentFileObject = (FNBTF) GetProcAddress (m_hBtDll, "vmsBt_CreateTorrentFileObject");
		if (_pfnCreateTorrentFileObject == NULL)
			return NULL;
	}

	return _pfnCreateTorrentFileObject ();
}

vmsUTorrentDownloadsDb* vmsBtSupport::CreateUTorrentDownloadsDbObject()
{
	if (m_hBtDll == NULL && false == LoadBtDll ())
		return NULL;
	
	typedef vmsUTorrentDownloadsDb* (WINAPI *FNUTD)();
	static FNUTD _pfnCreateUTorrentDownloadsDbObject = NULL;
	
	if (_pfnCreateUTorrentDownloadsDbObject == NULL)
	{
		_pfnCreateUTorrentDownloadsDbObject = (FNUTD) GetProcAddress (m_hBtDll, "vms_CreateUTorrentDownloadsDbObject");
		if (_pfnCreateUTorrentDownloadsDbObject == NULL)
			return NULL;
	}

	return _pfnCreateUTorrentDownloadsDbObject ();
}

bool vmsBtSupport::LoadBtDll()
{
	if (m_hBtDll)
		return true;
	if (!isBtDllValid ())
		return false;
	USES_CONVERSION;
	return NULL != (m_hBtDll = LoadLibrary (getBtDllFileName ().c_str ()));
}

int vmsBtSupport::getBtDllVersion()
{
	static int ver = -1;
	if (ver != -1)
		return ver;

	CString str = getBtDllFileName ().c_str ();

	if (GetFileAttributes (str) == DWORD (-1))
		return ver = INT_MAX;
	
	DWORD dw;
	DWORD dwSize = GetFileVersionInfoSize (str, &dw);
	if (dwSize == 0)
		return ver = 0;
	LPVOID pvVer = _alloca (dwSize);
	ZeroMemory (pvVer, dwSize);
	if (FALSE == GetFileVersionInfo (str, 0, dwSize, pvVer))
		return ver = 0;
	
	VS_FIXEDFILEINFO *pFileInfo = NULL;
	UINT u;
	if (FALSE == VerQueryValue (pvVer, _T ("\\"), (LPVOID*)&pFileInfo, &u))
		return ver = 0;
	
	return ver = pFileInfo->dwFileVersionLS >> 16;
}

int vmsBtSupport::getBtDllMinimumFdmBuildRequired()
{
	static int _val = -1;
	if (_val != -1)
		return _val;

	CString str = getBtDllFileName ().c_str ();

	if (GetFileAttributes (str) == DWORD (-1))
		return _val = 0;

	vmsDLL dll; 
	if (!dll.Load (str))
		return _val = 0;

	typedef int (WINAPI *FNgetMinimumFdmBuildRequired)();
	FNgetMinimumFdmBuildRequired pfn = (FNgetMinimumFdmBuildRequired)dll.GetProcAddress ("vmsBt_getMinimumFdmBuildRequired");
	if (!pfn)
		return _val = 0;

	return _val = pfn ();
}

tstring vmsBtSupport::getBtDllFileName(void)
{
	tstring tstrDll;

	TCHAR tsz [MAX_PATH] = _T ("");
	GetModuleFileName (NULL, tsz, MAX_PATH);
	LPTSTR ptsz = _tcsrchr (tsz, '\\');
	if (ptsz)
	{
		_tcscpy (ptsz + 1, _T ("fdmbtsupp.dll"));
		tstrDll = tsz;
	}

	if (GetFileAttributes (tstrDll.c_str ()) != DWORD (-1))
		return tstrDll;

	
	tstrDll = ((CFdmApp*)AfxGetApp ())->m_strAppPath;
	if (tstrDll [tstrDll.length () - 1] != _T('\\'))
		tstrDll += _T('\\');
	tstrDll += _T ("fdmbtsupp.dll");
	return tstrDll;
}

bool vmsBtSupport::isBtDllValid(void)
{
	static int _iResult = -1;
	
	if (_iResult != -1)
		return _iResult != 0;

	if (GetFileAttributes (getBtDllFileName ().c_str ()) == DWORD (-1))
	{
		_iResult = 1;
		return true;
	}

	_iResult = getBtDllVersion () >= BTSUPP_DLL_MINVERREQ &&
		getBtDllMinimumFdmBuildRequired () <= vmsFdmAppMgr::getBuildNumber ();

	return _iResult != 0;
}

void vmsBtSupport::LockSession(bool bForRead){
	if (bForRead)
		m_rwlSession.AcquireReaderLock();
	else
		m_rwlSession.AcquireWriterLock();

}

void vmsBtSupport::UnlockSession(bool bForRead){
	if (bForRead)
		m_rwlSession.ReleaseReaderLock();
	else
		m_rwlSession.ReleaseWriterLock();
}
