/*******************************************************************************

	Class fsHttpFile.

	Working with files on HTTP server.

*******************************************************************************/

#if !defined(AFX_FSHTTPFILE_H__28FE56A5_B9BC_43B2_A475_16CC7616FC32__INCLUDED_)
#define AFX_FSHTTPFILE_H__28FE56A5_B9BC_43B2_A475_16CC7616FC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsInternetFile.h"
#include "fsinet.h"	// Added by ClassView

class fsHttpFile : public fsInternetFile  
{
public:
	void set_Charset (LPCSTR psz);
	void set_EnableAutoRedirect (BOOL b);
	fsString GetCookiesFromResponse();
	void set_UseMultipart (BOOL b);
	fsInternetResult Write (LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten);
	fsInternetResult OpenEx(LPCSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize = _UI64_MAX, UINT64 uUploadTotalSize = _UI64_MAX);
	// additional headers
	// each line should be ended by \r\n characters
	void SetAdditionalHeaders(LPCSTR pszAdditionalHeaders);
	// post data for HTTP POST request
	LPCSTR GetPostData();
	// additional cookies to be send with request
	LPCSTR GetCookies();
	void SetPostData (LPCSTR pszPostData);
	void SetCookies (LPCSTR pszCookies);
	// is cookie enabled?
	BOOL get_UseCookie();
	// http referer 
	LPCSTR get_Referer ();
	// is http 1.1 version should be used?
	BOOL get_UseHttp11();
	// is it https?
	BOOL Is_Secure();
	// query file size (detailed in fsInternetFile.h)
	fsInternetResult QuerySize (LPCSTR pszFilePath);
	// On file opening request only file information
	// without file receiving
	void RetreiveHeadersOnly(BOOL b = TRUE);
	// Use cookies ?
	void UseCookie (BOOL bUse);
	// Use HTTPS?
	void UseSecure (BOOL bUse);
	// Set referer HTTP header field.
	void SetReferer (LPCSTR pszReferer);
	// Use HTTP 1.1?
	void UseHttp11 (BOOL bUse = TRUE);
	// Open file (see file fsInternetFile)
	fsInternetResult Open (LPCSTR pszFilePath, UINT64 uStartPos);
	// Read file (see file fsInternetFile)
	fsInternetResult Read(LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead);
	fsHttpFile();
	virtual ~fsHttpFile();

protected:
	fsString m_strCharset;
	BOOL m_bEnableAutoRedirect;
	BOOL m_bUseMultipart;
	fsString m_strLabel;
	UINT64 m_uLeftToUpload;
	// disable some wininet errors of https protocol
	void IgnoreSecurityProblems ();
	fsInternetResult Open_imp (LPCSTR pszFilePath, UINT64 uStartPos, int cTryings);
	// Check whether a server wrote real file name in a header.
	// If so, then init m_strSuggFileName variable (see file fsInternetFile)
	// Calling by Open () function.
	void RetreiveSuggFileName();
	// Parse HTTP header Range field, taking file size from it.
	fsInternetResult ProcessRangesResponse();
	// Use proxy parameters to a file handle. WinInet specific.
	fsInternetResult SetupProxy();

	
	LPSTR m_pszReferer;			// referer
	BOOL m_bHeadersOnly;		// receive only file header (file information without a file)
	DWORD m_dwFlags;			// connection parameters (HTTPS use, cookies)
	LPSTR m_pszHttpVersion;		// HTTP version (ex. "HTTP/1.1")
	LPSTR m_pszCookies;			// cookies
	LPSTR m_pszPostData;		// for HTTP post request
	LPSTR m_pszAdditionalHeaders; // additional HTTP headers to send
};

#endif // !defined(AFX_FSHTTPFILE_H__28FE56A5_B9BC_43B2_A475_16CC7616FC32__INCLUDED_)
