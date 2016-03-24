/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsStringHelper  
{
public:
	static tstring RemoveBadCharacters (LPCTSTR ptsz)
	{
		tstring tstr = ptsz;
		for (size_t i = 0; i < tstr.length (); i++)
		{
			if ((UINT)tstr [i] >= ' ')
				continue;
			TCHAR ch = tstr [i];
			if (ch == '\t' || ch == '\r' || ch == '\n')
				continue;
			tstr.erase (tstr.begin () + i--);
		}
		return tstr;
	}

	static tstring CutFrameSpaces (LPCTSTR ptsz)
	{
		return Trim(ptsz, _T(' '));
	}

	static tstring Trim(LPCTSTR source, TCHAR symbol)
	{
		if (!source)
			return tstring();

		tstring ret = TrimLeft(source, symbol);
		return TrimRight(ret.c_str(), symbol);
	}

	static tstring TrimLeft(LPCTSTR source, TCHAR symbol)
	{
		if (!source)
			return tstring();

		while (*source == symbol)
			++source;

		return tstring(source);
	}

	static tstring TrimRight(LPCTSTR source, TCHAR symbol)
	{
		if (!source)
			return tstring();

		LPCTSTR end = source + _tcslen(source) - 1;
		while (*end == symbol && end > source)
			--end;

		tstring ret;
		ret.assign(source, end - source + 1);
		return ret;
	}

	
	static LPCTSTR StrNIStr(LPCTSTR szStringToBeSearched, LPCTSTR szSubstringToSearchFor, int nStringLen = -1)
	{
		int            nLen;
		int            nOffset;
		int            nMaxOffset;
		LPCTSTR		pPos;
		int            nStringLenInt;

		
		if ( szStringToBeSearched == NULL ||
			szSubstringToSearchFor == NULL )
		{
			return szStringToBeSearched;
		}

		
		nLen = (int)_tcslen(szSubstringToSearchFor);

		
		if ( nLen == 0 ) {
			return szStringToBeSearched;
		}

		if ( nStringLen == -1 || nStringLen >
			(int)_tcslen(szStringToBeSearched) )
		{
			nStringLenInt = (int)_tcslen(szStringToBeSearched);
		} else {
			nStringLenInt = nStringLen;
		}

		nMaxOffset = nStringLenInt - nLen;

		pPos = szStringToBeSearched;

		for ( nOffset = 0; nOffset <= nMaxOffset; nOffset++ ) {

			if ( _tcsnicmp(pPos, szSubstringToSearchFor, nLen) == 0 ) {
				return pPos;
			}
			
			pPos++; 
		}

		return NULL;
	}
};

