/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __FS_INET_H_
#define __FS_INET_H_

#include <windows.h>
#include <wininet.h>

enum fsInternetResult
{
	IR_SUCCESS,				
	IR_S_FALSE,				
	IR_ALREADYCONNECTED,	
	IR_WININETUNKERROR,		
	IR_NOTINITIALIZED,		
	IR_LOGINFAILURE,		
	IR_FILENOTOPENED,		
	IR_BADURL,				
	IR_INVALIDPARAM,		
	IR_ERROR,				
	IR_TIMEOUT,				
	IR_CANTCONNECT,			
	IR_FILENOTFOUND,		
	IR_LOSTCONNECTION,		
	IR_NAMENOTRESOLVED,		
	IR_RANGESNOTAVAIL,		
	IR_PROXYAUTHREQ,		
	IR_WINERROR,			
	IR_NEEDREDIRECT,		
	IR_EXTERROR,			
	IR_SERVERBADREQUEST,	
	IR_SERVERUNKERROR,		
	IR_CONNECTIONABORTED,	
	IR_OUTOFMEMORY,			
	IR_S_REDIRECTED,		
	IR_INVALIDPASSWORD,		
	IR_INVALIDUSERNAME,		
	IR_NODIRECTACCESS,		
	IR_NOINTERNETCONNECTION,	
	IR_HTTPVERNOTSUP,		
	IR_BADFILESIZE,
	
	
	IR_DOUBTFUL_RANGESRESPONSE,	
	
	IR_E_NOTIMPL,
	
	IR_E_WININET_UNSUPP_RESOURCE,
	
	IR_SEC_CHECK_FAILURE,
};

enum fsSecurityCheckType
{
	SCT_NONE				= 0,
	SCT_UNSPECIFIED			= 1,
	SCT_CERT_CN_INVALID		= 1 << 1,
	SCT_CERT_DATE_INVALID	= 1 << 2,
	SCT_CERT_INVALID_CA		= 1 << 3,
	SCT_ALL					= 0xffffffff
};

extern fsInternetResult fsWinInetErrorToIR ();
extern fsInternetResult fsWinInetErrorToIR (DWORD dwErr);
extern fsInternetResult fsHttpStatusCodeToIR (DWORD dwStatusCode);
extern fsSecurityCheckType fsWinInetErrorToSCT ();
extern fsSecurityCheckType fsWinInetErrorToSCT (DWORD err);
extern DWORD fsSCTflagsToWinInetIgnoreFlags (DWORD flags);

extern fsInternetResult fsHttpOpenPath (LPCTSTR pszPath, class fsHttpConnection *pServer, class fsHttpFile *pFile, LPTSTR* ppRedirectedUrl, BOOL *pbRedirInner);

extern fsInternetResult fsHttpOpenUrl (LPCTSTR pszUrl, LPCTSTR pszUser, LPCTSTR pszPassword, class fsHttpConnection *pServer, class fsHttpFile *pFile, LPTSTR* ppRedirectedUrl, BOOL *pbRedirInner);

extern BOOL fsUrlToFullUrl (LPCTSTR pszUrlParent, LPCTSTR pszUrlCurrent, LPTSTR *ppszFullUrl);
extern fsInternetResult fsWSAErrorToIR ();

extern BOOL fsIsUrlRelative (LPCTSTR pszUrl);
extern fsInternetResult fsWinErrorToIR (DWORD dwErr);
extern fsInternetResult fsWinErrorToIR ();

extern void fsRemoveWWW (LPCTSTR pszUrl);

extern BOOL fsIsServersEqual (LPCTSTR pszServ1, LPCTSTR pszServ2, BOOL bExcludeSubDomainNameFrom2Site = FALSE);

extern fsInternetResult fsDownloadFile (class fsInternetURLFile* file, LPBYTE* ppBuf, UINT* puSize, BOOL* pbAbort);

#endif