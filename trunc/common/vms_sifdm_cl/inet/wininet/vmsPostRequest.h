/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsInternetOperationInitializationData.h"
#include "../../error/vmsErrorCodeProvider.h"

class vmsPostRequest :
	public vmsInternetOperationInitializationDataReceiver,
	public vmsErrorCodeProvider
{
public:
	struct WinInetFnsOverride
	{
		BOOL (WINAPI *pfnHttpSendRequestA)(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD);
		BOOL (WINAPI *pfnInternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD);
		WinInetFnsOverride () : 
			pfnHttpSendRequestA (HttpSendRequestA), 
			pfnInternetReadFile (InternetReadFile) {}
	};

public:
	void Close()
	{
		if (m_hRequest)
			InternetCloseHandle (m_hRequest);
		if (m_hConnect)
			InternetCloseHandle (m_hConnect);
		if (m_hInet)
			InternetCloseHandle (m_hInet);
		m_hRequest = m_hConnect = m_hInet = NULL;
	}

	static BOOL MyInternetWriteFile (HINTERNET hFile, LPCVOID lpBuffer, DWORD dwToWrite)
	{
		DWORD dw;
		while (dwToWrite)
		{
			dw = min (dwToWrite, 1024);
			if (FALSE == InternetWriteFile (hFile, lpBuffer, dw, &dw) || dw == 0)
				return FALSE;
			dwToWrite -= dw;
			lpBuffer = (LPCVOID)((LPBYTE)lpBuffer + dw);
		}
		return TRUE;
	}

	BOOL ReadResponse (LPVOID pData, DWORD dwToRead, LPDWORD pdwRead)
	{
		if (m_hRequest == NULL)
		{
			m_last_error = common_error::unexpected;
			return FALSE;
		}
		
		if (!m_wiFns.pfnInternetReadFile (m_hRequest, pData, dwToRead, pdwRead))
		{
			m_last_error = windows_error::last_error ();
			return false;
		}

		return TRUE;
	}

	BOOL Send (LPCTSTR ptszServer, LPCTSTR ptszFilePath, std::string *pstrResponse)
	{
		std::string str;
		for (size_t i = 0; i < m_vParts.size (); i++)
		{
			if (i)
				str += "&";
			str += HttpEncode (m_vParts[i].strName.c_str ());
			str += "=";
			str += HttpEncode ((LPCSTR)m_vParts[i].pData);
		}
		return Send (ptszServer, ptszFilePath, str.c_str (), (DWORD)str.length (), _T ("application/x-www-form-urlencoded"), pstrResponse);
	}

	BOOL SendMultipart (LPCTSTR ptszServer, LPCTSTR ptszFilePath, 
		std::string *pstrResponse = nullptr,
		INTERNET_PORT uPort = INTERNET_DEFAULT_HTTP_PORT)
	{
		m_last_error.clear ();

	USES_CONVERSION;

	LPBYTE pbSendData = NULL; LPBYTE pbSendDataPos = NULL;
	#define ADD_DATA_TO_SEND(data,len) {memcpy (pbSendDataPos, data, len); pbSendDataPos += len;}
	#define ADD_STRING_TO_SEND(s) ADD_DATA_TO_SEND (s, strlen (s));

	if (m_vParts.size () == 0)
	{
		m_last_error = common_error::unexpected;
		return FALSE;
	}

	if (!InitWinInetHandlesForRequest (ptszServer, ptszFilePath, uPort))
		return FALSE;

	

	std::string strLabel = "---------------------------284583012225az7";

	CHAR szHdr [10000] = "Content-Type: multipart/form-data; boundary=";
	strcat (szHdr, strLabel.c_str ());

	

	INTERNET_BUFFERSA buffs;
	ZeroMemory (&buffs, sizeof (buffs));
	buffs.dwStructSize = sizeof (buffs);
	buffs.lpcszHeader = szHdr;
	buffs.dwHeadersLength = buffs.dwHeadersTotal = (DWORD)strlen (szHdr);
	buffs.dwBufferTotal = (DWORD)strLabel.length () + 4; 

	for (int step = 0; step < 2; step++)
	for (size_t i = 0; i < m_vParts.size (); i++)
	{
		_inc_mpart_item &item = m_vParts [i];

		CHAR sz [10000];
		if (item.strFileName.empty () == false)
		{
			sprintf (sz, "\
--%s\r\n\
Content-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n\
Content-Type: application/octet-stream\r\n\r\n",
				strLabel.c_str (), item.strName.c_str (), item.strFileName.c_str ());
		}
		else
		{
			sprintf (sz, "--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n",
				strLabel.c_str (), item.strName.c_str ());
		}

		if (step == 0)
		{
			buffs.dwBufferTotal += (DWORD)strlen (sz) + m_vParts [i].dwSize + 2 ;
			continue;
		}
		else if (i == 0)
		{
			pbSendData = new BYTE [buffs.dwBufferTotal+1];
			pbSendDataPos = pbSendData;
		}

		ADD_STRING_TO_SEND (sz);

		ADD_DATA_TO_SEND (item.pData, item.dwSize);

		ADD_STRING_TO_SEND ("\r\n");
	}

	ADD_STRING_TO_SEND (strLabel.c_str ());

	ADD_STRING_TO_SEND ("--\r\n");

	assert (pbSendDataPos-pbSendData == buffs.dwBufferTotal);
	if (FALSE == m_wiFns.pfnHttpSendRequestA (m_hRequest, buffs.lpcszHeader, buffs.dwHeadersLength, pbSendData, (DWORD)(pbSendDataPos-pbSendData)))
	{
		m_last_error = windows_error::last_error ();
		return FALSE;
	}

	if (!check_status_code ())
		return FALSE;

	delete [] pbSendData;

	if (pstrResponse)
	{
		*pstrResponse = "";
		char sz [1025];
		DWORD dw;
		BOOL ok;
		while ((ok = m_wiFns.pfnInternetReadFile (m_hRequest, sz, sizeof (sz) - 1, &dw)) && dw != 0)
		{
			sz [dw] = 0;
			(*pstrResponse) += sz;
		}

		if (!ok)
		{
			m_last_error = windows_error::last_error ();
			return FALSE;
		}
	}

	return TRUE;
	}

	
	void AddPart (LPCSTR ptszName, LPCSTR ptszFileName, LPCVOID pData, DWORD dwSize, bool bFreeData = false)
	{
		assert (ptszName != NULL);
		assert (pData != NULL);

		_inc_mpart_item item;
		item.strName = ptszName;
		if (ptszFileName)
			item.strFileName = ptszFileName;
		item.pData = pData;
		item.dwSize = dwSize;
		item.bFreeData = bFreeData;
		m_vParts.push_back (item);
	}

	void RemoveAllParts()
	{
		for (size_t i = 0; i < m_vParts.size (); i++)
		{
			if (m_vParts[i].bFreeData)
				delete[] (LPVOID)m_vParts[i].pData;
		}

		m_vParts.clear ();
	}

	BOOL Send (LPCTSTR ptszServer, LPCTSTR ptszFilePath, LPCVOID pvData, 
		DWORD dwDataSize, LPCTSTR ptszContentType, std::string *pstrResponse,
		INTERNET_PORT uPort = INTERNET_DEFAULT_HTTP_PORT)
	{
		m_last_error.clear ();

		if (!InitWinInetHandlesForRequest (ptszServer, ptszFilePath, uPort))
			return FALSE;

		

		tstring tstrContentTypeHdr;
		if (ptszContentType)
		{
			tstrContentTypeHdr = _T ("Content-Type: ");
			tstrContentTypeHdr += ptszContentType;
			tstrContentTypeHdr += _T ("\r\n");
			HttpAddRequestHeaders (m_hRequest, tstrContentTypeHdr.c_str (), (DWORD)tstrContentTypeHdr.length (), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
		}

		if (FALSE == m_wiFns.pfnHttpSendRequestA (m_hRequest, NULL, 0, (LPVOID)pvData, dwDataSize))
		{
			m_last_error = windows_error::last_error ();
			return FALSE;
		}

		if (!check_status_code ())
			return FALSE;

		if (pstrResponse)
		{
			*pstrResponse = "";
			char sz[1025];
			DWORD dw;
			BOOL ok;
			while ((ok = m_wiFns.pfnInternetReadFile (m_hRequest, sz, sizeof (sz) - 1, &dw)) && dw != 0)
			{
				sz[dw] = 0;
				(*pstrResponse) += sz;
			}

			if (!ok)
			{
				m_last_error = windows_error::last_error ();
				return FALSE;
			}
		}

		return TRUE;
	}

	static std::string HttpEncode (LPCSTR psz)
	{
		if (psz == NULL || *psz == 0)
			return "";

		std::string str;

		while (*psz)
		{
			const BYTE chCurrent = (BYTE)*psz;
			if (isdigit (chCurrent) || isalpha (chCurrent) ||
				chCurrent == '$' || chCurrent == '-' || chCurrent == '_' ||
				chCurrent == '.' || chCurrent == '+' || chCurrent == '!' ||
				chCurrent == '*' || chCurrent == '\'' || chCurrent == '(' ||
				chCurrent == ')' || chCurrent == ',')
			{
				str += chCurrent;
			}
			else
			{
				char sz[4];
				sprintf (sz, "%%%02X", (int)chCurrent);
				str += sz;
			}

			psz++;
		}

		return str;
	}

	vmsPostRequest()
	{
		m_hInet = m_hConnect = m_hRequest = NULL;
	}

	virtual ~vmsPostRequest()
	{
		Close ();
		RemoveAllParts ();
	}

protected:
	static void PostInitWinInetHandle (HINTERNET hInet)
	{
		
		BOOL bDisable = TRUE;
		InternetSetOption (hInet, INTERNET_OPTION_DISABLE_AUTODIAL, &bDisable, sizeof (bDisable));

		UINT uTimeout = 1 * 60 * 1000;
		InternetSetOption (hInet, INTERNET_OPTION_RECEIVE_TIMEOUT, &uTimeout, sizeof (uTimeout));
		InternetSetOption (hInet, INTERNET_OPTION_SEND_TIMEOUT, &uTimeout, sizeof (uTimeout));
		InternetSetOption (hInet, INTERNET_OPTION_CONNECT_TIMEOUT, &uTimeout, sizeof (uTimeout));
	}

	bool InitWinInetHandlesForRequest (LPCTSTR ptszServer, LPCTSTR ptszFilePath,
		INTERNET_PORT uPort = INTERNET_DEFAULT_HTTP_PORT)
	{
		Close ();

		std::wstring user_agent;
		if (m_spInetOpInitData && m_spInetOpInitData->getUserAgentInfo ())
			user_agent = m_spInetOpInitData->getUserAgentInfo ()->getUserAgentString ();

		m_hInet = InternetOpen (user_agent.c_str (), 
			INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (m_hInet == NULL)
		{
			m_last_error = windows_error::last_error ();
			return false;
		}

		PostInitWinInetHandle (m_hInet);

		m_hConnect = InternetConnect (m_hInet, ptszServer, uPort,
			NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
		if (m_hConnect == NULL)
		{
			m_last_error = windows_error::last_error ();
			return false;
		}

		m_hRequest = HttpOpenRequest (m_hConnect, _T ("POST"), ptszFilePath, NULL, NULL, NULL,
			INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_UI |
			INTERNET_FLAG_PRAGMA_NOCACHE, 0);
		if (m_hRequest == NULL)
		{
			m_last_error = windows_error::last_error ();
			return false;
		}

		return true;
	}

protected:
	HINTERNET m_hInet, m_hConnect, m_hRequest;
	struct _inc_mpart_item {
		std::string strName;
		std::string strFileName;
		LPCVOID pData;
		DWORD dwSize;
		bool bFreeData;
	};
	std::vector <_inc_mpart_item> m_vParts;
public:
	HINTERNET getResponseHandle(void) const
	{
		return m_hRequest;
	}

	virtual vmsError last_error () const override
	{
		return m_last_error;
	}

protected:
	vmsError m_last_error;
	WinInetFnsOverride m_wiFns;

protected:
	bool status_code (DWORD& code) const
	{
		assert (m_hRequest);
		DWORD dwSize = sizeof (DWORD);
		return HttpQueryInfo (m_hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&code, &dwSize, nullptr) != FALSE;
	}

	bool check_status_code ()
	{
		DWORD code = 0;

		if (!status_code (code))
		{
			m_last_error = windows_error::last_error ();
			return false;
		}

		if ((code / 100) != 2)
		{
			m_last_error = (http_error::http_error) code;
			return false;
		}

		return true;
	}
};
