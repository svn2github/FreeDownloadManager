/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __STR_PARSING__H_
#define __STR_PARSING__H_

#include <tchar.h>

inline BOOL fsStrIsDividerA (CHAR c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
		return TRUE;
	
	return FALSE;
}

inline LPCSTR fsStrSkipDividersA (LPCSTR psz)
{
	while (fsStrIsDividerA (*psz)) 
		psz ++;

	return psz;
}

inline BOOL fsStrIsDividerW (WCHAR c)
{
	if (c == L' ' || c == L'\n' || c == L'\r' || c == L'\t')
		return TRUE;
	
	return FALSE;
}

inline LPCWSTR fsStrSkipDividersW (LPCWSTR psz)
{
	while (fsStrIsDividerW (*psz)) 
		psz ++;

	return psz;
}

#ifdef UNICODE
#define fsStrIsDivider fsStrIsDividerW
#define fsStrSkipDividers fsStrSkipDividersW
#else
#define fsStrIsDivider fsStrIsDividerA
#define fsStrSkipDividers fsStrSkipDividersA
#endif

inline const TCHAR* fsStrSkipSpaces (const TCHAR* psz)
{
	while (*psz == _T(' ')) 
		psz ++;

	return psz;
}

extern LPCWSTR fsStrStrNoCaseW(LPCWSTR pszIn, LPCWSTR pszWhat);
extern LPCSTR fsStrStrNoCaseA(LPCSTR pszIn, LPCSTR pszWhat);

extern LPCWSTR fsStrGetStrUpToCharW (LPCWSTR pszFrom, LPCWSTR pszCharTo, LPWSTR* ppszResult);
extern LPCSTR fsStrGetStrUpToCharA (LPCSTR pszFrom, LPCSTR pszCharTo, LPSTR* ppszResult);

#ifdef UNICODE
#define fsStrGetStrUpToChar fsStrGetStrUpToCharW
#define fsStrStrNoCase fsStrStrNoCaseW
#else
#define fsStrGetStrUpToChar fsStrGetStrUpToCharA
#define fsStrStrNoCase fsStrStrNoCaseA
#endif

#endif