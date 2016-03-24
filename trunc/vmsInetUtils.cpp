/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
#include "vmsInetUtils.h"
#include <exception>
#include <memory>

bool vmsAnsiUrlToUnicode(LPCSTR szAnsiUrl, LPWSTR wszUnicodeUrl, DWORD* pdwSize, UINT uiCodePage)
{
	ASSERT(szAnsiUrl != 0);
	ASSERT(wszUnicodeUrl != 0);
	ASSERT(pdwSize != 0);

	if (pdwSize == 0 || szAnsiUrl == 0 || wszUnicodeUrl == 0)
		throw std::exception("Cannot convert URL to UNICODE");

	int nLen = ::MultiByteToWideChar(uiCodePage, 0, szAnsiUrl, -1, 0, 0);
	if (*pdwSize < nLen + 1) {
		*pdwSize = nLen + 1;
		return false;
	}

	std::auto_ptr<WCHAR> apwszUnicodeUrl( new WCHAR[nLen + 1] );
	WCHAR* _wszUnicodeUrl = apwszUnicodeUrl.get();
	if (_wszUnicodeUrl == 0)
		throw std::exception("Out of memory");
	
	::MultiByteToWideChar(uiCodePage, 0, szAnsiUrl, -1, wszUnicodeUrl, nLen);
	wszUnicodeUrl[nLen] = 0;
	return true;
}

bool vmsUnicodeUrlToAnsi(LPCWSTR wszUnicodeUrl, LPSTR szAnsiUrl, DWORD* pdwSize, UINT uiCodePage)
{
	ASSERT(szAnsiUrl != 0);
	ASSERT(wszUnicodeUrl != 0);
	ASSERT(pdwSize != 0);

	if (pdwSize == 0 || szAnsiUrl == 0 || wszUnicodeUrl == 0)
		throw std::exception("Cannot convert URL to UNICODE");

	int nLen = ::WideCharToMultiByte(uiCodePage, 0, wszUnicodeUrl, -1, 0, 0, 0, 0);
	if (*pdwSize < nLen + 1) {
		*pdwSize = nLen + 1;
		return false;
	}

	std::auto_ptr<CHAR> apwszAnsiUrl( new CHAR[nLen + 1] );
	CHAR* _wszAnsiUrl = apwszAnsiUrl.get();
	if (_wszAnsiUrl == 0)
		throw std::exception("Out of memory");
	
	::WideCharToMultiByte(uiCodePage, 0, wszUnicodeUrl, -1, szAnsiUrl, nLen, 0, 0);
	szAnsiUrl[nLen] = 0;
	return true;
}
