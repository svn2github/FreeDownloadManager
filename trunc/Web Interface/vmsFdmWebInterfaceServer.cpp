/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "Web Interface.h"
#include "vmsFdmWebInterfaceServer.h"
#include "..\Fdm.h"
#include <comdef.h>
#include <atlbase.h>
#include <memory>

_COM_SMARTPTR_TYPEDEF (IWGUrlReceiver, __uuidof (IWGUrlReceiver));
_COM_SMARTPTR_TYPEDEF (IFDMDownloadsStat, __uuidof (IFDMDownloadsStat));
_COM_SMARTPTR_TYPEDEF (IFDMDownload, __uuidof (IFDMDownload));

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsFdmWebInterfaceServer::vmsFdmWebInterfaceServer()
{
	LoadDocuments ();
}

vmsFdmWebInterfaceServer::~vmsFdmWebInterfaceServer()
{

}

BOOL vmsFdmWebInterfaceServer::ProcessRequest(vmsHttpRequest &request, vmsHttpResponse &response)
{
	
	if (lstrcmpiA (request.get_RequestType (), "GET"))
		return FALSE;

	CString strU = AfxGetApp ()->GetProfileString (_T("Network"), _T("Login")), 
		strP = AfxGetApp ()->GetProfileString (_T("Network"), _T("Password"));
	
	CString strAuth = strU + ":" + strP;
#ifdef UNICODE
	_bstr_t bstrAuth((LPCWSTR)strAuth);
#else
	_bstr_t bstrAuth((LPCSTR)strAuth);
#endif

	if (strU.IsEmpty () == FALSE && 
			lstrcmpA (request.get_Auth (), (LPCSTR)bstrAuth))
	{
		response.set_ResponseCode ("401 Authorization Required");
		return FALSE;
	}

	LPCSTR pszRes = request.get_ResourcePath ();

	if (lstrcmpA (pszRes, "/") == 0)
		return RequestRootPage (response);

	if (strncmp (pszRes, "/adddownload.req?", lstrlenA ("/adddownload.req?")) == 0)
		return RequestCreateNewDownload (pszRes, response);

	if (strncmp (pszRes, "/compdlds.req", lstrlenA ("/compdlds.req")) == 0)
		return RequestListOfCompletedDownloads (pszRes, response);

	response.set_ResponseCode ("404 Not Found");
	return FALSE;
}

BOOL vmsFdmWebInterfaceServer::RequestRootPage(vmsHttpResponse &response)
{
	CoInitialize (NULL);

	IFDMDownloadsStatPtr spDlds;
	spDlds.CreateInstance (__uuidof (FDMDownloadsStat));
	ASSERT (spDlds != NULL);

	spDlds->BuildListOfDownloads (FALSE, TRUE);
	
	long lCount = 0;
	spDlds->get_DownloadCount (&lCount);

	CStringA strDldsHtml;

	if (lCount == 0)
	{
		strDldsHtml = "There are no active downloads currently.";
	}
	else
	{
		

		strDldsHtml += "<table width=\"100%\" border=\"1\">";
		strDldsHtml += "<tr>";
		strDldsHtml += "<td>File name<br>&nbsp;</td>";
		strDldsHtml += "<td>Size<br>&nbsp;</td>";
		strDldsHtml += "<td>Downloaded<br>&nbsp;</td>";
		strDldsHtml += "<td>Time remaining<br>&nbsp;</td>";
		strDldsHtml += "<td>Sections<br>&nbsp;</td>";
		strDldsHtml += "<td>Speed<br>&nbsp;</td>";
		strDldsHtml += "</tr>";
	
		for (long i = 0; i < lCount; i++)
		{
			IFDMDownloadPtr spDld;
			spDlds->get_Download (i, &spDld);
			ASSERT (spDld != NULL);

			CString str;

			str += "<tr>";
		
			for (int i = 0; i < 6; i++)
			{
				str += "<td>"; 
				CComBSTR bstr;
				spDld->get_DownloadText (i, &bstr);
				CString str2 = bstr;
				str += str2;
				str += "</td>";
			}

			str += "</tr>";

			strDldsHtml += str;
		}

		strDldsHtml += "</table>";
	}

	_bstr_t bstrStr(m_strRootHtml.c_str ());

	CStringA str = (const char*)bstrStr;
	str.Replace ("%ActiveDownloads%", strDldsHtml);

	response.set_Body (str, str.GetLength ());

	return TRUE;
}

BOOL vmsFdmWebInterfaceServer::RequestCreateNewDownload(LPCSTR pszRes, vmsHttpResponse &response)
{
	CoInitialize (NULL);

	IWGUrlReceiverPtr spUrlRcvr;
	spUrlRcvr.CreateInstance (__uuidof (WGUrlReceiver));
	ASSERT (spUrlRcvr != NULL);

	if (spUrlRcvr == NULL)
	{
		CoUninitialize ();
		_bstr_t bstrAddDownloadRes_err(m_strAddDownloadRes_err.c_str ());
		response.set_Body ((const char*)bstrAddDownloadRes_err, m_strAddDownloadRes_err.length ());
		return TRUE;
	}

	spUrlRcvr->put_ForceSilentEx (TRUE);
	spUrlRcvr->put_CheckExtension (FALSE);
	spUrlRcvr->put_ForceDownloadAutoStart (TRUE);
	spUrlRcvr->put_DisableMaliciousChecking (TRUE);
	spUrlRcvr->put_DisableURLExistsCheck (TRUE);

	CStringA strUrl;
	pszRes += lstrlenA ("/adddownload.req?");
	if (strnicmp (pszRes, "URL=", 4))
	{
		CoUninitialize ();
		_bstr_t bstrAddDownloadRes_err(m_strAddDownloadRes_err.c_str ());
		response.set_Body ((const char*)bstrAddDownloadRes_err, m_strAddDownloadRes_err.length ());
		return FALSE;
	}
	pszRes += 4;

	

	while (*pszRes)
	{
		char c;

		
		if (*pszRes == '%')
		{
			pszRes++;
			char sz [3];
			sz [0] = *pszRes++; 
			if (*pszRes == 0)
				return FALSE;
			sz [1] = *pszRes++;
			sz [2] = 0;
			int i;
			sscanf (sz, "%x", &i);
			c = (char)i;
		}
		else
		{
			c = *pszRes++;
			
			if (c == '+')
				c = ' ';
		}

		strUrl += c;
	}

	
	
	CString str;
	int i = 0;
	for (i = 0; i < strUrl.GetLength () - 1;)
	{
		if (strUrl [i] == '&' && strUrl [i+1] == '#')
		{
			int k = i + 2;
			CStringA str2; 
			bool bHex = false;
			if (k < strUrl.GetLength ())
				bHex = strUrl [k] == 'x';
			if (bHex)
				k++;
			while (isdigit (strUrl [k]))
				str2 += strUrl [k++];
			if (str2.IsEmpty () == FALSE && 
				 (strUrl [k] == ';' || strUrl.GetLength () == k))
			{
				i = ++k;
				wchar_t wsz [2];
				if (bHex)
				{
					int j;
					sscanf (str2, "%x", &j);
					wsz [0] = (wchar_t)j;
				}
				else
				{
					wsz [0] = atoi (str2);
				}
				wsz [1] = 0;
				char sz [2];
				WideCharToMultiByte (CP_ACP, 0, wsz, -1, sz, 1, NULL, NULL);
				sz [1] = 0;
				str += sz;
				continue;
			}

		}

		str += strUrl [i++];
	}
	if (i == strUrl.GetLength () - 1)
		str += strUrl [i];
	strUrl = str;
	

	USES_CONVERSION;
	CComBSTR bstr = A2W (strUrl);
	spUrlRcvr->put_Url (bstr);

	HRESULT hr = spUrlRcvr->AddDownload ();

	BOOL bAdded = FALSE;

	
	if (SUCCEEDED (hr))
	{
		CComBSTR bstrState;

		do {
			Sleep (10);
			spUrlRcvr->get_UIState (&bstrState);
		} while (bstrState == L"in_progress");

		bAdded = bstrState == L"added";
	}

	_bstr_t bstrAddDownloadRes_err(m_strAddDownloadRes_err.c_str ());
	_bstr_t bstrAddDownloadRes_ok(m_strAddDownloadRes_ok.c_str ());

	if (bAdded == FALSE)
		response.set_Body ((const char*)bstrAddDownloadRes_err, m_strAddDownloadRes_err.length ());
	else
		response.set_Body ((const char*)bstrAddDownloadRes_ok, m_strAddDownloadRes_ok.length ());

	spUrlRcvr = NULL;
	CoUninitialize ();

	return TRUE;
}

void vmsFdmWebInterfaceServer::LoadDocuments()
{
	CFile file;
	CStringA str;
	int nHtmlLen = 0;
	std::auto_ptr<WCHAR> apchStr;
	WCHAR* wszStr = 0;

	LPCSTR pszVer = "1.0";

	file.Open (_T("Server/index.html"), CFile::modeRead);
	int nLen = file.GetLength ();
	LPSTR psz = new char [nLen];
	file.Read (psz, nLen);
	strncpy (str.GetBuffer (nLen + 1), psz, nLen);
	str.ReleaseBuffer (nLen);
	str.Replace ("%ver%", pszVer);
	delete [] psz;

#ifdef UNICODE
	nHtmlLen = ::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, 0, 0);
	apchStr.reset( new WCHAR[nHtmlLen + 1] );
	wszStr = apchStr.get();
	memset(wszStr, 0, (nHtmlLen + 1) * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, wszStr, nHtmlLen);
	m_strRootHtml = wszStr;
#else
	m_strRootHtml = str;
#endif

	file.Close ();

	file.Open (_T("Server/adddownloadres_ok.html"), CFile::modeRead);
	nLen = file.GetLength ();
	psz = new char [nLen];
	file.Read (psz, nLen);
	strncpy (str.GetBuffer (nLen + 1), psz, nLen);
	str.ReleaseBuffer (nLen);
	str.Replace ("%ver%", pszVer);
	delete [] psz;

#ifdef UNICODE
	nHtmlLen = ::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, 0, 0);
	apchStr.reset( new WCHAR[nHtmlLen + 1] );
	wszStr = apchStr.get();
	memset(wszStr, 0, (nHtmlLen + 1) * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, wszStr, nHtmlLen);
	m_strRootHtml = wszStr;
#else
	m_strAddDownloadRes_ok = str;
#endif

	file.Close ();

	file.Open (_T("Server/adddownloadres_err.html"), CFile::modeRead);
	nLen = file.GetLength ();
	psz = new char [nLen];
	file.Read (psz, nLen);
	strncpy (str.GetBuffer (nLen + 1), psz, nLen);
	str.ReleaseBuffer (nLen);
	str.Replace ("%ver%", pszVer);
	delete [] psz;

#ifdef UNICODE
	nHtmlLen = ::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, 0, 0);
	apchStr.reset( new WCHAR[nHtmlLen + 1] );
	wszStr = apchStr.get();
	memset(wszStr, 0, (nHtmlLen + 1) * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, wszStr, nHtmlLen);
	m_strRootHtml = wszStr;
#else
	m_strAddDownloadRes_err = str;
#endif

	file.Close ();

	file.Open (_T("Server/compdlds.html"), CFile::modeRead);
	nLen = file.GetLength ();
	psz = new char [nLen];
	file.Read (psz, nLen);
	strncpy (str.GetBuffer (nLen + 1), psz, nLen);
	str.ReleaseBuffer (nLen);
	str.Replace ("%ver%", pszVer);
	delete [] psz;

#ifdef UNICODE
	nHtmlLen = ::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, 0, 0);
	apchStr.reset( new WCHAR[nHtmlLen + 1] );
	wszStr = apchStr.get();
	memset(wszStr, 0, (nHtmlLen + 1) * sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP, 0, str.GetBuffer(str.GetLength()), -1, wszStr, nHtmlLen);
	m_strRootHtml = wszStr;
#else
	m_strCompDldsHtml = str;
#endif

	file.Close ();
}

BOOL vmsFdmWebInterfaceServer::RequestListOfCompletedDownloads(LPCSTR pszRes, vmsHttpResponse &response)
{
	CoInitialize (NULL);

	IFDMDownloadsStatPtr spDlds;
	spDlds.CreateInstance (__uuidof (FDMDownloadsStat));
	ASSERT (spDlds != NULL);

	spDlds->BuildListOfDownloads (TRUE, FALSE);
	
	long lCount = 0;
	spDlds->get_DownloadCount (&lCount);

	CStringA strDldsHtml;

	if (lCount == 0)
	{
		strDldsHtml = "There are no completed downloads currently.";
	}
	else
	{
		

		strDldsHtml += "<table width=\"100%\" border=\"1\">";
		strDldsHtml += "<tr>";
		strDldsHtml += "<td>File name<br>&nbsp;</td>";
		strDldsHtml += "<td>Size<br>&nbsp;</td>";
		strDldsHtml += "<td>URL<br>&nbsp;</td>";
		strDldsHtml += "</tr>";
	
		for (long i = 0; i < lCount; i++)
		{
			IFDMDownloadPtr spDld;
			spDlds->get_Download (i, &spDld);
			ASSERT (spDld != NULL);

			CStringA str;

			str += "<tr>";
		
			for (int i = 0; i < 3; i++)
			{
				str += "<td>"; 
				CComBSTR bstr;
				if (i != 2)
				{
					spDld->get_DownloadText (i, &bstr);
					CString str2 = bstr;
					str += str2;
				}
				else
				{
					spDld->get_Url (&bstr);
					CStringA str2 = bstr;
					str += "<a href=\""; str += str2; str += "\">"; 
					str += str2; str += "</a>";
				}
				
				str += "</td>";
			}

			str += "</tr>";

			strDldsHtml += str;
		}

		strDldsHtml += "</table>";
	}

	_bstr_t bstrCompDldsHtml(m_strCompDldsHtml.c_str ());
	CStringA str = (const char*)bstrCompDldsHtml;
	str.Replace ("%compdlds%", strDldsHtml);

	response.set_Body (str, str.GetLength ());

	return TRUE;
}
