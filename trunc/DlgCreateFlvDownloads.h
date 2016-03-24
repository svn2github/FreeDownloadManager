/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_DLGCREATEFLVDOWNLOADS_H__3B06233F_69C9_47B3_B861_42D7014FAF1A__INCLUDED_)
#define AFX_DLGCREATEFLVDOWNLOADS_H__3B06233F_69C9_47B3_B861_42D7014FAF1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgCreateFlvDownloads : public CDialog
{
public:
	struct ui_customizations
	{
		bool no_url_column;
		CString title;
		CString flashVideosTitle;
		ui_customizations () : no_url_column (false) {}
	};

	struct FlvDownload
	{
		CString strTitle;
		CString strUrl;
		UINT64 uSize;
		CString strLinkedAudioUrl;
	};

public:
	bool m_bReqTopMostDialog;
	CDlgCreateFlvDownloads(CWnd* pParent = NULL, class CFdmUiWindow *pUiWindow = NULL);   

	//{{AFX_DATA(CDlgCreateFlvDownloads)
	enum { IDD = IDD_CREATEFLVDOWNLOADS };
	CListCtrl	m_wndFlvList;
	//}}AFX_DATA

	std::vector <FlvDownload> m_vDlds;
	std::vector <bool> m_selected;
	std::vector <bool> m_selectedByUserItself;
	CString m_strWebPageUrl;
	CString m_strDstFolder;
	vmsMediaFileConvertSettings m_convertSettings;
	bool m_bUseAutoConvert;
	ui_customizations m_ui_customizations;

protected:
	std::map <CString, size_t> m_urlToIndex;
	std::multimap <CString, CString> m_urlAudioToVideo;
	std::map <CString, CString> m_urlVideoToAudio;
	bool m_changing_dlg_state;
	bool m_initialized;
	bool m_showLinkedAudioSelectedNotification;
	bool m_showLinkedAudioUnselectedNotification;

	
	//{{AFX_VIRTUAL(CDlgCreateFlvDownloads)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	void ApplyLanguage();
	void UpdateEnabled();
	CFdmUiWindow *m_pUiWindow;

	
	//{{AFX_MSG(CDlgCreateFlvDownloads)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUseAutoConvert();
	afx_msg void OnCnvrtSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnItemchangedFlvlist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	void SelectLinkedAudioDownload(int videoDownloadIndex, bool select);
	void ShowLinkedAudioUnselectedWarningIfRequired(int audioDownloadIndex);
	int getLinkedAudioVideoDownloadCountSelected(int audioIndex);
};

//{{AFX_INSERT_LOCATION}}

#endif 
