/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_DLG_DOWNLOAD_H__E3399BF6_086C_4A61_90C8_D1D7DD78BBBB__INCLUDED_)
#define AFX_DLG_DOWNLOAD_H__E3399BF6_086C_4A61_90C8_D1D7DD78BBBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "resource.h"
#include "Wnd_DownloadProgress.h"	
#include "fsDownloadsMgr.h"

class CDlg_Download : 
	public CDialog,
	public vmsThreadSafe
{

public:
	
	void NeedUpdate();
	~CDlg_Download();
	
	void Create (vmsDownloadSmartPtr dld);
	CDlg_Download(CWnd* pParent = NULL);   

	//{{AFX_DATA(CDlg_Download)
	enum { IDD = IDD_DOWNLOAD };
		
	//}}AFX_DATA

	
	//{{AFX_VIRTUAL(CDlg_Download)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	bool ui_locked () const
	{
		vmsTHREAD_SAFE_SCOPE;
		return m_bUiLocked;
	}

	void schedule_close () 
	{
		vmsTHREAD_SAFE_SCOPE;
		assert (m_bUiLocked);
		m_bScheduleClose = true;
	}

protected:
	CWnd_DownloadProgress m_wndProgress;
	
	void Update();
	
	CFont m_fntBold;
	
	void SetDlgItemText2 (UINT nID, LPCTSTR pszText);
	void ApplyLanguage();
	vmsDownloadSmartPtr m_dld;
	bool m_bUiLocked;
	bool m_bScheduleClose;

	
	//{{AFX_MSG(CDlg_Download)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHide();
	afx_msg void OnAutoclose();
	virtual void OnCancel();
	afx_msg void OnStop();
	afx_msg void OnOpenfile();
	afx_msg void OnOpenfolder();
	afx_msg void OnAutolaunch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif 
