/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef INET_UTILS_INCLUDED_FILE
#define INET_UTILS_INCLUDED_FILE

bool vmsAnsiUrlToUnicode(LPCSTR szAnsiUrl, LPWSTR wszUnicodeUrl, DWORD* pdwSize, UINT uiCodePage = CP_ACP);
bool vmsUnicodeUrlToAnsi(LPCWSTR szUnicodeUrl, LPSTR wszAnsiUrl, DWORD* pdwSize, UINT uiCodePage = CP_ACP);

#endif
