/********************************************************************************

	Class fsInternetURLFile.

	Provide unified way for working with files on sever
	independently from used data transmission protocol (http, ftp, LAN).

	Depending on a protocol use either fsFtpFile, or
	fsHttpFile (fsInternetFile2 for files larger than 2GB) or fsLocalFile (LAN).

********************************************************************************/

#if !defined(AFX_FSINTERNETURLFILE_H__F1F640DC_4992_4104_AB67_5C29AFD245D4__INCLUDED_)
#define AFX_FSINTERNETURLFILE_H__F1F640DC_4992_4104_AB67_5C29AFD245D4__INCLUDED_

#include "fsinet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsHttpFile.h"
#include "fsFtpFile.h"
#include "fsHttpConnection.h"
#include "fsFtpConnection.h"
#include "fsLocalFile.h"
#include "fsInternetFile2.h"	// Added by ClassView
#include "fsSpeaking.h"

class fsInternetURLFile : public fsSpeaking  
{
public:
	void SetInterface (LPCSTR psz);
	void set_Charset (LPCSTR psz);
	void set_EnableAutoRedirect (BOOL b);
	fsString GetCookiesFromResponse();
	fsInternetResult OpenAnotherRequestOnServer (LPCSTR pszPath, UINT64 uStartPosition, UINT64 uUploadPartSize, UINT64 uUploadTotalSize);
	void set_UseMultipart (BOOL b);
	fsInternetResult Write (LPBYTE pBuffer, DWORD dwToWrite, DWORD *pdwWritten);
	// set uUploadFileSize to _UI64_MAX in order to download
	fsInternetResult OpenEx (INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser,
		LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, UINT64 uStartPosition, 
		BOOL bSendHTTPBasicAuthImmediately = FALSE, UINT64 uUploadPartSize = _UI64_MAX, 
		UINT64 uUploadTotalSize = _UI64_MAX);
	fsFtpTransferType FtpGetTransferType();
	// HTTP "POST" request data
	void SetPostData (LPCSTR pszHdrs);
	void SetCookies (LPCSTR pszCookies);
	// don't use "LIST" command of FTP protocol
	// so "SIZE" command will be used
	// however attributes of file will not be retreived
	// and "LIST" command is slower than "SIZE".
	// but "SIZE" command is not supported by all FTP servers.
	void FtpSetDontUseLIST (BOOL b);
	// Open file.
	// scheme - used protocol (ex. INTERNET_SCHEME_HTTP)
	// pszHostName - server address
	// pszUser, pszPassword - username+password for server
	// port - port number
	// pszPath - file path
	// dwStartPosition - start position in file (for download resuming)
	// bSendHTTPBasicAuthImmediately - send auth by us, not WinInet
	fsInternetResult Open (INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, UINT64 uStartPosition, BOOL bSendHTTPBasicAuthImmediately = FALSE);
	// Query size. Same parameters.
	fsInternetResult QuerySize (INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, BOOL bSendHTTPBasicAuthImmediately);
	// Read file (see fsInternetFile.h)
	fsInternetResult Read (BYTE *pBuffer, DWORD dwToRead, DWORD *pdwRead);
	// These methods described in fsInternetFile.h, fsFtpFile.h and fsHttpFile.h
	LPCSTR GetSuggestedFileName();
	fsResumeSupportType IsResumeSupported();
	BOOL GetLastModifiedDate (LPFILETIME pDate);
	BOOL GetContentType (LPSTR pszType);
	LPCSTR GetLastError();
	void FtpSetTransferType (fsFtpTransferType enType);
	void UseFtpPassiveMode (BOOL bUse);
	void UseCookie (BOOL bUse);
	void UseHttp11 (BOOL bUse);
	void SetReferer (LPCSTR pszReferer);
	UINT64 GetFileSize();
	fsInternetResult Initialize (class fsInternetSession *pSession, BOOL bAutoDelSession = FALSE);
	/////////////////
	
	// Close file and connection with server
	void CloseHandle();
	// Close file only
	void Close();
	
	// Described in fsSpeaking.h
	fsInternetURLFile();
	virtual ~fsInternetURLFile();

protected:
	fsString m_strInterface;
	bool isProxySpecified();
	fsString m_strRespFromServer;
	BOOL m_bCatchFromServerResponse;
	static void _InetFileDialogFunc (fsInetFileDialogDirection enDir, LPCSTR pszMsg, LPVOID lp1, LPVOID lp2);
	void FormHttpBasicAuthHdr (LPCSTR pszUser, LPCSTR pszPassword);
	void SetupProxyForFile2();
	bool m_bUseFile2;
	fsInternetFile2 m_ifile2;
	BOOL m_bAutoDelSession;					// delete session object at destruction
	CRITICAL_SECTION m_cs;					// Fot thread safety
	LPCSTR m_pszLastError;					// Last server response
	class fsInternetSession* m_pSession;	// Internet session object
	class fsInternetFile* m_pFile;			// Pointes on fsHttpFile, or on fsFtpFile, or on fsLocalFile
	class fsInternetServerConnection *m_pServer;	// Same, but for server
	fsFtpConnection m_ftpConnection;		// Connection with FTP server
	fsHttpConnection m_httpConnection;		// Connection with HTTP server
	fsHttpFile m_httpFile;					// File on HTTP server
	fsFtpFile m_ftpFile;					// File on FTP server
	fsLocalFile m_localFile;				// File in local net
};

#endif // !defined(AFX_FSINTERNETURLFILE_H__F1F640DC_4992_4104_AB67_5C29AFD245D4__INCLUDED_)
