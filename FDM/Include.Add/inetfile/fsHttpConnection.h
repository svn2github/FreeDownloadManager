/*********************************************************************************

	Class fsHttpConnection.

	Set connection with HTTP server.

*********************************************************************************/

#if !defined(AFX_FSHTTPCONNECTION_H__8E66E788_A627_4EC6_8570_B28876142D61__INCLUDED_)
#define AFX_FSHTTPCONNECTION_H__8E66E788_A627_4EC6_8570_B28876142D61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsServerConnection.h"
#include "fsinet.h"	// Added by ClassView

class fsHttpConnection : public fsInternetServerConnection  
{
public:
	// Connect with server. No connection indeed, only
	// preparing to connection (WinInet detail, based on
	// HTTP protocol).
	// Described in fsInternetServerConnection file.
	fsInternetResult Connect (LPCSTR pszServer, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT nPort = INTERNET_DEFAULT_HTTP_PORT);
	fsHttpConnection();
	virtual ~fsHttpConnection();
};

#endif // !defined(AFX_FSHTTPCONNECTION_H__8E66E788_A627_4EC6_8570_B28876142D61__INCLUDED_)
