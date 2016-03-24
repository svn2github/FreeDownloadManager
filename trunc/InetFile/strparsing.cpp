/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include <windows.h>
#include <tchar.h>
#include "common.h"

LPCWSTR fsStrStrNoCaseW(LPCWSTR pszIn, LPCWSTR pszWhat)
{
	UINT uLen = wcslen (pszWhat);

	if (pszIn == NULL)
		return NULL;

	while (*pszIn)
	{
		if (wcsnicmp (pszIn, pszWhat, uLen) == 0)
			return pszIn;
		else
			pszIn++;
	}

	return NULL;
}

LPCSTR fsStrStrNoCaseA(LPCSTR pszIn, LPCSTR pszWhat)
{
	UINT uLen = strlen (pszWhat);

	if (pszIn == NULL)
		return NULL;

	while (*pszIn)
	{
		if (strnicmp (pszIn, pszWhat, uLen) == 0)
			return pszIn;
		else
			pszIn++;
	}

	return NULL;
}

LPCWSTR fsStrGetStrUpToCharW (LPCWSTR pszFrom, LPCWSTR pszCharTo, LPWSTR* ppszResult)
{

	
	
	int tolen = wcscspn (pszFrom, pszCharTo);

	*ppszResult = NULL;

	if (pszFrom [tolen] == 0) 
	{

		return NULL;
	}

	fsnew (*ppszResult, WCHAR, tolen + 1);
	CopyMemory (*ppszResult, pszFrom, tolen * sizeof(WCHAR));
	(*ppszResult) [tolen] = 0;

	return pszFrom + tolen + 1; 
}

LPCSTR fsStrGetStrUpToCharA (LPCSTR pszFrom, LPCSTR pszCharTo, LPSTR* ppszResult)
{

	
	
	int tolen = strcspn (pszFrom, pszCharTo);

	*ppszResult = NULL;

	if (pszFrom [tolen] == 0) 
	{

		return NULL;
	}

	fsnew (*ppszResult, CHAR, tolen + 1);
	CopyMemory (*ppszResult, pszFrom, tolen);
	(*ppszResult) [tolen] = 0;

	return pszFrom + tolen + 1; 
}
