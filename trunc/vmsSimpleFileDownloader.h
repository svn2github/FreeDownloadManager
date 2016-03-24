/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSSIMPLEFILEDOWNLOADER_H__539BE448_3975_4D2A_858C_84EFAD3E3300__INCLUDED_)
#define AFX_VMSSIMPLEFILEDOWNLOADER_H__539BE448_3975_4D2A_858C_84EFAD3E3300__INCLUDED_

#include "fsDownloadMgr.h"	
#if _MSC_VER > 1000
#pragma once
#endif 

class vmsSimpleFileDownloader  
{
public:
	fsInternetResult Download (LPCTSTR pszUrl, LPCTSTR pszFileName);
	
	void Stop();
	
	fsInternetDownloaderResult GetLastError();
	bool IsRunning();

	LPCTSTR out_file_name () const
	{
		assert (m_dldr);
		return m_dldr->GetDP ()->pszFileName;
	}

	double getProgressInPercentage () const
	{
		assert (m_dldr);
		return m_dldr->GetDownloader ()->GetPercentDone ();
	}
	
	vmsSimpleFileDownloader();
	virtual ~vmsSimpleFileDownloader();

protected:
	void CreateDownloader();
	static DWORD _DownloadMgrEvents(fsDownloadMgr* pMgr, fsDownloaderEvent ev, UINT uInfo, LPVOID lp);
	static void _DownloadMgrDescEvents(fsDownloadMgr* , fsDownloadMgr_EventDescType , LPCTSTR pszDesc, LPVOID lp);
	fsDownloadMgr *m_dldr;
};

#endif 
