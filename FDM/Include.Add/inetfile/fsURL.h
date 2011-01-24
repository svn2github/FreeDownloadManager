/******************************************************************************

	Class fsURL.

	Split URL on components (protocol, server name, port, path to file).
	Create URL from components.

******************************************************************************/


#if !defined(AFX_FSURL_H__B92E5969_6142_4EC2_8379_56A47FDD6329__INCLUDED_)
#define AFX_FSURL_H__B92E5969_6142_4EC2_8379_56A47FDD6329__INCLUDED_

#include "fsinet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Buffers size for storing
#define URL_SCHEME_SIZE		100		// protocol type
#define URL_HOSTNAME_SIZE	1000	// server address
#define URL_USERNAME_SIZE	100		// user name
#define URL_PASSWORD_SIZE	100		// password
#define URL_PATH_SIZE		10000	// path to file

class fsURL  
{
public:
	// Split URL on components
	// bCheckScheme - check URL has valid type protocol
	fsInternetResult Crack (LPCSTR pszUrl, BOOL bCheckScheme = TRUE);
	// Create URL from components
	// nScheme - protocol type
	// lpszHostName - server address
	// nPort - server's port number
	// lpszUserName, lpszPassword - server's username and password
	// lpszUrlPath - path to file
	// lpszUrl, [out] - address of buffer we'll write result URL
	// pdwLength, [in, out] - buffer size (in), URL length (out)
	fsInternetResult Create (INTERNET_SCHEME nScheme, LPCTSTR lpszHostName, INTERNET_PORT nPort, LPCTSTR lpszUserName, LPCTSTR lpszPassword, LPCTSTR lpszUrlPath, LPSTR lpszUrl, DWORD* pdwUrlLength);
	// Returns URL components
	INTERNET_PORT GetPort();
	INTERNET_SCHEME GetInternetScheme();
	LPCSTR GetPassword();
	LPCSTR GetUserName();
	LPCSTR GetScheme();
	LPCSTR GetHostName();
	LPCSTR GetPath();
	// Bugfix to WinInet. 
	// A bug is occured then URL has such view: user:password@server/....
	// and a password contains a @ symbol.
	// Called automatically while parsing URL.
	void FixWinInetBug();
		
	fsURL();
	virtual ~fsURL();

protected:
	// escape unsafe characters of url
	// [in]  psz - source url
	// [out] sz  - result url. Buffer pointed by sz should has enough size.
	static void Encode (LPCSTR psz, LPSTR sz);
	URL_COMPONENTS m_url;
	CHAR m_szScheme [URL_SCHEME_SIZE];	// Protocol
	CHAR m_szHost [URL_HOSTNAME_SIZE];	// Server addres
	CHAR m_szUser [URL_USERNAME_SIZE];	// Username
	CHAR m_szPassword [URL_PASSWORD_SIZE];	// Pasword
	CHAR m_szPath [URL_PATH_SIZE];		// Path to file
};

#endif // !defined(AFX_FSURL_H__B92E5969_6142_4EC2_8379_56A47FDD6329__INCLUDED_)
