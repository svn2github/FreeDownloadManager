/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"
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
		throw _pszFDMCustomizationsErr;
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
		throw _pszFDMCustomizationsErr;
	}
	memset(wszTar, 0, sizeof(WCHAR) * (nLen + 1));
	::MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), sSrc.length(), wszTar, sSrc.length());
	sTar = wszTar;

}
