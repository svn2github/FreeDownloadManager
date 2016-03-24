/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "Utils.h"

#include <memory>
#include <exception>
#include <windows.h>

void UniToAnsi(const std::wstring& sSrc, std::string& sTar)
{

	int nLen = ::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), -1, 0, 0, 0, 0);
	std::auto_ptr<CHAR> pchStrGuard( new CHAR[nLen+1] );
	CHAR* szTar = pchStrGuard.get();
	if (szTar == 0) {
		throw std::exception("Out of memory");
	}
	memset(szTar, 0, sizeof(CHAR) * (nLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, sSrc.c_str(), sSrc.length(), szTar, sSrc.length(), 0, 0);
	sTar = szTar;

}

void AnsiToUni(const std::string& sSrc, std::wstring& sTar)
{

	int nLen = ::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), -1, 0, 0);
	std::auto_ptr<WCHAR> pwchTarGuard( new WCHAR[nLen+1] );
	WCHAR* wszTar = pwchTarGuard.get();
	if (wszTar == 0) {
		throw std::exception("Out of memory");
	}
	memset(wszTar, 0, sizeof(WCHAR) * (nLen + 1));
	::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), sSrc.length(), wszTar, sSrc.length());
	sTar = wszTar;

}

std::wstring getUniStrIniter(LPCSTR sz)
{
	std::wstring sTmp;
	AnsiToUni(std::string(sz), sTmp);
	return sTmp;
}

std::wstring _AnsiToUni(const std::string& sSrc)
{
	std::wstring sTmp;
	AnsiToUni(sSrc, sTmp);
	return sTmp;
}

std::string _UniToAnsi(const std::wstring& sSrc)
{
	std::string sTmp;
	UniToAnsi(sSrc, sTmp);
	return sTmp;
}

void UniToAnsi(LPCWSTR wszSrc, int nLen, LPSTR szDest)
{
	int _nLen = ::WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, 0, 0, 0, 0);
	if (_nLen < nLen)
		throw std::exception("Unknown error");
	::WideCharToMultiByte(CP_ACP, 0, wszSrc, -1, szDest, _nLen, 0, 0);
}

void AnsiToUni(LPCSTR szSrc, int nLen, LPWSTR wszDest)
{
	int _nLen = ::MultiByteToWideChar(CP_ACP, 0, szSrc, -1, 0, 0);
	if (_nLen < nLen)
		throw std::exception("Unknown error");
	::MultiByteToWideChar(CP_ACP, 0, szSrc, -1, wszDest, _nLen);
}

bool vmsAnsiUrlToUnicode(LPCSTR szAnsiUrl, LPWSTR wszUnicodeUrl, DWORD* pdwSize, UINT uiCodePage)
{
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
