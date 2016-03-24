/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsMozillaPrefs.h"

vmsMozillaPrefs::vmsMozillaPrefs()
{

}

vmsMozillaPrefs::~vmsMozillaPrefs()
{

}

bool vmsMozillaPrefs::LoadPrefs(LPCTSTR pszFile)
{
	Free ();

	

	HANDLE hFile = CreateFile (pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	
	int n = GetFileSize (hFile, NULL);
	std::auto_ptr<char> apchPrefsGuard( new char [n+1] );
	char* szPrefs = apchPrefsGuard.get();
	memset(szPrefs, 0, n + 1);

	DWORD dw;
	ReadFile (hFile, szPrefs, n, &dw, NULL);
	m_strPrefs = szPrefs;

	CloseHandle (hFile);

	return true;
}

void vmsMozillaPrefs::Free()
{
	m_strPrefs = "";
}

std::string vmsMozillaPrefs::get_Value(LPCSTR pszPrefName) const
{
	LPCSTR pszPrefs = m_strPrefs.c_str();

	std::string strPrefName; 
	strPrefName = "\""; 
	strPrefName += pszPrefName;
	strPrefName += "\""; 

	

	while (*pszPrefs && strnicmp (pszPrefs, strPrefName.c_str(), strPrefName.length ()))
		pszPrefs++;

	if (*pszPrefs == 0)
		return MOZILLAPREFS_VALUE_NOT_FOUND;

	pszPrefs += strPrefName.length ();
	
	
	while (*pszPrefs && *pszPrefs != ',')
		pszPrefs++;

	if (*pszPrefs == 0)
		return MOZILLAPREFS_UNEXPECTED_ERROR;

	
	pszPrefs++;
	while (*pszPrefs == ' ' || *pszPrefs == '\t')
		pszPrefs++;

	bool bInQ = false; 
	if (*pszPrefs == '"') {
		pszPrefs++;
		bInQ = true;
	}

	std::string strValue;

	
	
	if (bInQ)
	{
		while (*pszPrefs && *pszPrefs != '"')
		{
			char c = *pszPrefs++;

			
			if (c == '\\')
			{
				switch (*pszPrefs)
				{
				case 'n': c = '\n'; break;
				case 't': c = '\t'; break;
				case 'r': c = '\r'; break;
				default: c = *pszPrefs; break;
				}

				pszPrefs++;
			}

			strValue += c;
		}
	}
	else
	{
		while (IsCharAlphaNumeric (*pszPrefs))
			strValue += *pszPrefs++;
	}

	return strValue;
}
