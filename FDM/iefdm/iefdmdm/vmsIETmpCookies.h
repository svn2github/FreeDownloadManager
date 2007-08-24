/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/              

#if !defined(AFX_VMSIETMPCOOKIES_H__5CE05C63_3540_4132_9D7A_D08FA5CBC974__INCLUDED_)
#define AFX_VMSIETMPCOOKIES_H__5CE05C63_3540_4132_9D7A_D08FA5CBC974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <fslist.h>
#include <fsString.h>

class vmsIETmpCookies  
{
public:
	
	LPCSTR get_PostData (int nIndex);
	LPCSTR get_Referer (int nIndex);
	LPCSTR get_Cookies (int nIndex);
	
	int Find (LPCSTR pszUrl);

	vmsIETmpCookies();
	virtual ~vmsIETmpCookies();

protected:
	
	
	static fsString GetLevel2DomainName (LPCSTR pszHostName);
	
	void ProcessFile (LPCSTR pszFile);
	
	
	void GetListOfKnownCookies();
	
	HANDLE m_mxFile;
	
	fs::list <fsString> m_vUrls;
	fs::list <fsString> m_vCookies;
	fs::list <fsString> m_vPostDatas;
	fs::list <fsString> m_vBeforeNavUrls;
};

#endif 
