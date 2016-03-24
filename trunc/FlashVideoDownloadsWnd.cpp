/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "FlashVideoDownloadsWnd.h"
#include "plugincmds.h"
#include "Dlg_CreateFVDownload.h"
#include "DlgCreateFlvDownloads.h"
#include "DlgCreateFlvDownloadsByWebPageUrl.h"
#include "WaitDlg.h"
#include "vmsSharedData.h"
#include "MainFrm.h"
#include "FdmUiWindow.h"
#include "vmsYouTubeParserDllMgr.h"
#include "vmsFdmURLDownloadMgr.h"
#include "vmsYouTubeParserErrorSenderPOST.h"

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
	

	ON_MESSAGE (WM_FVDW_CREATEDOWNLOADS, OnFvdwCreateDownloads)
	ON_WM_DESTROY()
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
		wgTButtonInfo (ID_FVDLD_CREATE, TBSTYLE_BUTTON, _T("")),
		wgTButtonInfo (ID_FVDLD_START, TBSTYLE_BUTTON, _T("")),
		wgTButtonInfo (ID_FVDLD_STOP, TBSTYLE_BUTTON, _T("")),
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

void CFlashVideoDownloadsWnd::Plugin_GetPluginNames(LPCTSTR *ppszLong, LPCTSTR *ppszShort)
{
	static CString strName;
	strName = LSNP (L_FVDOWNLOADS).c_str ();
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
	float fRatio = _App.View_SplitterRatio (_T("FVDownloads_DL_PREVIEW"));
	if (fRatio < 0.1f)
		fRatio = 0.1f;
	m_splitter.SetRatio (fRatio);

	m_wndPreview.ShowWindow (SW_SHOW);

	DLDS_LIST v;

	_DldsMgr.LockList (true);
	size_t nCount = _DldsMgr.GetCount ();
	for (size_t i = 0; i < nCount; i++)
	{
		if (_DldsMgr.GetDownload (i)->dwFlags & DLD_FLASH_VIDEO)
			v.push_back (_DldsMgr.GetDownload (i));
	}
	_DldsMgr.UnlockList (true);

	for (size_t i = 0; i < v.size (); i++)
		m_wndTasks.AddDownload (v [i], TRUE);
	
	return 0;
}

void CFlashVideoDownloadsWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	m_splitter.ApplyRatio ();

	
}

void CFlashVideoDownloadsWnd::AddDownload(vmsDownloadSmartPtr dld, BOOL bPlaceToTop)
{
	ASSERT (dld->dwFlags & DLD_FLASH_VIDEO);
	m_wndTasks.AddDownload (dld, bPlaceToTop);
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
	SaveAll (0xffffffff);
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
	LPCTSTR apszCmds [] = {LS (L_FILENAME), LS (L_SIZE), LS (L_DOWNLOADED),
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

void CFlashVideoDownloadsWnd::SaveAll(DWORD dwWhat)
{
	if (dwWhat & 2)
	{
		ASSERT (::IsWindow (m_hWnd));
		m_wndTasks.SaveState ();
		_App.View_SplitterRatio (_T("FVDownloads_DL_PREVIEW"), m_splitter.GetRatio ());
	}
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

BOOL CFlashVideoDownloadsWnd::CreateDownload(LPCTSTR pszUrl, bool bTopMostErrMessages)
{
	DWORD dwMB = bTopMostErrMessages ? MB_SYSTEMMODAL : 0;

	CString strUrl;

	if (pszUrl == NULL || *pszUrl == 0)
	{
		CDlgCreateFlvDownloadsByWebPageUrl dlg;
		if (IDOK != AfxGetApp ()->m_pMainWnd->SendMessage (WM_DOMODAL, 0, (LPARAM)&dlg))
			return FALSE;
		strUrl = dlg.m_strWebPageUrl;
	}
	else
	{
		strUrl = pszUrl;
	}

	auto dllmgr = vmsYouTubeParserDllMgr::get ();
	assert (dllmgr);
	auto parser = dllmgr->objectsHolder ()->createResourceObject ((LPCTSTR)strUrl);
	assert (parser);
	if (parser && parser->object ()->IsYouTubeVideoPage ((LPCTSTR)strUrl))
	{
		bool cancelled_by_user;
		if (CreateDownloadFromYtParser (strUrl, parser->object (), 
			bTopMostErrMessages, cancelled_by_user))
		{
			return !cancelled_by_user;
		}
	}

	if (_App.FlvMonitoring_Enable () == FALSE)
	{
		MessageBox (LS (L_ENABLE_FLVMONITORING_FIRST), NULL, MB_ICONEXCLAMATION | dwMB);
		return FALSE;
	}
	
	CWaitDlg dlgWait;
	HRESULT hrRes;
	dlgWait.StartWaiting (LS (L_SEARCHING_FLVS), _threadPassWebPageUrlToFlvSniffDll, FALSE, (LPVOID)(LPCTSTR)strUrl, (LPVOID)&hrRes);

	if (hrRes != S_OK)
	{
		switch (hrRes)
		{
		case S_FALSE:
			MessageBox (LS (L_NOFLVSFOUND), NULL, MB_ICONEXCLAMATION | dwMB);
			break;

		case E_INVALIDARG:
			MessageBox (LS (L_FLVWEBPAGENOTFOUND), NULL, MB_ICONEXCLAMATION | dwMB);
			break;

		default:
			CString str;
			str.Format (LS (L_ERRGETFLVDLDS), hrRes);
			MessageBox (str, LS (L_ERROR), MB_ICONERROR | dwMB);
			break;
		}
	}

	return hrRes == S_OK;
}

LRESULT CFlashVideoDownloadsWnd::OnFvdwCreateDownloads(WPARAM, LPARAM lp)
{
	std::unique_ptr <WmFvdwLparam> lParam ((WmFvdwLparam*)lp);
	ASSERT (lParam != NULL && lParam->pvDlds != NULL);
	if (lParam == NULL || lParam->pvDlds == NULL)
		return 1;

	LRESULT result = 0;

	if (!CreateFlvDownloads (nullptr, lParam->strSrcWebPageUrl, 
		std::move (lParam->pvDlds), nullptr, lParam->pUiWindow, true))
	{
		result = 2;
	}

	lParam->pUiWindow->Release ();

	return result;
}

DWORD WINAPI CFlashVideoDownloadsWnd::_threadPassWebPageUrlToFlvSniffDll(LPVOID lp)
{
	ProcWaitInfo *info = (ProcWaitInfo*) lp;
	LPTSTR pszUrl = (LPTSTR)info->lpParam1;
	HRESULT *phrRes = (HRESULT*)info->lpParam2;

	*phrRes = S_FALSE;

	vmsSharedData sdata (_T("Fdm::mem::passUrlToFlvSniffDll"), FALSE, _tcslen (pszUrl)+1);
	LPTSTR psz = (LPTSTR)sdata.getData ();
	ASSERT (psz != NULL);
	if (!psz)
	{
		info->bWaitDone = TRUE;
		return 0;
	}
	_tcscpy (psz, pszUrl);
	
	HWND hwnd = NULL;
	
	std::vector <HWND> vWnds;
	
	do 
	{
		hwnd = ::FindWindowEx (NULL, hwnd, _T("FdmFlvSniffDllWnd"), NULL);
		if (hwnd)
			vWnds.push_back (hwnd);
	}
	while (hwnd);

	info->iProgress = 0;

	fsSetForegroundWindow (info->hwndDlg);

	for (size_t i = 0; i < vWnds.size (); i++)
	{
		HRESULT hr = ::SendMessage (vWnds [i], WM_COMMAND, 0, 0);

		if (info)
		{
			info->iProgress = 100 * (i+1) / vWnds.size ();

			if (hr == S_OK)
			{
				*phrRes = S_OK;	
				info->bWaitDone = true; 
				info = NULL;
			}
			else
			{
				if (*phrRes != S_OK)
					*phrRes = hr;
			}
		}
	}

	if (info)
		info->bWaitDone = true;

	return 0;
}

void CFlashVideoDownloadsWnd::OnDestroy()
{
	CWnd::OnDestroy();

	SAFE_DELETE (_pwndFVDownloads);
}

bool CFlashVideoDownloadsWnd::CreateFlvDownloads(CWnd *parentWindow, const CString &strSrcWebPageUrl, 
	std::unique_ptr <std::vector <vmsNewDownloadInfo>> pvDlds, 
	std::unique_ptr <std::vector <bool>> selectedDownloads,
	CFdmUiWindow *pUiWindow, bool bReqTopMostDialog,
	CDlgCreateFlvDownloads::ui_customizations *dlg_customizations)
{
	std::map <CString, size_t> urlToIndex;

	CDlgCreateFlvDownloads dlg (parentWindow, pUiWindow);

	dlg.m_strWebPageUrl = strSrcWebPageUrl;
	dlg.m_bReqTopMostDialog = bReqTopMostDialog;

	if (dlg_customizations)
		dlg.m_ui_customizations = *dlg_customizations;

	std::set <CString> linkedAudioUrls;

	for (size_t i = 0; i < pvDlds->size (); i++)
	{
		vmsNewDownloadInfo &dlInfo = pvDlds->at (i);

		urlToIndex [dlInfo.strUrl] = i;

		if (!dlInfo.ap.strFileName.IsEmpty ())
		{
			
			LPTSTR psz = dlInfo.ap.strFileName.GetBuffer (0);
			TCHAR szSymbls [] = {_T(":*?\"<>|\\/")};
			while (*psz) 
			{
				if (_tcschr (szSymbls, *psz))
					*psz = _T('_');
				psz++;
			}
			dlInfo.ap.strFileName.ReleaseBuffer ();
		}		

		CDlgCreateFlvDownloads::FlvDownload dld;

		dld.strTitle = dlInfo.ap.strFileName;
		dld.strUrl = dlInfo.strUrl;
		dld.uSize = dlInfo.ap.dwMask & DWCDAP_FILESIZE ? dlInfo.ap.uFileSize : _UI64_MAX;

		if (dlInfo.additionalDownload)
		{
			dld.strLinkedAudioUrl = dlInfo.additionalDownload->strUrl;
			linkedAudioUrls.insert (dlInfo.additionalDownload->strUrl);
		}

		dlg.m_vDlds.push_back (dld);
	}

	if (selectedDownloads)
		dlg.m_selected.swap (*selectedDownloads);

	UINT nRes = _DlgMgr.DoModal (&dlg);

	if (nRes != IDOK)
		return false;

	for (size_t i = 0; i < pvDlds->size (); i++)
	{
		vmsNewDownloadInfo &dlInfo = pvDlds->at (i);

		if (!dlg.m_selected [i])
			continue;

		if (!dlg.m_selectedByUserItself [i] &&
			linkedAudioUrls.find (dlInfo.strUrl) != linkedAudioUrls.end ())
		{
			if (!(dlInfo.ap.dwMask & DWCDAP_DOWNLOAD_ADDITIONAL_FLAGS))
			{
				dlInfo.ap.dwMask |= DWCDAP_DOWNLOAD_ADDITIONAL_FLAGS;
				dlInfo.ap.dwDownloadAdditionalFlags = 0;
			}
			dlInfo.ap.dwDownloadAdditionalFlags |= DLD_NOTIFICATIONS_LL;
		}

		if (dlInfo.additionalDownload)
		{
			if (!dlg.m_selected [urlToIndex [dlInfo.additionalDownload->strUrl]])
				dlInfo.additionalDownload = nullptr;
		}

		FinalConstructNewDownloadInfo (&dlInfo, nullptr, dlg);

		_pwndDownloads->CreateDownload (&dlInfo);
	}

	return true;
}

YouTubeDownloadDetails ytDownloadDetailsFromYtParserFileNode (const std::wstring& srcUrl, 
	const std::wstring &ytId, const YouTubeParser::fileNode &node, bool hidden)
{
	YouTubeDownloadDetails result;

	switch( node.mediaType )
	{
	case YouTubeParser::AUDIO:
		result.mediaType = YouTubeDownloadDetails::AUDIO;
		break;

	case YouTubeParser::VIDEO:
		result.mediaType = YouTubeDownloadDetails::VIDEO;
		break;

	case YouTubeParser::AUDIOVIDEO:
		result.mediaType = YouTubeDownloadDetails::AUDIOVIDEO;
		break;
	}

	result.originalURL = srcUrl;
	result.format = node.fFile.fileExtension;
	result.youTubeID = ytId;
	result.fileSize = node.fFile.nFileSize;
	result.isHidden = hidden;
	result.is3D = node.is3D;

	return result;
}

bool CFlashVideoDownloadsWnd::CreateDownloadFromYtParser(const CString &srcUrl, 
	YouTubeParser *parser, bool topmostDialog, bool& cancelled_by_user)
{
	auto dlmgr = std::make_shared <vmsFdmURLDownloadMgr> ();
	parser->setDownloader (dlmgr->createDownloader ());

	if (!parser->Extract ())
		return false;

	auto variants = parser->GetVariantsInfo ();
	
	if (variants.empty ())
	{
		std::wstring version = vmsYouTubeParserDllMgr::get ()->getYouTubeParserVersion();
		auto errorInfo = parser->getErrorInfo();
		auto YouTubeErrorSender = std::make_shared <vmsYouTubeParserErrorSenderPOST> (
			_T("cdn.freedownloadmanager.org"), _T("/admin/log_urls.php"));
		YouTubeErrorSender->sendErrorMessage( version, errorInfo );
		vmsYouTubeParserDllMgr::get ()->onYouTubeParserDllParseError ();
		return false;
	}

	std::unique_ptr <std::vector <vmsNewDownloadInfo>> pvDlds (
		new std::vector <vmsNewDownloadInfo> ());
	
	std::map <std::wstring, size_t> urlToIndex;
	for (size_t i = 0; i < variants.size (); ++i)
		urlToIndex [variants [i].url] = i;

	for (auto it = variants.begin(); it != variants.end(); ++it)
	{
		vmsNewDownloadInfo dl;

		dl.strUrl = it->url.c_str ();
		dl.strReferer = srcUrl;
		dl.strComment = srcUrl;

		dl.dwWhatIsValid = NDIV_AP;

		dl.ap.maxSectionCount = 2;

		dl.ap.dwMask = DWCDAP_FLAGS;
		dl.ap.dwFlags = DWDCDAP_F_FLASHVIDEODOWNLOAD;

		dl.ap.dwMask |= DWCDAP_FILENAME;
		dl.ap.strFileName = it->fFile.strName.c_str ();

		dl.ap.dwMask |= DWCDAP_FILESIZE;
		dl.ap.uFileSize = it->fFile.nFileSize;

		dl.ap.dwMask |= DWCDAP_YT_DOWNLOAD_DETAILS;
		dl.ap.ytDownloadDetails = ytDownloadDetailsFromYtParserFileNode (
			(LPCTSTR)srcUrl, parser->getYouTubeID (), *it, false);

		pvDlds->push_back (dl);
	}

	assert (variants.size () == pvDlds->size ());

	size_t i = 0;
	for (auto it = variants.begin(); it != variants.end(); ++it, ++i)
	{
		if (!it->complementaryURL.empty ())
		{
			assert (it->mediaType != YouTubeParser::AUDIO);
			auto itIndex = urlToIndex.find (it->complementaryURL);
			assert (itIndex != urlToIndex.end ());
			if (itIndex != urlToIndex.end ())
			{
				assert (itIndex->second < pvDlds->size ());
				
				pvDlds->at (itIndex->second).ap.dwFlags |= 
					DWDCDAP_F_CHECK_DOWNLOAD_EXISTS | DWDCDAP_F_NEED_FILE |
					DWDCDAP_F_NO_RESTART;

				auto additionalDownload = std::make_shared <vmsNewDownloadInfo> (
					pvDlds->at (itIndex->second));
				additionalDownload->ap.ytDownloadDetails.isHidden = true;
				if (!(additionalDownload->ap.dwMask & DWCDAP_DOWNLOAD_ADDITIONAL_FLAGS))
				{
					additionalDownload->ap.dwMask |= DWCDAP_DOWNLOAD_ADDITIONAL_FLAGS;
					additionalDownload->ap.dwDownloadAdditionalFlags = 0;
				}
				additionalDownload->ap.dwDownloadAdditionalFlags |= DLD_NOTIFICATIONS_LL;

				assert (!pvDlds->at (i).additionalDownload);
				pvDlds->at (i).additionalDownload = additionalDownload;
			}
		}
	}

	std::unique_ptr <std::vector <bool>> selectedDownloads (
		new std::vector <bool> (pvDlds->size ()));

	selectedDownloads->front () = true;
	if (pvDlds->front ().additionalDownload)
	{
		
		auto itIndex = urlToIndex.find (
			(LPCTSTR)pvDlds->front ().additionalDownload->strUrl);
		assert (itIndex != urlToIndex.end ());
		selectedDownloads->at (itIndex->second) = true;
	}

	CDlgCreateFlvDownloads::ui_customizations uic;
	uic.no_url_column = true;
	uic.title = LS (L_DOWNLOAD_YT_VIDEO);
	uic.flashVideosTitle = LS (L_VIDEO_FMTS_FOUND_ON_THIS_PAGE);

	cancelled_by_user = !CreateFlvDownloads (this, srcUrl, 
		std::move (pvDlds), std::move (selectedDownloads), 
		nullptr, topmostDialog, &uic);

	return true;
}

void CFlashVideoDownloadsWnd::FinalConstructNewDownloadInfo(vmsNewDownloadInfo *dlInfo, 
	const vmsNewDownloadInfo *parentDlInfo, const CDlgCreateFlvDownloads& dlg)
{
	dlInfo->dwWhatIsValid |= NDIV_AP;

	if ((dlInfo->ap.dwMask & DWCDAP_FLAGS) == 0)
	{
		dlInfo->ap.dwMask |= DWCDAP_FLAGS;
		dlInfo->ap.dwFlags = DWDCDAP_F_FLASHVIDEODOWNLOAD;
	}
	else
	{
		dlInfo->ap.dwFlags |= DWDCDAP_F_FLASHVIDEODOWNLOAD;
	}		

	if (!parentDlInfo && dlg.m_bUseAutoConvert)
	{
		dlInfo->ap.dwMask |= DWCDAP_MEDIA_CONVERT_SETTINGS;
		dlInfo->ap.stgsMediaConvert = dlg.m_convertSettings;
	}

	dlInfo->bAddSilent = true;

	dlInfo->ap.strDstFolder = dlg.m_strDstFolder;

	if (dlInfo->ap.pGroup == NULL)
	{
		if (parentDlInfo)
		{
			if (parentDlInfo->ap.dwMask & DWCDAP_GROUP)
			{
				dlInfo->ap.pGroup = parentDlInfo->ap.pGroup;
				dlInfo->ap.dwMask |= DWCDAP_GROUP;
			}
		}
		else
		{
			LPCTSTR pszExt = NULL;
			if (dlInfo->ap.strFileName.IsEmpty () == FALSE)
			{
				pszExt = _tcsrchr (dlInfo->ap.strFileName, _T('.'));
				if (pszExt)
					pszExt++;
			}

			if (pszExt && *pszExt)
				dlInfo->ap.pGroup = _DldsGrps.FindGroupByExt (pszExt);
			if (dlInfo->ap.pGroup == NULL)
				dlInfo->ap.pGroup = _DldsGrps.FindGroupByExt (_T("flv"));
			if (dlInfo->ap.pGroup == NULL)
				dlInfo->ap.pGroup = _DldsGrps.FindGroupByExt (_T("mp4"));
			if (dlInfo->ap.pGroup == NULL)
				dlInfo->ap.pGroup = _DldsGrps.FindGroupByExt (_T("avi"));

			if (dlInfo->ap.pGroup != NULL)
				dlInfo->ap.dwMask |= DWCDAP_GROUP;
		}
	}

	if (dlInfo->additionalDownload)
	{
		FinalConstructNewDownloadInfo (dlInfo->additionalDownload.get (), 
			dlInfo, dlg);
	}
}
