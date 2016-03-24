/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef UTILS_INCLUDED_FILE
#define UTILS_INCLUDED_FILE

#include <windows.h>
#include <string>
#include <tchar.h>

const TCHAR* const _pszFDMCustomizationsErr = _T("fdmcusterr");

void UniToAnsi(const std::wstring& sSrc, std::string& sTar);
void AnsiToUni(const std::string& sSrc, std::wstring& sTar);
void UniToAnsi(LPCWSTR wszSrc, int nLen, LPSTR szDest);
void AnsiToUni(LPCSTR szSrc, int nLen, LPWSTR wszDest);
bool vmsAnsiUrlToUnicode(LPCSTR szAnsiUrl, LPWSTR wszUnicodeUrl, DWORD* pdwSize, UINT uiCodePage = CP_ACP);
bool vmsUnicodeUrlToAnsi(LPCWSTR wszUnicodeUrl, LPSTR szAnsiUrl, DWORD* pdwSize, UINT uiCodePage = CP_ACP);

std::wstring getUniStrIniter(LPCSTR sz);
std::wstring _AnsiToUni(const std::string& sSrc);
std::string _UniToAnsi(const std::wstring& sSrc);

#ifdef UNICODE
#define GET_TSTR_INITER(x) getUniStrIniter(x)
#define ANSI_TO_TSTR(x) _AnsiToUni(x)
#define TSTR_TO_ANSI(x) _UniToAnsi(x)
#define _TSTR_TO_ANSI(src, len, dest) UniToAnsi(src, len, dest)
#define _ANSI_TO_TSTR(src, len, dest) AnsiToUni(src, len, dest)
#else
#define GET_TSTR_INITER(x) x
#define ANSI_TO_TSTR(x) x
#define TSTR_TO_ANSI(x) x
#define _TSTR_TO_ANSI(x, l, y) strncpy(y, x, l)
#define _ANSI_TO_TSTR(src, len, dest) strncpy(y, x, l)
#endif

#endif
