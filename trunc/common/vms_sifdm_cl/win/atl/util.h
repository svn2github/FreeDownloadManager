/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

inline LONG vmsRegKeyQueryStringValue (CRegKey& key, LPCTSTR pszValueName,
	LPTSTR pszValue, ULONG* pnChars)
{
	LONG lRes;
	DWORD dwType;
	ULONG nBytes;

	ATLASSUME(key.m_hKey != NULL);
	ATLASSERT(pnChars != NULL);

	nBytes = (*pnChars)*sizeof(TCHAR);
	lRes = ::RegQueryValueEx(key.m_hKey, pszValueName, NULL, &dwType, reinterpret_cast<LPBYTE>(pszValue),
		&nBytes);

	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	if(dwType != REG_SZ && dwType != REG_EXPAND_SZ)
	{
		return ERROR_INVALID_DATA;
	}

	if (pszValue != NULL)
	{
		if(nBytes!=0)
		{
			
			
			const auto pos = nBytes/sizeof(TCHAR);
			if (pos < *pnChars)
				pszValue [pos] = 0;
			else
				return ERROR_INSUFFICIENT_BUFFER;
		}
		else
		{
			pszValue[0]=_T('\0');
		}
	}

	*pnChars = nBytes/sizeof(TCHAR);

	return ERROR_SUCCESS;
}