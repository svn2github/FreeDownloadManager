/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "../Include.Add/fsString.h"

#ifdef UNICODE
#define fsSaveStrToFile fsSaveStrToFileW
#define fsReadStrFromFile fsReadStrFromFileW
#else
#define fsSaveStrToFile fsSaveStrToFileA
#define fsReadStrFromFile fsReadStrFromFileA
#endif

inline BOOL fsSaveStrToFileA(LPCSTR pszStr, HANDLE hFile)
{
	int len;

	if (pszStr == NULL)
		len = -1;
	else
		len = strlen (pszStr);

	DWORD dw;

	if (!WriteFile (hFile, &len, sizeof (len), &dw, NULL))
		return FALSE;

	if (len != -1)
		return WriteFile (hFile, pszStr, len, &dw, NULL);

	return TRUE;
}

inline BOOL fsSaveStrToFileW(LPCWSTR pszStr, HANDLE hFile)
{
	int len;

	if (pszStr == NULL)
		len = -1;
	else
		len = wcslen (pszStr);

	DWORD dw;

	if (!WriteFile (hFile, &len, sizeof (len), &dw, NULL))
		return FALSE;

	if (len != -1)
		return WriteFile (hFile, pszStr, len * sizeof(WCHAR), &dw, NULL);

	return TRUE;
}

inline BOOL fsReadStrFromFileA(LPSTR *ppszStr, HANDLE hFile)
{
	int len;

	DWORD dw;

	if (!ReadFile (hFile, &len, sizeof (len), &dw, NULL) || dw != sizeof (len))
		return FALSE;

	if (len != -1)
	{
		if (UINT (len) > 100000)
			return FALSE; 

		*ppszStr = new CHAR [len+1];
		if (!ReadFile (hFile, *ppszStr, len, &dw, NULL))
			return FALSE;

		if (len != int (dw))
			return FALSE;

		(*ppszStr) [len] = 0;
	}
	else
		*ppszStr = NULL;

	return TRUE;
}

inline BOOL fsReadStrFromFileW(LPWSTR *ppszStr, HANDLE hFile)
{
	int len;

	DWORD dw;

	if (!ReadFile (hFile, &len, sizeof (len), &dw, NULL) || dw != sizeof (len))
		return FALSE;

	if (len != -1)
	{
		if (UINT (len) > 100000)
			return FALSE; 

		*ppszStr = new WCHAR [len+1];
		if (!ReadFile (hFile, *ppszStr, len * sizeof(WCHAR), &dw, NULL))
			return FALSE;

		if (len * sizeof(WCHAR) != int (dw))
			return FALSE;

		(*ppszStr) [len] = 0;
	}
	else
		*ppszStr = NULL;

	return TRUE;
}