/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSAPPUTIL_H__E0C0FA99_0E90_4A12_9D11_213C6C368ADB__INCLUDED_)
#define AFX_VMSAPPUTIL_H__E0C0FA99_0E90_4A12_9D11_213C6C368ADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class vmsAppUtil  
{
public:
	static fsInternetResult UrlDownloadToFile (LPCTSTR pszUrl, LPCTSTR pszFileName);
	vmsAppUtil();
	virtual ~vmsAppUtil();

};

#endif 
