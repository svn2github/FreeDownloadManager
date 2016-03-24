/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef __FXS_STRING_H_
#define __FXS_STRING_H_

#include <WinNT.h>
#include <tchar.h>
#include <stdio.h>

struct fsString
{
	LPTSTR pszString;

	fsString () 
	{
		pszString = NULL;
	}

	~fsString ()
	{
		if (pszString)
			delete [] pszString;
	}

	fsString (const fsString& str)
	{
		pszString = NULL;
		*this = str.pszString;
	}

	fsString (LPCTSTR str)
	{
		pszString = NULL;
		*this = str;
	}

	LPCTSTR operator = (LPCTSTR pszStr)
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}

		if (pszStr)
		{
           	pszString = new TCHAR [_tcslen (pszStr) + 1];
			if (pszString) 
				_tcscpy ( pszString, pszStr );
		}

		return pszString;
	}

	fsString& operator = (const fsString& str)
	{
		*this = str.pszString;
		return *this;
	}

	LPCTSTR operator += (LPCTSTR pszStr)
	{
		if (pszStr == NULL)
			return pszString;

		if ( pszString )
		{
			LPTSTR pszOld = pszString;

			pszString = new TCHAR [ _tcslen (pszString) + _tcslen (pszStr) + 1 ];
			_tcscpy ( pszString, pszOld );
			_tcscat ( pszString, pszStr );

			delete [] pszOld;
		}
		else
		{
			*this = pszStr;
		}

		return pszString;
	}

	LPCTSTR  operator += (TCHAR c)
	{
			TCHAR sz [2];
			sz [0] = c; sz [1] = 0;
			return *this += sz;
	}

	fsString operator + (LPCTSTR psz) const
	{
		fsString str = *this;
		str += psz;
		return str;
	}

	BOOL operator == (const fsString& str) const
	{
		return str == (LPCTSTR)pszString;
	}

	BOOL operator != (const fsString& str) const
	{
		return str != (LPCTSTR)pszString;
	}

	BOOL operator == (LPCTSTR pszStr) const
	{
		if (pszString == NULL || pszStr == NULL)
			return pszStr == pszString;

		return _tcscmp ( pszString, pszStr ) == 0;
	}

	void clear ()
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}
	}

	void ncpy (LPCTSTR pszStr, int nch)
	{
		alloc (nch);
		_tcsncpy_s (pszString, nch + 1, pszStr, nch);
	}

	void alloc (int nch)
	{
		clear ();
		pszString = new TCHAR [nch+1];
		pszString [nch] = 0;
	}

	BOOL operator != (LPCTSTR pszStr) const
	{
		return !(*this == pszStr);
	}

	operator LPTSTR () const
	{
		return pszString;
	}

	int Length () const
	{
		if (pszString)
			return _tcslen (pszString);
		else
			return 0;
	}

	BOOL IsEmpty () const
	{
		return pszString == NULL || *pszString == 0;
	}

	TCHAR LastChar () const
	{
		return pszString ? pszString [GetLength () - 1] : (TCHAR)0;
	}

	int GetLength () const {return Length ();}

	void Replace (LPCSTR , LPCSTR ) {}

	void Format (LPCTSTR pszFormat ...)
	{
		LPTSTR psz = new TCHAR [100000];
                va_list ap;
		va_start (ap, pszFormat);
#ifdef UNICODE
		vswprintf_s (psz, 100000, pszFormat, ap);
                va_end (ap);
#else
		vsprintf_s (psz, 100000, pszFormat, ap);
                va_end (ap);
#endif
		*this = psz;
		delete [] psz;
	}
};

struct fsStringA
{
	LPSTR pszString;

	fsStringA () 
	{
		pszString = NULL;
	}

	~fsStringA ()
	{
		if (pszString)
			delete [] pszString;
	}

	fsStringA (const fsStringA& str)
	{
		pszString = NULL;
		*this = str.pszString;
	}

	fsStringA (LPCSTR str)
	{
		pszString = NULL;
		*this = str;
	}

	LPCSTR operator = (LPCSTR pszStr)
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}

		if (pszStr)
		{
           	pszString = new CHAR [strlen (pszStr) + 1];
			if (pszString) 
				strcpy ( pszString, pszStr );
		}

		return pszString;
	}

	fsStringA& operator = (const fsStringA& str)
	{
		*this = str.pszString;
		return *this;
	}

	LPCSTR operator += (LPCSTR pszStr)
	{
		if (pszStr == NULL)
			return pszString;

		if ( pszString )
		{
			LPSTR pszOld = pszString;

			pszString = new CHAR [ strlen (pszString) + strlen (pszStr) + 1 ];
			strcpy ( pszString, pszOld );
			strcat ( pszString, pszStr );

			delete [] pszOld;
		}
		else
		{
			*this = pszStr;
		}

		return pszString;
	}

	LPCSTR  operator += (char c)
	{
			CHAR sz [2];
			sz [0] = c; sz [1] = 0;
			return *this += sz;
	}

	fsStringA operator + (LPCSTR psz) const
	{
		fsStringA str = *this;
		str += psz;
		return str;
	}

	BOOL operator == (const fsStringA& str) const
	{
		return str == (LPCSTR)pszString;
	}

	BOOL operator != (const fsStringA& str) const
	{
		return str != (LPCSTR)pszString;
	}

	BOOL operator == (LPCSTR pszStr) const
	{
		if (pszString == NULL || pszStr == NULL)
			return pszStr == pszString;

		return strcmp ( pszString, pszStr ) == 0;
	}

	void clear ()
	{
		if (pszString)
		{
			delete [] pszString;
			pszString = NULL;
		}
	}

	void ncpy (LPCSTR pszStr, int nch)
	{
		alloc (nch);
		strncpy_s (pszString, nch + 1, pszStr, nch);
	}

	void alloc (int nch)
	{
		clear ();
		pszString = new CHAR [nch+1];
		pszString [nch] = 0;
	}

	BOOL operator != (LPCSTR pszStr) const
	{
		return !(*this == pszStr);
	}

	operator LPSTR () const
	{
		return pszString;
	}

	int Length () const
	{
		if (pszString)
			return strlen (pszString);
		else
			return 0;
	}

	BOOL IsEmpty () const
	{
		return pszString == NULL || *pszString == 0;
	}

	char LastChar () const
	{
		return pszString ? pszString [GetLength () - 1] : (char)0;
	}

	int GetLength () const {return Length ();}

	void Replace (LPCSTR , LPCSTR ) {}

	void Format (LPCSTR pszFormat ...)
	{
		LPSTR psz = new CHAR [100000];
                va_list ap;
		va_start (ap, pszFormat);
		vsprintf_s (psz, 100000, pszFormat, ap);
                va_end (ap);
		*this = psz;
		delete [] psz;
	}
};

#endif