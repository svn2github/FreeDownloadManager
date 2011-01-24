/**********************************************************************************

	Class fsFtpConnection.

	FTP server connections.

**********************************************************************************/

#if !defined(AFX_FSFTPCONNECTION_H__7A93846D_5A0C_4B2F_B07E_765B21315FB7__INCLUDED_)
#define AFX_FSFTPCONNECTION_H__7A93846D_5A0C_4B2F_B07E_765B21315FB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsServerConnection.h"
#include "fsinet.h"	// Added by ClassView

class fsFtpConnection : public fsInternetServerConnection  
{
public:
	// Connect with server. Described in fsInternetServerConnection.h file
	fsInternetResult Connect (LPCSTR pszServer, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT);
	// Switch on/off passive mode
	void UsePassiveMode (BOOL bUse);
	// Is passive mode ?
	BOOL IsPassiveMode();
	// Set current directory on server 
	fsInternetResult SetCurrentDirectory (LPCSTR pszDir);
	fsFtpConnection();
	virtual ~fsFtpConnection();

private:

protected:
	// Init m_pszLastError variable (base fsInternetServerConnection class member)
	// by a last response from a server in case then WinInet function returned the
	// ERROR_INTERNET_EXTENDED_ERROR error.
	void ReceiveExtError();
	BOOL m_bPassiveMode;	// using passive mode ?
};

#endif // !defined(AFX_FSFTPCONNECTION_H__7A93846D_5A0C_4B2F_B07E_765B21315FB7__INCLUDED_)
