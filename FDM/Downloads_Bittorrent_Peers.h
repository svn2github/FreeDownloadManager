/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#if !defined(AFX_DOWNLOADS_BITTORRENT_PEERS_H__0CDE7524_9A02_4941_B975_66D2AA398531__INCLUDED_)
#define AFX_DOWNLOADS_BITTORRENT_PEERS_H__0CDE7524_9A02_4941_B975_66D2AA398531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif           

class CDownloads_Bittorrent_Peers : public CListCtrl
{

public:
	CDownloads_Bittorrent_Peers();  

public:  

public:  

	
	//{{AFX_VIRTUAL(CDownloads_Bittorrent_Peers)
	//}}AFX_VIRTUAL  

public:
	void ApplyLanguage();
	void UpdatePeersDetails();
	void UpdateStat();
	void set_ActiveDownload(vmsDownloadSmartPtr dld);
	BOOL Create (CWnd *pwndParent);
	virtual ~CDownloads_Bittorrent_Peers();

	
protected:
	bool m_bDldChanged;
	vmsDownloadSmartPtr m_dld;
	//{{AFX_MSG(CDownloads_Bittorrent_Peers)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCmdUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};    

//{{AFX_INSERT_LOCATION}//}}  

#endif 
