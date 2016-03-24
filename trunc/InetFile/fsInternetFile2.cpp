/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef NOCURL

#include "common.h"
#include <memory>
#include <string>
#include <exception>
#include "Utils.h"
#include "fsInternetFile2.h"
#include <curl/curl.h>

fsInternetFile2::fsInternetFile2()
{
	m_curl = NULL;
	m_bDownloading = false;
	m_bNeedStop = false;
	m_irLastError = IR_SUCCESS;

	m_hevReadDataReq = m_hevReadDataDone = NULL;

	m_fileTime = -1;
	m_uFileSize = _UI64_MAX;

	m_bDoPause = FALSE;

	m_uStartPos = 0;

	m_enRST = RST_UNKNOWN;
}

fsInternetFile2::~fsInternetFile2()
{
	StopDownloading ();
	for (int i = 0; m_bDownloading && i < 33; i++)
		Sleep (33 * 3);

	if (m_hevReadDataReq)
		CloseHandle (m_hevReadDataReq);
	if (m_hevReadDataDone)
		CloseHandle (m_hevReadDataDone);
}

fsInternetResult fsInternetFile2::Initialize()
{
	if (m_curl == NULL)
	{
		
		m_curl = curl_easy_init ();
		
		if (m_curl)
		{
			
			if (m_hevReadDataReq == NULL)
			{
				m_hevReadDataReq = CreateEvent (NULL, TRUE, FALSE, NULL);
				m_hevReadDataDone = CreateEvent (NULL, TRUE, FALSE, NULL);
			}

			
			curl_easy_setopt (m_curl, CURLOPT_WRITEFUNCTION, _WriteData);
			curl_easy_setopt (m_curl, CURLOPT_WRITEDATA, this);

			curl_easy_setopt (m_curl, CURLOPT_HEADERFUNCTION, _WriteHeader);
			curl_easy_setopt (m_curl, CURLOPT_HEADERDATA, this);

			curl_easy_setopt (m_curl, CURLOPT_FOLLOWLOCATION, TRUE);

			
			{
				curl_easy_setopt (m_curl, CURLOPT_SSL_VERIFYPEER, FALSE);
				curl_easy_setopt (m_curl, CURLOPT_SSL_VERIFYHOST, FALSE);
			}
		}
	}

	return m_curl == NULL ? IR_ERROR : IR_SUCCESS;
}

size_t fsInternetFile2::_WriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
	fsInternetFile2* pthis = (fsInternetFile2*) stream;
	return pthis->OnWriteData ((LPBYTE)ptr, size * nmemb);
}

void fsInternetFile2::set_URL(LPCTSTR pszURL)
{
	std::string sUrl;
#ifdef UNICODE
	UniToAnsi(std::wstring(pszURL), sUrl);
#else
	sUrl = pszURL;
#endif

	curl_easy_setopt (m_curl, CURLOPT_URL, sUrl.c_str());

	if (_tcsstr (pszURL, _T(".megashares.com/")) != NULL)
		m_bDoPause = TRUE;
}

fsInternetResult fsInternetFile2::StartDownloading()
{
	if (m_bDownloading == false)
	{
		m_bDownloading = true;
		m_bNeedStop = false;
		m_bAnswrFromServRcvd = false;
		m_irLastError = IR_SUCCESS;
		m_strHttpHeader = _T("");
		m_enRST = RST_UNKNOWN;

		
		ResetEvent (m_hevReadDataReq);
		
		ResetEvent (m_hevReadDataDone);

		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadDownload, this, 0, &dw));

		
		while (m_bAnswrFromServRcvd == false)
			Sleep (10);

		if (m_irLastError == IR_SUCCESS)
			ExtractFileInfoFromResponse ();

		if (m_strHttpHeader.IsEmpty () == FALSE)
			Dialog (IFDD_FROMSERVER, m_strHttpHeader);

		
		return m_irLastError;
	}

	return IR_S_FALSE;	
}

DWORD WINAPI fsInternetFile2::_threadDownload(LPVOID lp)
{
	fsInternetFile2* pthis = (fsInternetFile2*)lp;

	try{

	if (pthis->m_bDoPause)
		Sleep (1500);

	
	
	
	
	
	
	
	
	
	
	CURLcode res = curl_easy_perform (pthis->m_curl);

	
	pthis->m_irLastError = CURLcodeToIR (res);
	if (pthis->m_irLastError == IR_SEC_CHECK_FAILURE)
		pthis->m_lastSctFailure = CURLcodeToSCT (res);

	if (pthis->m_irLastError == IR_SUCCESS)
	{
		long nRespCode = 200;
		curl_easy_getinfo (pthis->m_curl, CURLINFO_RESPONSE_CODE, &nRespCode);
		pthis->m_irLastError = fsHttpStatusCodeToIR (nRespCode);
	}

	
	curl_easy_cleanup (pthis->m_curl);
	pthis->m_curl = NULL;

	
	pthis->m_bAnswrFromServRcvd = true;

	pthis->m_bDownloading = false;

	
	SetEvent (pthis->m_hevReadDataReq);
	SetEvent (pthis->m_hevReadDataDone);

	}catch (...) {}

	return 0;
}

fsInternetResult fsInternetFile2::CURLcodeToIR(int code)
{
	switch (code)
	{
	case CURLE_OK:
		return IR_SUCCESS;

	case CURLE_COULDNT_RESOLVE_HOST:
		return IR_NAMENOTRESOLVED;

	case CURLE_COULDNT_CONNECT:
		return IR_CANTCONNECT;

	case CURLE_FTP_USER_PASSWORD_INCORRECT:
		return IR_LOGINFAILURE;

	case CURLE_OPERATION_TIMEOUTED:
		return IR_TIMEOUT;

	case CURLE_HTTP_RANGE_ERROR:
		return IR_RANGESNOTAVAIL;

	case CURLE_SSL_CACERT:
		return IR_SEC_CHECK_FAILURE;

	
	default:
		return IR_ERROR;
	}
}

fsSecurityCheckType fsInternetFile2::CURLcodeToSCT(int code)
{
	switch (code)
	{
	case CURLE_SSL_CACERT:
		return SCT_CERT_INVALID_CA;

	default:
		return SCT_UNSPECIFIED;
	}
}

fsInternetResult fsInternetFile2::Read(LPBYTE pBuffer, DWORD dwToRead, DWORD *pdwRead)
{
	if (pdwRead)
		*pdwRead = 0; 

	if (dwToRead == 0)
		return IR_SUCCESS;

	if (m_bDownloading == false)
		return m_irLastError;	

	
	m_ReadReq.abData.clear ();
	m_ReadReq.nDataLeft = dwToRead;

	
	SetEvent (m_hevReadDataReq);
	
	while (WAIT_TIMEOUT == WaitForSingleObject (m_hevReadDataDone, 1000))
	{
		if (m_bNeedStop)
			return IR_S_FALSE;
	}
	
	ResetEvent (m_hevReadDataDone);

	if (m_bNeedStop)
		return IR_S_FALSE;

	if (m_ReadReq.nDataLeft == dwToRead)
		return m_irLastError != IR_SUCCESS ? m_irLastError : IR_ERROR;

	
	
	DWORD dwRead = dwToRead - m_ReadReq.nDataLeft;

	CopyMemory (pBuffer, m_ReadReq.abData, dwRead);
	if (pdwRead)
		*pdwRead = dwRead;

	return m_ReadReq.nDataLeft ? m_irLastError : IR_SUCCESS;
}

size_t fsInternetFile2::OnWriteData(LPBYTE ptr, size_t size)
{
	
	m_bAnswrFromServRcvd = true;
	size_t sizeRcvd = size;

_lReadData:
	
	
	while (WAIT_TIMEOUT == WaitForSingleObject (m_hevReadDataReq, 1000))
	{
		if (m_bNeedStop)
			return 0;
	}

	if (m_bNeedStop || m_bDownloading == false)
		return 0;	

	
	int n; n  = min (m_ReadReq.nDataLeft, size);
	
	m_ReadReq.abData.add ((LPBYTE)ptr, n);
	
	m_ReadReq.nDataLeft -= n;
	size -= n;
	ptr += n;

	if (m_ReadReq.nDataLeft == 0) {
		
		
		ResetEvent (m_hevReadDataReq);
		
		SetEvent (m_hevReadDataDone);
	}

	if (size == 0)
		
		
		return sizeRcvd; 

	
	
	
	goto _lReadData;
}

void fsInternetFile2::StopDownloading()
{
	m_bNeedStop = true;

	
	SetEvent (m_hevReadDataReq);
	SetEvent (m_hevReadDataDone);
}

void fsInternetFile2::set_ResumeFrom (UINT64 uStart)
{
	curl_easy_setopt (m_curl, CURLOPT_RESUME_FROM_LARGE, (curl_off_t)uStart);
	m_uStartPos = uStart;
}

void fsInternetFile2::set_UserAgent(LPCTSTR psz)
{
	std::string sUserAgent;
#ifdef UNICODE
	UniToAnsi(std::wstring(psz), sUserAgent);
#else
	sUserAgent = pszURL;
#endif

	curl_easy_setopt (m_curl, CURLOPT_USERAGENT, psz);
}

void fsInternetFile2::set_UseHttp11(BOOL bUse)
{
	curl_easy_setopt (m_curl, CURLOPT_HTTP_VERSION, 
		bUse ? CURL_HTTP_VERSION_1_1 : CURL_HTTP_VERSION_1_0);
}

void fsInternetFile2::set_Referer(LPCTSTR psz)
{
	std::string sReferer;
#ifdef UNICODE
	UniToAnsi(std::wstring(psz), sReferer);
#else
	sReferer = pszURL;
#endif

	curl_easy_setopt (m_curl, CURLOPT_REFERER, sReferer.c_str());
}

void fsInternetFile2::set_Cookie(LPCTSTR psz)
{
	std::string sCookie;
#ifdef UNICODE
	UniToAnsi(std::wstring(psz), sCookie);
#else
	sCookie = pszURL;
#endif

	curl_easy_setopt (m_curl, CURLOPT_COOKIE, sCookie.c_str());
}

void fsInternetFile2::set_Auth(LPCTSTR pszUser, LPCTSTR pszPwd)
{
	char sz [1000];
#ifdef UNICODE
	sprintf (sz, "%S:%S", pszUser, pszPwd);
#else
	sprintf (sz, "%s:%s", pszUser, pszPwd);
#endif

	curl_easy_setopt (m_curl, CURLOPT_USERPWD, sz);
}

void fsInternetFile2::set_Proxy(LPCTSTR pszProxy, LPCTSTR pszUser, LPCTSTR pszPwd)
{
	std::string sProxy;
#ifdef UNICODE
	UniToAnsi(std::wstring(pszProxy), sProxy);
#else
	sProxy = pszProxy;
#endif

	curl_easy_setopt (m_curl, CURLOPT_PROXY, pszProxy);

	if (pszUser && *pszUser)
	{

		std::string sUser;
		std::string sPassword;
#ifdef UNICODE
	UniToAnsi(std::wstring(pszUser), sUser);
	UniToAnsi(pszPwd ? std::wstring(pszPwd) : std::wstring(L""), sPassword);
#else
	sUser = pszUser;
	sPassword = pszPwd ? pszPwd : "";
#endif

		curl_easy_setopt (m_curl, CURLOPT_PROXYUSERNAME, sUser.c_str());
		curl_easy_setopt (m_curl, CURLOPT_PROXYPASSWORD, sPassword.c_str());
				
	}
}

void fsInternetFile2::set_PostData(LPCSTR psz)
{
	BOOL bPost = psz != NULL && *psz != 0;
	curl_easy_setopt (m_curl, CURLOPT_POST, bPost);
	if (bPost)
	{
		curl_easy_setopt (m_curl, CURLOPT_POSTFIELDS, psz);
		curl_easy_setopt (m_curl, CURLOPT_POSTFIELDSIZE, strlen (psz));
	}
}

void fsInternetFile2::ExtractFileInfoFromResponse()
{
	if (m_curl != NULL)
	{
		LPSTR psz = NULL;
		curl_easy_getinfo (m_curl, CURLINFO_CONTENT_TYPE, &psz);

#ifdef UNICODE
		std::wstring sContentType;
		LPCSTR szContentType = psz ? psz : "";
		AnsiToUni(std::string(szContentType), sContentType);
		m_strContentType = sContentType.c_str();
#else
		m_strContentType = psz ? psz : "";
#endif

		curl_easy_getinfo (m_curl, CURLINFO_FILETIME, &m_fileTime);

		double f;
		curl_easy_getinfo (m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &f);
		m_uFileSize = f != 0 ? (UINT64)f + m_uStartPos : _UI64_MAX;
	
		if (m_uStartPos != 0)
			m_enRST = RST_PRESENT;

		ExtractSuggestedFileName ();
	}
	else
	{
		m_uFileSize = 0;
	}
}

LPCTSTR fsInternetFile2::get_ContentType()
{
	return m_strContentType;
}

void UnixTimeToFileTime(time_t t, LPFILETIME pft)
{
  
  LONGLONG ll;

  ll = Int32x32To64(t, 10000000) + 116444736000000000;
  pft->dwLowDateTime = (DWORD)ll;
  pft->dwHighDateTime = (DWORD) (ll >> 32);
}

BOOL fsInternetFile2::GetLastModifiedDate(FILETIME *pTime)
{
	if (m_fileTime == -1)
		return FALSE;

	UnixTimeToFileTime (m_fileTime, pTime);

	return TRUE;
}

fsResumeSupportType fsInternetFile2::IsResumeSupported()
{
	return m_enRST;
}

size_t fsInternetFile2::_WriteHeader(void *ptr, size_t size, size_t nmemb, void *stream)
{
	fsInternetFile2* pthis = (fsInternetFile2*) stream;
	char sz [10000];
	strncpy (sz, (LPCSTR)ptr, size * nmemb);
	sz [size * nmemb] = 0;

#ifdef UNICODE
		WCHAR wsz[10000] = {0,};
		int nLen = ::MultiByteToWideChar(CP_ACP, 0, sz, -1, 0, 0);
		::MultiByteToWideChar(CP_ACP, 0, sz, -1, wsz, nLen);
		pthis->m_strHttpHeader += wsz;
#else
		pthis->m_strHttpHeader += sz;
#endif

	return size * nmemb;
}

LPCTSTR fsInternetFile2::get_SuggestedFileName()
{
	return m_strSuggestedFileName;
}

UINT64 fsInternetFile2::GetFileSize()
{
	return m_uFileSize;
}

void fsInternetFile2::setUseFtpAsciiMode(bool bUse)
{
	curl_easy_setopt (m_curl, CURLOPT_TRANSFERTEXT, bUse);
}

void fsInternetFile2::setInterface(LPCTSTR psz)
{
	std::string sInterface;
#ifdef UNICODE
	UniToAnsi(std::wstring(psz), sInterface);
#else
	sInterface = psz;
#endif

	curl_easy_setopt (m_curl, CURLOPT_INTERFACE, sInterface.c_str());
}

void fsInternetFile2::ExtractSuggestedFileName ()
{
	assert (!m_strHttpHeader.IsEmpty ());

	if (m_strHttpHeader.IsEmpty ())
		return;

	std::string responseHeader = utf8FromWide ((LPCWSTR)m_strHttpHeader);

	size_t nPos = stringFindI (responseHeader, std::string ("content-disposition:"));
	if (nPos == std::string::npos)
		return;

	size_t nPos2 = nPos + 20;

	nPos = stringFindI (responseHeader, std::string ("\r\n"), nPos2);
	if (nPos == std::string::npos)
		return;

	std::string strCT (responseHeader.begin () + nPos2, responseHeader.begin () + nPos);
	if (strCT.empty ())
		return;

	nPos = stringFindI (strCT, std::string ("filename"));
	if (nPos == std::string::npos)
		return;

	const char* psz = strCT.c_str () + nPos + 8;

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

	bool bInvComms = false; 
	if (*psz == '"' || *psz == '\'')
	{
		bInvComms = true;
		psz++;
	}

	std::string strFile;

	while (*psz != ';' && *psz != 0)
		strFile += *psz++;	

	if (bInvComms)	
		strFile.erase (strFile.end () - 1); 

	if (bCharset)
	{
		nPos = strFile.find ("''");
		if (nPos != std::string::npos)
			strFile.erase (strFile.begin (), strFile.begin () + nPos + 2);
	}

	m_strSuggestedFileName = wideFromUtf8 (strFile).c_str ();
}

void fsInternetFile2::SetSecurityCheckIgnoreFlags (DWORD flags)
{
	m_sctIgnoreFlags = flags;
}

fsSecurityCheckType fsInternetFile2::get_lastSctFailure () const
{
	return m_lastSctFailure;
}

#endif 
