/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#if !defined(AFX_DOWNLOADS_BITTORRENT_FILES_H__2815A0B6_6537_42CE_84E1_6E60764728E0__INCLUDED_)
#define AFX_DOWNLOADS_BITTORRENT_FILES_H__2815A0B6_6537_42CE_84E1_6E60764728E0__INCLUDED_

#include "fsDownload.h"	
#if _MSC_VER > 1000
#pragma once
#endif           

class CDownloads_Bittorrent_Files : public CListCtrl
{

public:
	CDownloads_Bittorrent_Files();  

public:  

public:  

	
	//{{AFX_VIRTUAL(CDownloads_Bittorrent_Files)
	//}}AFX_VIRTUAL  

public:
	void UpdateProgress();
	void set_ActiveDownload(vmsDownloadSmartPtr dld);
	void ApplyLanguage();
	BOOL Create(CWnd *pwndParent);
	virtual ~CDownloads_Bittorrent_Files();

	
protected:
	int m_nFileNameOffset;
	vmsDownloadSmartPtr m_dld;
	//{{AFX_MSG(CDownloads_Bittorrent_Files)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};    

//{{AFX_INSERT_LOCATION}//}}  

#endif 
