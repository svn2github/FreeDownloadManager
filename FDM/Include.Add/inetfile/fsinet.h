#ifndef __FS_INET_H_
#define __FS_INET_H_

#include <windows.h>
#include <wininet.h>

// function return code
enum fsInternetResult
{
	IR_SUCCESS,				// successfully
	IR_S_FALSE,				// function wasn't executed (it couldn't been executed), but no error occured
	IR_ALREADYCONNECTED,	// already connected
	IR_WININETUNKERROR,		// library unknown error
	IR_NOTINITIALIZED,		// object is not initialized
	IR_LOGINFAILURE,		// authorization error
	IR_FILENOTOPENED,		// file not opened
	IR_BADURL,				// invalid URL
	IR_INVALIDPARAM,		// invalid paraneter
	IR_ERROR,				// error
	IR_TIMEOUT,				// net operation timeout
	IR_CANTCONNECT,			// can't connect
	IR_FILENOTFOUND,		// file not found
	IR_LOSTCONNECTION,		// lost connection
	IR_NAMENOTRESOLVED,		// can't resolve IP
	IR_RANGESNOTAVAIL,		// download resuming is not available
	IR_PROXYAUTHREQ,		// prexy requires authorization
	IR_WINERROR,			// Windows API error. Can get through GetLastError
	IR_NEEDREDIRECT,		// need redirect
	IR_EXTERROR,			// extended error. Generally, an object allows to get description via GetLastError
	IR_SERVERBADREQUEST,	// invalid request (HTTP)
	IR_SERVERUNKERROR,		// unknown server error
	IR_CONNECTIONABORTED,	// aborted connection
	IR_OUTOFMEMORY,			// not enough memory
	IR_S_REDIRECTED,		// is redirected
	IR_INVALIDPASSWORD,		// invalid password
	IR_INVALIDUSERNAME,		// invalid user name
	IR_NODIRECTACCESS,		// can't connect without proxy
	IR_NOINTERNETCONNECTION,	// missing internet connection
	IR_HTTPVERNOTSUP,		// requested HTTP version is not supported
	IR_BADFILESIZE,
	// server response with Accept-Ranges field only 
	// (without Content-Range field)
	IR_DOUBTFUL_RANGESRESPONSE,	
	// some of code was not implemented
	IR_E_NOTIMPL,
	// wininet can't download from this resource (url)
	IR_E_WININET_UNSUPP_RESOURCE,
};

extern fsInternetResult fsWinInetErrorToIR ();
extern fsInternetResult fsWinInetErrorToIR (DWORD dwErr);
extern fsInternetResult fsHttpStatusCodeToIR (DWORD dwStatusCode);
// Open HTTP path on server with passing through all redirects.
// pszPath - path
// pServer - HTTP server
// pFile - pointer to HTTP File object, what will contain all info about opened file
// ppRedirectedUrl, [out] - If was a redirect, then it will contain new URL
// otherwise - NULL. The application is responsible for memory releasing, allocated for it.
// pbRedirInner, [out] - was internal (relative) redirect ?
extern fsInternetResult fsHttpOpenPath (LPCSTR pszPath, class fsHttpConnection *pServer, class fsHttpFile *pFile, LPSTR* ppRedirectedUrl, BOOL *pbRedirInner);
// Open HTTP URL.
// pszUrl - URL
// pszUser, pszPassword - username and password for a server
// pServer - HTTP server.
// pFile - pointer to HTTP File object, what will contain all info about opened file
// ppRedirectedUrl, [out] - If was a redirect, then it will contain new URL
// otherwise - NULL. The application is responsible for memory releasing, allocated for it.
// pbRedirInner, [out] - was internal (relative) redirect ?
// Note. It differs from fsHttpOpenPath only in selecting ULR from path a
// calling pServer->Connect, and then call fsHttpOpenPath.
extern fsInternetResult fsHttpOpenUrl (LPCSTR pszUrl, LPCSTR pszUser, LPCSTR pszPassword, class fsHttpConnection *pServer, class fsHttpFile *pFile, LPSTR* ppRedirectedUrl, BOOL *pbRedirInner);
// Build full URL from URL pszUrlCurrent, linking from pszUrlParent page.
// ppszFullUrl, [out] - result
// Returns TRUE, if URL pszUrlCurrent is relative
// Returns FALSE and *ppszFullUrl != NULL, URL pszUrlCurrent - absolute;
// otherwise - error
extern BOOL fsUrlToFullUrl (LPCSTR pszUrlParent, LPCSTR pszUrlCurrent, LPSTR *ppszFullUrl);
extern fsInternetResult fsWSAErrorToIR ();
// relative URL ?
extern BOOL fsIsUrlRelative (LPCSTR pszUrl);
extern fsInternetResult fsWinErrorToIR (DWORD dwErr);
extern fsInternetResult fsWinErrorToIR ();
// remove "www" from URL
extern void fsRemoveWWW (LPCSTR pszUrl);
// servers pszServ1 and pszServ2 points on same server ?
// bExcludeSubDomainNameFrom2Site - ignore 2nd site subdomain ?
// ex. suppose site.com and subdom.site.com as one server, if TRUE
extern BOOL fsIsServersEqual (LPCSTR pszServ1, LPCSTR pszServ2, BOOL bExcludeSubDomainNameFrom2Site = FALSE);
// download file
// ppBuf, out - buffer contains downloaded file, application will release memory by itself
// puSize - file size
// pbAbort - abort downloading, if *pbAbort become TRUE
extern fsInternetResult fsDownloadFile (class fsInternetURLFile* file, LPBYTE* ppBuf, UINT* puSize, BOOL* pbAbort);

#endif