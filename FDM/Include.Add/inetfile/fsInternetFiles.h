/********************************************************************************

	Class fsInernetFiles.

	Used to get file list on server.
	Base class for fsFtpFiles and fsHttpFiles.

********************************************************************************/

#if !defined(AFX_FSINTERNETFILES_H__0E0074EB_2AD0_438E_A57C_4EFA0D8855E7__INCLUDED_)
#define AFX_FSINTERNETFILES_H__0E0074EB_2AD0_438E_A57C_4EFA0D8855E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "../list.h"
#include <fsString.h>
#include "fsinet.h"	// Added by ClassView
#include "fsInternetSession.h"

// File information
struct fsFileInfo
{
	fsString strName;		// name
	UINT64 uSize;			// size
	FILETIME date;			// last change date
	BOOL bAvailable;		// available foê downloading ?
	BOOL bFolder;			// it is a directory ?

	fsFileInfo ()
	{
		bFolder = FALSE;
		bAvailable = TRUE;
	}
};

class fsInternetFiles  
{
public:
	// Get file list.
	// pszPath - path to directory, we need to know what it contains.
	// For HTTP it also can be and URL of html file.
	// In this case all links in file will be "taked"
	virtual fsInternetResult GetList (LPCSTR pszPath) = NULL;
	// Abort file list receiving from server
	virtual void Abort () = NULL;
	// Returns number o files in a directory. Can call only after GetList
	UINT GetFileCount();
	// Returns current path. Same as pszPath in GetList.
	// Can call only after GetList.
	LPCSTR GetCurrentPath();
	// Get file information by it's order number (from 0 till files_number-1)
	fsFileInfo* GetFileInfo (UINT uIndex);
	virtual LPCSTR GetLastError () = NULL;
	fsInternetFiles();
	virtual ~fsInternetFiles();

protected:
	fsString m_strPath;					// current path
	fs::list <fsFileInfo> m_vFiles;		// file list
};

#endif // !defined(AFX_FSINTERNETFILES_H__0E0074EB_2AD0_438E_A57C_4EFA0D8855E7__INCLUDED_)
