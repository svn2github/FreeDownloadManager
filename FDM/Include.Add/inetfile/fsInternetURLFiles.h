/********************************************************************************

	Class fsInternetURLFiles.

	Unified server file list receiving without depending on file
	transmission protocol.

********************************************************************************/

#if !defined(AFX_FSINTERNETURLFILES_H__78826A95_6DE1_4BE1_AD42_A30F33A5FC9C__INCLUDED_)
#define AFX_FSINTERNETURLFILES_H__78826A95_6DE1_4BE1_AD42_A30F33A5FC9C__INCLUDED_

#include "fsHttpFiles.h"	// Added by ClassView
#include "fsFtpFiles.h"	// Added by ClassView
#include "fsinet.h"	// Added by ClassView
#include "fsInternetFiles.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fsInternetUrlServer.h"
#include "fsHttpFiles.h"
#include "fsFtpFiles.h"

class fsInternetURLFiles  
{
public:
	// All methods are described in fsFtpFiles.h, fsHttpFiles.h, fsInternetFiles.h
	fsInternetResult GetList (fsInternetURLServer *pServer, LPCSTR pszPath);
	void RetreiveInfoWhileGettingList (BOOL b);
	void SetHttpEventFunc (fntHttpFilesEventFunc pfn, LPVOID lpParam);
	LPCSTR GetCurrentPath();
	LPCSTR GetLastError();
	void Abort();
	fsFileInfo* GetFileInfo (UINT uIndex);
	UINT GetFileCount();
	fsInternetURLFiles();
	virtual ~fsInternetURLFiles();

protected:
	fsInternetFiles* m_pFiles;
	fsFtpFiles m_ftpFiles;
	fsHttpFiles m_httpFiles;
};

#endif // !defined(AFX_FSINTERNETURLFILES_H__78826A95_6DE1_4BE1_AD42_A30F33A5FC9C__INCLUDED_)
