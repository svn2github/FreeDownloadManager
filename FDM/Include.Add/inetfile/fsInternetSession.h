/*******************************************************************

	Class fsInternetSession.

	Class, responsible for Internet connection.
	Used by classes, working with HTTP(S)/FTP protocols.
	Set proxy parameters and a connection timeout.

********************************************************************/

#if !defined(AFX_FSINTERNETSESSION_H__3E047CCF_86D6_4197_9BB2_8C94D552A262__INCLUDED_)
#define AFX_FSINTERNETSESSION_H__3E047CCF_86D6_4197_9BB2_8C94D552A262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <wininet.h>
#include "fsinet.h"
#include <fsString.h>

enum fsInternetAccessType	// Internet connection type.
{
	IAT_NOPROXY,			// without proxy
	IAT_PRECONFIGPROXY,		// use proxy, taken from IE options
	IAT_MANUALPROXY			// use manually set proxy
};

class fsInternetSession  
{
public:
	// get proxy settings
	void get_Proxy (LPCSTR& pszProxyName, LPCSTR& pszProxyUser, LPCSTR& pszProxyPwd);
	// get HTTP user agent field
	LPCSTR get_UserAgent();
	// Init library. Init once the but primary
	// (till using any other library functions).
	static void InitializeWinInet();
	// Close Internet connection.
	void CloseHandle();
	// Set proxy username+password for selected WinInten handle.
	fsInternetResult ApplyProxyAuth (HINTERNET hInetHandle);
	// Init m_strProxyUser(Password) variables.
	void SetProxyAuth (LPCSTR pszUser, LPCSTR pszPassword);
	// Set connection and data transmission timeout.
	void SetTimeout (UINT uTimeout);
	// Returns m_hSession variable value.
	HINTERNET GetHandle();
	// Create connection with Internet.
	// pszAgent - Header Agent HTTP field (HTTP/HTTPS only)
	// accType - proxy using type (see above)
	// pszProxy - proxy address (ip or name) (used only then accType == IAT_MANUALPROXY).
	fsInternetResult Create (LPCSTR pszAgent, fsInternetAccessType accType, LPCSTR pszProxy);
	fsInternetSession();
	virtual ~fsInternetSession();

protected:
	fsString m_strUserAgent;		// HTTP user-agent
	fsString m_strProxy;			// proxy address
	fsString m_strProxyUser;		// Proxy user name
	fsString m_strProxyPassword;	// Proxy password
	HINTERNET m_hSession;			// WinInet connection handle
private:
};

#endif // !defined(AFX_FSINTERNETSESSION_H__3E047CCF_86D6_4197_9BB2_8C94D552A262__INCLUDED_)
