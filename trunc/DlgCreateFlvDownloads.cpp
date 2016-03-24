/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fdm.h"
#include "DlgCreateFlvDownloads.h"
#include "mfchelp.h"
#include "CreateDownloadDlg.h"
#include "FdmUiWindow.h"
#include "MyMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgCreateFlvDownloads::CDlgCreateFlvDownloads(CWnd* pParent , CFdmUiWindow *pUiWindow)
	: CDialog(CDlgCreateFlvDownloads::IDD, pParent),
	m_changing_dlg_state (false),
	m_initialized (false)
{
	//{{AFX_DATA_INIT(CDlgCreateFlvDownloads)
		
	//}}AFX_DATA_INIT

	m_bUseAutoConvert = false;
	m_bReqTopMostDialog = false;
	m_pUiWindow = pUiWindow;

	m_showLinkedAudioSelectedNotification = _App.ShowUserNotification (L"LinkedAudioSelected") != FALSE;
	m_showLinkedAudioUnselectedNotification = _App.ShowUserNotification (L"LinkedAudioUnselected") != FALSE;
}

void CDlgCreateFlvDownloads::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCreateFlvDownloads)
	DDX_Control(pDX, IDC_FLVLIST, m_wndFlvList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCreateFlvDownloads, CDialog)
	//{{AFX_MSG_MAP(CDlgCreateFlvDownloads)
	ON_BN_CLICKED(IDC_USE_AUTO_CONVERT, OnUseAutoConvert)
	ON_BN_CLICKED(IDC_CNVRT_SETTINGS, OnCnvrtSettings)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FLVLIST, &CDlgCreateFlvDownloads::OnItemchangedFlvlist)
END_MESSAGE_MAP()

BOOL CDlgCreateFlvDownloads::OnInitDialog() 
{
	assert (!m_initialized);
	m_changing_dlg_state = true;

	CDialog::OnInitDialog();

	if (m_selected.size () != m_vDlds.size ())
		m_selected.resize (m_vDlds.size (), true);
	m_selectedByUserItself.resize (m_selected.size ());

	if (m_pUiWindow)
		m_pUiWindow->setWindow (m_hWnd);
	
	SetDlgItemText (IDC_WPURL, m_strWebPageUrl);

	CRect rc; m_wndFlvList.GetClientRect (&rc);

	m_wndFlvList.InsertColumn (0, LS (L_TITLE), LVCFMT_LEFT);
	m_wndFlvList.InsertColumn (1, LS (L_SIZE), LVCFMT_LEFT);
	if (!m_ui_customizations.no_url_column)
		m_wndFlvList.InsertColumn (2, _T("URL"), LVCFMT_LEFT);

	int nMaxW0 = 0;
	int nMaxW1 = 0;

	for (size_t i = 0; i < m_vDlds.size (); i++)
	{
		const auto& dl = m_vDlds [i];

		m_urlToIndex [dl.strUrl] = i;
		if (!dl.strLinkedAudioUrl.IsEmpty ())
		{
			m_urlVideoToAudio [dl.strUrl] = dl.strLinkedAudioUrl;
			m_urlAudioToVideo.insert (std::make_pair (dl.strLinkedAudioUrl, dl.strUrl));
		}

		m_wndFlvList.InsertItem (i, m_vDlds [i].strTitle);
		int nW0 = m_wndFlvList.GetStringWidth (m_vDlds [i].strTitle);
		CString strSize; 
		if (m_vDlds [i].uSize != _UI64_MAX)
			strSize = BytesToString (m_vDlds [i].uSize, true);
		int nW1 = m_wndFlvList.GetStringWidth (strSize);
		m_wndFlvList.SetItemText (i, 1, strSize);
		if (!m_ui_customizations.no_url_column)
			m_wndFlvList.SetItemText (i, 2, m_vDlds [i].strUrl);
		nMaxW0 = max (nMaxW0, nW0);
		nMaxW1 = max (nMaxW1, nW1);
	}
	
	auto vscroll_payload = GetSystemMetrics (SM_CXVSCROLL) + 3;
	if (!m_ui_customizations.no_url_column)
	{
		int n = rc.Width () / 3;
		if (n > nMaxW0)
			n = nMaxW0;
		m_wndFlvList.SetColumnWidth (0, n+20);
		m_wndFlvList.SetColumnWidth (1, nMaxW1+20);
		m_wndFlvList.SetColumnWidth (2, rc.Width () - n-20 - nMaxW1-20 - vscroll_payload);
	}
	else
	{
		nMaxW0 += 20;
		nMaxW1 += 20;
		int n = nMaxW0;
		if (rc.Width () < (nMaxW0 + nMaxW1 + 20 + vscroll_payload))
			n = rc.Width () - nMaxW1 - 20 - vscroll_payload;
		m_wndFlvList.SetColumnWidth (0, n + 20);
		m_wndFlvList.SetColumnWidth (1, nMaxW1);
	}
	
	m_wndFlvList.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_CHECKBOXES);

	
	for (size_t i = 0; i < m_vDlds.size (); i++)
		ListView_SetItemState (m_wndFlvList, i, UINT((int(m_selected [i]) + 1) << 12), LVIS_STATEIMAGEMASK);

	CComboBox *pDirs = (CComboBox*) GetDlgItem (IDC_OUTFOLDER);
	for (size_t i = 0; i < (size_t)_LastFlashVideoDstFolders.GetRecordCount (); i++)
		pDirs->AddString (_LastFlashVideoDstFolders.GetRecord (i));
	if (_LastFlashVideoDstFolders.GetRecordCount ())
	{
		pDirs->SetCurSel (0);
	}
	else
	{
		vmsDownloadsGroupSmartPtr pGrp = _DldsGrps.FindGroupByExt (_T("flv"));
		if (pGrp == NULL)
			pGrp = _DldsGrps.FindGroupByExt (_T("mp4"));
		if (pGrp == NULL)
			pGrp = _DldsGrps.FindGroupByExt (_T("avi"));
		if (pGrp == NULL)
			pGrp = _DldsGrps.FindGroup (GRP_OTHER_ID);
		ASSERT (pGrp != NULL);
		if (pGrp != NULL)
			pDirs->SetWindowText (pGrp->strOutFolder);
	}

	if (_App.FVDownloads_AutoConvertVideo () && _AppMgr.IsMediaFeaturesInstalled ())
	{
		CheckDlgButton (IDC_USE_AUTO_CONVERT, BST_CHECKED);
		UpdateEnabled ();
	}
	
	vmsMediaConvertMgr::GetDefaultSettings (m_convertSettings);

	ApplyLanguage ();

	if (m_bReqTopMostDialog)
	{
		mfcSetForegroundWindow (this);
		mfcSetTopmostWindow (this);
	}

	m_changing_dlg_state = false;
	m_initialized = true;
	
	return TRUE;  
	              
}

void CDlgCreateFlvDownloads::OnOK() 
{
	if (FALSE == CCreateDownloadDlg::_CheckFolderName (this, IDC_OUTFOLDER))
		return;
	GetDlgItemText (IDC_OUTFOLDER, m_strDstFolder);

	fsPathToGoodPath ((LPTSTR)(LPCTSTR)m_strDstFolder);
	if (m_strDstFolder.GetLength () == 0)
	{
		MessageBox (LS (L_ENTERFLRNAME), LS (L_INPERR), MB_ICONEXCLAMATION);
		GetDlgItem (IDC_OUTFOLDER)->SetFocus ();
		return;
	}
	_LastFlashVideoDstFolders.AddRecord (m_strDstFolder);

	if (m_strDstFolder [m_strDstFolder.GetLength () - 1] != '\\' && 
			m_strDstFolder [m_strDstFolder.GetLength () - 1] != '/')
		m_strDstFolder += '\\';

	m_bUseAutoConvert = IsDlgButtonChecked (IDC_USE_AUTO_CONVERT) == BST_CHECKED;
	
	CDialog::OnOK();
}

void CDlgCreateFlvDownloads::UpdateEnabled()
{
	GetDlgItem (IDC_CNVRT_SETTINGS)->EnableWindow (
		IsDlgButtonChecked (IDC_USE_AUTO_CONVERT) == BST_CHECKED);
}

void CDlgCreateFlvDownloads::ApplyLanguage()
{
	fsDlgLngInfo lnginfo [] =  {
		fsDlgLngInfo (IDC__WEBPAGE, L_WEBPAGE, TRUE),
		fsDlgLngInfo (IDC__FLVONTHISPAGE, L_FLVFOUNDONTHISPAGE, TRUE),
		fsDlgLngInfo (IDC__SAVETO, L_SAVETO),
		fsDlgLngInfo (IDC_USE_AUTO_CONVERT, L_USE_AUTO_CONVERT),
		fsDlgLngInfo (IDC_CNVRT_SETTINGS, L_CUSTOMIZE),
		fsDlgLngInfo (IDCANCEL, L_CANCEL),
	};
	
	_LngMgr.ApplyLanguage (this, lnginfo, sizeof (lnginfo) / sizeof (fsDlgLngInfo), L_CREATEFVDLD);

	if (!m_ui_customizations.title.IsEmpty ())
		SetWindowText (m_ui_customizations.title);

	if (!m_ui_customizations.flashVideosTitle.IsEmpty ())
	{
		CString str = m_ui_customizations.flashVideosTitle;
		str += ':';
		SetDlgItemText (IDC__FLVONTHISPAGE, str);
	}
}

void CDlgCreateFlvDownloads::OnUseAutoConvert() 
{
	UpdateEnabled ();	
}

void CDlgCreateFlvDownloads::OnCnvrtSettings() 
{
	vmsMediaConvertMgr::ShowSettingsUI (m_convertSettings);	
}

void CDlgCreateFlvDownloads::OnItemchangedFlvlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	if (!m_initialized)
		return;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	bool selectedWas = (((pNMLV->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1) != 0;
	bool selectedNow = (((pNMLV->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1) != 0;

	if (selectedNow != selectedWas)
		m_selected [pNMLV->iItem] = selectedNow;

	if (m_changing_dlg_state)
		return;

	if (selectedWas != selectedNow)
	{
		m_selectedByUserItself [pNMLV->iItem] = selectedNow;

		if (selectedNow)
		{
			
			SelectLinkedAudioDownload (pNMLV->iItem, true);
		}
		else
		{
			
			SelectLinkedAudioDownload (pNMLV->iItem, false);

			
			ShowLinkedAudioUnselectedWarningIfRequired (pNMLV->iItem);
		}
	}
}

void CDlgCreateFlvDownloads::SelectLinkedAudioDownload(int videoDownloadIndex, bool select)
{
	auto it = m_urlVideoToAudio.find (m_vDlds [videoDownloadIndex].strUrl);
	if (it == m_urlVideoToAudio.end ())
		return;

	auto index = m_urlToIndex [it->second];

	if (select == m_selected [index])
		return; 

	if (!select && 
		(m_selectedByUserItself [index] || getLinkedAudioVideoDownloadCountSelected (index)))
	{
		
		
		return; 
	}

	m_changing_dlg_state = true;
	ListView_SetItemState (m_wndFlvList, index, UINT((int(select) + 1) << 12), LVIS_STATEIMAGEMASK);
	m_changing_dlg_state = false;

	if (select && m_showLinkedAudioSelectedNotification)
	{
		m_showLinkedAudioSelectedNotification = false;
		auto result = MyMessageBox2 (this, LS (L_LINKED_AUDIO_SELECTED),
			L"Free Download Manager", LS (L_DONTSHOWTHISWINDOWAGAIN), IDI_INFORMATION);
		if (result.second)
			_App.ShowUserNotification (L"LinkedAudioSelected", false);
	}
}

void CDlgCreateFlvDownloads::ShowLinkedAudioUnselectedWarningIfRequired(int audioDownloadIndex)
{
	if (!m_showLinkedAudioUnselectedNotification)
		return;

	if (!getLinkedAudioVideoDownloadCountSelected (audioDownloadIndex))
		return;

	auto result = MyMessageBox2 (this, LS (L_LINKED_AUDIO_UNSELECTED_WARNING),
		L"Free Download Manager", LS (L_DONTSHOWTHISWINDOWAGAIN), IDI_WARNING, LS (L_YES), LS (L_NO));
	if (result.second)
		_App.ShowUserNotification (L"LinkedAudioUnselected", false);

	if (result.first != IDC_BTN1)
	{
		m_changing_dlg_state = true;
		ListView_SetItemState (m_wndFlvList, audioDownloadIndex, 
			UINT((int(true) + 1) << 12), LVIS_STATEIMAGEMASK);
		m_changing_dlg_state = false;
	}
}

int CDlgCreateFlvDownloads::getLinkedAudioVideoDownloadCountSelected(int audioIndex)
{
	auto range = m_urlAudioToVideo.equal_range (m_vDlds [audioIndex].strUrl);
	if (range.first == m_urlAudioToVideo.end ())
		return 0;

	int cVideoSelected = 0;

	for (auto it = range.first; it != range.second; ++it)
	{
		auto index = m_urlToIndex [it->second];
		if (m_selected [index]) 
			++cVideoSelected;
	}

	return cVideoSelected;
}
