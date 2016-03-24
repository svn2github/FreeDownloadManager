/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSSTRINGLIST_H__C29CF9E3_9C2B_42BF_A976_4F2B69F7DF0B__INCLUDED_)
#define AFX_VMSSTRINGLIST_H__C29CF9E3_9C2B_42BF_A976_4F2B69F7DF0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"

class vmsStringList  
{
public:
	void Clear();
	
	BOOL Load (HANDLE hFile, WORD wVer);
	BOOL Save (HANDLE hFile);
	LPCTSTR get_String (int nIndex) const;
	void Del (int nIndex);
	int get_Count() const;
	void Add (LPCTSTR psz);

	vmsStringList();
	virtual ~vmsStringList();

protected:
	bool LoadItems(HANDLE hFile, int nItemCount, WORD wVer);
	bool LoadItems_old(HANDLE hFile, int nItemCount, WORD wVer);

	fs::list <fsString> m_vList;
};

#endif 
