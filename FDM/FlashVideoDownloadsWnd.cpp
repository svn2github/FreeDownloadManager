/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/      

#include "stdafx.h"
#include "FdmApp.h"
#include "FlashVideoDownloadsWnd.h"
#include "plugincmds.h"
#include "Dlg_CreateFVDownload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlashVideoDownloadsWnd *_pwndFVDownloads = NULL;      

CFlashVideoDownloadsWnd::CFlashVideoDownloadsWnd()
{
	m_wndPreview.set_InFVDownloads (TRUE);
}

CFlashVideoDownloadsWnd::~CFlashVideoDownloadsWnd()
{
}  

BEGIN_MESSAGE_MAP(CFlashVideoDownloadsWnd, CWnd)
	//{{AFX_MSG_MAP(CFlashVideoDownloadsWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FVDLD_CREATE, OnFvdldCreate)
	ON_COMMAND(ID_FVDLD_AUTOSTART, OnFvdldAutostart)
	ON_COMMAND(ID_FVDLD_DELETE, OnFvdldDelete)
	ON_COMMAND(ID_FVDLD_LAUNCH, OnFvdldLaunch)
	ON_COMMAND(ID_FVDLD_OPENFOLDER, OnFvdldOpenfolder)
	ON_COMMAND(ID_FVDLD_PASSTODLDS, OnFvdldPasstodlds)
	ON_COMMAND(ID_FVDLD_PROPERTIES, OnFvdldProperties)
	ON_COMMAND(ID_FVDLD_START, OnFvdldStart)
	ON_COMMAND(ID_FVDLD_STOP, OnFvdldStop)
	ON_COMMAND(ID_FVDLD_CONVERT, OnFvdldConvert)
	//}}AFX_MSG_MAP

	
	ON_MESSAGE (WM_WGP_SHUTDOWN, OnAppExit)
	ON_MESSAGE (WM_WGP_READYTOSHUTDOWN, OnAppQueryExit)
	ON_MESSAGE (WM_WGP_UPDATE_MENUMAIN, OnUpdateMenu)
	ON_MESSAGE (WM_WGP_UPDATE_MENUVIEW, OnUpdateMenuView)
	ON_MESSAGE (WM_WGP_UPDATE_CMDITEM, OnUpdateToolBar)
	ON_MESSAGE (WM_WGP_INSERT, OnInsert)

	ON_COMMAND_RANGE(ID_DLLIST_1, ID_DLLIST_8, OnDLListShowCol)

	

END_MESSAGE_MAP()        

BOOL CFlashVideoDownloadsWnd::Create(CWnd *pwndParent)
{
	CRect rc (0, 0, 50, 50);

	if (FALSE == CWnd::Create (AfxRegisterWndClass (0, NULL,
			NULL, NULL), NULL, WS_CHILD | WS_VISIBLE, rc, pwndParent, 0x1af))
		return FALSE;

	return TRUE;
}

HWND CFlashVideoDownloadsWnd::Plugin_CreateMainWindow(HWND hWndParent)
{
	fsnew1 (_pwndFVDownloads, CFlashVideoDownloadsWnd);

	_pwndFVDownloads->Create (CWnd::FromHandle (hWndParent));

	return _pwndFVDownloads->m_hWnd;
}

void CFlashVideoDownloadsWnd::Plugin_GetToolBarInfo(wgTButtonInfo **ppButtons, int *pcButtons)
{
	static wgTButtonInfo btns [] = 
	{
		wgTButtonInfo (ID_FVDLD_CREATE, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_FVDLD_START, TBSTYLE_BUTTON, ""),
		wgTButtonInfo (ID_FVDLD_STOP, TBSTYLE_BUTTON, ""),
	};

	btns [0].pszToolTip = LS (L_NEWDLD);
	btns [1].pszToolTip = LS (L_STARTDLDS);
	btns [2].pszToolTip = LS (L_STOPDLDS);

	*ppButtons = btns;
	*pcButtons = sizeof (btns) / sizeof (wgTButtonInfo);
}

void CFlashVideoDownloadsWnd::Plugin_GetMenuImages(fsSetImage **ppImages, int *pcImages)
{
	static fsSetImage images [] = 
	{
		fsSetImage (ID_FVDLD_CREATE, 0),
		fsSetImage (ID_FVDLD_START, 1),
		fsSetImage (ID_FVDLD_STOP, 2),
		fsSetImage (ID_FVDLD_PROPERTIES, 6),
		fsSetImage (ID_FVDLD_DELETE, 7),
	};

	*ppImages = images;
	*pcImages = sizeof (images) / sizeof (fsSetImage);
}

void CFlashVideoDownloadsWnd::Plugin_GetMenuViewItems(wgMenuViewItem **ppItems, int* )
{
	*ppItems = NULL;
}

void CFlashVideoDownloadsWnd::Plugin_GetPluginNames(LPCSTR *ppszLong, LPCSTR *ppszShort)
{
	static CString strName;
	strName = LSNP (L_FVDOWNLOADS);
	*ppszLong = *ppszShort = strName;
}

void CFlashVideoDownloadsWnd::Plugin_SetLanguage(wgLanguage, HMENU hMenuMain, HMENU hMenuView)
{
	CMenu *menu = CMenu::FromHandle (hMenuMain);
	_pwndFVDownloads->m_wndTasks.ApplyLanguageToMenu (menu);
	_pwndFVDownloads->m_wndTasks.ApplyLanguage ();
	_pwndFVDownloads->m_wndPreview.ApplyLanguage ();
	_pwndFVDownloads->ApplyLanguageToMenuView (CMenu::FromHandle (hMenuView));
}

int CFlashVideoDownloadsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndTasks.Create (this);
	m_wndPreview.Create (this);

	m_splitter.Create (AfxGetInstanceHandle (), m_hWnd, WST_HORIZONTAL);
	m_splitter.SetWnd1 (m_wndTasks.m_hWnd);
	m_splitter.SetWnd2 (m_wndPreview.m_hWnd);
	m_splitter.SetMinDimensions (50, 50);
	float fRatio = _App.View_SplitterRatio ("FVDownloads_DL_PREVIEW");
	if (fRatio < 0.1f)
		fRatio = 0.1f;
	m_splitter.SetRatio (fRatio);

	m_wndPreview.ShowWindow (SW_SHOW);

	for (size_t i = 0; i < _DldsMgr.GetCount (); i++)
	{
		if (_DldsMgr.GetDownload (i)->dwFlags & DLD_FLASH_VIDEO)
			m_wndTasks.AddDownload (_DldsMgr.GetDownload (i));
	}
	
	return 0;
}

void CFlashVideoDownloadsWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	m_splitter.ApplyRatio ();

	
}

void CFlashVideoDownloadsWnd::AddDownload(vmsDownloadSmartPtr dld)
{
	ASSERT (dld->dwFlags & DLD_FLASH_VIDEO);
	m_wndTasks.AddDownload (dld);
}

void CFlashVideoDownloadsWnd::OnFvdldCreate() 
{
	m_wndTasks.OnFvdldCreate ();
}

void CFlashVideoDownloadsWnd::OnDownloadDone(vmsDownloadSmartPtr dld)
{
	
}

LRESULT CFlashVideoDownloadsWnd::OnInsert(WPARAM, LPARAM)
{
	m_wndTasks.OnFvdldCreate ();
	return 0;
}

LRESULT CFlashVideoDownloadsWnd::OnAppExit(WPARAM, LPARAM)
{
	SaveAll ();
	return 0;
}

LRESULT CFlashVideoDownloadsWnd::OnAppQueryExit(WPARAM, LPARAM)
{
	return 0;
}

LRESULT CFlashVideoDownloadsWnd::OnUpdateMenu(WPARAM, LPARAM lp)
{
	m_wndTasks.UpdateMenu (CMenu::FromHandle (*((HMENU*)lp)));
	return 0;
}

LRESULT CFlashVideoDownloadsWnd::OnUpdateMenuView(WPARAM, LPARAM lp)
{
	HMENU* pMenus = (HMENU*) lp;
	CMenu* menu = CMenu::FromHandle (pMenus [0]);

	BOOL bEn = m_wndTasks.GetHeaderCtrl ()->GetItemCount () > 1;
	for (int i = 0; i < 8; i++)
	{
		if (m_wndTasks.IsColumnShown (i))
		{
			menu->CheckMenuItem (ID_DLLIST_1+i, MF_CHECKED);
			if (bEn == FALSE)
				menu->EnableMenuItem (ID_DLLIST_1+i, MF_GRAYED);
		}
	}

	return 0;
}

LRESULT CFlashVideoDownloadsWnd::OnUpdateToolBar(WPARAM wp, LPARAM)
{
	return m_wndTasks.OnUpdateToolBar (wp);	
}

void CFlashVideoDownloadsWnd::ApplyLanguageToMenuView(CMenu *menu)
{
	menu->ModifyMenu (0, MF_BYPOSITION | MF_STRING, 0, LS (L_LISTOFDLDS));

	UINT aCmds [] = {ID_DLLIST_1, ID_DLLIST_2, ID_DLLIST_3, ID_DLLIST_4, ID_DLLIST_5, 
		ID_DLLIST_6, ID_DLLIST_7, ID_DLLIST_8 };
	LPCSTR apszCmds [] = {LS (L_FILENAME), LS (L_SIZE), LS (L_DOWNLOADED),
		LS (L_TIMELEFT), LS (L_SECTIONS), LS (L_SPEED), LS (L_COMMENT), LS (L_ADDED) };
	
	for (int i = 0; i < sizeof (aCmds) / sizeof (UINT); i++)
		menu->ModifyMenu (aCmds [i], MF_BYCOMMAND|MF_STRING, aCmds [i], apszCmds [i]);
}

void CFlashVideoDownloadsWnd::OnDLListShowCol(UINT nCmd)
{
	int iCol = nCmd - ID_DLLIST_1;
	m_wndTasks.ChangeColumnVisibility (iCol);
}

void CFlashVideoDownloadsWnd::OnFvdldAutostart() 
{
	m_wndTasks.OnFvdldAutostart ();
}

void CFlashVideoDownloadsWnd::OnFvdldDelete() 
{
	m_wndTasks.OnFvdldDelete ();	
}

void CFlashVideoDownloadsWnd::OnFvdldLaunch() 
{
	m_wndTasks.OnFvdldLaunch ();	
}

void CFlashVideoDownloadsWnd::OnFvdldOpenfolder() 
{
	m_wndTasks.OnFvdldOpenfolder ();	
}

void CFlashVideoDownloadsWnd::OnFvdldPasstodlds() 
{
	m_wndTasks.OnFvdldPasstodlds ();	
}

void CFlashVideoDownloadsWnd::OnFvdldProperties() 
{
	m_wndTasks.OnFvdldProperties ();	
}

void CFlashVideoDownloadsWnd::OnFvdldStart() 
{
	m_wndTasks.OnFvdldStart ();	
}

void CFlashVideoDownloadsWnd::OnFvdldStop() 
{
	m_wndTasks.OnFvdldStop ();	
}

void CFlashVideoDownloadsWnd::SaveAll()
{
	ASSERT (::IsWindow (m_hWnd));

	m_wndTasks.SaveState ();
	_App.View_SplitterRatio ("FVDownloads_DL_PREVIEW", m_splitter.GetRatio ());
}

void CFlashVideoDownloadsWnd::SetActiveDownload(vmsDownloadSmartPtr dld)
{
	m_wndPreview.Set_ActiveDownload (dld);
}

void CFlashVideoDownloadsWnd::OnFvdldConvert() 
{
	m_wndTasks.OnFvdldConvert ();	
}

HMENU CFlashVideoDownloadsWnd::Plugin_GetMainMenu()
{
	return LoadMenu (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDM_FVDOWNLOADS));
}

HMENU CFlashVideoDownloadsWnd::Plugin_GetViewMenu()
{
	return LoadMenu (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDM_FVDOWNLOADS_VIEW));
}

BOOL CFlashVideoDownloadsWnd::CreateDownload(LPCSTR pszUrl)
{
	CDlg_CreateFVDownload dlg;
	if (pszUrl)
		dlg.m_strUrl = pszUrl;
	if (IDOK != _DlgMgr.DoModal (&dlg))
		return FALSE;

	vmsDWCD_AdditionalParameters ap;
	ap.dwMask = DWCDAP_FLAGS;
	ap.dwFlags = DWDCDAP_F_FLASHVIDEODOWNLOAD;

	if (dlg.m_bAutoConvert)
	{
		ap.dwMask |= DWCDAP_MEDIA_CONVERT_SETTINGS;
		ap.stgsMediaConvert = dlg.m_stgsAutoConvert;
	}

	

	return _pwndDownloads->CreateDownload (dlg.m_strUrl, FALSE, NULL, NULL, 
		dlg.m_bSetupAdvStgs == FALSE, DWCD_NOFORCEAUTOLAUNCH, NULL, &ap);
}
