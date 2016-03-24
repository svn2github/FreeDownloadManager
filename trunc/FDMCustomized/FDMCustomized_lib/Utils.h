/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef UTILS_INCLUDED_FILE
#define UTILS_INCLUDED_FILE

#include <string>
#include <tchar.h>

const TCHAR* const _pszFDMCustomizationsErr = _T("fdmcusterr");

void UniToAnsi(const std::wstring& sSrc, std::string& sTar);
void AnsiToUni(const std::string& sSrc, std::wstring& sTar);

#endif
