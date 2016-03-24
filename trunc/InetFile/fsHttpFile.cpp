/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "fsHttpFile.h"
#include <stdio.h>
#include <memory>
#include "common.h"
#include <limits.h>
#include "fsInternetSession.h"
#include "strparsing.h"
#include "fsURL.h"

fsHttpFile::fsHttpFile()
{
	m_pszReferer = NULL;
	m_pszCookies = NULL;
	m_pszPostData = NULL;
	m_pszHttpVersion = new TCHAR [100];
	m_dwFlags = INTERNET_FLAG_KEEP_CONNECTION;
	UseHttp11 (FALSE);
	m_bHeadersOnly = FALSE;
	m_pszAdditionalHeaders = NULL;
	m_uLeftToUpload = 0;
	m_bUseMultipart = TRUE;
	m_bEnableAutoRedirect = FALSE;
}

fsHttpFile::~fsHttpFile()
{
	if (m_pszReferer)
		delete [] m_pszReferer;

	if (m_pszCookies)
		delete [] m_pszCookies;

	if (m_pszPostData)
		delete [] m_pszPostData;

	delete [] m_pszHttpVersion;
}

fsInternetResult fsHttpFile::Open(LPCTSTR pszFilePath, UINT64 uStartPos)
{
	return OpenEx (pszFilePath, uStartPos, _UI64_MAX);
}

fsInternetResult fsHttpFile::Read(LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead)
{
	if (m_hFile == NULL)
	{
		*pdwRead = 0;
		return IR_NOTINITIALIZED;
	}

	if (!InternetReadFile (m_hFile, pBuffer, dwToRead, pdwRead))
		return fsWinInetErrorToIR ();

	return IR_SUCCESS;
}

void fsHttpFile::UseHttp11(BOOL bUse)
{
	if (bUse)
		_tcscpy (m_pszHttpVersion, _T("HTTP/1.1"));
	else
		_tcscpy (m_pszHttpVersion, _T("HTTP/1.0"));
}

void fsHttpFile::SetReferer(LPCTSTR pszReferer)
{
	SAFE_DELETE_ARRAY (m_pszReferer);

	if (pszReferer)
	{
		fsnew (m_pszReferer, TCHAR, _tcslen (pszReferer) + 1);
		_tcscpy_s (m_pszReferer, _tcslen (pszReferer) + 1, pszReferer);
	}
}

void fsHttpFile::UseSecure(BOOL bUse)
{
	if (bUse)
		m_dwFlags |= INTERNET_FLAG_SECURE;	
	else
		m_dwFlags &= ~INTERNET_FLAG_SECURE;
}

void fsHttpFile::UseCookie(BOOL bUse)
{
	if (bUse)
		m_dwFlags &= ~ INTERNET_FLAG_NO_COOKIES;
	else
		m_dwFlags |= INTERNET_FLAG_NO_COOKIES;
}

fsInternetResult fsHttpFile::SetupProxy()
{
	if (m_hFile == NULL)
		return IR_NOTINITIALIZED;

	return m_pServer->GetSession ()->ApplyProxyAuth (m_hFile);
}

void fsHttpFile::RetreiveHeadersOnly(BOOL b)
{
	m_bHeadersOnly = b;
}

fsInternetResult fsHttpFile::ProcessRangesResponse()
{
	CHAR sz [10000];
	DWORD dw = _countof (sz);

	
	
	BOOL bAcceptRanges = FALSE;
	if (HttpQueryInfoA (m_hFile, HTTP_QUERY_ACCEPT_RANGES, sz, &dw, NULL))
	{
		if (stricmp (sz, "bytes") == 0)
			bAcceptRanges = TRUE;
	}

	m_enRST = RST_NONE;

	dw = _countof (sz);

	
	
	if (!HttpQueryInfoA (m_hFile, HTTP_QUERY_CONTENT_RANGE, sz, &dw, NULL))
		return bAcceptRanges ? IR_DOUBTFUL_RANGESRESPONSE : IR_RANGESNOTAVAIL;

	
	if (strncmp (sz, "bytes", 5))
		return bAcceptRanges ? IR_DOUBTFUL_RANGESRESPONSE : IR_RANGESNOTAVAIL;

	int pos = 0;
	while (sz [pos++] != ' '); 

	if (isdigit (sz [pos]) == false)
		return IR_RANGESNOTAVAIL;
	
	UINT64 first = (UINT64) _atoi64 (sz + pos);	

	while (sz [pos] >= '0' && sz [pos] <= '9')	
		pos++;
	pos++;

	UINT64 last = (UINT64) _atoi64 (sz + pos);	

	if (last < first)	
		return IR_RANGESNOTAVAIL;	

	while (sz [pos] >= '0' && sz [pos] <= '9') 
		pos++;
	pos++;	

	m_uFileSize = (UINT64) _atoi64 (sz + pos);	

	if (m_uFileSize < last)	
		return IR_RANGESNOTAVAIL;

	m_enRST = RST_PRESENT;

	return IR_SUCCESS;
}

void fsHttpFile::RetreiveSuggFileName()
{
	m_strSuggFileName = _T("");

	char sz [MAX_PATH];
	char szFile [MAX_PATH] = {0,};
	DWORD dwFL = MAX_PATH;
	
	if (FALSE == HttpQueryInfoA (m_hFile, HTTP_QUERY_CONTENT_DISPOSITION, sz, &dwFL, NULL))
		return;

	
	LPCSTR psz = fsStrStrNoCaseA (sz, "filename");
	if (psz == NULL)
		return;

	psz += 8;	
	while (*psz == ' ') 
		psz++;
	bool bCharset = false;
	if (*psz == '*') 
	{
		bCharset = true;
		psz++;
	}

	if (*psz++ != '=')
		return;
	while (*psz == ' ') psz++;

	BOOL bInvComms = FALSE; 
	if  (*psz == '"' || *psz == '\'')
	{
		bInvComms = TRUE;
		psz++;
	}

	LPSTR pszFile = szFile;

	while (*psz != ';' && *psz != 0)
		*pszFile++ = *psz++;	

	if (bInvComms)	
		*(pszFile-1) = 0;	
	else
		*pszFile = 0;

	if (bCharset)
	{
		LPCSTR psz = strstr (szFile, "''");
		if (psz != NULL)
		{
			if (strnicmp (szFile, "utf-8", 5) == 0)
			{
				wchar_t wsz [MAX_PATH];
				MultiByteToWideChar (CP_UTF8, 0, psz+2, -1, wsz, MAX_PATH);
				WideCharToMultiByte (CP_ACP, 0, wsz, -1, szFile, MAX_PATH, "_", NULL);
			}
			else
			{
				
				strcpy (szFile, psz+2);
			}			
		}
	}

#ifdef UNICODE
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, szFile, -1, 0, 0);
	std::auto_ptr<WCHAR> apwszFileGuard( new WCHAR[nLen + 1] );
	WCHAR* wszFile = apwszFileGuard.get();
	memset(wszFile, 0, sizeof(WCHAR) * (nLen + 1));
	::MultiByteToWideChar(CP_ACP, 0, szFile, -1, wszFile, nLen);
	m_strSuggFileName = wszFile;
#else
	m_strSuggFileName = szFile;
#endif
}

fsInternetResult fsHttpFile::QuerySize(LPCTSTR pszFilePath)
{
	
	fsInternetResult ir;
	
							 
							 
							 
	ir = Open (pszFilePath, 0);	
	
	CloseHandle ();	
	return ir;
}

fsInternetResult fsHttpFile::Open_imp(LPCTSTR pszFilePath, UINT64 uStartPos, int cTryings)
{
	if (!m_pServer) 
		return IR_NOTINITIALIZED;

	HINTERNET hServer = m_pServer->GetHandle ();  

	if (!hServer)	
		return IR_NOTINITIALIZED;

	CloseHandle ();

	if (lstrlen (pszFilePath) > 9000)
		return IR_BADURL;

	if (cTryings > 1)
		return IR_WININETUNKERROR;

	DWORD dwFlags = m_dwFlags;
	if (m_pszCookies)
		dwFlags |= INTERNET_FLAG_NO_COOKIES;
	if (m_bEnableAutoRedirect == FALSE)
		dwFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;

	LPTSTR ppszAcceptedTypes [2] = { _T("*/*"), NULL }; 

	
	
	
	

	LPCTSTR pszVerb = _T("GET");
	if (m_pszPostData)
		pszVerb = _T("POST");
	else if (m_bHeadersOnly)
		pszVerb = _T("HEAD");

	
	m_hFile = HttpOpenRequest (hServer, pszVerb, pszFilePath, m_pszHttpVersion,
		m_pszReferer, (LPCTSTR*) ppszAcceptedTypes, 
		dwFlags | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | 
		INTERNET_FLAG_NO_CACHE_WRITE, NULL);

	if (m_hFile == NULL)
		return fsWinInetErrorToIR ();

	fsInternetResult ir = SetupProxy ();
	if (ir != IR_SUCCESS)
	{
		CloseHandle ();
		return ir;
	}

	
	TCHAR szHdr [20000] = _T("");

	if (uStartPos)
		_stprintf_s (szHdr, _T("Range: bytes=%I64u-\r\n"), uStartPos); 

	if (m_pszCookies)
		_stprintf_s (szHdr + lstrlen (szHdr), 20000 - lstrlen (szHdr), _T("Cookie: %s\r\n"), m_pszCookies); 

	if (m_pszPostData)
		_tcscat_s (szHdr, 20000, _T("Content-Type: application/x-www-form-urlencoded\r\n"));

	if (m_pszAdditionalHeaders)
		_tcscat_s (szHdr, 20000, m_pszAdditionalHeaders);

	if (cTryings == 0)
	{
		
		TCHAR szReq [90000] = {0,};
		_stprintf_s (szReq, _T("%s %s %s\r\nReferer: %s"), pszVerb, 
			pszFilePath, m_pszHttpVersion, 
			m_pszReferer ? m_pszReferer : _T("-"));

		if (*szHdr)
		{
			_tcscat_s (szReq, _T("\r\n"));
			_tcscat_s (szReq, szHdr);
			szReq [_tcslen (szReq) - 2] = 0;	
		}

		if ((dwFlags & INTERNET_FLAG_NO_COOKIES) == 0)
		{
			TCHAR szUrl [10000]; DWORD dw = _countof (szUrl);
			fsURL url;
			url.Create (m_dwFlags & INTERNET_FLAG_SECURE ? INTERNET_SCHEME_HTTPS : INTERNET_SCHEME_HTTP,
				m_pServer->GetServerName (), m_pServer->GetServerPort (), 
				NULL, NULL, pszFilePath, szUrl, &dw);

			TCHAR szCookie [10000]; dw = _countof (szCookie);
			*szCookie = 0;
			
			InternetGetCookie (szUrl, NULL, szCookie, &dw);

			if (*szCookie)
			{
				_tcscat_s (szReq, _T("\r\n"));
				_tcscat_s (szReq, _T("Cookie: "));
				_tcscat_s (szReq, szCookie);
			}
		}

		_tcscat_s (szReq, _T("\r\nHost: "));

#ifdef UNICODE
		_stprintf_s (szReq + wcslen(szReq), 90000 - wcslen(szReq), L"%s", m_pServer->GetServerName ());
#else
		_tcscat_s (szReq, m_pServer->GetServerName ());
#endif

		if (m_pszPostData)
		{
			_tcscat_s  (szReq, _T("\r\n"));
#ifdef UNICODE
			_stprintf_s (szReq + _tcslen(szReq), 90000 - _tcslen(szReq), _T("%S"), m_pszPostData);
#else
			strcat (szReq, m_pszPostData);
#endif
		}

		Dialog (IFDD_TOSERVER, szReq);	
	}

	
	IgnoreSecurityProblems ();

	auto headersToSend = utf8FromWide (szHdr);

	
	
	
	if (!HttpSendRequestA (m_hFile, 
		!headersToSend.empty () ? headersToSend.c_str () : nullptr, (UINT)-1, 
		m_pszPostData, m_pszPostData ? strlen (m_pszPostData) : 0))
	{
		ir = fsWinInetErrorToIR ();
		if (ir == IR_SEC_CHECK_FAILURE)
			m_lastSctFailure = fsWinInetErrorToSCT ();

		DialogHttpResponse (m_hFile);	
									
		CloseHandle ();
		return  ir; 
	}

	char szResp [10000];
	DWORD dwRespLen = _countof (szResp), dwIndex = 0;
	
	
	if (HttpQueryInfoA (m_hFile, HTTP_QUERY_RAW_HEADERS_CRLF, szResp, &dwRespLen, &dwIndex))
	{
		int cLines = 0; 

		

		LPCSTR pszLine = szResp;
		while (pszLine)
		{
			pszLine = strchr (pszLine, '\n');
			if (pszLine)
			{
				while (*pszLine == '\r' || *pszLine == '\n')
					pszLine++;
				cLines++;
			}
		}

		if (cLines == 0 || cLines == 1)
		{
			
			
			return Open_imp (pszFilePath, uStartPos, ++cTryings);
		}
	}

	DialogHttpResponse (m_hFile);	

	DWORD dwStatusCode;	
	DWORD dwSize = sizeof (DWORD);
	if (!HttpQueryInfo(m_hFile, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, 
			&dwStatusCode, &dwSize, NULL))	
	{
		return fsWinInetErrorToIR ();
	}

	if (dwStatusCode < 200 || dwStatusCode >= 300)	
	{
		ir = fsHttpStatusCodeToIR (dwStatusCode);

		if (ir == IR_NEEDREDIRECT)	
		{
			DWORD dwNeedLen = 0;

			HttpQueryInfoA (m_hFile, HTTP_QUERY_LOCATION, NULL, &dwNeedLen,	NULL);

			if (::GetLastError () == ERROR_INSUFFICIENT_BUFFER)
			{
				SAFE_DELETE_ARRAY (m_pszLastError);
				std::string sLocation;
				sLocation.resize (++dwNeedLen);
				
				if (!HttpQueryInfoA (m_hFile, HTTP_QUERY_LOCATION, &sLocation.front (), &dwNeedLen, NULL)) 
					return IR_SERVERUNKERROR;

				tstring location = tstringFromString (sLocation);

				m_pszLastError = new TCHAR [location.length () + 1];
				_tcscpy (m_pszLastError, location.c_str ());
			}
			else
			{
				return IR_SERVERUNKERROR;
			}

		}

		return ir;
	}

	
	char szContLen [1000];
	DWORD dwLen = _countof (szContLen);
	if (HttpQueryInfoA (m_hFile, HTTP_QUERY_CONTENT_LENGTH,	szContLen, &dwLen, NULL)) {
		__int64 iSize = _atoi64 (szContLen);
		if (iSize < 0)
			return IR_SERVERUNKERROR;
		m_uFileSize = (UINT64) iSize;
	}
	else
		m_uFileSize = _UI64_MAX; 

	ir = IR_SUCCESS;
	if (uStartPos)
	{
		
		ir = ProcessRangesResponse ();
		if (ir == IR_RANGESNOTAVAIL) 
			return ir;
	}
	else
	{
		CHAR sz [10000];
		DWORD dw = _countof (sz);
		
		
		if (HttpQueryInfoA (m_hFile, HTTP_QUERY_ACCEPT_RANGES, sz, &dw, NULL))
		{
			if (stricmp (sz, "bytes") == 0)
				m_enRST = RST_PRESENT;
			else
				m_enRST = RST_NONE;
		}
		else
			m_enRST = RST_UNKNOWN;
	}

	m_bContentTypeValid = FALSE;
	m_bDateValid = FALSE;

	CHAR szContentType [10000];	
	DWORD dwCL = _countof (szContentType);
	if (HttpQueryInfoA (m_hFile, HTTP_QUERY_CONTENT_TYPE, szContentType, &dwCL, NULL))
	{
#ifdef UNICODE
		int nLen = ::MultiByteToWideChar(CP_ACP, 0, szContentType, -1, 0, 0);
		std::auto_ptr<WCHAR> apwszContentTypeGuard;
		try {
			apwszContentTypeGuard.reset( new WCHAR[nLen] );
		} catch (...) {
			return IR_OUTOFMEMORY;
		}
		WCHAR* wszContentType = apwszContentTypeGuard.get();
		if (wszContentType == 0)
			return IR_OUTOFMEMORY;

		::MultiByteToWideChar(CP_ACP, 0, szContentType, -1, wszContentType, nLen);
		m_strContentType = wszContentType;
#else
		m_strContentType = szContentType;
#endif
		m_bContentTypeValid = TRUE;
	}

	SYSTEMTIME time; 
	DWORD dwTL = sizeof (time);
	if (HttpQueryInfo (m_hFile, HTTP_QUERY_LAST_MODIFIED | HTTP_QUERY_FLAG_SYSTEMTIME,
		&time, &dwTL, NULL))
	{
		SystemTimeToFileTime (&time, &m_date);
		m_bDateValid = TRUE;
	}

	RetreiveSuggFileName ();	

	return ir;
}

BOOL fsHttpFile::Is_Secure()
{
	return m_dwFlags & INTERNET_FLAG_SECURE;
}

void fsHttpFile::IgnoreSecurityProblems()
{
	DWORD dwFlags = 0;
    DWORD dwBuffLen = sizeof(dwFlags);

    InternetQueryOption (m_hFile, INTERNET_OPTION_SECURITY_FLAGS,
         (LPVOID)&dwFlags, &dwBuffLen);

    dwFlags |= fsSCTflagsToWinInetIgnoreFlags (m_sctIgnoreFlags); 
	
    InternetSetOption (m_hFile, INTERNET_OPTION_SECURITY_FLAGS,
         &dwFlags, sizeof (dwFlags));
}

BOOL fsHttpFile::get_UseHttp11()
{
	return lstrcmpi (m_pszHttpVersion, _T("HTTP/1.1")) == 0;
}

LPCTSTR fsHttpFile::get_Referer()
{
	return m_pszReferer;
}

BOOL fsHttpFile::get_UseCookie()
{
	return (m_dwFlags & INTERNET_FLAG_NO_COOKIES) == 0;
}

void fsHttpFile::SetCookies(LPCTSTR pszCookies)
{
	SAFE_DELETE_ARRAY (m_pszCookies);

	if (pszCookies && *pszCookies)
	{
		fsnew (m_pszCookies, TCHAR, lstrlen (pszCookies) + 1);
		lstrcpy (m_pszCookies, pszCookies);
	}
}

void fsHttpFile::SetPostData(LPCSTR pszPostData)
{
	SAFE_DELETE_ARRAY (m_pszPostData);

	if (pszPostData && *pszPostData)
	{
		fsnew (m_pszPostData, CHAR, strlen (pszPostData) + 1);
		strcpy (m_pszPostData, pszPostData);
	}
}

LPCTSTR fsHttpFile::GetCookies()
{
	return m_pszCookies;
}

LPCSTR fsHttpFile::GetPostData()
{
	return m_pszPostData;
}

void fsHttpFile::SetAdditionalHeaders(LPCTSTR pszAdditionalHeaders)
{
	SAFE_DELETE_ARRAY (m_pszAdditionalHeaders);

	if (pszAdditionalHeaders && *pszAdditionalHeaders)
	{
		
		int l = lstrlen (pszAdditionalHeaders);
		if (l < 2)
			return;
		if (pszAdditionalHeaders [l - 2] != _T('\r'))
			return;
		if (pszAdditionalHeaders [l - 1] != _T('\n'))
			return;

		fsnew (m_pszAdditionalHeaders, TCHAR, lstrlen (pszAdditionalHeaders) + 1);
		lstrcpy (m_pszAdditionalHeaders, pszAdditionalHeaders);
	}
}

fsInternetResult fsHttpFile::OpenEx(LPCTSTR pszFilePath, UINT64 uStartPos, UINT64 uUploadPartSize, UINT64 uUploadTotalSize)
{
	if (uUploadTotalSize == _UI64_MAX)
		return Open_imp (pszFilePath, uStartPos, 0);

	if (uStartPos + uUploadPartSize > uUploadTotalSize)
		return IR_INVALIDPARAM;

	if (!m_pServer) 
		return IR_NOTINITIALIZED;

	HINTERNET hServer = m_pServer->GetHandle ();  

	if (!hServer)
		return IR_NOTINITIALIZED;

	CloseHandle ();

	if (lstrlen (pszFilePath) > 9000)
		return IR_BADURL;

	fsString strFilePath = pszFilePath;
	fsString strFileName;
	if (m_bUseMultipart)
	{
		LPTSTR psz = _tcsrchr (strFilePath, _T('/'));
		if (psz)
		{
			strFileName = psz + 1;
			psz [1] = 0;
		}
		else
			strFileName = pszFilePath;
	}

	LPTSTR ppszAcceptedTypes [2] = { _T("*/*"), NULL }; 

	m_hFile = HttpOpenRequest (hServer, _T("POST"), strFilePath, m_pszHttpVersion,
		NULL, (LPCTSTR*)ppszAcceptedTypes, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (m_hFile == NULL)
	{
		auto ir = fsWinInetErrorToIR ();
		if (ir == IR_SEC_CHECK_FAILURE)
			m_lastSctFailure = fsWinInetErrorToSCT ();
		return ir;
	}

	fsInternetResult ir = SetupProxy ();
	if (ir != IR_SUCCESS)
	{
		CloseHandle ();
		return ir;
	}

	
	TCHAR szHdr [10000] = _T("");
	
	if (m_bUseMultipart)
		lstrcpy (szHdr, _T("Content-Type: multipart/form-data; boundary=---------------------------284583012225247"));
	else
	{
		lstrcpy (szHdr, _T("Content-Type: application/x-www-form-urlencoded"));
		if (m_strCharset.IsEmpty () == FALSE)
		{
			lstrcat (szHdr, _T("; charset="));
			lstrcat (szHdr, m_strCharset);
		}
	}

	if (uStartPos || uUploadPartSize != uUploadTotalSize)
	{
		if (*szHdr)
			lstrcat (szHdr, _T("\r\n"));
		_stprintf_s (szHdr + lstrlen (szHdr), 10000 - lstrlen (szHdr), _T("Range: bytes=%I64u-%I64u/%I64u"), uStartPos, 
			uStartPos + uUploadPartSize - 1, uUploadTotalSize); 
	}

	if (m_pszCookies)
	{
		if (*szHdr)
			lstrcat (szHdr, _T("\r\n"));
		_stprintf_s (szHdr + lstrlen (szHdr), 10000 - lstrlen (szHdr), _T("Cookie: %s"), m_pszCookies); 
	}

	if (m_pszAdditionalHeaders)
	{
		if (*szHdr)
			lstrcat (szHdr, _T("\r\n"));
		lstrcat (szHdr, m_pszAdditionalHeaders);
	}

	
	IgnoreSecurityProblems ();

	int nSizeAdd = 0;
	fsString strMultipartHdr;

	if (m_bUseMultipart)
	{
		m_strLabel = _T("-----------------------------284583012225247");

		strMultipartHdr = m_strLabel; strMultipartHdr += _T("\r\n");
		strMultipartHdr += _T("Content-Disposition: form-data; name=\"uploadFormFile\"; filename=\"");
		strMultipartHdr += strFileName; strMultipartHdr += _T("\"\r\n");
		strMultipartHdr += _T("Content-Type: application/octet-stream\r\n\r\n");

		nSizeAdd = strMultipartHdr.GetLength () + m_strLabel.GetLength () + 6;
	}
 
	INTERNET_BUFFERS BufferIn = {0};
	BufferIn.dwStructSize = sizeof (INTERNET_BUFFERS);
	BufferIn.lpcszHeader = szHdr;
	BufferIn.dwHeadersLength = BufferIn.dwHeadersTotal = lstrlen (szHdr);
	BufferIn.dwBufferTotal = (DWORD) (uUploadPartSize + nSizeAdd);

	if (!HttpSendRequestEx (m_hFile, &BufferIn, NULL, HSR_INITIATE, 0))
	{
		ir = fsWinInetErrorToIR ();
		if (ir == IR_SEC_CHECK_FAILURE)
			m_lastSctFailure = fsWinInetErrorToSCT ();
		CloseHandle ();
		return  ir; 
	}

	if (m_bUseMultipart)
	{
		DWORD dw;
		if (FALSE == InternetWriteFile (m_hFile, strMultipartHdr, strMultipartHdr.GetLength (), &dw))
		{
			ir = fsWinInetErrorToIR ();
			CloseHandle ();
			return  ir; 
		}
	}

	m_uLeftToUpload = uUploadPartSize;

	return IR_SUCCESS;
}

fsInternetResult fsHttpFile::Write(LPBYTE pBuffer, DWORD dwSize, DWORD *pdwWritten)
{
	if (m_hFile == NULL)
	{
		if (pdwWritten)
			*pdwWritten = 0;
		return IR_NOTINITIALIZED;
	}		

	if (m_uLeftToUpload == 0)
		return IR_S_FALSE;

	DWORD dwWritten;

	BOOL bRet = InternetWriteFile (m_hFile, pBuffer, dwSize, &dwWritten);

	if (pdwWritten)
		*pdwWritten = dwWritten;

	if (!bRet)
		return fsWinInetErrorToIR ();

	m_uLeftToUpload -= dwWritten;

	if (m_uLeftToUpload == 0)
	{
		if (m_bUseMultipart)
		{
			fsString str = _T("\r\n"); str += m_strLabel; str += _T("--\r\n");
			DWORD dw;
			if (FALSE == InternetWriteFile (m_hFile, str, str.GetLength (), &dw))
				return fsWinInetErrorToIR ();
		}

		bRet = HttpEndRequest (m_hFile, NULL, 0, NULL);
		if (!bRet)
			return fsWinInetErrorToIR ();
	}

	return IR_SUCCESS;
}

void fsHttpFile::set_UseMultipart(BOOL b)
{
	m_bUseMultipart = b;
}

fsString fsHttpFile::GetCookiesFromResponse()
{
	char sz [10000];
	DWORD dw = _countof (sz) - 1;

	if (FALSE == HttpQueryInfoA (m_hFile, HTTP_QUERY_SET_COOKIE, sz, &dw, NULL))
		return _T("");

	WCHAR apwszCookies[10000] = {0,};
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, sz, -1, 0, 0);
	::MultiByteToWideChar(CP_ACP, 0, sz, -1, apwszCookies, nLen);

	return apwszCookies;
}

void fsHttpFile::set_EnableAutoRedirect(BOOL b)
{
	m_bEnableAutoRedirect = b;
}

void fsHttpFile::set_Charset(LPCTSTR psz)
{
	m_strCharset = psz;
}
