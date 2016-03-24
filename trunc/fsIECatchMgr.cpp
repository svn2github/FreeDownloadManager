/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsIECatchMgr.h"
#include "misc.h"
#include "inetutil.h"
#include "DownloadsWnd.h"
#include "iefdm/iefdmdm/iefdmdm.h"
#include "iefdm/fdmiebho/iecooks.h"
#include "vmsBrowsersSharedData.h"
#include "vmsLogger.h"

extern CDownloadsWnd* _pwndDownloads;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

_COM_SMARTPTR_TYPEDEF(IFDMIEStat, __uuidof(IFDMIEStat));

fsIECatchMgr::fsIECatchMgr()
{
	m_pfnEvents = NULL;
	m_bNeedStop = false;
	m_bMonitorIEActivityRunning = false;
}

fsIECatchMgr::~fsIECatchMgr()
{
	m_bNeedStop = true;
	while (m_bMonitorIEActivityRunning)
		Sleep (10);
}

HRESULT fsIECatchMgr::Initialize()
{
	DWORD dw;
	m_bMonitorIEActivityRunning = true;
	CloseHandle (
		CreateThread (NULL, 0, _threadMonitorIEActivity, this, 0, &dw));

	return S_OK;
}

BOOL fsIECatchMgr::OnBeforeNavigate(LPCTSTR pszUrl, BOOL bCheckALT)
{
	try 
	{
		TCHAR szFile [MY_MAX_PATH] = _T("");
		LPCTSTR pszExt;
		fsURL url;

		if (bCheckALT)
		{
			if ( (GetKeyState (VK_MENU) & 0x8000) == FALSE && m_bALTShouldPressed)
				return TRUE;
		}

		if (url.Crack (pszUrl) != IR_SUCCESS)
			return TRUE;

		

		fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
			TRUE, szFile, sizeof (szFile));

		if (*szFile == 0)
			return TRUE;

		pszExt = _tcsrchr (szFile, '.');
		if (pszExt) 
			pszExt++;
		else
			return TRUE;

		BOOL bExtPresent = IsExtInExtsStr (m_strSkipExts, pszExt);

		if (bExtPresent)
			return TRUE;	

		return UINT_MAX == _pwndDownloads->CreateDownload (pszUrl, TRUE);

	}
	catch (const std::exception& ex)
	{
		ASSERT (FALSE);
		vmsLogger::WriteLog("fsIECatchMgr::OnBeforeNavigate " + std::string(ex.what()));
		return TRUE;
	}
	catch (...)
	{
		ASSERT (FALSE);
		vmsLogger::WriteLog("fsIECatchMgr::OnBeforeNavigate unknown exception");
		return TRUE;
	}
}

void fsIECatchMgr::ReadSettings()
{
	m_strSkipExts = _App.Monitor_SkipExts ();

	m_bALTShouldPressed = _App.Monitor_ALTShouldPressed ();	
	m_bCatchClicks = FALSE; 
}

void fsIECatchMgr::Detach()
{
}

BOOL fsIECatchMgr::IsActive()
{
	return m_bMonitorIEActivityRunning;
}

void fsIECatchMgr::SetEventsFunc(fntIECatchMgrEvents pfn, LPVOID lpParam)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lpParam;
}

void fsIECatchMgr::Event(fsIECatchMgrEvent ev)
{
	if (m_pfnEvents)
		m_pfnEvents (ev, m_lpEventsParam);
}

BOOL fsIECatchMgr::IsIE2Active()
{
	static bool _bCreated = FALSE;

	if (_bCreated == FALSE)
	{
		if (IsMonitoringDllRegistered ())
		{
			_bCreated = TRUE; 
			return _App.Monitor_IE2 ();
		}

		_App.Monitor_IE2 (FALSE);
		return FALSE;
	}

	return _App.Monitor_IE2 ();
}

BOOL fsIECatchMgr::InstallIeIntegration(BOOL bInstall, BOOL bCurrentUserOnly)
{
	typedef HRESULT (_stdcall *fntInstaller)(bool, bool);
	fntInstaller pfnInstaller = NULL;

	bool bFdmdm = false, bBho = false;

	
	HMODULE hLib = LoadLibrary (_T("iefdmdm.dll"));
	if (hLib)
	{
		pfnInstaller = (fntInstaller) GetProcAddress (hLib, "vmsInstaller");
		if (pfnInstaller)
			bFdmdm = SUCCEEDED (pfnInstaller (bInstall, bCurrentUserOnly));
		FreeLibrary (hLib);
	}

	
	hLib = LoadLibrary (_T("iefdm2.dll"));

	if (hLib)
	{
		pfnInstaller = (fntInstaller) GetProcAddress (hLib, "vmsInstaller");
		if (pfnInstaller)
			bBho = SUCCEEDED (pfnInstaller (bInstall, bCurrentUserOnly));
		FreeLibrary (hLib);
	}

	return bFdmdm && bBho;
}

void fsIECatchMgr::CleanIEPluginKey()
{
	CRegKey key;

	if (ERROR_SUCCESS != key.Open (HKEY_LOCAL_MACHINE, 
			_T("Software\\Microsoft\\Internet Explorer\\Plugins\\Extension")))
		return;

	int i = 0;
	TCHAR szKey [1000];
	fs::list <fsString> vKeys;
	while (RegEnumKey (key, i++, szKey, sizeof (szKey)) == ERROR_SUCCESS)
	{
		CRegKey key2;
		key2.Open (key, szKey);

		TCHAR szValue [1000]; DWORD dw = sizeof (szValue);
		key2.QueryValue (szValue, NULL, &dw);
		
		key2.Close ();

		if (_tcsncicmp (szValue, _T("Free Download Manager"), _tcslen (_T("Free Download Manager"))) == 0)
			vKeys.add (szKey);
	}

	for (i = 0; i < vKeys.size (); i++)
		key.RecurseDeleteKey (vKeys [i]);
}

BOOL fsIECatchMgr::IsMonitoringDllRegistered()
{
	IUnknownPtr p;
	
	return SUCCEEDED (CoCreateInstance (
		CLSID_IEWGDM, NULL, CLSCTX_ALL, IID_IUnknown, (void**) &p));
}

DWORD WINAPI fsIECatchMgr::_threadMonitorIEActivity(LPVOID lp)
{
	vmsBrowsersSharedData BrowsersSharedData;

	fsIECatchMgr* pthis = (fsIECatchMgr*)lp;

	int iStarted = -1;

	while (pthis->m_bNeedStop == false)
	{
		long lCount;
		lCount = BrowsersSharedData.getActiveDownloadsCount ();

		
		
		if (iStarted == -1 || (!iStarted != !lCount))
		{
			iStarted = lCount != 0;
			pthis->Event (iStarted ? ICME_DLSTARTED : ICME_DLCOMPLETED);
		}

		Sleep (500);
	}
	
	pthis->m_bMonitorIEActivityRunning = false;
	return 0;
}
