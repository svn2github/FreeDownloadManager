/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "fdm.h"
#include "Dlg_Options_Downloads_Monitoring.h"
#include "vmsFirefoxMonitoring.h"
#include "DlgCustomizeIEMenu.h"
#include "DownloaderProperties_MonitorPage_SkipList.h"
#include "FolderBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif        

CDlg_Options_Downloads_Monitoring::CDlg_Options_Downloads_Monitoring(CWnd* pParent )
	: CDlg_Options_Page(CDlg_Options_Downloads_Monitoring::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Options_Downloads_Monitoring)
		
	//}}AFX_DATA_INIT
}  

void CDlg_Options_Downloads_Monitoring::DoDataExchange(CDataExchange* pDX)
{
	CDlg_Options_Page::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Options_Downloads_Monitoring)
		
	//}}AFX_DATA_MAP
}  

BEGIN_MESSAGE_MAP(CDlg_Options_Downloads_Monitoring, CDlg_Options_Page)
	//{{AFX_MSG_MAP(CDlg_Options_Downloads_Monitoring)
	ON_BN_CLICKED(IDC_CUSTOMIZE, OnCustomize)
	ON_BN_CLICKED(IDC_SKIPLIST, OnSkiplist)
	ON_BN_CLICKED(IDC_IE2, OnIe2)
	ON_BN_CLICKED(IDC_FIREFOX, OnFirefox)
	ON_BN_CLICKED(IDC_DONTMONSMALL, OnDontmonsmall)
	ON_BN_CLICKED(IDC_ADDTOIEMENU, OnAddtoiemenu)
	ON_BN_CLICKED(IDC_FFPORTVER, OnFfportver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()      

BOOL CDlg_Options_Downloads_Monitoring::OnInitDialog() 
{
	CDlg_Options_Page::OnInitDialog();
	
	BOOL bIE2 = _IECatchMgr.IsIE2Active ();	
	
	CheckDlgButton (IDC_IE2, bIE2 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CLIPBOARD, _App.Monitor_Clipboard () ? BST_CHECKED : BST_UNCHECKED);
	
	BOOL bFF = _App.Monitor_Firefox () && vmsFirefoxMonitoring::IsInstalled () &&
		vmsFirefoxMonitoring::IsFlashGotInstalled () == false;
	CheckDlgButton (IDC_FIREFOX, bFF ? BST_CHECKED : BST_UNCHECKED);
	
	
	_IECMM.ReadState ();
	m_bIEMenuWas = _IECMM.IsIEMenusPresent ();
	CheckDlgButton (IDC_ADDTOIEMENU, 
		(m_bIEMenuWas || _App.Monitor_IEMenu_Enable ()) ? BST_CHECKED : BST_UNCHECKED);
	
	CheckDlgButton (IDC_ALTPRESSED, _App.Monitor_ALTShouldPressed () ? BST_CHECKED : BST_UNCHECKED);
	
	CheckDlgButton (IDC_ALLOWIETODL, _App.Monitor_AllowDownload () ? BST_CHECKED : BST_UNCHECKED);
	
	CheckDlgButton (IDC_OPERA, _NOMgr.IsOperaPluginInstalled () ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_NETSCAPE, _NOMgr.IsNetscapePluginInstalled () ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_MOZILLA,  _NOMgr.IsMozillaSuitePluginInstalled () ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_SAFARI,  _NOMgr.IsSafariPluginInstalled () ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton (IDC_CHROME,  _NOMgr.IsChromePluginInstalled () ? BST_CHECKED : BST_UNCHECKED);
	
	CheckDlgButton (IDC_SILENT, _App.Monitor_Silent () ? BST_CHECKED : BST_UNCHECKED);
	
	int iSize = _App.Monitor_SkipSmaller ();
	CheckDlgButton (IDC_DONTMONSMALL, iSize == 0 ? BST_UNCHECKED : BST_CHECKED);
	SetDlgItemInt (IDC_SMALLSIZE, iSize ? iSize/1000 : 10);

	if (IS_PORTABLE_MODE)
		GetDlgItem (IDC_FFPORTVER)->ShowWindow (SW_SHOW);
	
	ApplyLanguage ();
	UpdateEnabled ();
	
	return TRUE;  
	              
}

CString CDlg_Options_Downloads_Monitoring::get_PageTitle()
{
	return LS (L_MONITOR);
}

CString CDlg_Options_Downloads_Monitoring::get_PageShortTitle()
{
	return LS (L_MONITOR);
}

BOOL CDlg_Options_Downloads_Monitoring::Apply()
{
	BOOL bIE2 = FALSE;

	DWORD dwMUSO = 0;	
		
	bIE2 = IsDlgButtonChecked (IDC_IE2) == BST_CHECKED;

	
	_App.Monitor_IE1 ( FALSE);
	
	BOOL bIE2Active = _IECatchMgr.IsIE2Active ();

	BOOL bRR = FALSE;	

	BOOL bFF = IsDlgButtonChecked (IDC_FIREFOX) == BST_CHECKED;
	if (bFF && vmsFirefoxMonitoring::IsInstalled () == false)
	{
		if (vmsFirefoxMonitoring::Install (true) == false)
		{
			MessageBox (LS (L_CANTINITFFMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_FIREFOX, BST_UNCHECKED);
			bFF = FALSE;
		}
	}
	if (bFF && vmsFirefoxMonitoring::IsFlashGotInstalled ())
	{
		MessageBox (LS (L_FLASHGOTDETECTED), vmsFdmAppMgr::getAppName (), MB_ICONWARNING);
		CheckDlgButton (IDC_FIREFOX, BST_UNCHECKED);
		bFF = FALSE;
	}
	if (bFF)
		dwMUSO |= MONITOR_USERSWITCHEDON_FIREFOX;
	_App.Monitor_Firefox (bFF);
	if (bFF)
		vmsFirefoxMonitoring::Install (true); 

	
	if ((bIE2 == FALSE && bIE2Active) || (bIE2 && bIE2Active == FALSE))
	{
		if (bIE2 && _IECatchMgr.IsMonitoringDllRegistered () == FALSE)
		{
			if (_IECatchMgr.ActivateIE2 (TRUE) == FALSE)
			{
				MessageBox (LS (L_ERRIE), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_IE2, bIE2Active ? BST_CHECKED : BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
		else
		{
			_App.Monitor_IE2 (bIE2);
		}
	}
	if (bIE2)
		dwMUSO |= MONITOR_USERSWITCHEDON_IE;

	BOOL b = IsDlgButtonChecked (IDC_ADDTOIEMENU) == BST_CHECKED;
	if (b)
		_IECMM.AddIEMenus ();
	else
		_IECMM.DeleteIEMenus ();
	_App.Monitor_IEMenu_Enable (b);
	if (b != m_bIEMenuWas)
	{
		bRR = TRUE;
		m_bIEMenuWas = b;
	}

	b = IsDlgButtonChecked (IDC_CLIPBOARD) == BST_CHECKED;
	_App.Monitor_Clipboard (b);
	_pwndDownloads->m_ClpbrdCatch.Enable (b);

	_App.Monitor_ALTShouldPressed (IsDlgButtonChecked (IDC_ALTPRESSED) == BST_CHECKED);
	_App.Monitor_AllowDownload (IsDlgButtonChecked (IDC_ALLOWIETODL) == BST_CHECKED);

	
	_IECatchMgr.ReadSettings ();
	
	
	CString strOpDir = _App.Monitor_OperaPDInstalledTo ();
	CString strNDir = _App.Monitor_NetscapePDInstalledTo ();
	CString strMozDir = _App.Monitor_MozillaSuitePDInstalledTo ();

	
	if (IsDlgButtonChecked (IDC_OPERA) == BST_CHECKED)
	{
		dwMUSO |= MONITOR_USERSWITCHEDON_OPERA;

		
		if (_NOMgr.IsOperaPluginInstalled (TRUE) == FALSE)
		{
			if (FALSE == _NOMgr.InstallOperaPlugin ())
			{
				MessageBox (LS (L_CANTINITOPMONITOR), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_OPERA, BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
	}
	else if (_NOMgr.IsOperaPluginInstalled (strOpDir.GetLength ()))
	{
		if (FALSE == _NOMgr.DeinstallOperaPlugin ())
		{
			MessageBox (LS (L_CANTDEINITOPMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_OPERA, BST_CHECKED);
		}
		else
			bRR = TRUE;
	}

	

	if (IsDlgButtonChecked (IDC_NETSCAPE) == BST_CHECKED)
	{
		dwMUSO |= MONITOR_USERSWITCHEDON_NETSCAPE;

		if (_NOMgr.IsNetscapePluginInstalled (TRUE) == FALSE)
		{
			if (FALSE == _NOMgr.InstallNetscapePlugin ())
			{
				MessageBox (LS (L_CANTINITNETMONITOR), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_NETSCAPE, BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
	}
	else if (_NOMgr.IsNetscapePluginInstalled (strNDir.GetLength ()))
	{
		if (FALSE == _NOMgr.DeinstallNetscapePlugin ())
		{
			MessageBox (LS (L_CANTDEINITNETMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_NETSCAPE, BST_CHECKED);
		}
		else
			bRR = TRUE;
	}

	

	if (IsDlgButtonChecked (IDC_MOZILLA) == BST_CHECKED)
	{
		dwMUSO |= MONITOR_USERSWITCHEDON_SEAMONKEY;

		if (_NOMgr.IsMozillaSuitePluginInstalled (TRUE) == FALSE)
		{
			if (FALSE == _NOMgr.InstallMozillaSuitePlugin ())
			{
				MessageBox (LS (L_CANTINITMOZMONITOR), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_MOZILLA, BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
	}
	else if (_NOMgr.IsMozillaSuitePluginInstalled (strMozDir.GetLength ()))
	{
		if (FALSE == _NOMgr.DeinstallMozillaSuitePlugin ())
		{
			MessageBox (LS (L_CANTDEINITMOZMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_MOZILLA, BST_CHECKED);
		}
		else
			bRR = TRUE;
	}

	
	
	if (IsDlgButtonChecked (IDC_SAFARI) == BST_CHECKED)
	{
		dwMUSO |= MONITOR_USERSWITCHEDON_SAFARI;
		
		if (_NOMgr.IsSafariPluginInstalled (TRUE) == FALSE)
		{
			if (FALSE == _NOMgr.InstallSafariPlugin ())
			{
				MessageBox (LS (L_CANTINITSAFARIMONITOR), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_SAFARI, BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
	}
	else if (_NOMgr.IsSafariPluginInstalled (strNDir.GetLength ()))
	{
		if (FALSE == _NOMgr.DeinstallSafariPlugin ())
		{
			MessageBox (LS (L_CANTDEINITSAFARIMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_SAFARI, BST_CHECKED);
		}
		else
			bRR = TRUE;
	}

	
	
	if (IsDlgButtonChecked (IDC_CHROME) == BST_CHECKED)
	{
		dwMUSO |= MONITOR_USERSWITCHEDON_CHROME;
		
		if (_NOMgr.IsChromePluginInstalled (TRUE) == FALSE)
		{
			if (FALSE == _NOMgr.InstallChromePlugin ())
			{
				MessageBox (LS (L_CANTINITCHROMEMONITOR), LS (L_ERR), MB_ICONERROR);
				CheckDlgButton (IDC_CHROME, BST_UNCHECKED);
			}
			else
				bRR = TRUE;
		}
	}
	else if (_NOMgr.IsChromePluginInstalled (strNDir.GetLength ()))
	{
		if (FALSE == _NOMgr.DeinstallChromePlugin ())
		{
			MessageBox (LS (L_CANTDEINITCHROMEMONITOR), LS (L_ERR), MB_ICONERROR);
			CheckDlgButton (IDC_CHROME, BST_CHECKED);
		}
		else
			bRR = TRUE;
	}

	_App.Monitor_UserSwitchedOn (dwMUSO);

	_App.Monitor_Silent (IsDlgButtonChecked (IDC_SILENT) == BST_CHECKED);
	int iSize = 0;
	if (IsDlgButtonChecked (IDC_DONTMONSMALL) == BST_CHECKED)
		iSize = GetDlgItemInt (IDC_SMALLSIZE);
	_App.Monitor_SkipSmaller (iSize*1000);

	if (bRR)
		MessageBox (LS (L_BROWSERRESTARTREQ));

	UpdateEnabled ();
	
	return TRUE;
}

void CDlg_Options_Downloads_Monitoring::ApplyLanguage()
{
	fsDlgLngInfo lnginfo [] =  {
		fsDlgLngInfo (IDC__MONITORFOR, L_MONITORFOR),
		fsDlgLngInfo (IDC_CLIPBOARD, L_CLIPBOARD),
		fsDlgLngInfo (IDC_ADDTOIEMENU, L_ADDTOBRMENU),
		fsDlgLngInfo (IDC_ALTPRESSED, L_ALTSHOULD),
		fsDlgLngInfo (IDC_ALLOWIETODL, L_ALLOWBRTODL),
		fsDlgLngInfo (IDC_SILENT, L_SILENTMONITORING),
		fsDlgLngInfo (IDC_CUSTOMIZE, L_CUSTOMIZE),
		fsDlgLngInfo (IDC_DONTMONSMALL, L_DONTMONSMALL),
		fsDlgLngInfo (IDC__KBYTES, L_KBYTES),
		fsDlgLngInfo (IDC_SKIPLIST, L_SKIPLIST),
	};
	
	_LngMgr.ApplyLanguage (this, lnginfo, sizeof (lnginfo) / sizeof (fsDlgLngInfo), 0);
}

void CDlg_Options_Downloads_Monitoring::UpdateEnabled()
{
	BOOL bIE = IsDlgButtonChecked (IDC_IE2) == BST_CHECKED ||
		IsDlgButtonChecked (IDC_FIREFOX) == BST_CHECKED;
	
	GetDlgItem (IDC_ALTPRESSED)->EnableWindow (bIE);
	GetDlgItem (IDC_ALLOWIETODL)->EnableWindow (bIE);
	GetDlgItem (IDC_DONTMONSMALL)->EnableWindow (bIE);
	
	BOOL b = bIE || IsDlgButtonChecked (IDC_CLIPBOARD) == BST_CHECKED;
	GetDlgItem (IDC_SKIPLIST)->EnableWindow (b);
	
	GetDlgItem (IDC_CUSTOMIZE)->EnableWindow (
		IsDlgButtonChecked (IDC_ADDTOIEMENU) == BST_CHECKED);
	
	b = bIE && IsDlgButtonChecked (IDC_DONTMONSMALL) == BST_CHECKED;
	GetDlgItem (IDC_SMALLSIZE)->EnableWindow (b);
	GetDlgItem (IDC__KBYTES)->EnableWindow (b);
}

void CDlg_Options_Downloads_Monitoring::OnCustomize() 
{
	CDlgCustomizeIEMenu dlg;
	_DlgMgr.DoModal (&dlg);	
}

void CDlg_Options_Downloads_Monitoring::OnSkiplist() 
{
	CDownloaderProperties_MonitorPage_SkipList dlg;
	_DlgMgr.DoModal (&dlg);	
}

void CDlg_Options_Downloads_Monitoring::OnIe2() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Monitoring::OnFirefox() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Monitoring::OnDontmonsmall() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Monitoring::OnAddtoiemenu() 
{
	UpdateEnabled ();	
}

void CDlg_Options_Downloads_Monitoring::OnFfportver() 
{
	MessageBox (LS (L_SPECIFYFFPORTVERFOLDER));
	CString str; 
	CFolderBrowser *fb = CFolderBrowser::Create (LS (L_SPECIFYFFPORTVERFOLDER), str, NULL, this);
	if (fb == NULL)
		return;
	_App.Firefox_PortableVersionPath (fb->GetPath ());
}      

