/**********************************************************************

	Class fsInternetServerConnection.

	Set connection with server.
	Base class for HTTP and FTP server classes.

**********************************************************************/

#if !defined(AFX_FSSERVERCONNECTION_H__0CD5653D_2D57_4456_B496_204556B72225__INCLUDED_)
#define AFX_FSSERVERCONNECTION_H__0CD5653D_2D57_4456_B496_204556B72225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <wininet.h>
#include "fsinet.h"	// Added by ClassView
#include <fsString.h>
#include "fsSpeaking.h"

class fsInternetServerConnection : public fsSpeaking
{
public:
	// Returns server's port number, we connected on
	INTERNET_PORT GetServerPort();
	// Get server address
	LPCSTR GetServerName();
	// Get pointer to fsInternetSession object (see file fsInternetSession.h), 
	// with which connection was set
	class fsInternetSession* GetSession();
	// Close connection with server
	void CloseHandle();
	// Returns last server response
	LPCSTR GetLastError ();
	// Connect with server
	// pszServer - server address (ip or name)
	// pszUser, pszPassword - username and password for server
	// nPort - server's port number
	virtual fsInternetResult Connect (LPCSTR pszServer, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT nPort) = NULL;
	// Init object. Sets pointer to internet session object,
	// which will be used to set connection with server.
	fsInternetResult Initialize (class fsInternetSession *pSession);
	// Returns WinInet handle of server connection
	HINTERNET GetHandle();
	fsInternetServerConnection();
	virtual ~fsInternetServerConnection();

protected:
	INTERNET_PORT m_uPort;	// server's port
	fsString m_strServer;	// server's address
	char *m_pszLastError;	// last response
	HINTERNET m_hServer;	// connection handle
	class fsInternetSession* m_pSession;	// pointer to internet session object
};

#endif // !defined(AFX_FSSERVERCONNECTION_H__0CD5653D_2D57_4456_B496_204556B72225__INCLUDED_)
