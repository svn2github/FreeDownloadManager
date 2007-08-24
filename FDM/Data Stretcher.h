/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_DATASTRETCHER_H__0DFAE8B5_1F34_4956_8DFA_5AE79C242CB7__INCLUDED_)
#define AFX_DATASTRETCHER_H__0DFAE8B5_1F34_4956_8DFA_5AE79C242CB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       
#include "DataStretcher.h"          

const char* const _pszAppMutex = "Free Download Manager";

class CDataStretcherApp : public CWinApp
{
public:
	CDataStretcherApp();  

	
	//{{AFX_VIRTUAL(CDataStretcherApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL    

public:
	static void ScheduleExitProcess (DWORD dwSeconds);
	
	void SaveSettings();
	
	CString m_strAppPath;
	
	BOOL Is_Starting();
	
	struct _inc_UrlToAdd {
		fsString strUrl;
		BOOL bForceSilent;
	};
	
	fs::list <_inc_UrlToAdd> m_vUrlsToAdd, m_vTorrentFilesToAdd;
	~CDataStretcherApp ();
	void SaveHistory();		
	//{{AFX_MSG(CDataStretcherApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void CheckRegistry();
	static DWORD WINAPI _threadExitProcess (LPVOID);
	void IntegrationSettings();
	
	void Install_UnregisterServer();
	
	void Install_RegisterServer();
	
	BOOL m_bForceSilentSpecified;
	
	int m_nNoLngsErrReason;
	
	
	
	
	BOOL RegisterServer(BOOL bGlobal);
	
	BOOL m_bCOMInited;
	
	void UninstallCustomizations();
	
	
	void CheckLocked();
	
	BOOL m_bStarting;
	
	static LONG WINAPI _UEF (_EXCEPTION_POINTERS*);
	
	BOOL Check3wGetStartedAlready(BOOL bSetForIfEx = TRUE);
	BOOL m_bEmbedding;		
	BOOL InitLanguage();	
	BOOL m_bSaveAllOnExit;	
	void LoadHistory();		
	HANDLE m_hAppMutex;		
private:
	
	BOOL m_bATLInited;
	
	BOOL m_bATLInited2;
private:
	
	BOOL InitATL();
};      

//{{AFX_INSERT_LOCATION}//}}  

#endif 
