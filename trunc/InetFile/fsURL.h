/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSURL_H__B92E5969_6142_4EC2_8379_56A47FDD6329__INCLUDED_)
#define AFX_FSURL_H__B92E5969_6142_4EC2_8379_56A47FDD6329__INCLUDED_

#include "fsinet.h"	
#if _MSC_VER > 1000
#pragma once
#endif 

#define URL_SCHEME_SIZE		100		
#define URL_HOSTNAME_SIZE	1000	
#define URL_USERNAME_SIZE	100		
#define URL_PASSWORD_SIZE	100		
#define URL_PATH_SIZE		10000	

class fsURL  
{
public:
	
	
	fsInternetResult Crack (LPCTSTR pszUrl, BOOL bCheckScheme = TRUE);
	
	
	
	
	
	
	
	
	fsInternetResult Create (INTERNET_SCHEME nScheme, LPCTSTR lpszHostName, INTERNET_PORT nPort, LPCTSTR lpszUserName, LPCTSTR lpszPassword, LPCTSTR lpszUrlPath, LPTSTR lpszUrl, DWORD* pdwUrlLength);
	
	INTERNET_PORT GetPort();
	INTERNET_SCHEME GetInternetScheme();
	LPCTSTR GetPassword();
	LPCTSTR GetUserName();
	LPCTSTR GetScheme();
	LPCTSTR GetHostName();
	LPCTSTR GetPath();
	
	
	
	
	void FixWinInetBug();
		
	fsURL();
	virtual ~fsURL();

protected:
	
	
	
	static void Encode (LPCTSTR psz, LPTSTR sz);
	URL_COMPONENTS m_url;
	TCHAR m_szScheme [URL_SCHEME_SIZE];	
	TCHAR m_szHost [URL_HOSTNAME_SIZE];	
	TCHAR m_szUser [URL_USERNAME_SIZE];	
	TCHAR m_szPassword [URL_PASSWORD_SIZE];	
	TCHAR m_szPath [URL_PATH_SIZE];		
};

#endif 
