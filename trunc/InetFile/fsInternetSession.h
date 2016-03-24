/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSINTERNETSESSION_H__3E047CCF_86D6_4197_9BB2_8C94D552A262__INCLUDED_)
#define AFX_FSINTERNETSESSION_H__3E047CCF_86D6_4197_9BB2_8C94D552A262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <windows.h>
#include <wininet.h>
#include "fsinet.h"
#include <fsString.h>

enum fsInternetAccessType	
{
	IAT_NOPROXY,			
	IAT_PRECONFIGPROXY,		
	IAT_MANUALPROXY			
};

class fsInternetSession  
{
public:
	
	void get_Proxy (LPCTSTR& pszProxyName, LPCTSTR& pszProxyUser, LPCTSTR& pszProxyPwd);
	
	LPCTSTR get_UserAgent();
	
	
	static void InitializeWinInet();
	
	void CloseHandle();
	
	fsInternetResult ApplyProxyAuth (HINTERNET hInetHandle);
	
	void SetProxyAuth (LPCTSTR pszUser, LPCTSTR pszPassword);
	
	void SetTimeout (UINT uTimeout);
	
	HINTERNET GetHandle();
	
	
	
	
	fsInternetResult Create (LPCTSTR pszAgent, fsInternetAccessType accType, LPCTSTR pszProxy);
	static void AdjustWinInetConnectionLimit ();
	fsInternetSession();
	virtual ~fsInternetSession();

protected:
	fsString m_strUserAgent;		
	fsString m_strProxy;			
	fsString m_strProxyUser;		
	fsString m_strProxyPassword;	
	HINTERNET m_hSession;			
private:
};

#endif 
