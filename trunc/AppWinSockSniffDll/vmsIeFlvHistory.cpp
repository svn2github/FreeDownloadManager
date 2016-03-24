/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsIeFlvHistory.h"
#include <wininet.h>
#include "vmsHttpTrafficCollector.h"

vmsIeFlvHistory::vmsIeFlvHistory()
{

}

vmsIeFlvHistory::~vmsIeFlvHistory()
{

}

void vmsIeFlvHistory::Gather()
{
	m_vItems.clear ();

	DWORD dwSize = 0; DWORD dwBufSize = 0;
	FindFirstUrlCacheEntry (NULL, NULL, &dwSize);
	if (dwSize == 0)
		return;
	
	INTERNET_CACHE_ENTRY_INFO *cei = (INTERNET_CACHE_ENTRY_INFO*) new BYTE [(dwBufSize = dwSize)+1];
	cei->dwStructSize = dwSize;
	HANDLE hFind = FindFirstUrlCacheEntry (NULL, cei, &dwSize);
	((LPBYTE)cei) [dwSize] = 0; 
	
	while (cei != NULL)
	{
		if ((cei->CacheEntryType & COOKIE_CACHE_ENTRY) == 0 &&
				cei->lpHeaderInfo != NULL && cei->dwHeaderInfoSize != 0)
		{
			LPCTSTR pszUrl2 = cei->lpszSourceUrlName;
			if (_tcsncmp (pszUrl2, _T("Visited: "), 9) == 0)
				pszUrl2 += 9;
			pszUrl2 = pszUrl2 + 0;

			LPCTSTR pszCT = _tcsstr (cei->lpHeaderInfo, _T("Content-Type:"));
			if (pszCT)
			{
				pszCT += 13+1;
				tstring strCT;
				while (!_istspace ((unsigned)*pszCT) && cei->dwHeaderInfoSize > (pszCT - cei->lpHeaderInfo))
					strCT += *pszCT++;
				if (vmsHttpTrafficCollector::isFlashVideoCT (stringFromTstring (strCT).c_str ()))
				{
					vmsBrowserFlvHistory::Item item;
					item.strUrl = stringFromTstring (pszUrl2);
					item.strHttpResponse = stringFromTstring ((LPCTSTR)cei->lpHeaderInfo);
					item.strContentType = stringFromTstring (strCT);
					m_vItems.push_back (item);
				}
			}
		}
		
		dwSize = 0;
		FindNextUrlCacheEntry (hFind, NULL, &dwSize);

		if (dwSize)
		{
			if (dwBufSize < dwSize)
			{
				delete [] (LPBYTE) cei;
				cei = (INTERNET_CACHE_ENTRY_INFO*) new BYTE [(dwBufSize = dwSize)+1];
				((LPBYTE)cei) [dwBufSize] = 0;
			}

			cei->dwStructSize = dwSize;
			FindNextUrlCacheEntry (hFind, cei, &dwSize);
			((LPBYTE)cei) [dwSize] = 0; 
		}
		else
		{
			delete [] (LPBYTE) cei;
			cei = NULL;
		}
	}
	
	FindCloseUrlCache (hFind);
}
