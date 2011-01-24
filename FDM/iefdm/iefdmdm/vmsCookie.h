/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSCOOKIE_H__EC0577CA_4887_4432_8D95_1E758F0F6C62__INCLUDED_)
#define AFX_VMSCOOKIE_H__EC0577CA_4887_4432_8D95_1E758F0F6C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <fslist.h>
#include <fsString.h>

struct vmsSingleCookie
{
	fsString strName;
	fsString strValue;
};

class vmsCookie  
{
public:
	void Add (vmsCookie *pc);
	int GetCommonItemCount (vmsCookie *pc);
	fsString toString();
	int FindItemByName (LPCSTR pszName);
	vmsSingleCookie* get_Item (int nIndex);
	int get_ItemCount();
	void Set (LPCSTR pszCookie);
	vmsCookie();
	virtual ~vmsCookie();

protected:
	fs::list <vmsSingleCookie> m_vItems;

};

#endif 
