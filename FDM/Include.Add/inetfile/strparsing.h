#ifndef __STR_PARSING__H_
#define __STR_PARSING__H_

// ñ - is a divider ?
inline BOOL fsStrIsDivider (char c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
		return TRUE;
	
	return FALSE;
}

// Skip dividers (spaces, etc.)
// Returns pointer to a area after dividers
inline LPCSTR fsStrSkipDividers (LPCSTR psz)
{
	while (fsStrIsDivider (*psz)) 
		psz ++;

	return psz;
}

// Skip spaces
// [in] - pointer to start position in string
// ret: pointer to position after spaces.
// e.g. : 
// if psz is "   abc 1"
// it will return "abc 1".
inline const char* fsStrSkipSpaces (const char* psz)
{
	while (*psz == ' ') 
		psz ++;

	return psz;
}

// find substring in string without regard to case 
extern LPCSTR fsStrStrNoCase(LPCSTR pszIn, LPCSTR pszWhat);
// Get string, beginning with pszFrom and ending with any symbol from pszCharTo
// ppszResult, [out] - Result. Application frees memory by itself
// Returns pointer to a area after the result (passing also a border symbol)
// in pszFrom string
extern LPCSTR fsStrGetStrUpToChar (LPCSTR pszFrom, LPCSTR pszCharTo, LPSTR* ppszResult);

#endif