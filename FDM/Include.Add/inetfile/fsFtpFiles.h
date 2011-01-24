/********************************************************************************

	Class fsFtpFiles.

	Used to get a file list on FTP server.

********************************************************************************/

#if !defined(AFX_FSFTPFILES_H__9903206C_EB0F_46C8_A778_FF0D15F795F5__INCLUDED_)
#define AFX_FSFTPFILES_H__9903206C_EB0F_46C8_A778_FF0D15F795F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsInternetFiles.h"
#include "fsinet.h"	// Added by ClassView
#include "fsFtpConnection.h"	// Added by ClassView

class fsFtpFiles : public fsInternetFiles  
{
public:
	// Described in fsInternetFiles.h file
	fsInternetResult GetList (LPCSTR pszUrl);
	void Abort();
	LPCSTR GetLastError();
	//////

	// Stop server, we'll work with
	void SetServer (fsFtpConnection* pServer);
	// Skip a cache while receiving a file list (IE cache) ?
	void Reload (BOOL bReload = TRUE);
	
	
	fsFtpFiles();
	virtual ~fsFtpFiles();

protected:
	fsFtpConnection* m_pServer;	// connecting with server object
	BOOL m_bReload;				// skip cache ?
	BOOL m_bAbort;				// abort file list receiving operation ?
	// Build a file list. Called from the GetList ()
	fsInternetResult BuildList();
};

#endif // !defined(AFX_FSFTPFILES_H__9903206C_EB0F_46C8_A778_FF0D15F795F5__INCLUDED_)
