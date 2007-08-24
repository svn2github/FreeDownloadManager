/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_DOWNLOADERPROPERTIES_NETWORKPAGE_H__83DF2107_6D6B_40D7_81BC_F7B0ABAC0442__INCLUDED_)
#define AFX_DOWNLOADERPROPERTIES_NETWORKPAGE_H__83DF2107_6D6B_40D7_81BC_F7B0ABAC0442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif   

#include "fsDownloadsMgr.h"    

class CDownloaderProperties_NetworkPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDownloaderProperties_NetworkPage)  

public:
	CDownloaderProperties_NetworkPage();
	~CDownloaderProperties_NetworkPage();  

	//{{AFX_DATA(CDownloaderProperties_NetworkPage)
	enum { IDD = IDD_DLDR_NETWORK };
	CComboBox	m_wndTLD;
	CComboBox	m_wndMode;
	//}}AFX_DATA    

	
	//{{AFX_VIRTUAL(CDownloaderProperties_NetworkPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL  

protected:
	void PrepareCHMgr(CPoint point);
	void ApplyLanguage();
	
	
	BOOL m_bIModifing;	
	void UpdateEnabled();
	
	
	
	
	
	
	
	void WriteTUS();
	fsTrafficUsageMode m_aTUM[3];	
	int m_iCurMode;					
	
	BOOL ReadTUS();
	
	//{{AFX_MSG(CDownloaderProperties_NetworkPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnUsemaxconns();
	afx_msg void OnChangeMaxconns();
	afx_msg void OnUsemaxconnsperserver();
	afx_msg void OnChangeMaxconnsperserver();
	afx_msg void OnChangeTrafficlimitval();
	afx_msg void OnSelchangeTrafficlimitdimension();
	afx_msg void OnSelchangeMode();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWhatisthis();
	afx_msg void OnDetectactivity();
	afx_msg void OnDecbylevel();
	afx_msg void OnSettolight();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeMaxdldsval();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}//}}  

#endif 
