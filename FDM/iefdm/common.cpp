/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#include "stdafx.h"

#include "..\DataStretcher.h"
#include <comdef.h>
#include <inetfile/inetfile.h>

BOOL fsOnNavigateUrl (LPCSTR pszUrl)
{
	BOOL bALT = GetKeyState (VK_MENU) & 0x8000;
	BOOL bCTRL = GetKeyState (VK_CONTROL) & 0x8000;
	
	
	if (bCTRL && bALT)
		return TRUE;
	
	CRegKey wgKey;

	DWORD bALTShouldPressed = FALSE;

	if (ERROR_SUCCESS == wgKey.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Monitor",
			KEY_READ))
		wgKey.QueryValue (bALTShouldPressed, "ALTShouldPressed");

	
	if (bALT == FALSE && bALTShouldPressed)
		return FALSE; 

	return TRUE;
}

LPCSTR strcmp_m (LPCSTR pszWhere, LPCSTR pszWhat)
{
	if (*pszWhere == 0)
		return *pszWhat == 0 ? pszWhere : NULL;

	if (*pszWhat == 0)
		return NULL;

	if (*pszWhat == '*')
	{
		if (pszWhat [1] == 0)
			return pszWhere;
		
		
		
		LPCSTR psz = strcmp_m (pszWhere, pszWhat+1);
		if (psz)
			return psz;

		
		return strcmp_m (pszWhere+1, pszWhat);
	}

	if (*pszWhat != '?')
	{
		if (*pszWhere != *pszWhat)
			return NULL;
	}

	return strcmp_m (pszWhere+1, pszWhat+1) ? pszWhere : NULL;
}

LPCSTR strcmpi_m (LPCSTR pszWhere, LPCSTR pszWhat)
{
	char *psz1 = new char [lstrlen (pszWhere) + 1];
	char *psz2 = new char [lstrlen (pszWhat) + 1];

	lstrcpy (psz1, pszWhere);
	lstrcpy (psz2, pszWhat);

	CharLower (psz1);
	CharLower (psz2);

	LPCSTR psz = strcmp_m (psz1, psz2);
	LPCSTR pszRet = NULL;
	if (psz)
		pszRet = pszWhere + (psz - psz1);

	delete [] psz1;
	delete [] psz2;

	return pszRet;
}

BOOL IsExtStrEq (LPCSTR pszMasked, LPCSTR psz2)
{
	return strcmpi_m (psz2, pszMasked) != NULL;

	
}

BOOL IsExtInExtsStr (LPCSTR pszExts, LPCSTR pszExt)
{
	if (pszExt == NULL)
		return FALSE;

	int len = lstrlen (pszExts);
	int i = 0;
	CHAR szExt [10000];

	do
	{
		int j = 0;

		while (i < len && pszExts [i] != ' ')
			szExt [j++] = pszExts [i++];

		szExt [j] = 0;
		i++;

		if (IsExtStrEq (szExt, pszExt))
			return TRUE;

	} while (i < len);

	return FALSE;
}

BOOL IsServerInServersStr (LPCSTR pszServers, LPCSTR pszServer)
{
	if (pszServer == NULL)
		return FALSE;

	int len = lstrlen (pszServers);
	int i = 0;
	CHAR szServer [10000];

	do
	{
		int j = 0;

		while (i < len && pszServers [i] != ' ')
			szServer [j++] = pszServers [i++];

		szServer [j] = 0;
		i++;

		if (IsExtStrEq (szServer, pszServer))
			return TRUE;

		fsString str;
		str = "*."; str += szServer;
		if (IsExtStrEq (str, pszServer))
			return TRUE;

	} while (i < len);

	return FALSE;
}

BOOL fsFileNameFromUrlPath (LPCSTR pszUrl, BOOL bUsingFTP, BOOL bDecode, LPSTR pszBuf, UINT uSize)
{
	*pszBuf = 0;

	int len = strlen (pszUrl);
	int pos;

	if (len == 0)
		return TRUE;

	int end = bUsingFTP ? strlen (pszUrl)-1 : strcspn (pszUrl, "?=#") - 1;
	
	if (end < 0)
		return FALSE;
	
	if (end >= len)
		end = len - 1;

	pos = end;

	while (pos && pszUrl [pos] != '/' && pszUrl [pos] != '\\')
		pos--;

	if (pszUrl [pos] != '/' && pszUrl [pos] != '\\')
		return FALSE;

	if (UINT(len - pos) > uSize)
		return FALSE;

	strcpy (pszBuf, pszUrl + pos + 1);

	pszBuf [end - pos] = 0;

	return TRUE;
}    

LPCSTR Monitor_OnlyExts ()
{
	static char* pszExts = "\
EXE ZIP ARJ RAR LZH Z GZ TGZ GZIP TAR BIN MP3 M4A WAV RA RAM AAC AIF AVI MPG\
MPEG QT PLJ ASF MOV RM MP4 WMA WMV MPE MPA R0* R1* A0* A1* TIF TIFF PDF MSI\
ACE ISO OGG 7Z SEA SIT SITX PPT PPS";

	return pszExts;
}

LPCSTR Monitor_SkipExts ()
{
	static char szExts [10000];
	lstrcpy (szExts, "pls m3u");
	CRegKey wgKey;
	if (ERROR_SUCCESS == wgKey.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Monitor",
			KEY_READ))
	{
		DWORD dw = sizeof (szExts);
		wgKey.QueryValue (szExts, "SkipExtensions", &dw);
	}
	return szExts;
}

LPCSTR Monitor_SkipServers ()
{
	static char szServers [10000];
	CRegKey wgKey;
	if (ERROR_SUCCESS == wgKey.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Monitor",
			KEY_READ))
	{
		DWORD dw = sizeof (szServers);
		wgKey.QueryValue (szServers, "SkipServers", &dw);
	}
	return szServers;
}

BOOL is_ExtToSkip (LPCSTR pszURL, BOOL bUseSkipExtsList)
{
	fsURL url;

	if (url.Crack (pszURL) != IR_SUCCESS)
		return bUseSkipExtsList == FALSE;

	char szFile [10000];
	fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
		TRUE, szFile, sizeof (szFile));

	if (*szFile == 0)
		return bUseSkipExtsList == FALSE;

	LPCSTR pszExt = strrchr (szFile, '.');
	if (pszExt++ == NULL) 
		return bUseSkipExtsList == FALSE;

	if (bUseSkipExtsList == FALSE && IsExtInExtsStr (Monitor_OnlyExts (), pszExt) == FALSE)
		return TRUE;

	return IsExtInExtsStr (Monitor_SkipExts (), pszExt);
}

BOOL is_ServerToSkip (LPCSTR psz)
{
	fsURL url;
	if (IR_SUCCESS != url.Crack (psz))
		return TRUE;

	return IsServerInServersStr (Monitor_SkipServers (), url.GetHostName ());
}

BOOL fsUrlToFdm (LPCSTR pszUrl, LPCSTR pszReferer, LPCSTR pszCookies, LPCSTR pszPostData, BOOL bUseSkipExtsList)
{
	if (is_ServerToSkip (pszUrl))
		return FALSE;

	if (pszReferer && is_ServerToSkip (pszReferer))
		return FALSE;

	if (is_ExtToSkip (pszUrl, bUseSkipExtsList))
		return FALSE;

	IWGUrlReceiver* wg;
	HRESULT hr;
	if (FAILED (hr=CoCreateInstance (CLSID_WGUrlReceiver, NULL, CLSCTX_ALL, IID_IWGUrlReceiver, (void**) &wg)))
	{
		char szMsg [1000];
		lstrcpy (szMsg, "Free Download Manager is not properly installed! Please reinstall Free Download Manager or try to reboot your PC.\n\nError code: 0x");
		char sz [100];
		itoa ((UINT)hr, sz, 16);
		lstrcat (szMsg, sz);
		MessageBox (NULL, szMsg, "Error", MB_ICONERROR);
		return FALSE;
	}

	wg->put_IgnoreSize (FALSE);

	_bstr_t url = pszUrl;
	wg->put_Url (url);
	if (pszCookies)
	{
		_bstr_t cookies = pszCookies;
		wg->put_Cookies (cookies);
	}
	if (pszReferer)
	{
		_bstr_t referer = pszReferer;
		wg->put_Referer (referer);
	}
	if (pszPostData)
	{
		_bstr_t ah = pszPostData;
		wg->put_PostData (ah);
	}
	wg->put_CheckExtension (FALSE);
	hr = wg->AddDownload ();

	if (FAILED (hr))
		return FALSE;

	CRegKey wgKey;
	if (ERROR_SUCCESS == wgKey.Open (HKEY_CURRENT_USER, "Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Monitor", KEY_READ))
	{
		DWORD b = FALSE;
		if (ERROR_SUCCESS != wgKey.QueryValue (b, "AllowDownload"))
			b = TRUE;
		if (b == FALSE)	
		{
			wg->Release ();
			return TRUE;
		}
	}
	
	CComBSTR bstrState;
	do {
		Sleep (10);
		wg->get_UIState (&bstrState);
	} while (bstrState == L"in_progress");

	wg->Release ();

	return bstrState == L"added";
}

BOOL fsIsMonitoringTurnedOn ()
{
	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, 
			"Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Monitor", KEY_READ))
		return TRUE;

	DWORD dwMonitor;

	if (ERROR_SUCCESS != key.QueryValue (dwMonitor, "IE2"))
		return TRUE;

	return dwMonitor;
}