/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"

#include "Data Stretcher.h"
#include "dbghelp.h"

#include "MainFrm.h"
#include "UrlWnd.h"
#include <initguid.h>
#include "WGUrlReceiver.h"
#include "DataStretcher_i.c"
#include "UEDlg.h"
#include "fsIEUserAgent.h"
#include "WgUrlListReceiver.h"
#include "FDM.h"
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif      

BEGIN_MESSAGE_MAP(CDataStretcherApp, CWinApp)
	//{{AFX_MSG_MAP(CDataStretcherApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

CDataStretcherApp::CDataStretcherApp()
{
	m_bCOMInited = m_bATLInited = m_bATLInited2 = FALSE;
	m_bSaveAllOnExit = FALSE;
	m_hAppMutex = NULL;
	m_bEmbedding = FALSE;
	m_bStarting = TRUE;

	SYSTEMTIME time;
	GetLocalTime (&time);
	SystemTimeToFileTime (&time, &_timeAppHasStarted);
}      

CDataStretcherApp theApp;    

BOOL CDataStretcherApp::InitInstance()
{
	SetUnhandledExceptionFilter (_UEF);

	AfxEnableControlContainer ();

	
	SetRegistryKey (IDS_COMPANY);

	#ifdef _USELOGGING
	char szL [MAX_PATH];
	GetModuleFileName (NULL, szL, MAX_PATH);
	CString strLogFile = szL;
	fsGetPath (strLogFile, szL);
	strLogFile = szL;
	if (strLogFile.Right (1) != "\\")
		strLogFile += "\\";
	strLogFile += "fdm.log";
	_Log.Initialize (strLogFile);
	_Log.EraseLog ();
	#endif

	LOG ("========================================================" << nl);
	LOG ("starting..." << nl);
	LOG ("initializing..." << nl);

	CheckRegistry ();

	m_bSaveAllOnExit = FALSE;

	fsIECatchMgr::CleanIEPluginKey ();

	
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
		
		
		
		if (m_strAppPath.CompareNoCase (szExeDir) &&
			 DWORD (-1) != GetFileAttributes (strDataFldr))
		{
			
			_dwAppState |= APPSTATE_PORTABLE_MODE;
			_dwAppState |= APPSTATE_PORTABLE_MODE_NOREG;
			m_strAppPath = szExeDir;
		}
	}

	if (IS_PORTABLE_MODE)
	{
		
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
		
		
		
		char szTmpFile [MY_MAX_PATH];
		CString str = m_strAppPath; str += "Data";
		CreateDirectory (str, NULL);
		if (0 == GetTempFileName (str, "fdm", 0, szTmpFile))
			MessageBox (NULL, LS (L_NOWRITEACCESSTODATAFOLDER), PRG_NAME, MB_ICONWARNING);
		else
			DeleteFile (szTmpFile);
	}

	_SkinMgr.Initialize ();

	_IECatchMgr.ReadSettings ();
	_NOMgr.Initialize ();
	_IECMM.ReadState ();

	
	HRESULT hRes = OleInitialize (NULL);
	
	if (FAILED(hRes))
		return FALSE;
	
	m_bCOMInited = TRUE;

	fsFDMCmdLineParser cmdline;
	cmdline.Parse ();
	m_bForceSilentSpecified = cmdline.is_ForceSilentSpecified ();

	
	if (GetVersion () & 0x80000000)
	{
		
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

	
	
	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_RegisterServer ();

	
	
	
	
	vmsFilesToDelete::Process ();

	if (bNeedLocalRegister)
		RegisterServer (FALSE);

	

#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif

	CheckLocked ();

	m_bSaveAllOnExit = TRUE;

	_UpdateMgr.ReadSettings ();
	
	if (_UpdateMgr.IsStartUpdaterNeeded ())
	{
		if (_UpdateMgr.StartUpdater ())	
			return FALSE;	
		else
			::MessageBox (NULL, LS (L_CANTFINDUPDATER), LS (L_ERR), MB_ICONERROR);
	}

	LoadHistory ();

	_Snds.ReadSettings ();

	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
		ua.RemovePP ("Free Download Manager"); 
	}

	CMainFrame* pFrame = NULL;
	fsnew1 (pFrame, CMainFrame);
	m_pMainWnd = pFrame;

	LOG ("creating main window and client area..." << nl);

	
	if (FALSE == pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		NULL, NULL))
	{
		return FALSE;
	}

	LOG ("main window and client area was created" << nl);

	_App.View_ReadWndPlacement (pFrame, "MainFrm");

	
	if (_tcscmp (m_lpCmdLine, _T ("-autorun")) == 0)
		pFrame->ShowWindow (SW_HIDE);	
	else if (_App.Prg_StartMinimized ())
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

	m_bStarting = FALSE;

	
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
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
				);
			
			
			
			MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
			
			LocalFree( lpMsgBuf );
	}
	m_bATLInited2 = SUCCEEDED (hRes);

	return TRUE;
}            

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();  

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CUrlWnd	m_wndDLURL;
	CUrlWnd	m_wndFirm;
	//}}AFX_DATA

	
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL  

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
	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()  

void CDataStretcherApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
   	_DlgMgr.OnDoModal (&aboutDlg);
	aboutDlg.DoModal();
    _DlgMgr.OnEndDialog (&aboutDlg);
}        

int CDataStretcherApp::ExitInstance() 
{
	LOG ("shutting down..." << nl);

	if (m_bATLInited2)
	{
		LOG ("deinitializing ATL...");
		_Module.RevokeClassObjects();
	}

	if (IS_PORTABLE_MODE && (_dwAppState & APPSTATE_PORTABLE_MODE_NOREG) == 0)
		Install_UnregisterServer ();
	
	if (m_bATLInited)
	{
		_Module.Term();
		LOG ("done." << nl);
	}

	
	if (m_bSaveAllOnExit)
	{
		SaveSettings ();
		SaveHistory ();
		_UpdateMgr.SaveSettings ();
	}

	

	LOG ("exit now" << nl);
	LOG ("========================================================" << nl);

	
	
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
	char szVer [] = "%s %s build %d";
	strVer.Format (szVer, LS (L_VERSION), PRG_VERSION, PRG_BUILD_NUMBER);
	SetDlgItemText (IDC__VER, strVer);

	

	
	

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
		pDC->SetTextColor (GetSysColor (26));
        pDC->SelectObject (&m_fntUnderline);
	}

	if (pWnd->m_hWnd == GetDlgItem (IDC__WARN)->m_hWnd)
		pDC->SelectObject (&m_fontWarn);
		
	return hbr;
}

void CDataStretcherApp::LoadHistory()
{
	LOG ("loading history file...");

	_HsMgr.ReadSettings ();

	
	HANDLE hFile = CreateFile (fsGetDataFilePath ("history.sav"), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (GetLastError () != ERROR_FILE_NOT_FOUND)
			goto _lErr;
		LOG ("done (no history file found)" << nl);
		return;
	}

	
	if (!_LastUrlFiles.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastUrlPaths.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastFolders.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastBatchUrls.ReadFromFile (hFile))
		goto _lErr;

	
	if (!_LastFind.ReadFromFile (hFile))
		goto _lErr;

	if (!_LastFlashVideoUrls.ReadFromFile (hFile))
		goto _lErr;

	CloseHandle (hFile);

	LOG ("ok." << nl);

	return;

_lErr:
	LOG ("failed (error #" << GetLastError () << ")" << nl);
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	
}

void CDataStretcherApp::SaveHistory()
{
	HANDLE hFile = CreateFile (fsGetDataFilePath ("history.sav"), GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	LOG ("saving history...");

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

	LOG ("ok." << nl);

	return;

_lErr:
	LOG ("failed (error #" << GetLastError () << ")" << nl);

	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle (hFile);
	
		DeleteFile (fsGetDataFilePath ("history.sav"));
}  

	
CDataStretcherModule _Module;

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

LONG CDataStretcherModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CDataStretcherModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CDataStretcherModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
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

BOOL CDataStretcherApp::InitATL()
{
	m_bEmbedding = FALSE;

	LPTSTR lpCmdLine = GetCommandLine(); 
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = TRUE;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);

	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("Embedding"))==0)
		{
			
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

BOOL CDataStretcherApp::InitLanguage()
{
	LOG ("initializing language..." << nl);

	m_nNoLngsErrReason = 0;
	
	
	if (FALSE == _LngMgr.Initialize ())
	{
		LOG ("failed : no files found" << nl);
		m_nNoLngsErrReason = 1;
		return FALSE;
	}

	LOG ("cDSA::IL: cl: " << _App.View_Language () << nl);

	
	int iLang = _LngMgr.FindLngByName (_App.View_Language ());
	if (iLang == -1) 
	{
		LOG ("cDSA::IL: cl: not found" << nl);
		iLang = 0;
	}
	

	if (FALSE == _LngMgr.LoadLng (iLang))
	{
		LOG ("cDSA::IL: cl: failed to load" << nl);
		
		if (iLang == 0 || FALSE == _LngMgr.LoadLng (0))
		{
			m_nNoLngsErrReason = 2;
			LOG ("failed : no valid lang files found" << nl);
			return FALSE;
		}
	}

	LOG ("ok." << nl);
	return TRUE;
}

BOOL CDataStretcherApp::CheckFdmStartedAlready(BOOL bSetForIfEx)
{
	LPCSTR pszMainWnd = "Free Download Manager Main Window";

	LOG ("checking FDM started already...");

	m_hAppMutex = CreateMutex (NULL, TRUE, _pszAppMutex);
	
	
	if (GetLastError () == ERROR_ALREADY_EXISTS)
	{
		CloseHandle (m_hAppMutex);
		m_hAppMutex = NULL;

		LOG ("FDM detected" << nl);

		if (bSetForIfEx)
		{
			

			HWND hWnd = FindWindow (pszMainWnd, NULL);

			LOG ("FDM window: " << (DWORD)hWnd);

			if (IsIconic (hWnd))
			{
				LOG (" is iconic" << nl);
				ShowWindow (hWnd, SW_RESTORE);
			}
			else
			{
				LOG (" is not iconic" << nl);
				WINDOWPLACEMENT wc;
				GetWindowPlacement (hWnd, &wc);
				LOG ("#cp1" << nl);
				if (wc.showCmd == SW_HIDE)
					ShowWindow (hWnd, SW_RESTORE);
			}

			LOG ("#cp2" << nl);
			SetForegroundWindow (hWnd);
			LOG ("#cp3" << nl);
			SetFocus (hWnd);
			LOG ("#cp4" << nl);
		}
		
		return TRUE;
	}

	LOG ("ok." << nl);

	return FALSE;
}  

CDataStretcherApp::~CDataStretcherApp()
{
	if (m_bCOMInited)
		CoUninitialize();
}

LONG CDataStretcherApp::_UEF(_EXCEPTION_POINTERS *info)
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

	LOG ("Unhandled Exception was raised !!!" << nl);
	
	
	
	MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = info;
    eInfo.ClientPointers = FALSE;

	LOG ("address = " << DWORD (info->ExceptionRecord->ExceptionAddress) << nl);
	LOG ("creating dump file...");

	char szFile [MAX_PATH]; char szName [100];
	wsprintf (szName, "fdm%d.dmp", PRG_BUILD_NUMBER);
	GetTempPath (MAX_PATH, szFile);
	lstrcat (szFile, szName);
	HANDLE hFile = CreateFile (szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		0, NULL);

    
    pfnMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpNormal,
        &eInfo,
        NULL,
        NULL);

	CloseHandle (hFile);

	LOG ("done." << nl);
	LOG ("========================================================" << nl);

	CUEDlg dlg;
	dlg.DoModal ();

	return EXCEPTION_EXECUTE_HANDLER;
}
#include "FDM.h"
#include "FDMDownloadsStat.h"
#include "FDMDownload.h"
#include "FDMUploader.h"
#include "FDMUploadPackage.h"

BOOL CDataStretcherApp::Is_Starting()
{
	return m_bStarting;
}

void CDataStretcherApp::CheckLocked()
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

void CDataStretcherApp::UninstallCustomizations()
{
	CRegKey key;
	key.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager");

	char sz [MY_MAX_PATH];
	DWORD dw = MY_MAX_PATH;
	
	
	key.QueryValue (sz, "CLSM_File", &dw);
	DeleteFile (sz);

	
	dw = MY_MAX_PATH;
	key.QueryValue (sz, "CLFM_File", &dw);
	DeleteFile (sz);

	

	

	

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

BOOL CDataStretcherApp::RegisterServer(BOOL bGlobal)
{
	if (_App.ModifyIEUserAgent ())
	{
		fsIEUserAgent ua;
		ua.SetPP (IE_USERAGENT_ADDITION);
        ua.RemovePP ("Free Download Manager"); 
	}

	if (bGlobal)
	{
		CRegKey key;
		
		
		
		if (ERROR_SUCCESS == key.Open (HKEY_LOCAL_MACHINE, "Software\\FreeDownloadManager.ORG\\Free Download Manager", KEY_WRITE))
			vmsSHCopyKey (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager", key);

		
		_Module.UpdateRegistryFromResource(IDR_DATASTRETCHER, TRUE);
		_Module.RegisterServer(TRUE);
		
		
		ITypeLib *pLib = NULL;
		if (SUCCEEDED (LoadTypeLibEx (L"fuminterfaces.tlb", REGKIND_REGISTER, &pLib)))
			pLib->Release ();
		if (SUCCEEDED (LoadTypeLibEx (L"fdm.tlb", REGKIND_REGISTER, &pLib)))
			pLib->Release ();
		else
			MessageBox (0, "Failed to load Free Download Manager type lib","Error",0);

		if (_AppMgr.IsBtInstalled ())
		{
			
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
	_IECMM.DeleteIEMenus (); 
	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	_IECMM.AddIEMenus ();

	
	DWORD dwMUSO = _App.Monitor_UserSwitchedOn ();
				
	_IECatchMgr.ActivateIE2 ((dwMUSO & MONITOR_USERSWITCHEDON_IE) != 0);

	vmsFirefoxMonitoring::Install (true);
	if (vmsFirefoxMonitoring::IsInstalled () == false)
		_App.Monitor_Firefox (FALSE);
	else
		_App.Monitor_Firefox ((dwMUSO & MONITOR_USERSWITCHEDON_FIREFOX) != 0);
	
	_NOMgr.DeinstallFirefoxPlugin ();

	if (dwMUSO & MONITOR_USERSWITCHEDON_NETSCAPE)
		_NOMgr.InstallNetscapePlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_OPERA)
		_NOMgr.InstallOperaPlugin ();
	if (dwMUSO & MONITOR_USERSWITCHEDON_SEAMONKEY)
		_NOMgr.InstallMozillaSuitePlugin ();

	CRegKey key;
	if (ERROR_SUCCESS != key.Create (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Upload Manager"))
		key.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Upload Manager");
	key.SetValue (fsGetAppDataFolder (), "force_data_folder");

	return TRUE;
}

void CDataStretcherApp::Install_RegisterServer()
{
	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
	RegisterServer (TRUE);
}

void CDataStretcherApp::Install_UnregisterServer()
{
	if (m_bATLInited == FALSE)
	{
		m_bATLInited = TRUE;
		_Module.Init(ObjectMap, AfxGetInstanceHandle());
		_Module.dwThreadID = GetCurrentThreadId();
	}
			
	
	_Module.UpdateRegistryFromResource(IDR_DATASTRETCHER, FALSE);
	_Module.UnregisterServer(TRUE); 
	UnRegisterTypeLib (LIBID_DataStretcherLib, 0, 0, LOCALE_SYSTEM_DEFAULT, SYS_WIN32);
	
	
	
	if (_IECMM.IsIEMenusPresent ())
		WriteProfileInt ("Settings\\Monitor", "IECMInited", 0);

	fsIEContextMenuMgr::DeleteAllFDMsIEMenus ();
	
	_NOMgr.DeinstallNetscapePlugin ();
	_NOMgr.DeinstallOperaPlugin ();
    _NOMgr.DeinstallMozillaSuitePlugin ();
	_IECatchMgr.ActivateIE2 (FALSE);
	fsIEUserAgent ua;
	ua.RemovePP (IE_USERAGENT_ADDITION);
	UninstallCustomizations ();
	vmsFirefoxMonitoring::Install (false);

	
	if (vmsTorrentExtension::IsAssociatedWithUs ())
		vmsTorrentExtension::AssociateWith (_App.Bittorrent_OldTorrentAssociation ());
}

void CDataStretcherApp::SaveSettings()
{
	if (IS_PORTABLE_MODE)
	{
		
		vmsAppSettingsStore* pStgs = _App.get_SettingsStore ();
		CString strStgsFile = fsGetDataFilePath ("settings.dat");
		pStgs->SaveSettingsToFile (strStgsFile);
	}
}  

void CDataStretcherApp::IntegrationSettings()
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

DWORD WINAPI CDataStretcherApp::_threadExitProcess(LPVOID lp)
{
	LOG ("DSA::tEP:sleep " << (DWORD)lp << " seconds" << nl);
	Sleep (((DWORD)lp) * 1000);
	ASSERT (FALSE);
	LOG ("DSA::tEP:terminating" << nl);
	HANDLE hProcess = OpenProcess (PROCESS_TERMINATE, FALSE, GetCurrentProcessId ());
	TerminateProcess (hProcess, (DWORD)-1);
    LOG ("DSA::tEP:unexpected error " << GetLastError () << nl);
	return 0;
}

void CDataStretcherApp::ScheduleExitProcess(DWORD dwSeconds)
{
	DWORD dw;
	CloseHandle (
		::CreateThread (NULL, 0, _threadExitProcess, (LPVOID)dwSeconds, 0, &dw));
}

void CDataStretcherApp::CheckRegistry()
{
	fsString str = "%56%69%63%4D%61%6E%20%53%6F%66%74%77%61%72%65";
	fsDecodeHtmlUrl (str);
	CString strOldKey = "Software\\"; strOldKey += str; 
	CString strOldRKey = strOldKey;
	strOldKey += "\\Free Download Manager";

	
	CRegKey key;
	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, strOldKey))
	{
		LOG ("old key detected." << nl); 

		CRegKey key2;
		if (ERROR_SUCCESS != key2.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\View"))
		{
			LOG ("importing old key" << nl);

			if (ERROR_SUCCESS == key2.Create (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG"))
			{
				key.Open (HKEY_CURRENT_USER, strOldRKey);

				CString strPath = GetProfileString ("", "Path", "");
				LONG nRes = fsCopyKey (key, key2, "Free Download Manager", "Free Download Manager");
				nRes; 
				WriteProfileString ("", "Path", strPath); 
				
				key.RecurseDeleteKey ("Free Download Manager");

				LOG ("finished importing old key. result = " << nRes << nl);
			}
			else
			{
				LOG ("failed to create a new key" << nl);
			}
		}
		else
		{
			LOG ("importing of old key cancelled." << nl);
		}
	}
}
