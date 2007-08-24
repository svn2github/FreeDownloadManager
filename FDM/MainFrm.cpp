/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "Data Stretcher.h"
#include "EnterKeyDlg.h"
#include "fsUrlsToDownloadRegKeyMgr.h"
#include "mfchelp.h"
#include "SiteManagerSheet.h"

#include "MainFrm.h"
#include "fsPluginMgr.h"
#include "DownloadsWnd.h"
#include "ShedulerWnd.h"
#include "PrgSheet.h"
#include "DialDlg.h"
#include "system.h"
#include "SitesWnd.h"
#include "HFEWnd.h"
#include "SpiderWnd.h"
#include "inetutil.h"
#include "DlgExportDownloads.h"
#include "DlgFind.h"
#include "Dlg_SpreadHelp2.h"
#include "Dlg_TUMChanged.h"
#include "Dlg_ShutdownType.h"
#include "DlgExportHistory.h"
#include "Dlg_ChooseGroup.h"
#include "vmsFirefoxMonitoring.h"
#include "MyMessageBox.h"
#include "Dlg_MakePortableVer.h"
#include "FlashVideoDownloadsWnd.h"
#include "UploadsWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_TRAYMSG				(WM_APP + 100)
#define ID_TUM_CHANGED			36999
#define ID_SWITCH_TO_START		37000
#define ID_SWITCH_TO_END		(ID_SWITCH_TO_START+100)
#define ID_LNGMENU_START		(ID_SWITCH_TO_END+1)
#define ID_LNGMENU_END			(ID_LNGMENU_START+100)
#define ID_THREAD_UPDATE		(10000)      

extern fsPluginMgr _PluginMgr;
extern CDownloadsWnd* _pwndDownloads;
extern CHFEWnd *_pwndHFE;
extern CShedulerWnd* _pwndScheduler;
extern CSitesWnd* _pwndSites;
extern CSpiderWnd* _pwndSpider;

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_COMMAND(ID_DLDDEFOPTIONS, OnDlddefoptions)
	ON_COMMAND(ID_DLDROPTIONS, OnDldroptions)
	ON_COMMAND(ID_TUM_HEAVY, OnTumHeavy)
	ON_UPDATE_COMMAND_UI(ID_TUM_HEAVY, OnUpdateTumHeavy)
	ON_COMMAND(ID_TUM_LIGHT, OnTumLight)
	ON_UPDATE_COMMAND_UI(ID_TUM_LIGHT, OnUpdateTumLight)
	ON_COMMAND(ID_TUM_MEDIUM, OnTumMedium)
	ON_UPDATE_COMMAND_UI(ID_TUM_MEDIUM, OnUpdateTumMedium)
	ON_COMMAND(ID_OPTMASTER, OnOptmaster)
	ON_COMMAND(ID_DLD_CREATE, OnDldCreate)
	ON_COMMAND(ID_TASK_CREATE_NEW, OnTaskCreateNew)
	ON_COMMAND(ID_NEED_EXIT, OnNeedExit)
	ON_WM_SIZE()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_SAVEALL, OnSaveall)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_SHOWMAINWINDOW, OnShowmainwindow)
	ON_COMMAND(ID_STARTALL, OnStartall)
	ON_COMMAND(ID_STOPALL, OnStopall)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HOMEPAGE, OnHomepage)
	ON_COMMAND(ID_RECOMMEND, OnRecommend)
	ON_COMMAND(ID_SUPPORT, OnSupport)
	ON_COMMAND(ID_UPDATE, OnUpdate)
	ON_COMMAND(ID_PROGGENERALSETTINGS, OnProggeneralsettings)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_DIAL, OnDial)
	ON_WM_ENDSESSION()
	ON_COMMAND(ID_HOWREGISTER, OnHowregister)
	ON_COMMAND(ID_HANGUPWHENDONE, OnHangupwhendone)
	ON_UPDATE_COMMAND_UI(ID_HANGUPWHENDONE, OnUpdateHangupwhendone)
	ON_COMMAND(ID_TURNOFFWHENDONE, OnTurnoffwhendone)
	ON_UPDATE_COMMAND_UI(ID_TURNOFFWHENDONE, OnUpdateTurnoffwhendone)
	ON_COMMAND(ID_ENTERKEY, OnEnterkey)
	ON_COMMAND(ID_BUYONLINE, OnBuyonline)
	ON_COMMAND(ID_MONITORBROWSER, OnMonitorbrowser)
	ON_UPDATE_COMMAND_UI(ID_MONITORBROWSER, OnUpdateMonitorbrowser)
	ON_COMMAND(ID_MONITORCLIPBOARD, OnMonitorclipboard)
	ON_UPDATE_COMMAND_UI(ID_MONITORCLIPBOARD, OnUpdateMonitorclipboard)
	ON_COMMAND(ID_ADDSITE, OnAddsite)
	ON_COMMAND(ID_VIEW_SWBAR, OnViewSwbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SWBAR, OnUpdateViewSwbar)
	ON_WM_NCRBUTTONUP()
	ON_WM_TIMER()
	ON_WM_QUERYENDSESSION()
	ON_COMMAND(ID_INSERT, OnInsert)
	ON_COMMAND(ID_DLINFOBOX, OnDlinfobox)
	ON_COMMAND(ID_DLLANGFILES, OnDllangfiles)
	ON_COMMAND(ID_FDC, OnFDC)
	ON_COMMAND(ID_PROCCEEDFURTHERINIT, OnProceedFurherInitialization)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTLISTOFDOWNLOADS, OnExportlistofdownloads)
	ON_COMMAND(ID_FILE_IMPORT_IMPORTLISTOFDOWNLOADS, OnImportlistofdownloads)
	ON_COMMAND(ID_EXITWHENDONE, OnExitwhendone)
	ON_UPDATE_COMMAND_UI(ID_EXITWHENDONE, OnUpdateExitwhendone)
	ON_COMMAND(ID_FIND, OnFind)
	ON_COMMAND(ID_FINDNEXT, OnFindnext)
	ON_UPDATE_COMMAND_UI(ID_FINDNEXT, OnUpdateFindnext)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_DONATE, OnDonate)
	ON_COMMAND(ID_DLD_CREATEBATCH, OnDldCreatebatch)
	ON_COMMAND(ID_FILE_IMPORT_IMPORTLISTOFDOWNLOADSFROMCLIPBOARD, OnFileImportImportlistofdownloadsfromclipboard)
	ON_COMMAND(ID_OPTIONS_SM, OnOptionsSm)
	ON_COMMAND(ID_SPREADHELP, OnSpreadhelp)
	ON_COMMAND(ID_EXPORTDLHIST, OnExportdlhist)
	ON_UPDATE_COMMAND_UI(ID_EXPORTDLHIST, OnUpdateExportdlhist)
	ON_COMMAND(ID_LDF_CLEAR, OnLdfClear)
	ON_UPDATE_COMMAND_UI(ID_LDF_CLEAR, OnUpdateLdfClear)
	ON_COMMAND(ID_PAUSEALLDLDS, OnPausealldlds)
	ON_UPDATE_COMMAND_UI(ID_PAUSEALLDLDS, OnUpdatePausealldlds)
	ON_UPDATE_COMMAND_UI(ID_DLINFOBOX, OnUpdateDlinfobox)
	ON_COMMAND(ID_APP_EXIT_2, OnAppExit2)
	ON_COMMAND(ID_PORTABLE_FDM, OnPortableFdm)
	ON_COMMAND(ID_UPLDROPTIONS, OnUpldroptions)
	ON_COMMAND(ID_EXPORTSETTINGS, OnExportSettings)
	ON_COMMAND(ID_IMPORTSETTINGS, OnImportSettings)
	ON_COMMAND(ID_SPIDERDLWEBPAGE, OnSpiderDLWebPage)
	ON_COMMAND(ID_SPIDERDEFSETTINGS, OnSpiderDefSettings)
	ON_MESSAGE (WM_TRAYMSG, OnTrayMsg)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusbar)
	ON_COMMAND(ID_DROPBOX, OnDropBox)
	ON_UPDATE_COMMAND_UI(ID_DROPBOX, OnUpdateDropBox)
	ON_MESSAGE(WM_SHOWTRAYMENU, OnShowTrayMenu)
	ON_MESSAGE(WM_POWERBROADCAST, OnPowerBroadCast)
	ON_COMMAND(ID_LOADATSTARTUP, OnLoadatstartup)
	ON_UPDATE_COMMAND_UI(ID_LOADATSTARTUP, OnUpdateLoadatstartup)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SWITCH_TO_START, ID_SWITCH_TO_END, OnSwitchTo)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SWITCH_TO_START, ID_SWITCH_TO_START+100, OnUpdateSwitchTo)
	ON_UPDATE_COMMAND_UI_RANGE(WGP_MENU_VIEWSMPLITEM_CMDSTART, WGP_MENU_CMDLAST, OnUpdatePluginMenuView)
	ON_COMMAND_RANGE(ID_LNGMENU_START, ID_LNGMENU_END, OnLngMenu)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LNGMENU_START, ID_LNGMENU_END, OnUpdateLngMenu)
	ON_COMMAND_RANGE(ID_ALT1, ID_ALT9, OnAltPlusDigit)
	ON_COMMAND (ID_THREAD_UPDATE, OnIdUpdate)
	ON_COMMAND_RANGE (ID_SKIN_0, ID_SKIN_N, OnSkin)
	ON_UPDATE_COMMAND_UI_RANGE (ID_SKIN_0, ID_SKIN_N, OnUpdateSkin)
	ON_COMMAND (ID_TUM_CHANGED, OnTUMChanged)
	ON_COMMAND_RANGE (ID_LDF_0, ID_LDF_0+9, OnLDF)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LDF_0, ID_LDF_0+9, OnUpdateLdf0)
	ON_UPDATE_COMMAND_UI(ID_SB_TRAFFIC_THISMONTH, OnUpdateTrafficThisMonth)
	ON_UPDATE_COMMAND_UI(ID_SB_TOTALSPEED, OnUpdateTotalSpeed)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           
	ID_SB_TRAFFIC_THISMONTH,
	ID_SB_TOTALSPEED,
};      

CMainFrame::CMainFrame()
{
	m_cMenuMainItemsAdded = m_cMenuViewItemsAdded = 0;
	m_bQueryForExit = TRUE;
	ReadSettings ();
	m_pUpdateDlg = NULL;
	m_cPluginMenuViewInserted = 0;
	m_umsgTaskBarCreated = 0;

	m_bTrayMenuShowing = m_bBalloonShowing = FALSE;

	m_bInited = FALSE;
	m_btnFDC.m_pCusts = &m_Customizations;

	m_bNeedSpreadHelpDialog = m_bDontShowSHD = false;

	m_nTrafficUsedPrevMonth = 0;
	m_nTrafficUsedThisMonth = 0;

	m_nTrafficUpldPrevMonth = 0;
	m_nTrafficUpldThisMonth = 0;

	m_nShutdownMsg = RegisterWindowMessage ("FDM - shutdown");

	m_nUploadsMsg1 = RegisterWindowMessage (vmsUploadsDll::MSG_NAME_1);
}

CMainFrame::~CMainFrame()
{
	SAFE_DELETE (m_pUpdateDlg);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (FALSE == ReadCusomizationInfo ())
		return -1;

	LOG ("building title...");

	UpdateTitle ();

	LOG ("ok." << nl);

	LOG ("building main menu...");

	m_odmenu.Attach (GetMenu (), TRUE);
	LoadMenuImages ();

	LOG ("ok." << nl);

	LOG ("building main toolbar...");

	if (FALSE == _TBMgr.Create (this))
		return -1;

	CBitmap bmp, bmpd, bmph;

	bmp.Attach (SBMP (IDB_TOOL0));
	bmpd.Attach (SBMP (IDB_TOOL0_D));

	if (_TB_USE_ALPHA)
	{
		ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmp, GetSysColor (COLOR_3DFACE));
		ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmpd, GetSysColor (COLOR_3DFACE));
	}

	wgTButtonInfo btns [] = 
	{
		

		
		wgTButtonInfo (0, TBSTYLE_SEP, ""),
		wgTButtonInfo (ID_TUM_LIGHT, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_TUM_MEDIUM, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_TUM_HEAVY, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (0, TBSTYLE_SEP, ""),
		wgTButtonInfo (ID_STARTALL, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_STOPALL, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_PAUSEALLDLDS, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (0, TBSTYLE_SEP, ""),
		wgTButtonInfo (ID_DLDROPTIONS, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_DIAL, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_HELP, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (ID_APP_ABOUT, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (0, 0, ""),
		wgTButtonInfo (0, 0, ""),
		
	};
	
	
	if (FALSE == _TBMgr.InsertGroup (btns, &bmp, &bmpd, sizeof (btns) / sizeof (wgTButtonInfo)))
		return -1;

	
	_TBMgr.ShowGroup (0, 0);

	LOG ("ok." << nl);

	LOG ("building status bar...");

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      
	}

	UINT nID, nStyle;
	int cx;

	m_wndStatusBar.GetPaneInfo (0, nID, nStyle, cx);
	nStyle &= ~ SBPS_NOBORDERS;
	m_wndStatusBar.SetPaneInfo (0, nID, nStyle, cx);

	LOG ("ok." << nl);

	LOG ("creating rebar...");

	m_wndReBar.Create (this);

	LOG ("ok." << nl);

	if (m_Customizations.get_BannerCount ())
	{
		m_wndBanner.Create (this, 15);

		DWORD dw = GetTickCount ();
		int r = dw % (int)(m_Customizations.get_BannerCount () * 1.333);
		r = r % m_Customizations.get_BannerCount ();
		DisplayBanner (r);
		
		LOG ("building rebar..." << nl);
		
		m_wndReBar.AddBar (&m_wndBanner, NULL, NULL, RBBS_FIXEDBMP|RBBS_NOGRIPPER);
	}

	m_wndReBar.AddBar (&_TBMgr, NULL, NULL, RBBS_FIXEDBMP|RBBS_NOGRIPPER|RBBS_BREAK);
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_ID;
	rbbi.wID = 1;
	VERIFY (m_wndReBar.GetReBarCtrl ().SetBandInfo (m_wndReBar.GetReBarCtrl ().GetBandCount ()-1, &rbbi));
	
	if (m_Customizations.get_UseButton ())
		SetupButton ();

	if (0)
	{
		m_wndFDMCustBtn.Create (&m_wndReBar);
		m_wndFDMCustBtn.set_AffiliateID (m_Customizations.get_AffiliateID ());
		DWORD dwBarStyle = RBBS_NOGRIPPER|RBBS_FIXEDBMP;
		if (m_Customizations.get_BannerCount () == 0)
			dwBarStyle |= RBBS_FIXEDSIZE;
		m_wndReBar.AddBar (&m_wndFDMCustBtn, NULL, NULL, dwBarStyle);

		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof (rbbi);
		rbbi.fMask = RBBIM_IDEALSIZE | RBBIM_SIZE;
		int nIndex = m_wndReBar.GetReBarCtrl ().GetBandCount ()-1;
		CRect rc; m_wndFDMCustBtn.GetWindowRect (&rc);
		rbbi.cx = rbbi.cxIdeal = rc.Width ();
		m_wndReBar.GetReBarCtrl ().SetBandInfo (nIndex, &rbbi);
	}
		
	LOG ("ok." << nl);
	
	_ClipbrdMgr.Initialize (m_hWnd);

	
	if (!m_wndView.Create(AfxRegisterWndClass (0, LoadCursor (NULL, IDC_ARROW),
		(HBRUSH) (COLOR_3DFACE+1), NULL), NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if (_App.View_Toolbar () == FALSE)
	{
		CControlBar* pBar = GetControlBar(AFX_IDW_TOOLBAR);
		if (pBar != NULL)
			ShowControlBar (pBar, FALSE, FALSE);
	}

	if (_App.View_Statusbar () == FALSE)
	{
		CControlBar* pBar = GetControlBar(AFX_IDW_STATUS_BAR);
		if (pBar != NULL)
			ShowControlBar (pBar, FALSE, FALSE);
	}

	

	ApplyLanguage ();

	m_umsgTaskBarCreated = RegisterWindowMessage(TEXT ("TaskbarCreated"));

	UINT pnIcons [] = {IDI_TRAY_NORMAL, IDI_TRAY_DOWNLOADING, IDI_TRAY_ERRORS, IDI_TRAY_UNKNOWN};
	_TrayMgr.Create (m_hWnd, pnIcons, sizeof (pnIcons) / sizeof (UINT), 
		GetShell32Version () >= 5 ? "" : PRG_NAME, WM_TRAYMSG);

	m_pFloatWndsThread = (CFloatingWndsThread*) AfxBeginThread (RUNTIME_CLASS (CFloatingWndsThread));

	
	if (_UpdateMgr.m_enAUT != AUT_TURNOFF && IS_PORTABLE_MODE == FALSE)
	{
		LOG ("creating update thread...");

		DWORD dw;
		CreateThread (NULL, 0, _threadUpdate, this, 0, &dw);

		LOG ("ok." << nl);
	}

	ReadTotalTrafficInfo ();

	
	PostMessage (WM_COMMAND, ID_PROCCEEDFURTHERINIT);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	WNDCLASS wc;
	ZeroMemory (&wc, sizeof (wc));
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hIcon = LoadIcon (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDR_MAINFRAME));
	wc.hInstance = AfxGetInstanceHandle ();
	wc.lpfnWndProc = ::DefWindowProc;
	wc.lpszClassName = "Free Download Manager Main Window";
	cs.lpszClass = wc.lpszClassName;

	return AfxRegisterClass (&wc);
}      

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif     

void CMainFrame::OnSetFocus(CWnd* )
{
	
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	

	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::AddPluginMenu(HMENU hSrcMenu, fsSetImage* pImages, int cImages)
{
	if (hSrcMenu == NULL)
		return;

	
	if (m_cMenuMainItemsAdded)
		return;

	
	int iAddAfter = GetMenu ()->GetMenuItemCount () - 3;

	
	int cSrcItems = m_odmenu.InsertMenu (CMenu::FromHandle (hSrcMenu), iAddAfter);
	m_cMenuMainItemsAdded = cSrcItems;

	if (pImages) m_odmenu.SetImages (pImages, cImages);

	DrawMenuBar ();
}

void CMainFrame::RemovePluginMenu()
{
	int iWhere = GetMenu ()->GetMenuItemCount () - 4;

	m_odmenu.RemoveMenu (iWhere, m_cMenuMainItemsAdded);
	m_cMenuMainItemsAdded = 0;

	DrawMenuBar ();
}

void CMainFrame::OnClose() 
{
	static bool _bInFunc = false;
	if (_bInFunc)
		return;
	_bInFunc = true;

	
	if (_PluginMgr.IsWasExit ())
	{
		CFrameWnd::OnClose();
		
		LOG ("CMF::OnClose: exit" << nl);
		
		_bInFunc = false; 
		return;
	}

	

	
	SaveState ();

	
	if (m_bQueryForExit == FALSE || _PluginMgr.QueryExit ())
	{
		
		_UpdateMgr.SetEventsFunc (NULL, 0);
		_UpdateMgr.SetDescEventsFunc (NULL, 0);
		_UpdateMgr.Stop ();
		m_odmenu.Detach ();
		_TrayMgr.Remove ();

		m_pFloatWndsThread->m_wndFloating.HideAlways ();
		m_pFloatWndsThread->m_wndFloatingInfo.HideAlways ();

		ShowWindow (SW_HIDE);
		
		_PluginMgr.OnAppExit (FALSE);

		CDataStretcherApp::ScheduleExitProcess (30);

		LOG ("CMF::OC: proceed to default handler..." << nl);
		CFrameWnd::OnClose();
		LOG ("CMF::OC: default handler done.");
	}

	_bInFunc = false; 
}
void CMainFrame::OnDlddefoptions() 
{
	_pwndDownloads->OnDownloadDefProperties ();
}

void CMainFrame::OnDldroptions() 
{
	_pwndDownloads->OnDownloaderProperties ();	
}

void CMainFrame::OnTumHeavy() 
{
	if (_pwndDownloads->GetTUM () == TUM_HEAVY)
		return;

	_pwndDownloads->SetTUM (TUM_HEAVY);
	_App.CurrentTUM (TUM_HEAVY);
	
	if (_App.View_TUMChanged ())
	{
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_MEDIUM, TBSTATE_ENABLED);
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_LIGHT, TBSTATE_ENABLED);
		PostMessage (WM_COMMAND, ID_TUM_CHANGED);
	}
}

void CMainFrame::OnUpdateTumHeavy(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_pwndDownloads->GetTUM () == TUM_HEAVY);	
}

void CMainFrame::OnTumLight() 
{
	if (_pwndDownloads->GetTUM () == TUM_LIGHT)
		return;

	_pwndDownloads->SetTUM (TUM_LIGHT);
	_App.CurrentTUM (TUM_LIGHT);

	if (_App.View_TUMChanged ())
	{
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_HEAVY, TBSTATE_ENABLED);
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_MEDIUM, TBSTATE_ENABLED);
		PostMessage (WM_COMMAND, ID_TUM_CHANGED);
	}
}

void CMainFrame::OnUpdateTumLight(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_pwndDownloads->GetTUM () == TUM_LIGHT);
}

void CMainFrame::OnTumMedium() 
{
	if (_pwndDownloads->GetTUM () == TUM_MEDIUM)
		return;

	_pwndDownloads->SetTUM (TUM_MEDIUM);	
	_App.CurrentTUM (TUM_MEDIUM);

	if (_App.View_TUMChanged ())
	{
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_HEAVY, TBSTATE_ENABLED);
		_TBMgr.GetToolBarCtrl ().SetState (ID_TUM_LIGHT, TBSTATE_ENABLED);
		PostMessage (WM_COMMAND, ID_TUM_CHANGED);
	}
}

void CMainFrame::OnUpdateTumMedium(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_pwndDownloads->GetTUM () == TUM_MEDIUM);	
}

void CMainFrame::OnOptmaster() 
{
	_pwndDownloads->OnOptimizationWizard ();	
}

void CMainFrame::OnDldCreate() 
{
	if (_DldsMgr.IsDeletingNow ())
		return;
	_pwndDownloads->OnDownloadCreate ();	
}

void CMainFrame::OnTaskCreateNew() 
{
	_pwndScheduler->OnCreatenewtask ();	
}

void CMainFrame::OnNeedExit() 
{
	m_bQueryForExit = FALSE;
	SendMessage (WM_CLOSE);
}

LRESULT CMainFrame::OnTrayMsg(WPARAM , LPARAM lp)
{
	switch (lp)
	{
		case WM_MOUSEMOVE:
			if (m_bBalloonShowing == FALSE)
			{
				POINT pt;
				GetCursorPos (&pt);
				m_balloonX = pt.x; m_balloonY = pt.y;
				
				
				
				SetTimer (4, 1000, NULL);
			}
			break;
		
		case WM_LBUTTONDOWN:
			if (IsIconic ())
				OnShowmainwindow ();
			else
				ShowWindow (SW_MINIMIZE);
			break;

		case WM_RBUTTONUP:
			if (is_MouseInTray ())
			{
				KillTimer (2);
				HideBalloon ();
				ShowTrayMenu ();
			}
			break;

		case NIN_BALLOONUSERCLICK:
		case NIN_BALLOONTIMEOUT:
		case NIN_BALLOONHIDE:
			m_bBalloonShowing = FALSE;
			m_strTimeoutBallon_Msgs = "";
			break;
	}
	
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_MINIMIZED)
	{
		if (m_bMinToTray) 
			ShowWindow (SW_HIDE);
	}
	else
	{
		if (IsWindow (m_wndBanner))
			m_wndBanner.Invalidate ();

		CFrameWnd::OnSize(nType, cx, cy);

		if (nType == SIZE_MAXIMIZED || nType == SIZE_RESTORED)
			RecalcLayout ();
	}

	_TBMgr.Invalidate (TRUE);
	if (IsWindow (m_btnFDC))
		m_btnFDC.Invalidate ();
}

void CMainFrame::ShowTrayMenu(BOOL bModeTray)
{
	if (m_bTrayMenuShowing)
		return;

	m_bTrayMenuShowing = TRUE;
	
	CMenu menu;
	menu.LoadMenu (IDM_TRAY);
	CMenu *pPopup = menu.GetSubMenu (0);

	const vmsFileRecentList* pLFD = _DldsMgr.get_LastFilesDownloaded ();
	CMenu* pmenuLFD = pPopup->GetSubMenu (12);

	for (int i = 0; i < pLFD->get_Count (); i++)
	{
		if (i == 0)
			pmenuLFD->RemoveMenu (0, MF_BYPOSITION);

		LPCSTR pszDispName = pLFD->get_FileDispName (i);

		if (*pszDispName == 0)
			pszDispName = LS (L_UNKNOWN);
		
		pmenuLFD->InsertMenu (0, MF_STRING | MF_BYPOSITION, ID_LDF_0 + i, pszDispName);
	}

	pPopup->SetDefaultItem (ID_SHOWMAINWINDOW);

	CPoint pt;
	GetCursorPos (&pt);

	SetForegroundWindow ();
	
	m_odTrayMenu.Attach (&menu, FALSE);

	
	

	fsSetImage img [] = 
	{
		fsSetImage (ID_DLD_CREATE, 18),
		fsSetImage (ID_TUM_LIGHT, 0),
		fsSetImage (ID_TUM_MEDIUM, 1),
		fsSetImage (ID_TUM_HEAVY, 2),
		fsSetImage (ID_STARTALL, 3),
		fsSetImage (ID_STOPALL, 4),
		fsSetImage (ID_PAUSEALLDLDS, 5),
		fsSetImage (ID_DIAL, 7),
		fsSetImage (ID_APP_ABOUT, 8),
	};

	m_odTrayMenu.SetImages (img, sizeof (img) / sizeof (fsSetImage));

	CString strSMW = LS (L_SHOWMAINWND); strSMW += '\t';
	CString strCND = LS (L_NEWDL); strCND += "\tIns";
	CString strL = LS (L_LIGHT); strL += "\tCtrl+1";
	CString strM = LS (L_MEDIUM); strM += "\tCtrl+2";
	CString strH = LS (L_HEAVY); strH += "\tCtrl+3";
	CString strDial  = LS (L_DIAL); strDial += "\tCtrl+D";
	CString strExit = LS (L_EXIT); strExit += "\tAlt+X";

	fsSetText texts [] = {
		fsSetText (ID_SHOWMAINWINDOW, strSMW),
		fsSetText (ID_MONITORBROWSER, LS (L_MONITORBROWSER)),
		fsSetText (ID_MONITORCLIPBOARD, LS (L_MONITORCLIPBOARD)),
		fsSetText (ID_DLD_CREATE, strCND),
		fsSetText (ID_DLD_CREATEBATCH, LS (L_CREATEBATCHDLD)),
		fsSetText (ID_STARTALL, LS (L_STARTALLDLDS)),
		fsSetText (ID_STOPALL, LS (L_STOPALLDLDS)),
		fsSetText (ID_TUM_LIGHT, strL),
		fsSetText (ID_TUM_MEDIUM, strM),
		fsSetText (ID_TUM_HEAVY, strH),
		fsSetText (ID_DIAL, strDial),
		fsSetText (ID_DLINFOBOX, LS (L_SHOWDLDSINFOWHILEDOWNLOADING)),
		fsSetText (ID_DROPBOX, LS (L_DROPBOX)),
		fsSetText (ID_APP_ABOUT, LS (L_ABOUT)),
		fsSetText (ID_APP_EXIT, strExit),
		fsSetText (ID_LDF_CLEAR, LS (L_CLEAR)),
		fsSetText (ID_PAUSEALLDLDS, LS (L_PAUSEALLDOWNLOADS)),
		fsSetText (ID_LOADATSTARTUP, LS (L_AUTOSTART)),
	};
	m_odTrayMenu.SetMenuItemsText (&menu, texts, sizeof (texts) / sizeof (fsSetText), FALSE);

	m_odTrayMenu.SetMenuItemText (menu.GetSubMenu (0), LS (L_LASTDOWNLOADEDFILES), 12, TRUE);
	if (pLFD->get_Count () == 0)
		m_odTrayMenu.SetMenuItemText (menu.GetSubMenu (0)->GetSubMenu (12), LS (L_EMPTY), 0, TRUE);

	m_odTrayMenu.SetMenuItemText (menu.GetSubMenu (0), LS (L_TUM), 14, TRUE);
		
	UINT uFlags = TPM_RIGHTBUTTON;
	if (bModeTray)
		uFlags |=  TPM_BOTTOMALIGN | TPM_RIGHTALIGN;
	else
		uFlags |= TPM_LEFTALIGN | TPM_TOPALIGN;

	pPopup->TrackPopupMenu (uFlags,	pt.x, pt.y, this);
	m_odTrayMenu.Detach ();

	menu.DestroyMenu ();

	m_bTrayMenuShowing = FALSE;
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	
	
	HMENU ahMenus [100];
	
	for (int i = 0; i < m_cMenuMainItemsAdded; i++)
		ahMenus [i] = GetMenu ()->GetSubMenu (GetMenu ()->GetMenuItemCount () - 3 - m_cMenuMainItemsAdded + i)->m_hMenu;
	
	_PluginMgr.UpdateMenuMain ((HMENU*) ahMenus);

	if (m_cMenuViewItemsAdded)
	{
		for (i = 0; i < m_cMenuViewItemsAdded; i++)
			ahMenus [i] = GetMenu ()->GetSubMenu (1)->GetSubMenu (8+i)->m_hMenu;
		
		_PluginMgr.UpdateMenuView ((HMENU*) ahMenus);
	}
}

void CMainFrame::OnSaveall() 
{
	_pwndDownloads->SaveAll ();
	_pwndFVDownloads->SaveAll ();
	if (_pwndUploads)
		_pwndUploads->SaveAll ();
	_pwndScheduler->SaveAll ();
	_pwndHFE->SaveAll ();
	_pwndSites->SaveAll ();
	_pwndSpider->SaveAll (TRUE);
        ((CDataStretcherApp*)AfxGetApp ())->SaveSettings ();
	((CDataStretcherApp*) AfxGetApp ())->SaveHistory ();

	SaveState ();
}

void CMainFrame::OnAppExit() 
{
	

	OnAppExit2 ();
}

void CMainFrame::OnMeasureItem(int , LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	m_odmenu.OnMeasureItem (lpMeasureItemStruct);
}

void CMainFrame::OnDrawItem(int , LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_odmenu.OnDrawItem (lpDrawItemStruct);
}

void CMainFrame::LoadMenuImages()
{
	if (!m_imgsMenu.Create (16, 16, ILC_COLOR32 | ILC_MASK, 30, 5))
		return;

	if (!m_dimgsMenu.Create (16, 16, ILC_COLOR32 | ILC_MASK, 30, 5))
		return;

	UINT aImgs [] = {
		IDB_TOOL0_16, IDB_TOOL_DLD_16, IDB_TOOL_DLD_16,
		IDB_TOOL_DLD_16, 
		IDB_TOOL_SCH_16,
		IDB_TOOL_HFE_16, IDB_TOOL_SITES_16, IDB_TOOL_SPIDER_16,
	};

	UINT adImgs [] = {
		IDB_TOOL0_16_D, IDB_TOOL_DLD_16_D, IDB_TOOL_DLD_16_D,
		IDB_TOOL_DLD_16_D, 
		IDB_TOOL_SCH_16_D,
		IDB_TOOL_HFE_16_D, IDB_TOOL_SITES_16_D, IDB_TOOL_SPIDER_16_D,
	};

	const int _uplIndex = 3;

	for (int i = 0; i < sizeof (aImgs) / sizeof (UINT); i++)
	{
		if (i == _uplIndex && FALSE == CUploadsWnd::LoadFumCoreDll ())
			continue;

		CBitmap bmp, bmpd;

		bmp.Attach (SBMP (aImgs [i]));
		bmpd.Attach (SBMP (adImgs [i]));

		if (_TB_USE_ALPHA)
		{
			if (i == 0 || _SkinMgr.tbUseAlpha_ForAllPluginMenuImages ())
			{
				ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmp, GetSysColor (COLOR_MENU));
				ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmpd, GetSysColor (COLOR_MENU));

				m_imgsMenu.Add (&bmp, GetSysColor (COLOR_MENU));
				m_dimgsMenu.Add (&bmpd, RGB (255, 0, 255));
			}
			else
			{
				m_imgsMenu.Add (&bmp, RGB (255, 0, 255));
				m_dimgsMenu.Add (&bmpd, RGB (255, 0, 255));
			}
		}
		else
		{
			m_imgsMenu.Add (&bmp, RGB (255, 0, 255));
			m_dimgsMenu.Add (&bmpd, RGB (255, 0, 255));
		}
	}

	m_odmenu.SetImageList (&m_imgsMenu, &m_dimgsMenu);

	LoadToolImages ();
}

void CMainFrame::OnShowmainwindow() 
{
	if (IsIconic ())
		ShowWindow (SW_RESTORE);
	else
		ShowWindow (SW_SHOW);

	SetForegroundWindow ();	

	ShowSpreadHelpDialogIfRequired ();
}

void CMainFrame::OnStartall() 
{
	_DldsMgr.StartAllDownloads (TRUE);
}

void CMainFrame::OnStopall() 
{
	_pwndSpider->GetMgr ()->StopAll ();
	_DldsMgr.StopAllDownloads (TRUE);
}

void CMainFrame::OnHelp() 
{
	HtmlHelp (m_hWnd, "Help\\Free Download Manager.chm", HH_DISPLAY_TOC, NULL);	
}

void CMainFrame::OnHomepage() 
{
	fsOpenUrlInBrowser ("http://www.freedownloadmanager.org/");
}

void CMainFrame::OnRecommend() 
{
	ShellExecute (NULL, "open", "mailto:@?subject=Free%20Download%20Manager%20[http://www.freedownloadmanager.org]",	NULL, NULL, SW_SHOW);
}

void CMainFrame::OnSupport() 
{
	fsOpenUrlInBrowser ("http://www.freedownloadmanager.org/support.htm");
}

void CMainFrame::OnUpdate() 
{
	
	if (_UpdateMgr.IsStartUpdaterNeeded (FALSE))
	{
		MessageBox (LS (L_ALRUPDATED), NULL, MB_ICONEXCLAMATION);
		return;
	}

	StartUpdate (TRUE);
}

void CMainFrame::OnProggeneralsettings() 
{
	CPrgSheet sheet (LS (L_PRGGENSET), this);
	
	sheet.Init ();

	_DlgMgr.OnDoModal (&sheet);
	sheet.DoModal ();
	SaveSettings ();
	_DlgMgr.OnEndDialog (&sheet);
}

void CMainFrame::ReadSettings()
{
	m_bCloseAsMinimize = _App.Prg_CloseAsMinimize ();
	m_bMinToTray = _App.Prg_MinToTray ();
}

void CMainFrame::SaveSettings()
{
	_App.Prg_CloseAsMinimize (m_bCloseAsMinimize);
	_App.Prg_MinToTray (m_bMinToTray);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE && m_bCloseAsMinimize)
	{
		ShowWindow (SW_MINIMIZE);
		return;
	}
	
	CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnDial() 
{
	if (_DldsMgr.IsDeletingNow ())
		return;
	
	CDialDlg dlg (this);

	dlg.Init (NULL);

	_DlgMgr.OnDoModal (&dlg);
	dlg.DoModal ();
	_DlgMgr.OnEndDialog (&dlg);
}

void CMainFrame::_UpdateMgrEvents(fsUpdateMgrEvent , UINT* , LPVOID )
{

}

void CMainFrame::StartUpdate(BOOL bShowDlg)
{
	if (_UpdateMgr.IsRunning () == FALSE)
	{
		SAFE_DELETE (m_pUpdateDlg);
		fsnew1 (m_pUpdateDlg, CUpdateDlg);
                m_pUpdateDlg->m_bCheckingByUser = bShowDlg != FALSE;
		m_pUpdateDlg->Create (IDD_UPDATE, this);
	}
	
	if (bShowDlg)
		m_pUpdateDlg->ShowWindow (SW_SHOW);
}

void CMainFrame::OnEndSession(BOOL bEnding) 
{
	_PluginMgr.OnAppExit (FALSE);
        ((CDataStretcherApp*)AfxGetApp ())->SaveSettings ();
	((CDataStretcherApp*)AfxGetApp ())->SaveHistory ();
	CFrameWnd::OnEndSession(bEnding);
}

void CMainFrame::OnHowregister() 
{
	HtmlHelp (m_hWnd, "Help\\Free Download Manager.chm::/Topics/Registration.html", HH_DISPLAY_TOPIC, NULL);
}

void CMainFrame::OnHangupwhendone() 
{
	_pwndScheduler->HangupWhenDone (_pwndScheduler->HangupWhenDone () == FALSE);	
}

void CMainFrame::OnUpdateHangupwhendone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_pwndScheduler->HangupWhenDone () ? 1 : 0);	
}

void CMainFrame::OnTurnoffwhendone() 
{
	fsShutdownType enType = (fsShutdownType) _App.WD_LastShutdownType ();

	
	if (_pwndScheduler->TurnoffWhenDone (enType))
	{
		_pwndScheduler->TurnoffWhenDone (enType, FALSE);
		return;
	}

	if (_App.WD_LastShutdownType_DontAsk () == FALSE)
	{
		CDlg_ShutdownType dlg;
		dlg.m_enType = enType;
		if (IDCANCEL == _DlgMgr.DoModal (&dlg))
			return;

		enType = dlg.m_enType;
		_App.WD_LastShutdownType (enType);

		if (dlg.m_bRemember)
			_App.WD_LastShutdownType_DontAsk (TRUE);
	}

	_pwndScheduler->TurnoffWhenDone (enType, TRUE);	
}

void CMainFrame::OnUpdateTurnoffwhendone(CCmdUI* pCmdUI) 
{
	fsShutdownType enType = (fsShutdownType) _App.WD_LastShutdownType ();
	pCmdUI->SetCheck (_pwndScheduler->TurnoffWhenDone (enType) ? 1 : 0);	
}

void CMainFrame::OnEnterkey() 
{
	CEnterKeyDlg dlg;

	_DlgMgr.OnDoModal (&dlg);
	if (IDOK == dlg.DoModal ())
		UpdateTitle ();
	_DlgMgr.OnEndDialog (&dlg);
}

void CMainFrame::UpdateTitle()
{
	CString strCaption = PRG_NAME;

	LPCSTR pszCustomizer = m_Customizations.get_Customizer ();
	if (pszCustomizer != NULL && lstrlen (pszCustomizer) != 0)
	{
		strCaption += ", ";
		strCaption += pszCustomizer;
		strCaption += " version";
	}

	SetWindowText (strCaption);
}

void CMainFrame::OnBuyonline() 
{
	fsOpenBuyPage ();
}

void CMainFrame::OnMonitorbrowser() 
{
	BOOL bMonitor = _App.Monitor_IE2 () || _App.Monitor_Firefox () ||
		_NOMgr.IsNetscapePluginInstalled () || _NOMgr.IsOperaPluginInstalled () ||
		_NOMgr.IsMozillaSuitePluginInstalled ();
	
	BOOL bRR = FALSE; 

	DWORD dwMUSO = _App.Monitor_UserSwitchedOn ();

	DWORD dwMonitorNow = 0;

	if (bMonitor || (dwMUSO & MONITOR_USERSWITCHEDON_IE))
	{
		if (_IECatchMgr.ActivateIE2 (!bMonitor)) {
			_App.Monitor_IE2 (!bMonitor);
			if (!bMonitor)
				dwMonitorNow |= MONITOR_USERSWITCHEDON_IE;
		}
		else
			MessageBox (LS (L_ERRIE), LS (L_ERR), MB_ICONERROR);
	}

	if (bMonitor || (dwMUSO & MONITOR_USERSWITCHEDON_FIREFOX))
	{
		if (bMonitor && false == vmsFirefoxMonitoring::IsInstalled ())
			vmsFirefoxMonitoring::Install (true);
		_App.Monitor_Firefox (!bMonitor);
		if (!bMonitor && vmsFirefoxMonitoring::IsInstalled ())
			dwMonitorNow |= MONITOR_USERSWITCHEDON_FIREFOX;
	}

	
	if (bMonitor)
	{
		if (_NOMgr.IsNetscapePluginInstalled ())
		{
			_NOMgr.DeinstallNetscapePlugin ();
			bRR = TRUE;
		}

		if (_NOMgr.IsOperaPluginInstalled ())
		{
			_NOMgr.DeinstallOperaPlugin ();
			bRR = TRUE;
		}

		if (_NOMgr.IsMozillaSuitePluginInstalled ())
		{
			_NOMgr.DeinstallMozillaSuitePlugin ();
			bRR = TRUE;
		}
	}
	else
	{
		if ((dwMUSO & MONITOR_USERSWITCHEDON_OPERA) && 
				_NOMgr.IsOperaPluginInstalled () == FALSE)
		{
			if (_NOMgr.InstallOperaPlugin ()) {
				bRR = TRUE;
				dwMonitorNow |= MONITOR_USERSWITCHEDON_OPERA;
			}
		}
		
		if ((dwMUSO & MONITOR_USERSWITCHEDON_NETSCAPE) && 
				_NOMgr.IsNetscapePluginInstalled () == FALSE)
		{
			if (_NOMgr.InstallNetscapePlugin ()) {
				bRR = TRUE;
				dwMonitorNow |= MONITOR_USERSWITCHEDON_NETSCAPE;
			}
		}

		if ((dwMUSO & MONITOR_USERSWITCHEDON_SEAMONKEY) && 
				_NOMgr.IsMozillaSuitePluginInstalled () == FALSE)
		{
			if (_NOMgr.InstallMozillaSuitePlugin ()) {
				bRR = TRUE;
				dwMonitorNow |= MONITOR_USERSWITCHEDON_SEAMONKEY;
			}
		}
	}

	if (dwMonitorNow) {
		CString str;
		if (dwMonitorNow & MONITOR_USERSWITCHEDON_IE)
			str = "Internet Explorer";
		if (dwMonitorNow & MONITOR_USERSWITCHEDON_FIREFOX)
			str += str == "" ? "Firefox" : ", Firefox";
		if (dwMonitorNow & MONITOR_USERSWITCHEDON_OPERA)
			str += str == "" ? "Opera" : ", Opera";
		if (dwMonitorNow & MONITOR_USERSWITCHEDON_NETSCAPE)
			str += str == "" ? "Netscape" : ", Netscape";
		if (dwMonitorNow & MONITOR_USERSWITCHEDON_SEAMONKEY)
			str += str == "" ? "SeaMonkey" : ", SeaMonkey";
		
		CString str2;
		str2.Format (LS (L_MONITORING_TURNEDON_IN), str);
		if (bRR) {
			str2 += "\n\n";
			str2 += LS (L_BROWSERRESTARTREQ);
		}
		MessageBox (str2);
	}
	else if (bRR)
		MessageBox (LS (L_BROWSERRESTARTREQ));
}

void CMainFrame::OnUpdateMonitorbrowser(CCmdUI* pCmdUI) 
{
	BOOL bMonitor = _App.Monitor_IE2 () || _App.Monitor_Firefox () ||
		_NOMgr.IsNetscapePluginInstalled () || _NOMgr.IsOperaPluginInstalled () ||
		_NOMgr.IsMozillaSuitePluginInstalled ();
	pCmdUI->SetCheck (bMonitor ? 1 : 0);	
}

void CMainFrame::OnMonitorclipboard() 
{
	BOOL b = _App.Monitor_Clipboard () == FALSE;
	_App.Monitor_Clipboard (b);
	_pwndDownloads->m_ClpbrdCatch.Enable (b);
}

void CMainFrame::OnUpdateMonitorclipboard(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_App.Monitor_Clipboard () ? 1 : 0);	
}

void CMainFrame::OnAddsite() 
{
	_pwndSites->OnSitesAdd ();	
}

void CMainFrame::BuildSwitchToMenu()
{
	CMenu *pMenu = GetMenu ()->GetSubMenu (1)->GetSubMenu (6);
	m_odmenu.RemoveMenuItem (pMenu, 0, TRUE);
	for (int i = 0; i < _PluginMgr.GetPluginCount (); i++)
	{
		fsPluginInfo *plug = &_PluginMgr.Plugin (i);
		CString str;
		str.Format ("%s\tAlt+%d", plug->strName, i+1);
		fsODMenuItemData *pData = m_odmenu.AddItem (pMenu, ID_SWITCH_TO_START+i, str);
		if (pData)
			pData->iCheckImage = ODMENU_CHECKIMAGE_RADIO;
	}
}

void CMainFrame::OnSwitchTo(UINT nID)
{
	SetActivePlugin (nID - ID_SWITCH_TO_START);
}

void CMainFrame::OnUpdateSwitchTo(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (pCmdUI->m_nID - ID_SWITCH_TO_START == (UINT)m_wndView.m_wndClient.m_plugs2.GetActivePlugin () ? 1 : 0);
}

void CMainFrame::OnViewSwbar() 
{
}

void CMainFrame::OnUpdateViewSwbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndView.m_wndClient.m_bShowSWBar ? 1 : 0);	
}

void CMainFrame::RemovePluginMenuView()
{
	
	RemovePluginMenuView2 ();

	

	if (m_cPluginMenuViewInserted == 0)
		return;

	CMenu *pMenu = GetMenu ()->GetSubMenu (1);

	while (m_cPluginMenuViewInserted--)
	{
		m_odmenu.RemoveMenuItem (pMenu, 8, TRUE);
	}

	m_cPluginMenuViewInserted = 0;

	DrawMenuBar ();
}

void CMainFrame::AddPluginMenuView(HMENU hMenuView, fsSetImage* pImages, int cImages, wgMenuViewItem *pItems, int cItems)
{
	m_pPluginMenuViewItems = pItems;
	if (pItems)
	{
		

		m_cPluginMenuViewItems = cItems;

		CMenu *pMenu = GetMenu ()->GetSubMenu (1);
		int iStartPos = 8;

		for (int i = 0; i < cItems; i++)
			m_odmenu.InsertMenuItem (pMenu, pItems [i].pszName, WGP_MENU_VIEWSMPLITEM_CMDSTART+i, iStartPos++, TRUE);
	
		m_cPluginMenuViewInserted = cItems;
	}

	
	AddPluginMenuView (hMenuView, pImages, cImages);

	DrawMenuBar ();
}

void CMainFrame::OnAltPlusDigit(UINT nID)
{
	int iPlug = nID - ID_ALT1;
	if (iPlug < _PluginMgr.GetPluginCount ())
		m_wndView.m_wndClient.m_plugs2.SetActivePlugin (iPlug);
}

void CMainFrame::OnUpdatePluginMenuView(CCmdUI *pCmdUI)
{
	int iItem = pCmdUI->m_nID - WGP_MENU_VIEWSMPLITEM_CMDSTART;
	if (iItem >= m_cPluginMenuViewItems)
		return;

	pCmdUI->SetCheck (*m_pPluginMenuViewItems [iItem].pbView ? 1 : 0);
}

void CMainFrame::OnViewToolbar()
{
	CFrameWnd::OnBarCheck (AFX_IDW_TOOLBAR);
	
	CControlBar* pBar = GetControlBar(AFX_IDW_TOOLBAR);
	if (pBar != NULL)
		_App.View_Toolbar ((pBar->GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::OnViewStatusbar()
{
	CFrameWnd::OnBarCheck (AFX_IDW_STATUS_BAR);
	
	CControlBar* pBar = GetControlBar(AFX_IDW_STATUS_BAR);
	if (pBar != NULL)
		_App.View_Statusbar ((pBar->GetStyle() & WS_VISIBLE) != 0);
}

void CMainFrame::SaveState()
{
	m_wndView.m_wndClient.SaveState ();
	_App.View_SaveWndPlacement (this, "MainFrm");
	if (m_pFloatWndsThread)
	{
		m_pFloatWndsThread->m_wndFloating.SaveState ();
		m_pFloatWndsThread->m_wndFloatingInfo.SaveState ();
	}

	
	m_nTrafficUsedThisMonth += fsInternetDownloader::get_TotalTraffic ();
	vmsBtSession *pS = _BT.is_Initialized () ? _BT.get_Session () : NULL;
	UINT64 u = pS ? pS->get_TotalDownloadedByteCount () : 0;
	fsInternetDownloader::ResetTotalTrafficInfo ();
	_App.TotalTraffic (m_nTrafficUsedThisMonth + u);

	u = pS ? pS->get_TotalUploadedByteCount () : 0;
	_App.TotalUpldTraffic (m_nTrafficUpldThisMonth + u);
}

void CMainFrame::BuildLngMenu()
{
	CMenu *menu = GetMenu ()->GetSubMenu (1);
	menu = menu->GetSubMenu (menu->GetMenuItemCount () - 1);

	m_odmenu.RemoveMenuItem (menu, 0, TRUE);

	if (_LngMgr.GetLngCount ())
	{
		for (int i = 0; i < _LngMgr.GetLngCount (); i++)
			m_odmenu.InsertMenuItem (menu, _LngMgr.GetLngName (i), ID_LNGMENU_START+i, i, TRUE);
	}
	else 
	{
		m_odmenu.InsertMenuItem (menu, LS (L_EMPTY), ID_LNGMENU_START, 0, TRUE);
	}
}

void CMainFrame::OnLngMenu(UINT id)
{
	int iCurLng = _LngMgr.GetCurLng ();
	
	BOOL bIEMenus = _IECMM.IsIEMenusPresent ();
	if (bIEMenus)
		_IECMM.DeleteIEMenus ();	
	if (FALSE == _LngMgr.LoadLng (id - ID_LNGMENU_START))
	{
		_LngMgr.LoadLng (iCurLng);
		if (bIEMenus)
			_IECMM.AddIEMenus ();
		MessageBox ("Sorry, can't load this language", "Error", MB_ICONERROR);
		return;
	}

	_App.View_Language (_LngMgr.GetLngName (id - ID_LNGMENU_START));
	if (bIEMenus)
		_IECMM.AddIEMenus ();
	ApplyLanguage ();
}

void CMainFrame::OnUpdateLngMenu(CCmdUI *pCmdUI)
{
	if (_LngMgr.GetLngCount ())
		pCmdUI->SetCheck (pCmdUI->m_nID - ID_LNGMENU_START == (UINT)_LngMgr.GetCurLng () ? 1 : 0);
	else
		pCmdUI->Enable (FALSE);
}

void CMainFrame::ApplyLanguage()
{
	ApplyLanguageToMenu ();

	LPCSTR ppszToolTips [] = {
		"", LS (L_LIGHT), LS (L_MEDIUM), 
		LS (L_HEAVY), "", LS (L_STARTALLDLDS), LS (L_STOPALLDLDS), 
		LS (L_PAUSEALLDOWNLOADS), "", LS (L_DLDR_OPTIONS), LS (L_DIALUP), 
		LS (L_HELP), LS (L_ABOUT), "", "Make a donation",
		LS (L_SAVEALL), "", LS (L_CUT), LS (L_COPY), LS (L_PASTE), "", 
	};

	_TBMgr.UpdateGroupToolTips (0, ppszToolTips, sizeof (ppszToolTips) / sizeof (LPCSTR));

	m_wndStatusBar.SetToolTipText (ID_SB_TOTALSPEED, LS (L_TOTAL_SPEED_SBTEXT));

	UpdateTitle ();
}

void CMainFrame::ApplyLanguageToMenu()
{
	RemovePluginMenu ();
	RemovePluginMenuView ();

	m_odmenu.Detach ();
	SetMenu (NULL);

	SetMenu (CMenu::FromHandle (LoadMenu (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDR_MAINFRAME))));

	if (_pwndUploads == NULL || _pwndUploads->m_pwndUploads == NULL || IS_PORTABLE_MODE)
		GetMenu ()->GetSubMenu (2)->RemoveMenu (ID_UPLDROPTIONS, MF_BYCOMMAND);

	m_odmenu.Attach (GetMenu (), TRUE);

	
	_PluginMgr.OnNewLng (_LngMgr.GetLngName (_LngMgr.GetCurLng ()));

	SetMenuImages ();
	BuildSwitchToMenu ();
	BuildSkinsMenu ();
	BuildLngMenu ();

	fsSetText aMenuBarTexts [] = {
		fsSetText (0, LS (L_FILE)),
		fsSetText (1, LS (L_VIEW)),
		fsSetText (2, LS (L_OPTIONS)),
		fsSetText (3, LS (L_TOOLS)),
		fsSetText (4, LS (L_HELP)),
	};
	m_odmenu.SetMenuItemsText (GetMenu (), aMenuBarTexts, sizeof (aMenuBarTexts) / sizeof (fsSetText), TRUE);

	CString strCut = LS (L_CUT), strCopy = LS (L_COPY), strPaste = LS (L_PASTE), 
		strDldOpt = LS (L_DLDR_OPTIONS), strDldDef = LS (L_DLDDEFOPT),
		strPrgGen = LS (L_PRGGENSET), strLight = LS (L_LIGHT), strMedium = LS (L_MEDIUM),
		strHeavy = LS (L_HEAVY), strDial = LS (L_DIAL), strDoc = LS (L_DOCUMENTATION),
		strSpiderDefs = LS (L_SPIDERDEFSETTINGS), strExit = LS (L_EXIT),
		strExport = LS (L_EXPORTSETTINGS), strSaveAll = LS (L_SAVEALL),
		strImpFromClip = LS (L_IMPORTURLSFROMCLIPBOARD);

	strCut += "\tCtrl+X"; strCopy += "\tCtrl+C"; strPaste += "\tCtrl+V";
	strDldOpt += "\tCtrl+O"; strDldDef += "\tCtrl+E";
	strPrgGen += "\tCtrl+P"; strLight += "\tCtrl+1"; strMedium += "\tCtrl+2"; strHeavy += "\tCtrl+3";
	strDial += "\tCtrl+D"; strDoc += "\tF1";
	strSpiderDefs += "\tCtrl+Q"; strExit += "\tAlt+X";
	strExport += '\t'; strSaveAll += "\tCtrl+S";
	strImpFromClip += "\tCtrl+Shift+V";

	fsSetText aMenu0Texts [] = {
		fsSetText (0, LS (L_EXPORT)),
		fsSetText (1, LS (L_IMPORT)),
	};
	m_odmenu.SetMenuItemsText (GetMenu ()->GetSubMenu (0), aMenu0Texts,
		sizeof (aMenu0Texts) / sizeof (fsSetText), TRUE);

	fsSetText aTexts [] = {
		fsSetText (ID_EXPORTSETTINGS, strExport),
		fsSetText (ID_IMPORTSETTINGS, LS (L_IMPORTSETTINGS)),
		fsSetText (ID_FILE_EXPORT_EXPORTLISTOFDOWNLOADS, LS (L_EXPORTLISTOFDLDS)),
		fsSetText (ID_EXPORTDLHIST, LS (L_EXPORTDLHIST)),
		fsSetText (ID_FILE_IMPORT_IMPORTLISTOFDOWNLOADS, LS (L_IMPORTLISTOFDLDS)),
		fsSetText (ID_FILE_IMPORT_IMPORTLISTOFDOWNLOADSFROMCLIPBOARD, strImpFromClip),
		fsSetText (ID_PORTABLE_FDM, LS (L_MAKE_PORTABLE_VER)),
		fsSetText (ID_APP_EXIT, strExit),
		fsSetText (ID_VIEW_TOOLBAR, LS (L_TOOLBAR)),
		fsSetText (ID_VIEW_STATUS_BAR, LS (L_STATUSBAR)),
		fsSetText (ID_DLINFOBOX, LS (L_DLSINFO)),
		fsSetText (ID_DROPBOX, LS (L_DROPBOX)),
		fsSetText (ID_SKIN_0, LS (L_NEWSTYLE)),
		fsSetText (ID_DLDROPTIONS, strDldOpt),
		fsSetText (ID_UPLDROPTIONS, LS (L_UPLMGRINTEGRSTGS)),
		fsSetText (ID_OPTIONS_SM, LS (L_OPTIONS_SM)),
		fsSetText (ID_OPTMASTER, LS (L_OPTMASTER)),
		fsSetText (ID_DLDDEFOPTIONS, strDldDef),
		fsSetText (ID_SPIDERDEFSETTINGS, strSpiderDefs),
		fsSetText (ID_PROGGENERALSETTINGS, strPrgGen),
		fsSetText (ID_TUM_LIGHT, strLight),
		fsSetText (ID_TUM_MEDIUM, strMedium),
		fsSetText (ID_TUM_HEAVY, strHeavy),
		fsSetText (ID_DIAL, strDial),
		fsSetText (ID_HANGUPWHENDONE, LS (L_HANGUPWD)),
		fsSetText (ID_TURNOFFWHENDONE, LS (L_TURNOFFWD)),
		fsSetText (ID_EXITWHENDONE, LS (L_EXITWHENDONE)),
		fsSetText (ID_HELP, strDoc),
		fsSetText (ID_HOMEPAGE, LS (L_HOMEPAGE)),
		fsSetText (ID_RECOMMEND, LS (L_RECOMMEND)),
		fsSetText (ID_SUPPORT, LS (L_TOSUPPORT)),
		fsSetText (ID_UPDATE, LS (L_UPDATENOW)),
		fsSetText (ID_SPREADHELP, LS (L_PLACELINKTOFDM)),
		
		fsSetText (ID_APP_ABOUT, LS (L_ABOUT)),
	};
	m_odmenu.SetMenuItemsText (GetMenu (), aTexts, sizeof (aTexts) / sizeof (fsSetText), FALSE);

	m_odmenu.SetMenuItemText (GetMenu ()->GetSubMenu (1), LS (L_SWITCHTO), 6, TRUE);
	m_odmenu.SetMenuItemText (GetMenu ()->GetSubMenu (1), LS (L_SKIN), get_SkinsMenuPos (), TRUE);
	m_odmenu.SetMenuItemText (GetMenu ()->GetSubMenu (2), LS (L_TUM), GetTumMenuPosition (), TRUE);

	m_wndView.m_wndClient.m_plugs2.OnNewLng ();

	DrawMenuBar ();
}

void CMainFrame::SetMenuImages()
{
	fsSetImage images []  = 
	{
		

		

		fsSetImage (ID_DLDROPTIONS, 6),
		fsSetImage (ID_DLDDEFOPTIONS, 9),

		fsSetImage (ID_TUM_LIGHT, 0),
		fsSetImage (ID_TUM_MEDIUM, 1),
		fsSetImage (ID_TUM_HEAVY, 2),

		fsSetImage (ID_DIAL, 7),

		fsSetImage (ID_HELP, 8),
		fsSetImage (ID_APP_ABOUT, 10),
		fsSetImage (ID_HOMEPAGE, 11),
		fsSetImage (ID_SUPPORT, 12)
	};
	
	m_odmenu.SetImages (images, sizeof (images) / sizeof (fsSetImage));
}

DWORD WINAPI CMainFrame::_threadUpdate(LPVOID lp)
{
	CMainFrame *pThis = (CMainFrame*) lp;
	DWORD dw;

	
	while (InternetGetConnectedState (&dw, 0) == FALSE)
		Sleep (10*60*1000);
	
	SYSTEMTIME cur, last;
	GetLocalTime (&cur);

	if (_App.Update_LastCheck (&last) == FALSE)
	{
		
		
		_App.Update_LastCheck (cur);
		return 0;
	}

	
	if (_App.NewVerExists () || fsGetTimeDelta (&cur, &last) >= 5*24*60*60) 
	{
		if (_UpdateMgr.IsRunning () == FALSE)
			
			pThis->PostMessage (WM_COMMAND, ID_THREAD_UPDATE);
	}

	return 0;
}

void CMainFrame::OnSpiderDefSettings()
{
	_pwndSpider->SetupDefSettings ();
}

void CMainFrame::OnSpiderDLWebPage()
{
	_pwndSpider->OnSpiderDl ();
}

void CMainFrame::OnExportSettings()
{
	CString strFilter;
	strFilter.Format ("%s (*.reg)|*.reg||", LS (L_REGFILES));
	CFileDialog dlg (FALSE, "reg", NULL, OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter, this);

	if (_DlgMgr.DoModal (&dlg) == IDCANCEL)
		return;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof (si);
	ZeroMemory (&pi, sizeof (pi));

	CString strEx;
	strEx.Format ("regedit.exe /ea \"%s\" \"HKEY_CURRENT_USER\\Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\"", dlg.GetPathName ());

	if (FALSE == CreateProcess (NULL, (LPSTR)(LPCSTR)strEx, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		MessageBox (LS (L_REGEDITRUNFAILED), LS (L_ERR), MB_ICONERROR);
		return;
	}

	WaitForSingleObject (pi.hProcess, INFINITE);

	DWORD dwCode;
	GetExitCodeProcess (pi.hProcess, &dwCode);
	if (dwCode)
		MessageBox (LS (L_FAILEDTOEXPORT), LS (L_ERR), MB_ICONERROR);
}

void CMainFrame::OnImportSettings()
{
	CString strFilter;
	strFilter.Format ("%s (*.reg)|*.reg||", LS (L_REGFILES));
	CFileDialog dlg (TRUE, "reg", NULL, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, strFilter, this);

	if (dlg.DoModal () == IDCANCEL)
		return;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof (si);
	ZeroMemory (&pi, sizeof (pi));

	CString strEx;
	strEx.Format ("regedit.exe /s \"%s\"", dlg.GetPathName ());

	if (FALSE == CreateProcess (NULL, (LPSTR)(LPCSTR)strEx, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		MessageBox (LS (L_REGEDITRUNFAILED), LS (L_ERR), MB_ICONERROR);
		return;
	}

	WaitForSingleObject (pi.hProcess, INFINITE);

	DWORD dwCode;
	GetExitCodeProcess (pi.hProcess, &dwCode);
	if (dwCode)
		MessageBox (LS (L_FAILEDTOIMPORT), LS (L_ERR), MB_ICONERROR);
	else
		UpdateSettings ();
}

void CMainFrame::UpdateSettings()
{
	_DldsMgr.ReadSettings ();
	_DldsMgr.ApplyTrafficLimit ();
	_pwndHFE->GetMgr ()->ReadSettings ();
	_UpdateMgr.ReadSettings ();
	_IECatchMgr.ReadSettings ();

	_pwndDownloads->m_ClpbrdCatch.Enable (_App.Monitor_Clipboard  ());
	_IECatchMgr.ActivateIE2 (_App.Monitor_IE2 ());
}

void CMainFrame::OnDropBox()
{
	m_pFloatWndsThread->m_wndFloating.Show (!_App.View_FloatingWindow ());
}

void CMainFrame::OnUpdateDropBox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (_App.View_FloatingWindow () ? 1 : 0);
}

UINT CMainFrame::m_umsgTaskBarCreated = 0;

LRESULT CMainFrame::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == m_umsgTaskBarCreated)
		_TrayMgr.TestIcon ();

	else if (msg == m_nShutdownMsg) 
		PostMessage (WM_COMMAND, ID_APP_EXIT_2);

	else if (msg == m_nUploadsMsg1 && _pwndUploads && _pwndUploads->m_pwndUploads)
		_pwndUploads->m_pwndUploads->get_Wnd ()->SendMessage (msg, wp, lp);

	return CFrameWnd::WindowProc (msg, wp, lp);
}

void CMainFrame::OnNcRButtonUp(UINT nHitTest, CPoint point) 
{
	CFrameWnd::OnNcRButtonUp(nHitTest, point);
}

void CMainFrame::OnTBQueryDelete(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBQueryInsert(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBBeginDrag(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBEndDrag(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBBeginAdjust(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBEndAdjust(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBCustHelp(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBReset(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}

void CMainFrame::OnTBGetButtonInfo(NMHDR *nm, LRESULT *res)
{
	*res = _TBMgr.OnTBGetButtonInfo (nm);
}

void CMainFrame::OnTBChange(NMHDR *, LRESULT *res)
{
	
	*res = FALSE;
}  

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	

	LPNMHDR nm = (LPNMHDR) lParam;
	
	
	
	if (_TBMgr.OnNotify (nm) == FALSE && nm->code != TBN_GETINFOTIPW)
	{
		if (nm->idFrom == AFX_IDW_TOOLBAR)
		{
			switch (nm->code)
			{
				case TBN_QUERYDELETE:
					OnTBQueryDelete (nm, pResult);
					break;

				case TBN_QUERYINSERT:
					OnTBQueryInsert (nm, pResult);
					break;

				case TBN_BEGINADJUST:
					OnTBBeginAdjust (nm, pResult);
					break;

				case TBN_ENDADJUST:
					OnTBEndAdjust (nm, pResult);
					break;

				case TBN_BEGINDRAG:
					OnTBBeginDrag (nm, pResult);
					break;

				case TBN_ENDDRAG:
					OnTBEndDrag (nm, pResult);
					break;

				case TBN_CUSTHELP:
					OnTBCustHelp (nm, pResult);
					break;

				case TBN_RESET:
					OnTBReset (nm, pResult);
					break;

				case TBN_GETBUTTONINFO:
					OnTBGetButtonInfo (nm, pResult);
					break;

				case TBN_TOOLBARCHANGE:
					OnTBChange (nm, pResult);
					break;

				default:
					return DefWindowProc (WM_NOTIFY, wParam, lParam);
			}

			return TRUE;
		}
	}

	*pResult = FALSE;
	return FALSE;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	

	switch (nIDEvent)
	{
		
		case 1:
			KillTimer (1);
			
		break;

		
		
		case 2:
			if (FALSE == is_MouseInTray ())
			{
				KillTimer (2);
				HideBalloon ();
			}
		break;

		
		case 3:
			KillTimer (3);
			HideBalloon ();
			m_strTimeoutBallon_Msgs = "";
		break;

		
		case 4:
			if (m_bBalloonShowing == FALSE)
			{
				if (is_MouseInTray ())
				{
					Balloon_ShowDLInfo (FALSE);
					SetTimer (2, 200, NULL);
				}
			}
			KillTimer (4);
		break;

		case 5:
			DisplayBanner (0);
			KillTimer (5);
			break;

		case 6:
			TotalTraffic_CheckMonthChange ();
			break;
	}
}

void CMainFrame::AddPluginMenuView(HMENU hMenu, fsSetImage* pImages, int cImages)
{
	if (hMenu == NULL)
		return;

	if (m_cMenuViewItemsAdded)
		return;

	m_cMenuViewItemsAdded = m_odmenu.InsertMenu (CMenu::FromHandle (hMenu), 8, GetMenu ()->GetSubMenu (1));
	
	if (pImages) m_odmenu.SetImages (pImages, cImages);
}

void CMainFrame::RemovePluginMenuView2()
{
	if (m_cMenuViewItemsAdded)
	{
		
		
		m_odmenu.RemoveMenu (8 + m_cMenuViewItemsAdded - 1, m_cMenuViewItemsAdded, GetMenu ()->GetSubMenu (1));
		m_cMenuViewItemsAdded = 0;
	}
}

BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	return _PluginMgr.QueryExit ();
}

void CMainFrame::OnIdUpdate()
{
	StartUpdate (FALSE);
}

void CMainFrame::OnInsert() 
{
	fsPluginInfo& plug = _PluginMgr.Plugin (
		m_wndView.m_wndClient.m_plugs2.GetActivePlugin ());	
	::PostMessage (plug.hWnd, WM_WGP_INSERT, 0, 0);
}

void CMainFrame::HideBalloon()
{
	if (m_bBalloonShowing)
	{
		_TrayMgr.ShowBalloon ("", "");
		m_bBalloonShowing = FALSE;
	}
}

void CMainFrame::ShowBalloon(LPCSTR pszInfo, LPCSTR pszTitle, BOOL bCheckSettings, DWORD dwIcon)
{
	if (bCheckSettings && _App.Notif_UseBalloon () == FALSE)
		return;

	if (m_bTrayMenuShowing == FALSE)
	{
		_TrayMgr.ShowBalloon (pszInfo, pszTitle, dwIcon);
		m_bBalloonShowing = TRUE;
	}
}

void CMainFrame::Balloon_ShowDLInfo(BOOL bCheckSettings)
{
	CString str; 

	if (bCheckSettings && _App.Notif_UseBalloon () == FALSE)
		return;

	

	for (size_t i = 0; i < _DldsMgr.GetCount (); i++)
	{
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownload (i);

		if (dld->pMgr->IsRunning ())
		{
			
			if (i) str += '\n';

			CString strtmp; 
			char szFile [MY_MAX_PATH];
			
			CDownloads_Tasks::GetFileName (dld, szFile);

			if (dld->pMgr->GetDownloadingSectionCount ())
			{
				strtmp.Format ("%s - %d%%", szFile, 
					(int) dld->pMgr->GetPercentDone ());
			}
			else if (dld->pMgr->IsCantStart () == FALSE)
				strtmp.Format ("%s - %s...", szFile, LS (L_STARTING));
			else
				strtmp.Format ("%s - %s", szFile, LS (L_ERR));

			str += strtmp;
		}
	}

	

	fsWebPageDownloadsMgr *pages = _pwndSpider->GetMgr ();

	BOOL bAdded = FALSE;
	
	for (i = 0; (int)i < pages->GetWPDCount (); i++)
	{
		fsWebPageDownloader* wpd = pages->GetWPD (i);
		if (wpd->IsRunning ())
		{
			if (bAdded == FALSE)
			{
				bAdded = TRUE;
				if (str != "")
					str += "\n\n";
				str += LS (L_WEBSITES);
				str += ':';
			}

			CString strtmp; 
	
			char szUrl [10000];
			
			wpd->GetDownloadingSiteName (szUrl);

			strtmp.Format ("%s - %d%%", szUrl, (int) wpd->GetPercentDone ());

			if (str != "")
				str += '\n';

			str += strtmp;
		}
	}

	if (str != "")
	{
		CString s = LS (L_ACTIVEDLDS);
		s += ':';
		ShowBalloon (str, s, FALSE);
	}
	else
		ShowBalloon (LS (L_THEREARENOACTDLDS), PRG_NAME, FALSE);
}

void CMainFrame::ShowTimeoutBalloon(LPCSTR pszInfo, LPCSTR pszTitle, DWORD dwIcon, BOOL bClear)
{
	CMainFrame *pThis = (CMainFrame*) AfxGetApp ()->m_pMainWnd;

	if (bClear)
		pThis->m_strTimeoutBallon_Msgs = "";

	
	CString str = pThis->m_strTimeoutBallon_Msgs;

	
	pThis->ShowBalloon ("", "");

	
	pThis->m_strTimeoutBallon_Msgs = pszInfo;

	if (str != "")
	{
		
		pThis->m_strTimeoutBallon_Msgs += '\n';
		pThis->m_strTimeoutBallon_Msgs += str;
	}

	
	pThis->ShowBalloon (pThis->m_strTimeoutBallon_Msgs, pszTitle, dwIcon);
	
	pThis->SetTimer (3, _App.Notif_BalloonTimeout ()*1000, NULL);
}

void CMainFrame::OnDlinfobox() 
{
	if (_App.View_FloatingInfoWindow ())
	{
		m_pFloatWndsThread->m_wndFloatingInfo.TurnOffWindow ();
	}
	else
	{
		_App.View_FloatingInfoWindow (TRUE);
		_pwndDownloads->UpdateTrayIconPlusOthers ();
	}
}

void CMainFrame::RebuidDownloadsList()
{
	if (m_pFloatWndsThread)
		m_pFloatWndsThread->m_wndFloatingInfo.RebuildList ();
}

void CMainFrame::OnDllangfiles() 
{
	fsOpenUrlInBrowser ("http://www.freedownloadmanager.org/");	
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	CFrameWnd::RecalcLayout(bNotify);

	if (IsWindow (_TBMgr) == FALSE ||
			IsWindow (m_wndView) == FALSE )
		return;

	_TBMgr.AutoSize ();

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_IDEALSIZE | RBBIM_SIZE;
	int nIndex = m_wndReBar.GetReBarCtrl ().IDToIndex (1);
	CRect rc; _TBMgr.GetWindowRect (&rc);
	rbbi.cx = rbbi.cxIdeal = rc.Width ();
	VERIFY (m_wndReBar.GetReBarCtrl ().SetBandInfo (nIndex, &rbbi));

	if (::IsWindow (m_btnFDC))
		m_btnFDC.Invalidate (FALSE);

	
}

void CMainFrame::OnFDC() 
{
	fsOpenUrlInBrowser (m_Customizations.get_ButtonInfo ()->pszLinksTo);	
}

void CMainFrame::OnProceedFurherInitialization() 
{
	
	
	m_bInited = TRUE;

	_IECatchMgr.Initialize ();

	_pwndDownloads->m_ClpbrdCatch.Initialize (&_IECatchMgr); 
	_pwndDownloads->m_ClpbrdCatch.Create (_pwndDownloads);
	_pwndDownloads->m_ClpbrdCatch.Enable (_App.Monitor_Clipboard ());
	
	CDataStretcherApp* app = (CDataStretcherApp*) AfxGetApp ();
	for (int i = 0; i < app->m_vUrlsToAdd.size (); i++)
	{
		BOOL bSilent = app->m_vUrlsToAdd [i].bForceSilent ? TRUE : _App.Monitor_Silent ();
		BOOL bAdded = _pwndDownloads->CreateDownload (app->m_vUrlsToAdd [i].strUrl, FALSE, NULL, 
			NULL, bSilent);
		
		if (bAdded && bSilent)
			ShowTimeoutBalloon (app->m_vUrlsToAdd [i].strUrl, LS (L_DOWNLOADADDED), NIIF_INFO, TRUE);
	}

    for (i = 0; i < app->m_vTorrentFilesToAdd.size (); i++)
	{
		BOOL bSilent = app->m_vTorrentFilesToAdd [i].bForceSilent ? TRUE : _App.Monitor_Silent ();
		CString str = "file://"; str += app->m_vTorrentFilesToAdd [i].strUrl;
		BOOL bAdded = _pwndDownloads->CreateBtDownloadFromFile (app->m_vTorrentFilesToAdd [i].strUrl, str, bSilent);
		
		if (bAdded && bSilent)
			ShowTimeoutBalloon (app->m_vTorrentFilesToAdd [i].strUrl, LS (L_DOWNLOADADDED), NIIF_INFO, TRUE);
	}

	fsUrlsToDownloadRegKeyMgr mgr;
	mgr.CheckKey ();

	
	_pwndScheduler->GetMgr ()->ExciteEvent (EV_STARTUP);

	_DldsMgr.ProcessDownloads ();
	
	switch (_pwndDownloads->Get_DWWN ())
	{
		case DWWN_LISTOFDOWNLOADS:
			_pwndDownloads->ApplyCurrentFilter ();
			break;

		case DWWN_HISTORY:
			_pwndDownloads->ApplyHistoryCurrentFilter ();
			break;
	}

	if (SHR_NEXTSTART == _App.View_SpreadHelpDialog ())
		m_bNeedSpreadHelpDialog = true;

	if (_App.View_NOMShown () == FALSE)
	{
		_App.View_NOMShown (TRUE);
		PostMessage (WM_COMMAND, ID_OPTMASTER);
	}

	ShowSpreadHelpDialogIfRequired ();
}

void CMainFrame::OnAppAbout() 
{
	if (_DldsMgr.IsDeletingNow ())
		return;
	((CDataStretcherApp*) AfxGetApp ())->OnAppAbout ();
}

void CMainFrame::OnExportlistofdownloads() 
{
	CDlgExportDownloads dlg;
	_DlgMgr.DoModal (&dlg);
}

void CMainFrame::OnImportlistofdownloads() 
{
	CString strFilter;
	strFilter.Format ("%s (*.txt)|*.txt|%s (*.xml)|*.xml||", LS (L_URLLISTFILES), LS (L_DLINFOLISTFILES));
	CFileDialog dlg (TRUE, "txt", NULL, OFN_NOCHANGEDIR|OFN_HIDEREADONLY, strFilter, this);

	if (_DlgMgr.DoModal (&dlg) == IDCANCEL)
		return;

	if (dlg.GetFileExt ().CompareNoCase ("txt") == 0)
		ImportListOfDownloads_FromURLListFile (dlg.GetPathName ());
	else
		ImportListOfDownloads_FromDLInfoListFile (dlg.GetPathName ());
}

void CMainFrame::OnExitwhendone() 
{
	_pwndScheduler->ExitWhenDone (_pwndScheduler->ExitWhenDone () == FALSE);
}

void CMainFrame::OnUpdateExitwhendone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_pwndScheduler->ExitWhenDone () ? 1 : 0);
}

void CMainFrame::OnFind() 
{
	CDlgFind dlg;
	if (IDOK == _DlgMgr.DoModal (&dlg))
	{
		_pwndDownloads->FindDownloads_Start ();
		m_wndView.m_wndClient.m_plugs2.SetActivePlugin (0);
	}
}

void CMainFrame::OnFindnext() 
{
	m_wndView.m_wndClient.m_plugs2.SetActivePlugin (0);
	
	_pwndDownloads->FindDownloads_FindNext ();
}

void CMainFrame::OnUpdateFindnext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (_pwndDownloads->IsFindNextAvail () ? 1 : 0);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	
	
	
	
	if (_pwndDownloads->m_wndDownloads.m_info.m_video.m_wndVideo.m_player.Get_State () != VFPS_CLOSED)
	{
    		CRect rc;

		
		_pwndDownloads->m_wndDownloads.m_info.m_video.GetWindowRect(&rc);
		ScreenToClient(&rc);

		
		pDC->ExcludeClipRect(&rc);
	}

	return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::LoadToolImages()
{
	if (!m_imgsTool.Create (_TB_SIZE_X, _TB_SIZE_Y, ILC_COLOR32 | ILC_MASK, 10, 5))
		return;

	if (!m_dimgsTool.Create (_TB_SIZE_X, _TB_SIZE_Y, ILC_COLOR32 | ILC_MASK, 10, 5))
		return;

	CBitmap bmp, bmpd;

	bmp.Attach (SBMP (IDB_TOOL0));
	bmpd.Attach (SBMP (IDB_TOOL0_D));

	if (_TB_USE_ALPHA)
	{
		ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmp, GetSysColor (COLOR_3DFACE));
		ConvertBmp32WithAlphaToBmp32WithoutAlpha (bmpd, GetSysColor (COLOR_3DFACE));
	}

	m_imgsTool.Add (&bmp, RGB (255, 0, 255));
	m_dimgsTool.Add (&bmpd, RGB (255, 0, 255));
}

void CMainFrame::OnDonate() 
{
	fsOpenUrlInBrowser ("http://www.freedownloadmanager.org/donate.htm");	
}

LRESULT CMainFrame::OnShowTrayMenu(WPARAM, LPARAM)
{
	ShowTrayMenu (FALSE);
	return 0;
}

BOOL CMainFrame::ReadCusomizationInfo()
{
	CString str = ((CDataStretcherApp*)AfxGetApp ())->m_strAppPath;
	str += "fdmcs.dat";

	HANDLE hFile = CreateFile (str, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	
	if (GetFileSize (hFile, NULL) == 8)
		return TRUE;

	try {
		m_Customizations.Load (hFile);
	}catch (LPCSTR){
		return FALSE;
	}

	CloseHandle (hFile);

	return TRUE;
}

void CMainFrame::DisplayBanner(int iIndex)
{
	LOG ("CMF:DB: enter" << nl);

	vmsFDMBanner* banner = m_Customizations.get_Banner (iIndex);
	CString str = "fdmcsbr.";
	str += banner->szType;
	CString strFile = fsGetDataFilePath (str);

	LOG ("CMF:DB: file" << nl);
	
	HANDLE hFile = CreateFile (strFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD dw;
	WriteFile (hFile, banner->pbImage, banner->dwSize, &dw, NULL);
	CloseHandle (hFile);

	LOG ("CMF:DB: set" << nl);

	m_wndBanner.SetBanner (strFile, banner->pszLinksTo);
	DeleteFile (strFile);

	LOG ("CMF:DB: done." << nl);
}

void CMainFrame::SetupButton()
{
	m_btnFDC.Create (this);
	m_wndReBar.AddBar (&m_btnFDC, NULL, NULL, RBBS_FIXEDBMP|RBBS_NOGRIPPER);

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof (rbbi);
	rbbi.fMask = RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_CHILDSIZE;
	int nIndex = m_wndReBar.GetReBarCtrl ().GetBandCount ()-1;
	CRect rc; m_btnFDC.GetWindowRect (&rc);
	
	m_wndReBar.GetReBarCtrl ().GetBandInfo (nIndex, &rbbi);
	rbbi.cx = rbbi.cxIdeal = rbbi.cyMaxChild = rc.Width () + 4;
	m_wndReBar.GetReBarCtrl ().SetBandInfo (nIndex, &rbbi);
}

void CMainFrame::OnDldCreatebatch() 
{
	_pwndDownloads->OnDldcreatebatch ();	
}

vmsFDMCustomizations* CMainFrame::get_Customizations()
{
	return &m_Customizations;
}

void CMainFrame::ShowFloatingInfoWindow(BOOL bShow)
{
	m_pFloatWndsThread->m_wndFloatingInfo.NeedToShow (bShow);
}

BOOL CMainFrame::IsFloatingInfoWindowVisible()
{
	return ::IsWindowVisible (m_pFloatWndsThread->m_wndFloatingInfo.m_hWnd);
}

bool mainfrm__is_separator (char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void CMainFrame::OnFileImportImportlistofdownloadsfromclipboard() 
{
	vmsDWCD_AdditionalParameters ap;
	ap.dwMask = DWCDAP_GROUP;

	CString strURLs = _ClipbrdMgr.Text ();
	LPCSTR pszURLs = strURLs;

	DLDS_LIST v;
	vmsDownloadsGroupSmartPtr pGrp;

	while (*pszURLs)
	{
		

		while (mainfrm__is_separator (*pszURLs))
			*pszURLs++;

		CString strURL;
		
		while (*pszURLs)
		{
			if (mainfrm__is_separator (*pszURLs))
			{
				LPCSTR psz = pszURLs+1;
				while (mainfrm__is_separator (*psz))
					psz++;
				if (*psz == 0)
					break;
				if (_strnicmp (psz, "http://", 7) == 0 || 
						_strnicmp (psz, "https://", 8) == 0 ||
						_strnicmp (psz, "ftp://", 6) == 0)
				{
					
					pszURLs = psz;
					break;
				}
			}

			strURL += *pszURLs++;
		}

		
		if (strURL != "")
		{
			if (pGrp == NULL)
			{
				CDlg_ChooseGroup dlg;
				if (IDOK != _DlgMgr.DoModal (&dlg))
					return;
				pGrp = dlg.m_pGroup;
			}
		
			vmsDownloadSmartPtr dld;
			Download_CreateInstance (dld);

			if (FALSE == _pwndDownloads->CreateDownloadWithDefSettings (dld, strURL))
				continue;

			dld->pGroup = pGrp;

			v.push_back (dld);
		}
	}

	if (v.size () != 0)
		_pwndDownloads->CreateDownloads (v, NULL);
}

void CMainFrame::OnOptionsSm() 
{
	CSiteManagerSheet sheet (LS (L_OPTIONS_SM), this);
	_DlgMgr.DoModal (&sheet);
}

int CMainFrame::get_SkinsMenuPos()
{
	return GetMenu ()->GetSubMenu (1)->GetMenuItemCount () - 2;
}

void CMainFrame::BuildSkinsMenu()
{
	CMenu *menu = GetMenu ()->GetSubMenu (1);
	menu = menu->GetSubMenu (get_SkinsMenuPos ());

	for (int i = 0; i < _SkinMgr.get_Count (); i++)
	{
		CString str = _SkinMgr.get_Skin (i)->strName;
		if (str.CompareNoCase ("Old Style") == 0)
			str = LS (L_OLDSTYLE);
		m_odmenu.InsertMenuItem (menu, str, ID_SKIN_0+1+i, i+2, TRUE);
	}
}

void CMainFrame::OnSkin(UINT nID)
{
	int nSkin = nID - ID_SKIN_0 - 1;
	
	CString str = nSkin == -1 ? "" : _SkinMgr.get_Skin (nSkin)->strName;
	if (str == LS (L_OLDSTYLE))
		str = "Old Style";
	_App.Skin_Current (str);

	if (nSkin != _SkinMgr.get_Current ())
		MessageBox (LS (L_RESTARTISREQ));
}

void CMainFrame::OnUpdateSkin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (_SkinMgr.get_Current () == (int)pCmdUI->m_nID - ID_SKIN_0 - 1);
}

BOOL CMainFrame::is_MouseInTray()
{
	POINT pt;
	return GetCursorPos (&pt) && abs (pt.x-m_balloonX) < 3 && abs (pt.y-m_balloonY) < 3;
}

void CMainFrame::OnSpreadhelp() 
{
	m_bDontShowSHD = true;

	CDlg_SpreadHelp2 dlg (this);
	_DlgMgr.DoModal (&dlg);

	FILETIME ft; SYSTEMTIME st;
	GetLocalTime (&st);
	SystemTimeToFileTime (&st, &ft);
	_App.View_SpreadHelpDialog_LastTimeShown_save (ft);

	m_bDontShowSHD = false;
}

void CMainFrame::OnTUMChanged()
{
	CDlg_TUMChanged dlg;
	_DlgMgr.DoModal (&dlg);
}

BOOL CMainFrame::ImportListOfDownloads_FromURLListFile(LPCSTR pszFile)
{
	CStdioFile file;
	
	if (FALSE == file.Open (pszFile, CFile::modeRead))
	{
		MessageBox (LS (L_CANTOPENFILE), LS (L_ERR), MB_ICONERROR);
		return FALSE;
	}

	DLDS_LIST v;
	vmsDownloadsGroupSmartPtr pGrp;

	CString strUrl;
	do
	{
		file.ReadString (strUrl);

		if (strUrl != "")
		{
			if (pGrp == NULL)
			{
				CDlg_ChooseGroup dlg;
				if (IDOK != _DlgMgr.DoModal (&dlg))
					return FALSE;
				pGrp = dlg.m_pGroup;
			}

			vmsDownloadSmartPtr dld;
			Download_CreateInstance (dld);

			if (FALSE == _pwndDownloads->CreateDownloadWithDefSettings (dld, strUrl))
				continue;

			if (dld->pGroup != pGrp)
			{
				dld->pGroup = pGrp;
				LPSTR psz = new char [pGrp->strOutFolder.Length () + 1];
				lstrcpy (psz, pGrp->strOutFolder);
				delete [] dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName;
				dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName = psz;
			}

			v.push_back (dld);
		}
	}
	while (strUrl != "");

	if (v.size () != 0)
		_pwndDownloads->CreateDownloads (v, NULL);

	return TRUE;
}

BOOL CMainFrame::ImportListOfDownloads_FromDLInfoListFile(LPCSTR pszFile)
{
	IXMLDOMDocumentPtr spXML;
	IXMLDOMNodePtr spNode;

	spXML.CreateInstance (__uuidof (DOMDocument));

	if (spXML == NULL)
		return FALSE;

	spXML->put_async (FALSE);

	VARIANT_BOOL bRes;
	spXML->load (COleVariant (pszFile), &bRes);
	if (bRes == FALSE)
		return FALSE;

	spXML->selectSingleNode (L"FDM_Downloads_Info_List", &spNode);
	if (spNode == NULL)
		return FALSE;

	IXMLDOMNodeListPtr spNodeList;
	spNode->get_childNodes (&spNodeList);

	if (spNodeList == NULL)
		return FALSE;

	IXMLDOMNodePtr spItem;

	DLDS_LIST v;

	while (SUCCEEDED (spNodeList->nextNode (&spItem)) && spItem != NULL)
	{
		CComBSTR bstrName;
		spItem->get_nodeName (&bstrName);

		if (bstrName == L"download")
			ImportDownload (spItem, v);

		spItem = NULL;
	}

	if (v.size () != 0)
		_pwndDownloads->CreateDownloads (v, NULL);

	return TRUE;
}

BOOL CMainFrame::ImportDownload(IXMLDOMNode *pNode, DLDS_LIST_REF v)
{
	IXMLDOMNodeListPtr spNodeList;
	pNode->get_childNodes (&spNodeList);

	if (spNodeList == NULL)
		return FALSE;

	IXMLDOMNodePtr spItem;

	vmsDWCD_AdditionalParameters params;
	params.dwMask = 0;
	CString strUrl;
	CString strComment;

	while (SUCCEEDED (spNodeList->nextNode (&spItem)) && spItem != NULL)
	{
		CComBSTR bstrName;
		spItem->get_nodeName (&bstrName);

		if (bstrName == L"url")
		{
			COleVariant vtUrl;
			spItem->get_nodeTypedValue (&vtUrl);
			if (vtUrl.vt == VT_BSTR)
				strUrl = vtUrl.bstrVal;
		}
		else if (bstrName == L"comment")
		{
			COleVariant vt;
			spItem->get_nodeTypedValue (&vt);
			if (vt.vt == VT_BSTR)
				strComment = vt.bstrVal;
		}
		else if (bstrName == L"group")
		{
			COleVariant vt;
			spItem->get_nodeTypedValue (&vt);
			if (vt.vt == VT_BSTR)
			{
				CString str = vt.bstrVal;
				params.pGroup = _DldsGrps.FindGroupByName (str);
				if (params.pGroup == NULL)
					params.pGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);
				params.dwMask |= DWCDAP_GROUP;
			}
		}

		spItem = NULL;
	}

	if (strUrl == "")
		return FALSE;

	vmsDownloadSmartPtr dld;
	Download_CreateInstance (dld);

	if (FALSE == _pwndDownloads->CreateDownloadWithDefSettings (dld, strUrl))
		return FALSE;

	if (params.dwMask & DWCDAP_GROUP)
	{
		if (dld->pGroup != params.pGroup)
		{
			dld->pGroup = params.pGroup;
			LPSTR psz = new char [params.pGroup->strOutFolder.Length () + 1];
			lstrcpy (psz, params.pGroup->strOutFolder);
			delete [] dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName;
			dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName = psz;
		}
	}
	
	dld->strComment = strComment;

	v.push_back (dld);

	return TRUE;
}

void CMainFrame::OnExportdlhist() 
{
	CDlgExportHistory dlg;
	_DlgMgr.DoModal (&dlg);
}

void CMainFrame::OnUpdateExportdlhist(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (_DldsMgr.m_histmgr.GetRecordCount () != 0);	
}

bool CMainFrame::ShowSpreadHelpDialogIfRequired()
{
	if (m_bDontShowSHD)
		return false;

	if (FALSE == IsWindowVisible ())
		return false;

	bool bShow = false;

	if (m_bNeedSpreadHelpDialog)
	{
		m_bNeedSpreadHelpDialog = false;
		bShow = true;
	}

	if (bShow == false)
	{
		vmsSpreadHelpRemind en = _App.View_SpreadHelpDialog ();
		if (en != SHR_IN3DAYS && en != SHR_WAIT1STTIME)
			return false;

		if (en == SHR_WAIT1STTIME)
		{
			
			if (10 > _App.NumberOfDlCreatesByUser ())
				return false;
		}

		FILETIME ft1, ft2;

		if (_App.View_SpreadHelpDialog_LastTimeShown (ft1))
		{
			SYSTEMTIME st;
			GetLocalTime (&st);
			SystemTimeToFileTime (&st, &ft2);

			UINT cDays = en == SHR_WAIT1STTIME ? 5 : 3;

			if (fsGetTimeDelta (&ft2, &ft1) >= cDays*24*3600)
				bShow = true;
		}
		else
		{
			SYSTEMTIME st;
			FILETIME ft;
			GetLocalTime (&st);
			SystemTimeToFileTime (&st, &ft);
			_App.View_SpreadHelpDialog_LastTimeShown_save (ft);
		}
	}

	if (bShow)
	{
		m_bDontShowSHD = true;
		PostMessage (WM_COMMAND, ID_SPREADHELP);
	}

	return bShow;
}

void CMainFrame::OnUpdateLdf0(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (_DldsMgr.get_LastFilesDownloaded ()->get_Count () != 0);	
}

void CMainFrame::OnLDF(UINT nID)
{
	ShellExecute (NULL, "open", _DldsMgr.get_LastFilesDownloaded ()->get_FilePathName (nID - ID_LDF_0), 
		NULL, NULL, SW_SHOW);
}

void CMainFrame::OnLdfClear() 
{
	_DldsMgr.LastFilesDownloaded_Clear ();	
}

void CMainFrame::OnUpdateLdfClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (_DldsMgr.get_LastFilesDownloaded ()->get_Count () != 0);		
}

void CMainFrame::OnPausealldlds() 
{
	fsInternetDownloader::set_PauseMode (!fsInternetDownloader::is_PauseMode ());
	_BT.ApplyRestrainAllDownloadsMode ();
}

void CMainFrame::OnUpdatePausealldlds(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (fsInternetDownloader::is_PauseMode ());	
}

void CMainFrame::OnUpdateTrafficThisMonth(CCmdUI *pCmdUI)
{
	try {
	vmsBtSession *pS = _BT.is_Initialized () ? _BT.get_Session () : NULL;
	UINT64 u = pS ? pS->get_TotalDownloadedByteCount () : 0;
	UINT64 u2 = pS ? pS->get_TotalUploadedByteCount () : 0;
	CString str; 
	str.Format ("%s; %s", 
		BytesToString (m_nTrafficUsedThisMonth + fsInternetDownloader::get_TotalTraffic () + u),
		BytesToString (m_nTrafficUpldThisMonth + u2));
	pCmdUI->SetText (str);
	
	CString str2;
	str2.Format ("%s; %s", BytesToString (m_nTrafficUsedPrevMonth), BytesToString (m_nTrafficUpldPrevMonth));
	str.Format (LS (L_TRAFF_USED_SBTEXT), str2);
	m_wndStatusBar.SetToolTipText (ID_SB_TRAFFIC_THISMONTH, str);
	}
	catch (...) {}
}

void CMainFrame::OnUpdateTotalSpeed(CCmdUI *pCmdUI)
{
	vmsBtSession *pS = _BT.is_Initialized () ? _BT.get_Session () : NULL;
	UINT64 u = pS ? pS->get_TotalUploadSpeed () : 0;

	CString str;
	str.Format ("%s/%s; %s/%s", BytesToString (_pwndDownloads->get_TotalSpeed ()),
		LS (L_S), BytesToString (u), LS (L_S));
	pCmdUI->SetText (str);
}

void CMainFrame::ReadTotalTrafficInfo()
{
	m_nTrafficUsedThisMonth = _App.TotalTraffic ();
	m_nTrafficUsedPrevMonth = _App.TotalTraffic_PrevMonth ();
	m_nTrafficUpldThisMonth = _App.TotalUpldTraffic ();
	m_nTrafficUpldPrevMonth = _App.TotalUpldTraffic_PrevMonth ();
	m_nTotalTraffic_CurrMonth = _App.TotalTraffic_CurrMonth ();
	
	TotalTraffic_CheckMonthChange ();
	
	
	SetTimer (6, 1000, NULL);
}

void CMainFrame::TotalTraffic_CheckMonthChange()
{
	SYSTEMTIME time;
	GetLocalTime (&time);

	if (m_nTotalTraffic_CurrMonth == -1) 
		_App.TotalTraffic_CurrMonth (m_nTotalTraffic_CurrMonth = time.wMonth);

	if (m_nTotalTraffic_CurrMonth != time.wMonth)
	{
		m_nTrafficUsedPrevMonth = m_nTrafficUsedThisMonth;
		m_nTrafficUpldPrevMonth = m_nTrafficUpldThisMonth;

		_App.TotalTraffic (m_nTrafficUsedThisMonth = 0);
		_App.TotalUpldTraffic (m_nTrafficUpldThisMonth = 0);
		_App.TotalTraffic_PrevMonth (m_nTrafficUsedPrevMonth);
		_App.TotalUpldTraffic_PrevMonth (m_nTrafficUpldPrevMonth);
		_App.TotalTraffic_CurrMonth (m_nTotalTraffic_CurrMonth = time.wMonth);
	}
}

void CMainFrame::OnUpdateDlinfobox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_App.View_FloatingInfoWindow () ? 1 : 0);	
}

void CMainFrame::OnAppExit2() 
{
	_DlgMgr.EndAllDialogs ();
	PostMessage (WM_CLOSE); 
}

void CMainFrame::OnPortableFdm() 
{
	CDlg_MakePortableVer dlg;
	_DlgMgr.DoModal (&dlg);	
}

void CMainFrame::SetActivePlugin(int nIndex)
{
	m_wndView.m_wndClient.m_plugs2.SetActivePlugin (nIndex);
}

LRESULT CMainFrame::OnPowerBroadCast(WPARAM wParam, LPARAM)
{
	if (wParam == PBT_APMRESUMESUSPEND)
	{
		_pwndScheduler->GetMgr ()->RepairTasksNextTimes ();
		_pwndScheduler->m_wndTasks.Invalidate ();
	}
	return TRUE;
}

void CMainFrame::OnUpldroptions() 
{
	if (_pwndUploads && _pwndUploads->m_pwndUploads)
		_pwndUploads->m_pwndUploads->ShowProperties ();
}

void CMainFrame::OnLoadatstartup() 
{
	_AutorunMgr.SetAutoStart (!_AutorunMgr.IsAutoStart ());
}

void CMainFrame::OnUpdateLoadatstartup(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_AutorunMgr.IsAutoStart ());	
}  

int CMainFrame::GetTumMenuPosition()
{
	return _pwndUploads && _pwndUploads->m_pwndUploads ? 10 : 9;
}
