/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#if !defined(AFX_DLG_OPTIONS_DOWNLOADS_NETWORK_H__B5E47FAF_5F5E_4790_9979_7ABF3645B72B__INCLUDED_)
#define AFX_DLG_OPTIONS_DOWNLOADS_NETWORK_H__B5E47FAF_5F5E_4790_9979_7ABF3645B72B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif   

#include "Dlg_Options_Page.h"
#include "resource.h"    

class CDlg_Options_Downloads_Network : public CDlg_Options_Page
{

public:
	BOOL Apply();
	CString get_PageShortTitle();
	CString get_PageTitle();
	CDlg_Options_Downloads_Network(CWnd* pParent = NULL);     

	//{{AFX_DATA(CDlg_Options_Downloads_Network)
	enum { IDD = IDD_OPTIONS_DOWNLOADS_NETWORK };
	CComboBox	m_wndTLD;
	CComboBox	m_wndMode;
	//}}AFX_DATA    

	
	//{{AFX_VIRTUAL(CDlg_Options_Downloads_Network)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL  

protected:
	fsTrafficUsageMode m_aTUM[3];	
	void ApplyLanguage();
	void UpdateEnabled();
	BOOL m_bIModifing;
	void WriteTUS();
	BOOL ReadTUS();
	int m_iCurMode;

	
	//{{AFX_MSG(CDlg_Options_Downloads_Network)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMode();
	afx_msg void OnUsemaxconns();
	afx_msg void OnUsemaxconnsperserver();
	afx_msg void OnUsemaxdlds();
	afx_msg void OnDetectactivity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}//}}  

#endif 
