/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "fsURL.h"
#include <fsString.h>
#include "common.h"

fsURL::fsURL()
{

}

fsURL::~fsURL()
{

}

fsInternetResult fsURL::Crack(LPCTSTR pszUrl, BOOL bCheckScheme)
{
	
	if (lstrcmpi (pszUrl, _T("http://")) == 0)
		pszUrl = _T("http://url");
	else if (lstrcmpi (pszUrl, _T("ftp://")) == 0)
		pszUrl = _T("ftp://url");
	else if (lstrcmpi (pszUrl, _T("https://")) == 0)
		pszUrl = _T("https://url");

	DWORD urlLen = _tcslen (pszUrl) * 2;
	LPTSTR pszCanUrl = NULL;
	fsString strUrl;

	
	if (*pszUrl == _T('"') || *pszUrl == _T('\''))
	{
		
		
		strUrl = pszUrl + 1;
		if (strUrl [0] == 0)
			return IR_BADURL;
		strUrl [strUrl.Length () - 1] = 0;
		pszUrl = strUrl;
	}
		
	fsnew (pszCanUrl, TCHAR, urlLen+1);
	bool isMagnet = false;
	LPCTSTR magnetStart = _tcsstr (pszUrl, _T("magnet:"));
	isMagnet = (magnetStart != 0 && pszUrl == magnetStart);

	
	if ((pszUrl [0] == '\\' && pszUrl [1] == '\\') || isMagnet)
	{
		m_url.nScheme = INTERNET_SCHEME_FILE; 
		_tcscpy_s (m_szPath, 10000, pszUrl);
		_tcscpy_s (m_szScheme, 100, _T("file"));
		*m_szUser = 0;
		*m_szPassword = 0;
		m_url.nPort = 0;
		if (isMagnet)
		{
			m_szHost[0] = 0;
		}
	}
	else 
	{
		if (_tcsnicmp (pszUrl, _T ("file://"), 7)) 
		{
			
			if (!InternetCanonicalizeUrl (pszUrl, pszCanUrl, &urlLen, ICU_BROWSER_MODE))
			{
				delete pszCanUrl;

				if (GetLastError () == ERROR_INSUFFICIENT_BUFFER)
				{
					fsnew (pszCanUrl, TCHAR, urlLen+1);
					if (!InternetCanonicalizeUrl (pszUrl, pszCanUrl, &urlLen, ICU_BROWSER_MODE))
					{
						delete pszCanUrl;
						return fsWinInetErrorToIR ();
					}
				}
				else
					return fsWinInetErrorToIR ();
			}
		}
		else
		{
			
			_tcscpy (pszCanUrl, pszUrl);
		}

		ZeroMemory (&m_url, sizeof (m_url));
		m_url.dwStructSize = sizeof (m_url);
	
		m_url.lpszHostName = m_szHost;
		m_url.lpszPassword = m_szPassword;
		m_url.lpszScheme = m_szScheme;
		m_url.lpszUrlPath = m_szPath;
		m_url.lpszUserName = m_szUser;

		m_url.dwHostNameLength = URL_HOSTNAME_SIZE;
		m_url.dwPasswordLength = URL_PASSWORD_SIZE;
		m_url.dwSchemeLength = URL_SCHEME_SIZE;
		m_url.dwUrlPathLength = URL_PATH_SIZE;
		m_url.dwUserNameLength = URL_USERNAME_SIZE;

		
		if (!InternetCrackUrl (pszCanUrl, urlLen, 
				_tcsnicmp (pszCanUrl, _T ("ftp://"), 6) == 0 ? ICU_DECODE : 0, &m_url))
		{
			delete pszCanUrl;
			return fsWinInetErrorToIR ();
		}

		

		delete pszCanUrl;
	
		if (bCheckScheme)	
		{
			if (m_url.nScheme != INTERNET_SCHEME_HTTP && m_url.nScheme != INTERNET_SCHEME_FTP &&
				 m_url.nScheme != INTERNET_SCHEME_HTTPS && m_url.nScheme != INTERNET_SCHEME_FILE)
			{
				return IR_BADURL;
			}
		}
		FixWinInetBug ();
	}
	
	if (m_url.nScheme == INTERNET_SCHEME_FILE)
	{
		
		if (m_szPath [0] == _T('\\') && m_szPath [1] == _T('\\')) 
														
		{
			
			size_t iPathStart = _tcscspn (m_szPath + 2, _T("\\/")) + 2; 
			if (iPathStart == _tcslen (m_szPath))
				return IR_BADURL;
			_tcsncpy_s (m_szHost, m_szPath + 2, iPathStart - 2); 
			m_szHost [iPathStart - 2] = 0;
			_tcscpy_s (m_szPath, 10000, m_szPath + iPathStart);	
		}
	}

	return IR_SUCCESS;
}

LPCTSTR fsURL::GetPath()
{
	return m_szPath;
}

LPCTSTR fsURL::GetHostName()
{
	return m_szHost;
}

LPCTSTR fsURL::GetScheme()
{
	return m_szScheme;
}

LPCTSTR fsURL::GetUserName()
{
	return m_szUser;
}

LPCTSTR fsURL::GetPassword()
{
	return m_szPassword;
}

INTERNET_SCHEME fsURL::GetInternetScheme()
{
	return m_url.nScheme;
}

INTERNET_PORT fsURL::GetPort()
{
	return m_url.nPort;
}

fsInternetResult fsURL::Create(INTERNET_SCHEME nScheme, LPCTSTR lpszHostName, INTERNET_PORT nPort, LPCTSTR lpszUserName, LPCTSTR lpszPassword, LPCTSTR lpszUrlPath, LPTSTR lpszUrl, DWORD *pdwUrlLength)
{
	if (!lpszUrl)
		return IR_ERROR;

	*lpszUrl = 0;

	ZeroMemory (&m_url, sizeof (m_url));
	m_url.dwStructSize = sizeof (m_url);

	if (lpszUserName && *lpszUserName == 0)
		lpszUserName = NULL;

	if (lpszPassword && *lpszPassword == 0)
		lpszPassword = NULL;

	fsString strHost, strPath;

	strHost = lpszHostName;
	strPath = lpszUrlPath;

	if (nScheme == INTERNET_SCHEME_FILE)	
	{
		

		lpszUserName = NULL;	
		lpszPassword = NULL;

		if (strHost.Length ())
		{
			fsString str;
			str = _T("\\\\"); 
			str += strHost; 
			if (strPath [0] != _T('\\') && strPath [0] != _T('/'))
				str += _T('\\');
			str += strPath;	
			strHost = _T("");
			strPath = str;
		}

		
		LPCTSTR magnetStart = _tcsstr (strPath, _T("magnet:"));
		if (magnetStart == 0 || strPath != magnetStart)
		{
			_tcscpy (lpszUrl, _T ("file://"));
		}
		_tcscat (lpszUrl, strPath);	
		return IR_SUCCESS;
	}

	TCHAR szUser [1000] = _T(""), szPwd [1000] = _T("");
	if (lpszUserName)
	{
		Encode (lpszUserName, szUser);
		lpszUserName = szUser;
	}

	if (lpszPassword)
	{
		Encode (lpszPassword, szPwd);
		lpszPassword = szPwd;	
	}
	
	m_url.lpszHostName = (LPTSTR) strHost;
	m_url.lpszPassword = (LPTSTR) lpszPassword;
	m_url.lpszUrlPath = (LPTSTR) strPath;
	m_url.lpszUserName = (LPTSTR) lpszUserName;
	m_url.nScheme = nScheme;
	m_url.nPort = nPort;

	
	if (!InternetCreateUrl (&m_url, 0, lpszUrl, pdwUrlLength))
		return fsWinInetErrorToIR ();

	return IR_SUCCESS;
}

void fsURL::FixWinInetBug()
{
	LPTSTR psz = _tcschr (m_szHost, _T('@'));
	if (psz == NULL)
		return;

	_tcscat_s (m_szPassword, 100, _T("@"));
	_tcsncat_s (m_szPassword, 100, m_szHost, psz - m_szHost);
	_tcscpy_s  (m_szHost, 1000, psz + 1);
}

void fsURL::Encode(LPCTSTR psz, LPTSTR sz)
{
	while (*psz)
	{
		char chChar = 0;
		TCHAR c = *psz++;
		TCHAR sz2 [10];

		if (c == _T(':') || c == _T('@') || c == _T('%'))
		{
#ifdef UNICODE
			wctomb(&chChar, c);
#else
			chChar = c;
#endif

			_stprintf (sz2, _T("%%%x"), (int)(BYTE)chChar);
		}
		else
		{
			sz2 [0] = c; sz2 [1] = 0;
		}

		lstrcat (sz, sz2);
	}
}
