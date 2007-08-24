/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "fsIECatchMgr.h"
#include "misc.h"
#include "inetutil.h"
#include "DownloadsWnd.h"
#include "iefdm/iefdm.h"
#include "iefdm/iefdmdm/iefdmdm.h"
#include "iefdm/iecooks/iecooks.h"

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

BOOL fsIECatchMgr::OnBeforeNavigate(LPCSTR pszUrl, BOOL bCheckALT)
{
	try {

	CHAR szFile [MY_MAX_PATH] = "";
	LPCSTR pszExt;
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

	

	pszExt = strrchr (szFile, '.');
	if (pszExt) 
		pszExt++;
	else
		return TRUE;

	BOOL bExtPresent = IsExtInExtsStr (m_strSkipExts, pszExt);

	if (bExtPresent)
		return TRUE;	

	return _pwndDownloads->CreateDownload (pszUrl, TRUE) == FALSE;

	}
	catch (...)
	{
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

typedef HRESULT (_stdcall *fntDllRegUnregServer)(void);

BOOL fsIECatchMgr::ActivateIE2(BOOL bActivate)
{
	fntDllRegUnregServer pfnDll = NULL;

	DWORD dw1, dw2, dw3, dw4; 
	GetIEVersion (&dw1, &dw2, &dw3, &dw4);

	
	HMODULE hLib = LoadLibrary ("iefdm.dll");
	if (hLib == NULL)
		return FALSE;

	if (bActivate && dw1 < 6)
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllRegisterServer");
	else
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllUnregisterServer");

	if (pfnDll == NULL)
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	if (FAILED (pfnDll ()))
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	FreeLibrary (hLib);

	
	hLib = LoadLibrary ("iefdmdm.dll");
	if (hLib == NULL)
		return FALSE;

	if (bActivate && dw1 >= 6)
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllRegisterServer");
	else
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllUnregisterServer");

	if (pfnDll == NULL)
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	if (FAILED (pfnDll ()))
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	FreeLibrary (hLib);

	
	hLib = LoadLibrary ("iefdm2.dll");
	if (hLib == NULL)
		return FALSE;

	if (bActivate && dw1 >= 6)
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllRegisterServer");
	else
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllUnregisterServer");

	if (pfnDll == NULL)
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	if (FAILED (pfnDll ()))
	{
		FreeLibrary (hLib);
		return FALSE;
	}

	FreeLibrary (hLib);

	return TRUE;
}

void fsIECatchMgr::CleanIEPluginKey()
{
	CRegKey key;

	if (ERROR_SUCCESS != key.Open (HKEY_LOCAL_MACHINE, 
			"Software\\Microsoft\\Internet Explorer\\Plugins\\Extension"))
		return;

	int i = 0;
	char szKey [1000];
	fs::list <fsString> vKeys;
	while (RegEnumKey (key, i++, szKey, sizeof (szKey)) == ERROR_SUCCESS)
	{
		CRegKey key2;
		key2.Open (key, szKey);

		char szValue [1000]; DWORD dw = sizeof (szValue);
		key2.QueryValue (szValue, NULL, &dw);
		
		key2.Close ();

		if (strnicmp (szValue, "Free Download Manager", strlen ("Free Download Manager")) == 0)
			vKeys.add (szKey);
	}

	for (i = 0; i < vKeys.size (); i++)
		key.RecurseDeleteKey (vKeys [i]);
}

BOOL fsIECatchMgr::IsMonitoringDllRegistered()
{
	IUnknown* p;
	
	HRESULT hr1 = CoCreateInstance (CLSID_WgBHO, NULL, CLSCTX_ALL, IID_IUnknown, (void**) &p);

	if (SUCCEEDED (hr1)) 
		p->Release ();

	HRESULT hr2 = CoCreateInstance (CLSID_IEWGDM, NULL, CLSCTX_ALL, IID_IUnknown, (void**) &p);

	if (SUCCEEDED (hr2)) 
		p->Release ();

	if (SUCCEEDED (hr1) && SUCCEEDED (hr2))
		ActivateIE2 (TRUE);	

	
	if (SUCCEEDED (hr2))
		ActivateIE2 (TRUE);

	return SUCCEEDED (hr1) || SUCCEEDED (hr2);
}

DWORD WINAPI fsIECatchMgr::_threadMonitorIEActivity(LPVOID lp)
{
	fsIECatchMgr* pthis = (fsIECatchMgr*)lp;
	CoInitialize (NULL);

	IFDMIEStatPtr spStat;
	spStat.CreateInstance (__uuidof (FDMIEStat));

	if (spStat != NULL)
	{
		int iStarted = -1;

		while (pthis->m_bNeedStop == false)
		{
			long lCount;
			spStat->get_DownloadCount (&lCount);

			
			
			if (iStarted == -1 || (!iStarted != !lCount))
			{
				iStarted = lCount != 0;
				pthis->Event (iStarted ? ICME_DLSTARTED : ICME_DLCOMPLETED);
			}

			Sleep (500);
		}
	}
	
	CoUninitialize ();
	pthis->m_bMonitorIEActivityRunning = false;
	return 0;
}
