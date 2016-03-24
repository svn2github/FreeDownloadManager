/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSBATCHLIST_H__882E64F6_5B91_4027_9B79_EFBBA47FF941__INCLUDED_)
#define AFX_VMSBATCHLIST_H__882E64F6_5B91_4027_9B79_EFBBA47FF941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"

#define BATCHLIST_STEP_SYMBOL	's'
#define BATCHLIST_WILDCARD_SYMBOL	'w'

class vmsBatchList  
{
public:
	
	void Clear();
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	bool Create (LPCTSTR pszTemplate, LPCTSTR pszNumbers, char chStart = 0, char chEnd = 0);

	int get_ResultCount();
	LPCTSTR get_Result (int nIndex);
	
	vmsBatchList();
	virtual ~vmsBatchList();

protected:
	
	bool CreateAZBatch (LPCTSTR pszTemplate, char chStart, char chEnd);
	fs::list <CString> m_v; 

};

#endif 
