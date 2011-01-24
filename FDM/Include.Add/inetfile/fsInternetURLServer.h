/********************************************************************************

	Class fsInternetURLServer.

	Provide unified way to work with server	independing
	on used data transmission protocol.

	Depending on a protocol is using either fsFtpServer, or
	fsHttpServer.

********************************************************************************/

#if !defined(AFX_FSINTERNETURLSERVER_H__8100FC18_F1BF_4F7F_B866_0CB57B83B9DD__INCLUDED_)
#define AFX_FSINTERNETURLSERVER_H__8100FC18_F1BF_4F7F_B866_0CB57B83B9DD__INCLUDED_

#include "fsinet.h"	// Added by ClassView
#include "fsURL.h"	// Added by ClassView
#include "fsFtpConnection.h"	// Added by ClassView
#include "fsHttpConnection.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class fsInternetURLServer  
{
public:
	// All methods described in fsInternetServerConnection.h
	fsInternetResult Connect (LPCSTR pszUrl, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT nPort);
	void CloseHandle();
	INTERNET_PORT GetServerPort();
	LPCSTR GetServerName();
	HINTERNET GetHandle();
	fsInternetResult Initialize (fsInternetSession* pSession);

	// Use FTP passive mode ?
	void UseFtpPassiveMode (BOOL bUse);
	// Returns used protocol type
	INTERNET_SCHEME GetScheme();
	// Is using HTTPS protocol?
	BOOL IsHttpsServer();
	// Is using FTP protocol ?
	BOOL IsFtpServer();
	// Is using HTTP protocol ?
	BOOL IsHttpServer();
	// Get access to the object connecting with FTP server
	fsFtpConnection* GetFtpServer();
	// Get access to the object connecting with HTTP server
	fsHttpConnection* GetHttpServer();
	
	fsInternetURLServer();
	virtual ~fsInternetURLServer();

protected:
	INTERNET_SCHEME m_scheme;					// used protocol
	fsInternetServerConnection* m_pServer;		// connected server
	fsHttpConnection m_httpServer;
	fsFtpConnection m_ftpServer;
};

#endif // !defined(AFX_FSINTERNETURLSERVER_H__8100FC18_F1BF_4F7F_B866_0CB57B83B9DD__INCLUDED_)
