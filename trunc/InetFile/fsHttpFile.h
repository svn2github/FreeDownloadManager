/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSHTTPFILE_H__28FE56A5_B9BC_43B2_A475_16CC7616FC32__INCLUDED_)
#define AFX_FSHTTPFILE_H__28FE56A5_B9BC_43B2_A475_16CC7616FC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "fsInternetFile.h"
#include "fsinet.h"	

class fsHttpFile : public fsInternetFile  
{
public:
	void set_Charset (LPCTSTR psz);
	void set_EnableAutoRedirect (BOOL b);
	fsString GetCookiesFromResponse();
	void set_UseMultipart (BOOL b);
	fsInternetResult Write (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten);
	fsInternetResult OpenEx(LPCTSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize = _UI64_MAX, UINT64 uUploadTotalSize = _UI64_MAX);
	
	
	void SetAdditionalHeaders(LPCTSTR pszAdditionalHeaders);
	
	LPCSTR GetPostData();
	
	LPCTSTR GetCookies();
	void SetPostData (LPCSTR pszPostData);
	void SetCookies (LPCTSTR pszCookies);
	
	BOOL get_UseCookie();
	
	LPCTSTR get_Referer ();
	
	BOOL get_UseHttp11();
	
	BOOL Is_Secure();
	
	fsInternetResult QuerySize (LPCTSTR pszFilePath);
	
	
	void RetreiveHeadersOnly(BOOL b = TRUE);
	
	void UseCookie (BOOL bUse);
	
	void UseSecure (BOOL bUse);
	
	void SetReferer (LPCTSTR pszReferer);
	
	void UseHttp11 (BOOL bUse = TRUE);
	
	fsInternetResult Open (LPCTSTR pszFilePath, UINT64 uStartPos);
	
	fsInternetResult Read(LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead);
	fsHttpFile();
	virtual ~fsHttpFile();

protected:
	fsString m_strCharset;
	BOOL m_bEnableAutoRedirect;
	BOOL m_bUseMultipart;
	fsString m_strLabel;
	UINT64 m_uLeftToUpload;
	
	void IgnoreSecurityProblems ();
	fsInternetResult Open_imp (LPCTSTR pszFilePath, UINT64 uStartPos, int cTryings);
	
	
	
	void RetreiveSuggFileName();
	
	fsInternetResult ProcessRangesResponse();
	
	fsInternetResult SetupProxy();

	
	LPTSTR m_pszReferer;			
	BOOL m_bHeadersOnly;		
	DWORD m_dwFlags;			
	LPTSTR m_pszHttpVersion;		
	LPTSTR m_pszCookies;			
	LPSTR m_pszPostData;		
	LPTSTR m_pszAdditionalHeaders; 
};

#endif 
