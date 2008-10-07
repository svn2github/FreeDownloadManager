/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "fsInternetURLFile.h"
#include "fsInternetSession.h"
#include "fsInternetFile.h"
#include "fsServerConnection.h"
#include "fsURL.h"
#include "fsHttpConnection.h"
#include "fsFtpConnection.h"
#include "fsFtpFile.h"
#include "fsHttpFile.h"
#include <atlbase.h>
#include "base64.c"        

fsInternetURLFile::fsInternetURLFile()
{
	m_pSession = NULL;
	m_bAutoDelSession = FALSE;
	m_pFile = NULL;
	m_pszLastError = NULL;

	m_pServer = NULL;
	m_pFile = NULL;

	m_bUseFile2 = false;

	InitializeCriticalSection (&m_cs);

	m_ftpConnection.SetDialogFunc (_InetFileDialogFunc, this, NULL);
	m_httpConnection.SetDialogFunc (_InetFileDialogFunc, this, NULL);
	m_ftpFile.SetDialogFunc (_InetFileDialogFunc, this, NULL);
	m_httpFile.SetDialogFunc (_InetFileDialogFunc, this, NULL);
	m_ifile2.SetDialogFunc (_InetFileDialogFunc, this, NULL);

	m_bCatchFromServerResponse = FALSE;
}

fsInternetURLFile::~fsInternetURLFile()
{
	CloseHandle ();

	if (m_bAutoDelSession && m_pSession)
		delete m_pSession;

	DeleteCriticalSection (&m_cs);
}

fsInternetResult fsInternetURLFile::Read(BYTE *pBuffer, DWORD dwToRead, DWORD *pdwRead)
{
	if (m_bUseFile2)
		return m_ifile2.Read (pBuffer, dwToRead, pdwRead);

	if (m_pFile == NULL)
		return IR_FILENOTOPENED;

	return m_pFile->Read (pBuffer, dwToRead, pdwRead);
}

fsInternetResult fsInternetURLFile::Initialize(fsInternetSession *pSession, BOOL bAutoDelSession)
{
	m_pSession = pSession;
	m_bAutoDelSession = bAutoDelSession;
	return IR_SUCCESS;
}

UINT64 fsInternetURLFile::GetFileSize()
{
	if (m_bUseFile2)
		return m_ifile2.GetFileSize ();

	if (m_pFile == NULL)
		return _UI64_MAX;

	return m_pFile->GetFileSize ();
}

void fsInternetURLFile::SetReferer(LPCSTR pszReferer)
{
	m_httpFile.SetReferer (pszReferer);
}

void fsInternetURLFile::UseHttp11(BOOL bUse)
{
	m_httpFile.UseHttp11 (bUse);
}

void fsInternetURLFile::UseCookie(BOOL bUse)
{
	m_httpFile.UseCookie (bUse);
}

void fsInternetURLFile::UseFtpPassiveMode(BOOL bUse)
{
	m_ftpConnection.UsePassiveMode (bUse);
}

void fsInternetURLFile::FtpSetTransferType(fsFtpTransferType enType)
{
	m_ftpFile.SetTransferType (enType);
}

void fsInternetURLFile::Close()
{
	if (m_bUseFile2) {
		m_ifile2.StopDownloading ();
		return;
	}

	if (m_pFile) 
		m_pFile->CloseHandle ();
}

LPCSTR fsInternetURLFile::GetLastError()
{
	return m_pszLastError;
}

fsInternetResult fsInternetURLFile::Open(INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, UINT64 uStartPosition, BOOL bSendHTTPBasicAuthImmediately)
{
	return OpenEx (scheme, pszHostName, pszUser, pszPassword, port, pszPath,
		uStartPosition, bSendHTTPBasicAuthImmediately, _UI64_MAX);
}

void fsInternetURLFile::CloseHandle()
{
	EnterCriticalSection (&m_cs);

	if (m_bUseFile2) 
		m_ifile2.StopDownloading ();

	if (m_pFile)
		m_pFile->CloseHandle ();
	
	if (m_pServer)
		m_pServer->CloseHandle ();

	if (m_bAutoDelSession)
		m_pSession->CloseHandle ();

	LeaveCriticalSection (&m_cs);
}

BOOL fsInternetURLFile::GetContentType(LPSTR pszType)
{
	if (m_bUseFile2)
	{
		LPCSTR psz = m_ifile2.get_ContentType ();
		lstrcpy (pszType, psz ? psz : "");
		return *pszType != 0;
	}

	return m_pFile->GetContentType (pszType);
}

BOOL fsInternetURLFile::GetLastModifiedDate(LPFILETIME pDate)
{
	return m_bUseFile2 ? m_ifile2.GetLastModifiedDate (pDate) :
		m_pFile->GetLastModifiedDate (pDate);
}

fsResumeSupportType fsInternetURLFile::IsResumeSupported()
{
	if (m_bUseFile2)
		return m_ifile2.IsResumeSupported ();

	if (m_pFile == NULL)
		return RST_UNKNOWN;
	return m_pFile->IsResumeSupported ();
}

LPCSTR fsInternetURLFile::GetSuggestedFileName()
{
	try {
		return m_bUseFile2 ? m_ifile2.get_SuggestedFileName () : 
			m_pFile->GetSuggestedFileName ();
	}
	catch (...) {return NULL;}
}

fsInternetResult fsInternetURLFile::QuerySize(INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, BOOL bSendHTTPBasicAuthImmediately)
{
	

	fsInternetResult ir;

	if (scheme == INTERNET_SCHEME_HTTP || scheme == INTERNET_SCHEME_HTTPS)
	{
		ir = Open (scheme, pszHostName, pszUser, pszPassword, port, pszPath, 0, bSendHTTPBasicAuthImmediately);
	}
	else
	{
		switch (scheme)
		{
			case INTERNET_SCHEME_FTP:
				m_pServer = &m_ftpConnection;
				m_pFile = &m_ftpFile;
				break;

			case INTERNET_SCHEME_FILE:
				m_pServer = NULL;
				m_pFile = &m_localFile;
				break;

			default:
				return IR_BADURL;
		}

		if (m_pServer)
		{
			ir = m_pServer->Initialize (m_pSession);
			if (ir != IR_SUCCESS)
				return ir;

			ir = m_pServer->Connect (pszHostName, pszUser, pszPassword, port);
			if (ir != IR_SUCCESS)
			{
				m_pszLastError = m_pServer->GetLastError ();
				return ir;
			}

			ir = m_pFile->Initialize (m_pServer);
			if (ir != IR_SUCCESS)
				return ir;
		}
		else
		{
			m_localFile.Initialize (pszHostName);
		}

		ir = m_pFile->QuerySize (pszPath);
	}

	if (ir != IR_SUCCESS)
	{
		m_pszLastError = m_pFile->GetLastError ();
		return ir;
	}

	CloseHandle ();

	return IR_SUCCESS;
}

void fsInternetURLFile::FtpSetDontUseLIST(BOOL b)
{
	m_ftpFile.SetDontUseLIST (b);
}

void fsInternetURLFile::SetupProxyForFile2()
{
	LPCSTR psz1, psz2, psz3;
	m_pSession->get_Proxy (psz1, psz2, psz3);

	if (psz1 == NULL || *psz1 == 0)
		return;

	char szProxy [1000];
	if (lstrcmpi (psz1, "Internet Explorer") == 0)
	{
		CRegKey key;
		if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
				"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 
				KEY_READ))
			return;

		DWORD dw;
		if (ERROR_SUCCESS != key.QueryValue (dw, "ProxyEnable"))
			return;
		if (dw == FALSE)
			return;

		dw = sizeof (szProxy);
		if (ERROR_SUCCESS != key.QueryValue (szProxy, "ProxyServer", &dw))
			return;

		psz1 = szProxy;
	}

	if (psz1 && *psz1)
		m_ifile2.set_Proxy (psz1, psz2, psz3);
}

void fsInternetURLFile::SetCookies(LPCSTR pszCookies)
{
	m_httpFile.SetCookies (pszCookies);
}

void fsInternetURLFile::SetPostData(LPCSTR pszHdrs)
{
	m_httpFile.SetPostData (pszHdrs);
}

fsFtpTransferType fsInternetURLFile::FtpGetTransferType()
{
	return m_ftpFile.GetTransferType ();
}

fsInternetResult fsInternetURLFile::OpenEx(INTERNET_SCHEME scheme, LPCSTR pszHostName, LPCSTR pszUser, LPCSTR pszPassword, INTERNET_PORT port, LPCSTR pszPath, UINT64 uStartPosition, BOOL bSendHTTPBasicAuthImmediately, UINT64 uUploadPartSize, UINT64 uUploadTotalSize)
{
	fsInternetResult ir;

	if (scheme == INTERNET_SCHEME_FTP && isProxySpecified ())
	{
		m_bUseFile2 = true;
		goto _lUseFile2;
	}

	m_bUseFile2 = false;
	
	
	
	switch (scheme)
	{
		case INTERNET_SCHEME_HTTP:
		case INTERNET_SCHEME_HTTPS:
			m_pServer = &m_httpConnection;
			m_pFile = &m_httpFile;
			if (bSendHTTPBasicAuthImmediately)
				FormHttpBasicAuthHdr (pszUser, pszPassword);
			break;

		case INTERNET_SCHEME_FTP:
			m_pServer = &m_ftpConnection;
			m_pFile = &m_ftpFile;
			break;

		case INTERNET_SCHEME_FILE:
			m_pServer = NULL;
			m_pFile = &m_localFile;
			break;

		default:
			return IR_BADURL;
	}

	m_httpFile.UseSecure (scheme == INTERNET_SCHEME_HTTPS);

	if (m_pServer)
	{
		if (pszHostName)
		{
			ir = m_pServer->Initialize (m_pSession);
			if (ir != IR_SUCCESS)
				return ir;

			
			ir = m_pServer->Connect (pszHostName, pszUser, pszPassword, port);
			if (ir != IR_SUCCESS)
			{
				m_pszLastError = m_pServer->GetLastError ();
				return ir;
			}

			ir = m_pFile->Initialize (m_pServer);
			if (ir != IR_SUCCESS)
				return ir;
		}
	}
	else
	{
		m_localFile.Initialize (pszHostName);
	}

	m_bCatchFromServerResponse = TRUE;

	
	ir = m_pFile->OpenEx (pszPath, uStartPosition, uUploadPartSize, uUploadTotalSize);
	
	bool bUseFile2; bUseFile2 = false;
	m_ifile2.Mute (FALSE);

	if (ir == IR_E_WININET_UNSUPP_RESOURCE)
	{
		if (uUploadTotalSize == _UI64_MAX)	
		{
			bUseFile2 = true;
			ir = IR_SUCCESS; 
			m_ifile2.Mute (FALSE);
		}
		else
		{
			ir = IR_ERROR;
		}
	}

	if (ir != IR_SUCCESS) 
	{
		if (m_strRespFromServer.IsEmpty () == FALSE)
		{
			Dialog (IFDD_FROMSERVER, m_strRespFromServer);
			m_strRespFromServer = "";
		}
		m_bCatchFromServerResponse = FALSE;
		m_pszLastError = m_pFile->GetLastError ();
		return ir;
	}

	if (bUseFile2 == false)
	{
		if (uUploadTotalSize == _UI64_MAX)
		{
			
			
			
			if (scheme == INTERNET_SCHEME_HTTP || scheme == INTERNET_SCHEME_HTTPS)
			{
				if (m_pFile->GetFileSize () != _UI64_MAX &&
						m_pFile->GetFileSize () >= 2*1024*1024*1024ui64)
					bUseFile2 = true;
			}
		}
	}

	if (bUseFile2)
	{
_lUseFile2:
		CloseHandle ();
		m_bUseFile2 = true;
		fsURL url;
		char szUrl [10000];
		DWORD dw = sizeof (szUrl);
		url.Create (scheme, pszHostName, port, NULL, NULL, pszPath, szUrl, &dw);
		m_ifile2.Initialize ();
		m_ifile2.set_URL (szUrl);
		m_ifile2.set_ResumeFrom (uStartPosition);
		m_ifile2.set_UserAgent (m_pSession->get_UserAgent ());
		m_ifile2.set_UseHttp11 (m_httpFile.get_UseHttp11 ());
		m_ifile2.set_Referer (m_httpFile.get_Referer ());
		m_ifile2.setUseFtpAsciiMode (m_ftpFile.GetTransferType () == FTT_ASCII);
		if (pszUser && *pszUser)
			m_ifile2.set_Auth (pszUser, pszPassword);

		if (m_httpFile.get_UseCookie ())
		{
			if (m_httpFile.GetCookies () && *m_httpFile.GetCookies ())
			{
				m_ifile2.set_Cookie (m_httpFile.GetCookies ());
			}
			else
			{
				char szCookie [10000]; dw = sizeof (szCookie);
				*szCookie = 0;
				InternetGetCookie (szUrl, NULL, szCookie, &dw);
				if (*szCookie)
					m_ifile2.set_Cookie (szCookie);
			}
		}

		if (m_httpFile.GetPostData ())
			m_ifile2.set_PostData (m_httpFile.GetPostData ());

		SetupProxyForFile2 ();

		ir = m_ifile2.StartDownloading ();
	}

	if (m_strRespFromServer.IsEmpty () == FALSE)
	{
		Dialog (IFDD_FROMSERVER, m_strRespFromServer);
		m_strRespFromServer = "";
	}
	m_bCatchFromServerResponse = FALSE;
		
	return ir;
}

fsInternetResult fsInternetURLFile::Write(LPBYTE pBuffer, DWORD dwToWrite, DWORD *pdwWritten)
{
	if (m_bUseFile2)
		return IR_ERROR;

	if (m_pFile == NULL)
		return IR_FILENOTOPENED;

	return m_pFile->Write (pBuffer, dwToWrite, pdwWritten);
}

void fsInternetURLFile::FormHttpBasicAuthHdr(LPCSTR pszUser, LPCSTR pszPassword)
{
	if (pszUser == NULL || *pszUser == 0)
		return;

	char szHdr [1000];

	char szLogin [1000];
	lstrcpy (szLogin, pszUser);
	lstrcat (szLogin, ":");
	if (pszPassword)
		lstrcat (szLogin, pszPassword);

	LPSTR pszL;
	base64_encode (szLogin, lstrlen (szLogin), &pszL);
	
	sprintf (szHdr, "Authorization: Basic %s\r\n", pszL);
	delete [] pszL;

	m_httpFile.SetAdditionalHeaders (szHdr);
}

void fsInternetURLFile::set_UseMultipart(BOOL b)
{
	m_httpFile.set_UseMultipart (b);
}

fsInternetResult fsInternetURLFile::OpenAnotherRequestOnServer(LPCSTR pszPath, UINT64 uStartPosition, UINT64 uUploadPartSize, UINT64 uUploadTotalSize)
{
	fsInternetResult ir;

	if (m_pFile == NULL)
		return IR_NOTINITIALIZED;

	if (m_bUseFile2) 
		m_ifile2.StopDownloading ();
	m_pFile->CloseHandle ();
	m_bUseFile2 = false;
	
	
	ir = m_pFile->OpenEx (pszPath, uStartPosition, uUploadPartSize, uUploadTotalSize);
	if (ir != IR_SUCCESS) 
	{
		m_pszLastError = m_pFile->GetLastError ();
		return ir;
	}

	if (uUploadTotalSize == _UI64_MAX)
	{
		
		
		
		if (m_pServer == &m_httpConnection)
		{
			if (m_pFile->GetFileSize () != _UI64_MAX &&
					m_pFile->GetFileSize () >= 2*1024*1024*1024ui64)
			{
				return IR_E_NOTIMPL;
			}
		}
	}

	return IR_SUCCESS;
}

fsString fsInternetURLFile::GetCookiesFromResponse()
{
	return m_httpFile.GetCookiesFromResponse ();
}

void fsInternetURLFile::set_EnableAutoRedirect(BOOL b)
{
	m_httpFile.set_EnableAutoRedirect (b);
}

void fsInternetURLFile::set_Charset(LPCSTR psz)
{
	m_httpFile.set_Charset (psz);
}

void fsInternetURLFile::_InetFileDialogFunc(fsInetFileDialogDirection enDir, LPCSTR pszMsg, LPVOID lp1, LPVOID )
{
	fsInternetURLFile *pthis = (fsInternetURLFile*) lp1;

	if (pthis->m_bCatchFromServerResponse && enDir == IFDD_FROMSERVER)
	{
		pthis->m_strRespFromServer = pszMsg;
	}
	else
	{
		pthis->Dialog (enDir, pszMsg);
	}
}

bool fsInternetURLFile::isProxySpecified()
{
	LPCSTR psz1, psz2, psz3;
	m_pSession->get_Proxy (psz1, psz2, psz3);

	if (psz1 && lstrcmpi (psz1, "Internet Explorer") == 0)
	{
		CRegKey key;
		if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
				"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", KEY_READ))
			return false;
		
		DWORD dw;
		if (ERROR_SUCCESS != key.QueryValue (dw, "ProxyEnable"))
			return false;
		if (dw == FALSE)
			return false;

		char szProxy [1000] = "";
		dw = sizeof (szProxy);
		if (ERROR_SUCCESS != key.QueryValue (szProxy, "ProxyServer", &dw))
			return false;
		
		if (*szProxy == 0)
			return false;

		return true;
	}

	return psz1 != NULL && *psz1 != 0;
}
