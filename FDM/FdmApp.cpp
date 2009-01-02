// FdmApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "FdmApp.h"
#include "dbghelp.h"

#include "MainFrm.h"
#include "UrlWnd.h"
#include <initguid.h>
#include "WGUrlReceiver.h"
#include "Fdm_i.c"
#include "UEDlg.h"
#include "fsIEUserAgent.h"
#include "WgUrlListReceiver.h"
#include "CFDM.h"
#include "fsFDMCmdLineParser.h"
#include "mfchelp.h"
#include "vmsFilesToDelete.h"
#include "vmsDLL.h"
#include "vmsFirefoxMonitoring.h"
#include "FDMDownloadsStat.h"
#include "FDMDownload.h"
#include "FDMUploader.h"
#include "FDMUploadPackage.h"
#include "vmsTorrentExtension.h"
#include "FdmTorrentFilesRcvr.h"
#include "FDMFlashVideoDownloads.h"
#include "vmsUploadsDllCaller.h"
#include "vistafx/vistafx.h"
#include "inetutil.h"
#include "vmsNotEverywhereSupportedFunctions.h"
#include "SpiderWnd.h"
extern CSpiderWnd *_pwndSpider;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFdmApp

BEGIN_MESSAGE_MAP(CFdmApp, CWinApp)
	//{{AFX_MSG_MAP(CFdmApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFdmApp construction

CFdmApp::CFdmApp()
{
	m_bCOMInited = m_bATLInited = m_bATLInited2 = FALSE;
	m_bSaveAllOnExit = FALSE;
	m_hAppMutex = NULL;
	m_bEmbedding = FALSE;
	m_bStarting = TRUE;

	SYSTEMTIME time;
	GetLocalTime (&time);
	SystemTimeToFileTime (&time, &_timeAppHasStarted);

	m_pModuleState;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFdmApp object

CFdmApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFdmApp initialization
BOOL CFdmApp::InitInstance()
{
	SetUnhandledExceptionFilter (_UEF);

	AfxEnableControlContainer ();

	// Change the registry key under which our settings are stored.
	SetRegistryKey (IDS_COMPANY);

	CheckRegistry ();

	m_bSaveAllOnExit = FALSE;

	fsIECatchMgr::CleanIEPluginKey ();

	// application working directory
	CString strPath = GetProfileString ("", "Path", "");
	BOOL bNeedLocalRegister = FALSE;
	if (strPath == "")
	{
		CRegKey key;
		if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager", KEY_WRITE))
			vmsSHCopyKey (HKEY_LOCAL_MACHINE, "Software\\FreeDownloadManager.ORG\\Free Download Manager", key);
		strPath = GetProfileString ("", "Path", "");
		bNeedLocalRegister = strPath != "";
	}

	if (GetFileAttributes (strPath + "\\fdm.exe") == DWORD (-1))
	{
		strPath = "";
		bNeedLocalRegister = false;
	}

	// activate portable mode if fdm is not registered on this machine
	// or it was registered but installation is damaged
	if (strPath == "" || FALSE == SetCurrentDirectory (strPath))
		_dwAppState |= APPSTATE_PORTABLE_MODE;

	char szExeDir [MY_MAX_PATH], szExeFile [MY_MAX_PATH];
	GetModuleFileName (NULL, szExeFile, sizeof (szExeFile));
	fsGetPath (szExeFile, szExeDir);

	if (IS_PORTABLE_MODE)
	{
		strPath = szExeDir;
		SetCurrentDirectory (strPath);
	}

	m_strAppPath = strPath;
	if (m_strAppPath.IsEmpty () == FALSE)
	{
		if (m_strAppPath [m_strAppPath.GetLength ()-1] != '\\' &&
				m_strAppPath [m_strAppPath.GetLength ()-1] != '/')
			m_strAppPath += '\\';
	}

	if (IS_PORTABLE_MODE == false)
	{
		CString strDataFldr = szExeDir; strDataFldr += "Data";
		// if "Data" folder exists in app's exe folder that
		// activate portable mode, because this folder is created in portable mode
		// however fdm is installed on this machine also
		if (m_strAppPath.CompareNoCase (szExeDir) &&
			 DWORD (-1) != GetFileAttributes (strDataFldr))
		{
			// portable fdm was launched, but there is installed fdm on this machine
			_dwAppState |= APPSTATE_PORTABLE_MODE;
			_dwAppState |= APPSTATE_PORTABLE_MODE_NOREG;
			m_strAppPath = szExeDir;
		}
	}

	if (IS_PORTABLE_MODE)
	{
		// load portable settings
		vmsAppSettingsStore* pStgs = _App.get_SettingsStore ();
		CString strStgsFile = m_strAppPath + "Data\\settings.dat";
		fsBuildPathToFile (strStgsFile);
		pStgs->LoadSettingsFromFile (strStgsFile);
	}

	BOOL bNoLng = FALSE;

	if (FALSE == InitLanguage ())
		bNoLng = TRUE;

	if (strcmp (m_lpCmdLine, "-suis") == 0 || 
			strcmp (m_lpCmdLine, "-euis") == 0 ||
			strcmp (m_lpCmdLine, "-duis") == 0)
	{
		IntegrationSettings ();
		return FALSE;
	}

	if (IS_PORTABLE_MODE)
	{
		// when in portable mode, there is may be the situation that we have no
		// write access in fdm's exe folder. 
		// so we should check this here
		char szTmpFile [MY_MAX_PATH];
		CString str = m_strAppPath; str += "Data";
		CreateDirectory (str, NULL);
		if (0 == GetTempFileName (str, "fdm", 0, szTmpFile))
			MessageBox (NULL, LS (L_NOWRITEACCESSTODATAFOLDER), vmsFdmAppMgr::getAppName (), MB_ICONWARNING);
		else
			DeleteFile (szTmpFile);
	}

	_SkinMgr.Initialize ();

	_IECatchMgr.ReadSettings ();
	_NOMgr.Initialize ();
	_IECMM.ReadState ();

	//HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hRes = OleInitialize (NULL);
	//HRESULT hRes = CoInitialize (0);
	if (FAILED(hRes))
		return FALSE;
	
	m_bCOMInited = TRUE;

	fsFDMCmdLineParser cmdline;
	cmdline.Parse ();
	m_bForceSilentSpecified = cmdline.is_ForceSilentSpecified ();

	// fix for 2.3 beta 6
	if (GetVersion () & 0x80000000)
	{
		// win 9x
		CFileFind f; 
		BOOL b = f.FindFile (m_strAppPath + "*.sav");
		std::vector <CString> v;
		while (b)
		{
			b = f.FindNextFile ();
			v.push_back (f.GetFileName ());
		}
		for (size_t i = 0; i < v.size (); i++)
			MoveFile (m_strAppPath + v [i], fsGetDataFilePath (v [i]));
	}
	MoveFile (fsGetDataFilePath ("uploads.sav"), fsGetDataFilePath ("uploads.1.sav"));
	//////////////////////////////

	if (!InitATL())
		return FALSE;

	if (VistaFx::IsVista () && strncmp (m_lpCmdLine, "-nelvcheck", 10) && stricmp (m_lpCmdLine, "-autorun"))
	{
		VistaFx vf;
		vf.LoadDll (m_strAppPath + "vistafx.dll");
		if (vf.IsProcessElevated ())
		{
			char sz [MAX_PATH];
			GetModuleFileName (NULL, sz, MAX_PATH);
			CString str = "-nelvcheck ";
			str += m_lpCmdLine;
			if (m_hAppMutex)
			{
				CloseHandle (m_hAppMutex);
				m_hAppMutex = NULL;
			}
			if (vf.RunNonElevatedProcess (sz, str, ""))
				return FALSE;
			m_hAppMutex = CreateMutex (NULL, TRUE, _pszAppMutex);
		}
	}

	// register portable fdm in system
	// at the exit we'll unregister it
	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_RegisterServer ();

	// todo: remove this, because vmsFilesToDeleted was replaced
	//  by vmsDeleteFileAtWinBoot function.
	// it was kept only in order to get users of older versions (build < 590, 2.1 version)
	// update correctly (get deleted scheduled to delete files).
	vmsFilesToDelete::Process ();

	if (bNeedLocalRegister)
		RegisterServer (FALSE);

	/*if (bNoLng)
	{
		CString str;
		str.Format ("Free Download Manager can't find language files.\nPlease reinstall it.\n\nError code: %d.", m_nNoLngsErrReason);
		MessageBox (NULL, str, LS (L_ERR), MB_ICONWARNING);
	}*/

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CheckLocked ();

	m_bSaveAllOnExit = TRUE;

	_UpdateMgr.ReadSettings ();
	// was the file with updates downloaded, need we run it ?
	if (_UpdateMgr.IsStartUpdaterNeeded ())
	{
		if (_UpdateMgr.StartUpdater ())	// running ?
			return FALSE;	// exit and update
		else
			::MessageBox (NULL, LS (L_CANTFINDUPDATER), LS (L_ERR), MB_ICONERROR);
	}

	LoadHistory ();

	_Snds.ReadSettings ();

	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
		ua.RemovePP ("Free Download Manager"); // old u-a
	}

	if (FALSE == GetProfileInt ("EnvCheck", "778", FALSE))
	{
		if (vmsBtSupport::getBtDllVersion () < 778)
			MessageBox (NULL, "Please update Bittorrent module.\nSome functions will be disabled until you update it.", "Warning", MB_ICONEXCLAMATION);
		WriteProfileInt ("EnvCheck", "778", TRUE);
	}

	CMainFrame* pFrame = NULL;
	fsnew1 (pFrame, CMainFrame);
	m_pMainWnd = pFrame;

	// creating the main window
	if (FALSE == pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		NULL, NULL))
	{
		return FALSE;
	}

	// -autorun is used to start fdm at the system startup
	BOOL bHidden = _tcscmp (m_lpCmdLine, _T ("-autorun")) == 0;

	_App.View_ReadWndPlacement (pFrame, "MainFrm", bHidden);

	if (!bHidden)
	{
		if (_App.Prg_StartMinimized ())
		{
			if (IsWindowVisible (pFrame->m_hWnd))
				pFrame->ShowWindow (SW_MINIMIZE);
		}
		else
		{
			pFrame->UpdateWindow();
			if (pFrame->IsWindowVisible ())
				pFrame->SetForegroundWindow ();
		}
	}

	m_bStarting = FALSE;

	// register fdm's COM objects factories in the system
	hRes = _Module.RegisterClassObjects (CLSCTX_LOCAL_SERVER, 
				REGCLS_MULTIPLEUSE);
	if (FAILED (hRes))
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hRes,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			// Process any inserts in lpMsgBuf.
			// ...
			// Display the string.
			MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
			// Free the buffer.
			LocalFree( lpMsgBuf );
	}
	m_bATLInited2 = SUCCEEDED (hRes);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFdmApp message handlers

////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CUrlWnd	m_wndDLURL;
	CUrlWnd	m_wndFirm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_fntUnderline;
	CFont m_fontRegInfo;
	CFont m_fontWarn;
	CUrlWnd m_wndSupport;
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_fontRegInfo.CreateFont (10, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "MS Sans Serif");
	m_fontWarn.CreateFont (12, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Arial");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//DDX_Control(pDX, IDC__LICTO, m_wndRegUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CFdmApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
   	_DlgMgr.OnDoModal (&aboutDlg);
	aboutDlg.DoModal();
    _DlgMgr.OnEndDialog (&aboutDlg);
}

/////////////////////////////////////////////////////////////////////////////
// CFdmApp message handlers


int CFdmApp::ExitInstance() 
{
	if (m_bATLInited2)
		_Module.RevokeClassObjects();

	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_UnregisterServer ();
	
	if (m_bATLInited)
		_Module.Term();

	// need to save history and ... ?
	if (m_bSaveAllOnExit)
	{
		SaveSettings ();
		SaveHistory ();
		_UpdateMgr.SaveSettings ();
	}

	/*if (m_hAppMutex)
		CloseHandle (m_hAppMutex);*/

	//ScheduleExitProcess (7);
	
	return CWinApp::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CFont* fnt = GetFont ();
	LOGFONT lf;
	fnt->GetLogFont (&lf);
	lf.lfUnderline = TRUE;
	m_fntUnderline.CreateFontIndirect (&lf);
	
	m_wndDLURL.SubclassDlgItem (IDC__DOWNLOAD, this);
	m_wndDLURL.Init ();
	m_wndDLURL.SetUrl ("http://www.freedownloadmanager.org/download.htm");
	
	CString strVer;
	char szVer [] = "%s %s build %s";
	strVer.Format (szVer, LS (L_VERSION), vmsFdmAppMgr::getVersion ()->m_strProductVersion.c_str (), 
		vmsFdmAppMgr::getBuildNumber ());
	SetDlgItemText (IDC__VER, strVer);

	//SetDlgItemText (IDC__LICTO, LS (L_NOTREGISTERED));

	/*CString str;
	str.Format ("%s 2003-2004, FreeDownloadManager.ORG\n%s", LS (L_COPYRIGHT), LS (L_ALLRIGHTS));
	SetDlgItemText (IDC__COPYRIGHT, str);*/
	//SetDlgItemText (IDC__REGTO, LS (L_LICTO));

	m_wndFirm.SubclassDlgItem (IDC__FIRM, this);
	m_wndFirm.Init ();
	m_wndFirm.SetUrl ("http://www.freedownloadmanager.org");

	SetDlgItemText (IDC__SUPPORT, LS (L_SUPPORTANDOTHER));
	m_wndSupport.SubclassDlgItem (IDC__SUPPORT, this);
	m_wndSupport.Init ();
	m_wndSupport.SetUrl ("http://www.freedownloadmanager.org/support.htm");

	SetDlgItemText (IDC__DOWNLOAD, LS (L_DLLATESTVERSION));

	SetDlgItemText (IDC__WARN, LS (L_LICWARN));

	SetWindowText (LS (L_ABOUT2));

	if (_AppMgr.IsBtInstalled () == FALSE || _AppMgr.IsMediaFeaturesInstalled () == FALSE)
	{
		CString str;
		GetDlgItemText (IDC__NAME, str);
		str += " (Light)";
		SetDlgItemText (IDC__NAME, str);
	}

	return TRUE;
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->m_hWnd == m_wndFirm.m_hWnd ||
		 pWnd->m_hWnd == m_wndDLURL.m_hWnd || 
		 pWnd->m_hWnd == m_wndSupport.m_hWnd)
	{
		pDC->SetTextColor (GetSysColor (26/*COLOR_HOTLIGHT*/));
        pDC->SelectObject (&m_fntUnderline);
	}

	if (pWnd->m_hWnd == GetDlgItem (IDC__WARN)->m_hWnd)
		pDC->SelectObject (&m_fontWarn);
		
	return hbr;
}

void CFdmApp::LoadHistory()
{
	_HsMgr.ReadSettings ();

	// history file
	HANDLE hFile = CreateFile (fsGetDataFilePath ("history.sav"), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (GetLastError () != ERROR_FILE_NOT_FOUND)
			goto _lErr;
		return;
	}

	// urls
	if (!_LastUrlFiles.ReadFromFile (hFile))
		goto _lErr;

	// web-sites
	if (!_LastUrlPaths.ReadFromFile (hFile))
		goto _lErr;

	// output directories
	if (!_LastFolders.ReadFromFile (hFile))
		goto _lErr;

	// batch downloads templates
	if (!_LastBatchUrls.ReadFromFile (hFile))
		goto _lErr;

	// history of "what to search" in find download dialog
	if (!_LastFind.ReadFromFile (hFile))
		goto _lErr;

	if (!_LastFlashVideoUrls.ReadFromFile (hFile))
		goto _lErr;

	CloseHandle (hFile);

	return;

_lErr:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	/*if (IDRETRY == MessageBox (NULL, LS (L_CANTLOADHIST), LS (L_ERR), MB_ICONERROR|MB_RETRYCANCEL))
		LoadHistory ();*/
}

void CFdmApp::SaveHistory()
{
	HANDLE hFile = CreateFile (fsGetDataFilePath ("history.sav"), GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto _lErr;
	}

	if (!_LastUrlFiles.SaveToFile (hFile))
		goto _lErr;

	if (!_LastUrlPaths.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFolders.SaveToFile (hFile))
		goto _lErr;

	if (!_LastBatchUrls.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFind.SaveToFile (hFile))
		goto _lErr;

	if (!_LastFlashVideoUrls.SaveToFile (hFile))
		goto _lErr;

	CloseHandle (hFile);

	return;

_lErr:
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	/*if (IDRETRY == MessageBox (NULL, LS (L_CANTSAVEHIST), LS (L_ERR), MB_ICONERROR|MB_RETRYCANCEL))
		SaveHistory ();
	else*/
		DeleteFile (fsGetDataFilePath ("history.sav"));
}

/* the standard class, derived from CComModule and it's standard implementation */
	
CFdmModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_WGUrlReceiver, CWGUrlReceiver)
OBJECT_ENTRY(CLSID_WGUrlListReceiver, CWgUrlListReceiver)
OBJECT_ENTRY(CLSID_FDM, CFDM)
OBJECT_ENTRY(CLSID_FDMDownloadsStat, CFDMDownloadsStat)
OBJECT_ENTRY(CLSID_FDMDownload, CFDMDownload)
OBJECT_ENTRY(CLSID_FDMUploader, CFDMUploader)
OBJECT_ENTRY(CLSID_FDMUploadPackage, CFDMUploadPackage)
OBJECT_ENTRY(CLSID_FdmTorrentFilesRcvr, CFdmTorrentFilesRcvr)
OBJECT_ENTRY(CLSID_FDMFlashVideoDownloads, CFDMFlashVideoDownloads)
END_OBJECT_MAP()

LONG CFdmModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CFdmModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CFdmModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

BOOL CFdmApp::InitATL()
{
	m_bEmbedding = FALSE;

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);

	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("Embedding"))==0)
		{
			//m_bEmbedding = TRUE;
		}

		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			BOOL bWaitShutdown = FALSE;
			if (CheckFdmStartedAlready (FALSE))
			{
				UINT nMsg = RegisterWindowMessage ("FDM - shutdown");
				if (nMsg)
				{
					PostMessage (HWND_BROADCAST, nMsg, 0, 0);
					bWaitShutdown = TRUE;
				}
				else
				{
					MessageBox (NULL, "Unable to shutdown Free Download Manager.\nPlease do that yourself.", "Error", MB_ICONEXCLAMATION);
				}
			}

			HANDLE hMxWi = CreateMutex (NULL, FALSE, "FDM - remote control server");
			if (GetLastError () == ERROR_ALREADY_EXISTS)
			{
				UINT nMsg = RegisterWindowMessage ("FDM - remote control server - shutdown");
				if (nMsg)
					PostMessage (HWND_BROADCAST, nMsg, 0, 0);
			}
			if (hMxWi)
				CloseHandle (hMxWi);

			if (bWaitShutdown)
			{
				while (CheckFdmStartedAlready (FALSE))
				{
					Sleep (400);
				}
			}

			Install_UnregisterServer ();
			bRun = FALSE;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			Install_RegisterServer ();
			bRun = FALSE;
			break;
		}

		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (bRun && CheckFdmStartedAlready (m_bForceSilentSpecified == FALSE))
		bRun = FALSE;

	if (!bRun)
		return FALSE;

	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init (ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId ();
	}

	return TRUE;
}

BOOL CFdmApp::InitLanguage()
{
	m_nNoLngsErrReason = 0;
	
	// init localization manager
	if (FALSE == _LngMgr.Initialize ())
	{
		m_nNoLngsErrReason = 1;
		return FALSE;
	}

	// selected language
	int iLang = _LngMgr.FindLngByName (_App.View_Language ());
	if (iLang == -1) 
		iLang = 0;

	// loading a language file ...

	if (FALSE == _LngMgr.LoadLng (iLang))
	{
		// ... error, trying to load the first file
		if (iLang == 0 || FALSE == _LngMgr.LoadLng (0))
		{
			m_nNoLngsErrReason = 2;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFdmApp::CheckFdmStartedAlready(BOOL bSetForIfEx)
{
	LPCSTR pszMainWnd = "Free Download Manager Main Window";

	m_hAppMutex = CreateMutex (NULL, TRUE, _pszAppMutex);
	
	// application is already running ?
	if (GetLastError () == ERROR_ALREADY_EXISTS)
	{
		CloseHandle (m_hAppMutex);
		m_hAppMutex = NULL;

		if (bSetForIfEx)
		{
			// activating the main window

			HWND hWnd = FindWindow (pszMainWnd, NULL);

			if (IsIconic (hWnd))
				ShowWindow (hWnd, SW_RESTORE);
			else
			{
				WINDOWPLACEMENT wc;
				GetWindowPlacement (hWnd, &wc);
				if (wc.showCmd == SW_HIDE)
					ShowWindow (hWnd, SW_RESTORE);
			}

			SetForegroundWindow (hWnd);
			SetFocus (hWnd);
		}
		
		return TRUE;
	}

	return FALSE;
}


CFdmApp::~CFdmApp()
{
	if (m_bCOMInited)
		CoUninitialize();
}

LONG CFdmApp::_UEF(_EXCEPTION_POINTERS *info)
{
	static BOOL _b = FALSE;

	if (_b)
		return EXCEPTION_EXECUTE_HANDLER;

	_b = TRUE;

	typedef BOOL (WINAPI *FNMDWD)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, 
		PMINIDUMP_EXCEPTION_INFORMATION, PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
	vmsDLL dll ("dbghelp.dll");
	FNMDWD pfnMiniDumpWriteDump;
	pfnMiniDumpWriteDump = (FNMDWD) dll.GetProcAddress ("MiniDumpWriteDump");
	if (pfnMiniDumpWriteDump == NULL)
		return EXCEPTION_EXECUTE_HANDLER;
	
	/*CMainFrame* pFrame = (CMainFrame*) AfxGetApp ()->m_pMainWnd;
	if (pFrame)
		pFrame->OnSaveall ();*/
	
	MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = info;
    eInfo.ClientPointers = FALSE;

	char szFile [MAX_PATH]; char szName [100];
	wsprintf (szName, "fdm%s.dmp", vmsFdmAppMgr::getBuildNumber ());
	GetTempPath (MAX_PATH, szFile);
	lstrcat (szFile, szName);
	HANDLE hFile = CreateFile (szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		0, NULL);

    // note:  MiniDumpWithIndirectlyReferencedMemory does not work on Win98
    pfnMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpNormal,
        &eInfo,
        NULL,
        NULL);

	CloseHandle (hFile);

	CUEDlg dlg;
	dlg.DoModal ();

	return EXCEPTION_EXECUTE_HANDLER;
}

#include "FDM.h"
#include "FDMDownloadsStat.h"
#include "FDMDownload.h"
#include "FDMUploader.h"
#include "FDMUploadPackage.h"

BOOL CFdmApp::Is_Starting()
{
	return m_bStarting;
}

void CFdmApp::CheckLocked()
{
	DWORD dwRes;

	do 
	{
		HANDLE hMx = CreateMutex (NULL, TRUE, "_mx_FDM_Lock_Start_");
		dwRes = GetLastError ();
		CloseHandle (hMx);

		if (dwRes == ERROR_ALREADY_EXISTS)
			Sleep (100);
	}
	while (dwRes == ERROR_ALREADY_EXISTS);
}

void CFdmApp::UninstallCustomizations()
{
	CRegKey key;
	key.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager");

	char sz [MY_MAX_PATH];
	DWORD dw = MY_MAX_PATH;
	
	// link to site in start menu
	key.QueryValue (sz, "CLSM_File", &dw);
	DeleteFile (sz);

	// link to site in favorites
	dw = MY_MAX_PATH;
	key.QueryValue (sz, "CLFM_File", &dw);
	DeleteFile (sz);

	/*CString str = m_strAppPath;
	str += "fdmcsiebtn.dll";

	typedef HRESULT (_stdcall *fntDllRegUnregServer)(void);
	HMODULE hLib = LoadLibrary (str);
	if (hLib)
	{
		fntDllRegUnregServer pfnDll;
		pfnDll = (fntDllRegUnregServer) GetProcAddress (hLib, "DllUnregisterServer");

		if (pfnDll)
			pfnDll ();

		FreeLibrary (hLib);
	}

	DeleteFile (str);

	str = m_strAppPath; str += "fdmcsiebtn.ico";
	DeleteFile (str);

	str = m_strAppPath; str += "fdmcs.ico";
	DeleteFile (str);*/

	/**/

	// restore Internet Explorer home page

	char szIEOP [10000] = "about:blank";
	char szIECP [10000] = "about:blank";

	CRegKey fdmkey;
	fdmkey.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager");

	dw = sizeof (szIECP);
	fdmkey.QueryValue (szIECP, "CustSite", &dw);
	dw = sizeof (szIEOP);
	if (ERROR_SUCCESS == fdmkey.QueryValue (szIEOP, "CIEOP", &dw))
	{
		key.Open (HKEY_CURRENT_USER, "Software\\Microsoft\\Internet Explorer\\Main");

		char sz2 [10000] = "about:blank";
		dw = sizeof (sz2);
		key.QueryValue (sz2, "Start Page", &dw);

		if (lstrcmp (sz2, szIECP) == 0)
			key.SetValue (szIEOP, "Start Page");
	}
}

BOOL CFdmApp::RegisterServer(BOOL bGlobal)
{
	LOG ("Registering server (%s)...", bGlobal ? "global" : "local");

	bool bRegisterForUserOnly = IS_PORTABLE_MODE && !Is9xME;

	if (bRegisterForUserOnly)
	{
		LOGsnl ("Portable mode: overriding HKCR...");
		HKEY hKey;
		LONG lRes;
		lRes = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\Classes", 0, KEY_ALL_ACCESS, &hKey);
		LOGRESULT (" open cu key", lRes);
		lRes = vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, hKey);
		LOGRESULT (" override HKCR key", lRes);
		RegCloseKey (hKey);
	}

	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
        ua.RemovePP ("Free Download Manager"); // old user agent
	}

	if (bGlobal)
	{
		CRegKey key;
		// if fdm was installed by admin, for example, and now some user
		// tries to start fdm, we should copy all info from local machine key
		// to key of current user in order to get fdm registered properly
		if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, "Software\\FreeDownloadManager.ORG\\Free Download Manager", KEY_WRITE))
			vmsSHCopyKey (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager", key);

		// register COM objects
		HRESULT hr = _Module.UpdateRegistryFromResource (IDR_FDM, TRUE);
		LOGRESULT ("UpdateRegistryFromResource", hr);
	
		hr = _Module.RegisterServer (!bRegisterForUserOnly);
		LOGRESULT ("_Module.RegisterServer", hr);

		if (bRegisterForUserOnly)
		{
			CComBSTR bstrPath;
			CComPtr<ITypeLib> pTypeLib;
			hr = AtlModuleLoadTypeLib (&_Module, NULL, &bstrPath, &pTypeLib);
			if (SUCCEEDED(hr))
			{
				OLECHAR szDir[_MAX_PATH];
				ocscpy(szDir, bstrPath);
				szDir[AtlGetDirLen(szDir)] = 0;
			
				typedef HRESULT (WINAPI *FNRTLFU)(ITypeLib*,OLECHAR*,OLECHAR*);
				FNRTLFU pfn = (FNRTLFU) GetProcAddress (GetModuleHandle ("oleaut32.dll"), "RegisterTypeLibForUser");
				if (pfn)
					hr = pfn (pTypeLib, bstrPath, szDir);
				else
					hr = ::RegisterTypeLib(pTypeLib, bstrPath, szDir);
			}
			LOGRESULT ("register typelib", hr);
		}
		
		// register type lib
		ITypeLib *pLib = NULL;
		/*if (SUCCEEDED (hr = LoadTypeLibEx (L"fuminterfaces.tlb", REGKIND_REGISTER, &pLib)))
			pLib->Release ();
		LOGRESULT ("register fuminterfaces.tlb", hr);*/
		if (SUCCEEDED (hr = LoadTypeLibEx (L"fdm.tlb", REGKIND_REGISTER, &pLib)))
			pLib->Release ();
		else
			MessageBox (0, "Failed to load Free Download Manager type lib","Error",0);
		LOGRESULT ("register fdm.tlb", hr);

		if (_AppMgr.IsBtInstalled ())
		{
			// register .torrent extension to be opened with us
			if (vmsTorrentExtension::IsAssociatedWithUs () == FALSE)
			{
				if (_App.Bittorrent_Enable () || vmsTorrentExtension::IsAssociationExist () == FALSE)
				{
					_App.Bittorrent_OldTorrentAssociation (vmsTorrentExtension::GetCurrentAssociation ());
					vmsTorrentExtension::Associate ();
				}
			}
		}
	}

	WriteProfileInt ("", "cverID", 0);

	if (GetProfileInt ("Settings\\Monitor", "IECMInited", 0) == 0)
		WriteProfileInt ("Settings\\Monitor", "IECMInited", TRUE);
	_IECMM.DeleteIEMenus (); // remove menus of builds < 460 
	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	_IECMM.AddIEMenus ();

	// get list of browser for which user enabled monitoring
	DWORD dwMUSO = _App.Monitor_UserSwitchedOn ();
				
	_IECatchMgr.ActivateIE2 ((dwMUSO & MONITOR_USERSWITCHEDON_IE) != 0);

	vmsFirefoxMonitoring::Install (true);
	if (vmsFirefoxMonitoring::IsInstalled () == false)
		_App.Monitor_Firefox (FALSE);
	else
		_App.Monitor_Firefox ((dwMUSO & MONITOR_USERSWITCHEDON_FIREFOX) != 0);
	// remove old-method integration (versions of fdm < 2.1)
	_NOMgr.DeinstallFirefoxPlugin ();

	if (dwMUSO & MONITOR_USERSWITCHEDON_NETSCAPE)
		_NOMgr.InstallNetscapePlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_OPERA)
		_NOMgr.InstallOperaPlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_SEAMONKEY)
		_NOMgr.InstallMozillaSuitePlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_SAFARI)
		_NOMgr.InstallSafariPlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_CHROME)
		_NOMgr.InstallChromePlugin ();

	CRegKey key;
	if (ERROR_SUCCESS != key.Create (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Upload Manager"))
		key.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Upload Manager");
	key.SetValue (fsGetAppDataFolder (), "force_data_folder");

	if (bRegisterForUserOnly)
		vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, NULL);

	return TRUE;
}

void CFdmApp::Install_RegisterServer()
{
	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
	RegisterServer (TRUE);
}

void CFdmApp::Install_UnregisterServer()
{
	LOGsnl ("Unregister server...");

	bool bUnregisterForUserOnly = IS_PORTABLE_MODE;
	
	if (bUnregisterForUserOnly)
	{
		HKEY hKeyCurrentUser;
		LONG l = RegOpenKeyEx (HKEY_CURRENT_USER, _T("Software\\Classes"), 0, KEY_ALL_ACCESS, &hKeyCurrentUser);
		LOGRESULT ("open cu", l);
		l = vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, hKeyCurrentUser);
		LOGRESULT ("override key", l);
		RegCloseKey (hKeyCurrentUser);
	}

	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
			
	// unregister COM objects and type lib
	HRESULT hr = _Module.UpdateRegistryFromResource(IDR_FDM, FALSE);
	LOGRESULT ("_Module.UpdateRegistryFromResource", hr);
	hr = _Module.UnregisterServer(!bUnregisterForUserOnly); //TRUE means typelib is unreg'd
	LOGRESULT ("_Module.UnregisterServer", hr);
	UnRegisterTypeLib (LIBID_FdmLib, 0, 0, LOCALE_SYSTEM_DEFAULT, SYS_WIN32);

	if (bUnregisterForUserOnly)
	{
		typedef HRESULT (WINAPI *PFNRTL)(REFGUID, WORD, WORD, LCID, SYSKIND);
		CComBSTR bstrPath;
		CComPtr<ITypeLib> pTypeLib;
		hr = AtlModuleLoadTypeLib(&_Module, NULL, &bstrPath, &pTypeLib);
		if (SUCCEEDED(hr))
		{
			TLIBATTR* ptla;
			HRESULT hr = pTypeLib->GetLibAttr(&ptla);
			if (SUCCEEDED(hr))
			{
				HINSTANCE h = LoadLibrary(_T("oleaut32.dll"));
				if (h != NULL)
				{
					PFNRTL pfn = (PFNRTL) GetProcAddress(h, "UnRegisterTypeLibForUser");
					if (pfn == NULL)
						pfn = (PFNRTL) GetProcAddress(h, "UnRegisterTypeLib");
					if (pfn != NULL)
						hr = pfn(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
					FreeLibrary (h);
				}
				pTypeLib->ReleaseTLibAttr(ptla);
			}
		}
		LOGRESULT ("unregister typelib", hr);
	}
	
	// if the menu is present, then we need to show it again while installation,
	// point on it
	if (_IECMM.IsIEMenusPresent ())
		WriteProfileInt ("Settings\\Monitor", "IECMInited", 0);

	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	//_IECMM.DeleteIEMenus ();
	_NOMgr.DeinstallNetscapePlugin ();
	_NOMgr.DeinstallOperaPlugin ();
    _NOMgr.DeinstallMozillaSuitePlugin ();
	_IECatchMgr.ActivateIE2 (FALSE);
	fsIEUserAgent ua;
	ua.RemovePP (IE_USERAGENT_ADDITION);
	UninstallCustomizations ();
	vmsFirefoxMonitoring::Install (false);

	// restore old association with .torrent extension
	if (vmsTorrentExtension::IsAssociatedWithUs ())
		vmsTorrentExtension::AssociateWith (_App.Bittorrent_OldTorrentAssociation ());

	if (bUnregisterForUserOnly)
		vmsNotEverywhereSupportedFunctions::RegOverridePredefKey (HKEY_CLASSES_ROOT, NULL);
}

void CFdmApp::SaveSettings()
{
	if (IS_PORTABLE_MODE)
	{
		// flush settings to file
		vmsAppSettingsStore* pStgs = _App.get_SettingsStore ();
		CString strStgsFile = fsGetDataFilePath ("settings.dat");
		pStgs->SaveSettingsToFile (strStgsFile);
	}
}


void CFdmApp::IntegrationSettings()
{
	vmsUploadsDllCaller udc;
	HMODULE hUploadsDll;

	#ifndef _DEBUG
	CString strFP = fsGetFumProgramFilesFolder ();
	hUploadsDll = LoadLibrary (strFP + "fumcore.dll");
	#else
	hUploadsDll = LoadLibrary ("E:\\VCW\\FDM\\FDM\\Uploader\\CoreDll\\Debug\\fumcore.dll");
	ASSERT (hUploadsDll != NULL);
	#endif

	if (hUploadsDll == NULL)
		return;

	vmsUploadsDll::FNSDC pfnSetCaller = (vmsUploadsDll::FNSDC) GetProcAddress (hUploadsDll, 
		"_SetDllCaller");
	ASSERT (pfnSetCaller != NULL);
	pfnSetCaller (&udc);

	if (lstrcmpi (m_lpCmdLine, "-suis") == 0)
	{
		vmsUploadsDll::FNSIS pfnShowIntegrationSettings = (vmsUploadsDll::FNSIS) GetProcAddress (hUploadsDll, 
			"_ShowIntegrationSettings");
		ASSERT (pfnShowIntegrationSettings != NULL);
		pfnShowIntegrationSettings (NULL);
	}
	else
	{
		vmsUploadsDll::FNEI pfnEnableIntegration = (vmsUploadsDll::FNEI) GetProcAddress (hUploadsDll, 
			"_EnableIntegration");			 
		ASSERT (pfnEnableIntegration != NULL);
		pfnEnableIntegration  (lstrcmpi (m_lpCmdLine, "-euis") == 0, 0);
	}

	FreeLibrary (hUploadsDll);
}

DWORD WINAPI CFdmApp::_threadExitProcess(LPVOID lp)
{
	Sleep (((DWORD)lp) * 1000);
	ASSERT (FALSE);
	HANDLE hProcess = OpenProcess (PROCESS_TERMINATE, FALSE, GetCurrentProcessId ());
	TerminateProcess (hProcess, (DWORD)-1);
	return 0;
}

void CFdmApp::ScheduleExitProcess(DWORD dwSeconds)
{
	DWORD dw;
	CloseHandle (
		::CreateThread (NULL, 0, _threadExitProcess, (LPVOID)dwSeconds, 0, &dw));
}

void CFdmApp::CheckRegistry()
{
	fsString str = "%56%69%63%4D%61%6E%20%53%6F%66%74%77%61%72%65";
	fsDecodeHtmlUrl (str);
	CString strOldKey = "Software\\"; strOldKey += str; 
	CString strOldRKey = strOldKey;
	strOldKey += "\\Free Download Manager";

	// move registry settings from old key to FreeDownloadManager.ORG key.
	CRegKey key;
	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, strOldKey))
	{
		CRegKey key2;
		if (ERROR_SUCCESS != key2.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\History"))
		{
			LONG lRes = key2.Create (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG");
			if (lRes != ERROR_SUCCESS)
				lRes = key2.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG");

			if (ERROR_SUCCESS == lRes)
			{
				key.Open (HKEY_CURRENT_USER, strOldRKey);

				CString strPath = GetProfileString ("", "Path", "");
				LONG nRes = fsCopyKey (key, key2, "Free Download Manager", "Free Download Manager");
				nRes; //disable warning
				WriteProfileString ("", "Path", strPath); // keep Path value unchanged
				
				key.RecurseDeleteKey ("Free Download Manager");
			}
		}
	}
}

AFX_MODULE_STATE* CFdmApp::GetModuleState()
{
	return m_pModuleState;
}
