/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSUPLOADERDLLPERSIST_H__A3F75517_0BA7_4E82_8F79_BCBB3F87F7F3__INCLUDED_)
#define AFX_VMSUPLOADERDLLPERSIST_H__A3F75517_0BA7_4E82_8F79_BCBB3F87F7F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "Uploader\CoreDll\dll.h"

class vmsUploaderDllPersist : public vmsUploadsDll::vmsDllPersist
{
public:
	BOOL GetProfileString (LPCTSTR pszSecion, LPCTSTR pszName, LPCTSTR pszDefValue, LPTSTR pszValue, DWORD *pdwValueSize);
	void WriteProfileString (LPCTSTR pszSecion, LPCTSTR pszName, LPCTSTR pszValue);
	void WriteProfileInt(LPCTSTR pszSection, LPCTSTR pszName, UINT nValue);
	UINT GetProfileInt(LPCTSTR pszSection, LPCTSTR pszName, UINT nDefValue);
	void WriteProfileBinary (LPCTSTR pszSection, LPCTSTR pszName, LPVOID pvData, UINT nDataSize);
	BOOL GetProfileBinary (LPCTSTR pszSecion, LPCTSTR pszName, LPBYTE *ppData, UINT *pnDataSize);
	
	void FreeBuffer (LPBYTE pb);

	vmsUploaderDllPersist();
	virtual ~vmsUploaderDllPersist();

};

#endif 
