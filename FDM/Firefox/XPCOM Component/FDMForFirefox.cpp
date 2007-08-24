/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#include "FDMForFirefox.h"
#include <windows.h>
#include "../../lngphrases.h"
#include "vmsFirefoxExtensionInstaller.h"
#include "FDMUrlReceiver.h"
#include <assert.h>
#include <atlbase.h>
#include <fsString.h>

_COM_SMARTPTR_TYPEDEF(IFDMFlashVideoDownloads, __uuidof(IFDMFlashVideoDownloads));  

#define FLASHGOT_CID	"{19503e42-ca3c-4c27-b1e2-9cdb2170ee34}"

NS_IMPL_ISUPPORTS1(CFDMForFirefox, IFDMForFirefox)

CFDMForFirefox::CFDMForFirefox()
{
	m_keyFDM.Open (HKEY_CURRENT_USER, 
		"Software\\FreeDownloadManager.ORG\\Free Download Manager");

	m_keyFDMMonitor.Open (m_keyFDM,	"Settings\\Monitor", KEY_READ);

	char szPath [MAX_PATH] = ""; 
	DWORD dw = sizeof (szPath);
	m_keyFDM.QueryValue (szPath, "Path", &dw);
	if (*szPath)
	{
		if (szPath [lstrlen (szPath) - 1] != '\\')
			lstrcat (szPath, "\\");

		lstrcat (szPath, "Language");

		
		m_lm.Initialize (szPath);

		char szLang [100] = "English";	
		dw = sizeof (szLang);
		CRegKey key;
		key.Open (m_keyFDM, "Settings\\View", KEY_READ);
		
		key.QueryValue (szLang, "Language", &dw);
		
		int nIndex = m_lm.FindLngByName (szLang);
		if (nIndex != -1)
			m_lm.LoadLng (nIndex);
	}

	
	
	if (ERROR_SUCCESS != m_keyFDMFfStgs.Open (m_keyFDM, "FirefoxSettings\\Proxy"))
		m_keyFDMFfStgs.Create (m_keyFDM, "FirefoxSettings\\Proxy");

	CoInitialize (NULL);
}

CFDMForFirefox::~CFDMForFirefox()
{
  
	CoUninitialize ();
}  

NS_IMETHODIMP CFDMForFirefox::GetLngString(const char *strIDString, PRUnichar **_retval)
{
    *_retval = new wchar_t [1000];

	LPCSTR psz = NULL;
	
	if (lstrcmp (strIDString, "dllink") == 0)
	{
		psz = m_lm.GetString (L_DLTHISIEMENU);
		if (psz == NULL || *psz == 0)
			psz = "Download with Free Download Manager";
	}

	else if (lstrcmp (strIDString, "dlall") == 0)
	{
		psz = m_lm.GetString (L_DLALLIEMENU);
		if (psz == NULL || *psz == 0)
			psz = "Download all with Free Download Manager";
	}

	else if (lstrcmp (strIDString, "dlselected") == 0)
	{
		psz = m_lm.GetString (L_DLSELECTEDIEMENU);
		if (psz == NULL || *psz == 0)
			psz = "Download selected with Free Download Manager";
	}

	else if (lstrcmp (strIDString, "dlvideo") == 0)
	{
		psz = m_lm.GetString (L_DLFLASHVIDEOIEMENU);
		if (psz == NULL || *psz == 0)
			psz = "Download video with Free Download Manager";
	}

	**_retval = 0;

	if (psz && *psz)
	{
		MultiByteToWideChar (CP_ACP, 0, psz, -1, *_retval, 1000);
		(*_retval) [lstrlen (psz)] = 0;
	}

	return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::IsContextMenuItemShouldBeHidden(const char *strMenuItemID, PRBool *_retval)
{
	if (is_FlashGotInstalled ())
	{
		*_retval = TRUE;
		return NS_OK;
	}

	CRegKey key;

	

	key.Open (m_keyFDM, "Settings\\Monitor\\IEMenu", KEY_READ);

	DWORD dwShow = TRUE;

	key.QueryValue (dwShow, "Enable");
	
	if (dwShow)
	{
		if (lstrcmp (strMenuItemID, "dllink") == 0)
			key.QueryValue (dwShow, "DLThis");

		else if (lstrcmp (strMenuItemID, "dlall") == 0)
			key.QueryValue (dwShow, "DLAll");

		else if (lstrcmp (strMenuItemID, "dlselected") == 0)
			key.QueryValue (dwShow, "DLSelected");

		else if (lstrcmp (strMenuItemID, "dlvideo") == 0)
			key.QueryValue (dwShow, "DLFlashVideo");
	}

	*_retval = !dwShow;

    return NS_OK;
}

bool CFDMForFirefox::is_FlashGotInstalled()
{
	static int _isInstalled = -1;
	if (_isInstalled == -1)
		_isInstalled = vmsFirefoxExtensionInstaller::IsInstalled (FLASHGOT_CID, true);
	return _isInstalled != 0;
}  

NS_IMETHODIMP CFDMForFirefox::IsALTShouldBePressed(PRBool *_retval)
{
	if (is_FlashGotInstalled ())
	{
		*_retval = FALSE;
		return NS_OK;
	}

   	DWORD dw1 = TRUE, dw2 = FALSE;
	
	m_keyFDMMonitor.QueryValue (dw1, "Firefox");

	if (dw1)
		m_keyFDMMonitor.QueryValue (dw2, "ALTShouldPressed");

	*_retval = dw1 && dw2;

	return NS_OK;
}

NS_IMETHODIMP CFDMForFirefox::IsLinkShouldBeSkipped(IFDMUrl *url, const PRUnichar *wstrSuggFileName, PRBool *_retval)
{
	*_retval = TRUE;

	if (is_FlashGotInstalled ())
		return NS_OK;

	DWORD dw = TRUE;

	m_keyFDMMonitor.QueryValue (dw, "Firefox");
	if (dw == FALSE)
		return NS_OK;

	dw = FALSE;
	m_keyFDMMonitor.QueryValue (dw, "ALTShouldPressed");
	if (dw)
		return NS_OK;

	*_retval = IsUrlShouldBeSkipped (url, wstrSuggFileName);

    return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::CatchLink(IFDMUrl *url, const PRUnichar *wstrSuggFileName, PRBool *_retval)
{
	IsLinkShouldBeSkipped (url, wstrSuggFileName, _retval);
	if (*_retval)
		return NS_OK;

	*_retval = TransferUrlToFDM (url);

    return NS_OK;
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

fsString DomainFromUrl(LPCSTR pszUrl)
{
	LPCSTR pszS = strstr (pszUrl, "://");
	if (pszS == NULL)
		pszS = strstr (pszUrl, ":\\\\");
	if (pszS == NULL)
		pszS = pszUrl;
	else
		pszS += 3;
	LPCSTR pszE = pszS;
	while (*pszE && *pszE != '\\' && *pszE != '/')
		pszE++;

	char sz [1000];
	if (pszE - pszS > sizeof (sz))
		return "";

	lstrcpyn (sz, pszS, pszE - pszS + 1);
	return sz;
}

bool CFDMForFirefox::IsServerToSkip (IFDMUrl *url)
{
	USES_CONVERSION;

	wchar_t *wsz;
	url->GetUrl (&wsz);
	fsString strDomain = DomainFromUrl (W2A (wsz));
	delete [] wsz;

	char szServers [10000] = ""; DWORD dw = sizeof (szServers);
	m_keyFDMMonitor.QueryValue (szServers, "SkipServers", &dw);

	return IsServerInServersStr (szServers, strDomain) != 0;
}

bool CFDMForFirefox::IsUrlShouldBeSkipped(IFDMUrl *url, const wchar_t *pwszSuggFileName)
{
	char szExts [10000] = "pls m3u"; DWORD dw = sizeof (szExts);
	m_keyFDMMonitor.QueryValue (szExts, "SkipExtensions", &dw);

	char szFile [1000] = "";
	WideCharToMultiByte (CP_ACP, 0, pwszSuggFileName, -1, szFile, sizeof (szFile), 
		NULL, NULL);
	szFile [wcslen (pwszSuggFileName)] = 0;

	LPCSTR pszExt = strrchr (szFile, '.');
	if (pszExt++ == NULL) 
		return false;

	return IsExtInExtsStr (szExts, pszExt) != FALSE ||
		IsServerToSkip (url);
}

PRBool CFDMForFirefox::TransferUrlToFDM(IFDMUrl *url)
{
	IWGUrlReceiverPtr spUrlRcvr;
	spUrlRcvr.CreateInstance (__uuidof (WGUrlReceiver));

	assert (spUrlRcvr != NULL);
    if (spUrlRcvr == NULL)
		return FALSE;

	if (NS_OK != CFDMUrlReceiver::ApplyUrlToReceiver (spUrlRcvr, url))
		return FALSE;

	spUrlRcvr->put_IgnoreSize (FALSE);

	if (FAILED (spUrlRcvr->ShowAddDownloadDialog ()))
		return FALSE;

	DWORD dwAllow = TRUE;
	m_keyFDMMonitor.QueryValue (dwAllow, "AllowDownload");
	if (dwAllow == FALSE)
		return TRUE;	

	BSTR bstrState = NULL;
	do {
		if (bstrState)
			SysFreeString (bstrState);
		Sleep (5);
		spUrlRcvr->get_UIState (&bstrState);
	} while (wcsicmp (bstrState, L"in_progress") == 0);

	BOOL bAdded = wcsicmp (bstrState, L"added") == 0;
	SysFreeString (bstrState);

	return bAdded;
}  

NS_IMETHODIMP CFDMForFirefox::SetProxyType(PRInt32 nType)
{
	m_keyFDMFfStgs.SetValue (nType, "ProxyType");
    return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::SetProxy(const char *strProtocol, const char *strAddr, PRInt32 nPort)
{
	fsString str = strProtocol;
	fsString strAddrName = str; strAddrName += "_addr";
	fsString strPortName = str; strPortName += "_port";

	m_keyFDMFfStgs.SetValue (strAddr, strAddrName);
	m_keyFDMFfStgs.SetValue (nPort, strPortName);
	
    return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::IsDomainSupportedForVideoDownloads(const PRUnichar *wstrDomain, PRBool *_retval)
{
	if (_wcsnicmp (wstrDomain, L"www.", 4) == 0)
		wstrDomain += 4;

	*_retval = wcsicmp (wstrDomain, L"youtube.com") == 0;

    return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::ProcessVideoDocument(const PRUnichar *wstrDomain, const PRUnichar *wstrReferer, const PRUnichar *wstrHTML, PRBool *_retval)
{
	*_retval = FALSE;

	IFDMFlashVideoDownloadsPtr spFVDownloads;
	spFVDownloads.CreateInstance (__uuidof (FDMFlashVideoDownloads));

	assert (spFVDownloads != NULL);
	if (spFVDownloads == NULL)
		return NS_OK;

	BSTR bstrDomain = SysAllocString (wstrDomain);
	BSTR bstrHtml = SysAllocString (wstrHTML);

	spFVDownloads->ProcessHtml (bstrDomain, bstrHtml);

	*_retval = TRUE;

    return NS_OK;
}  

NS_IMETHODIMP CFDMForFirefox::CreateVideoDownloadFromUrl(const PRUnichar *wstrUrl, PRBool *_retval)
{
	*_retval = FALSE;

	IFDMFlashVideoDownloadsPtr spFVDownloads;
	spFVDownloads.CreateInstance (__uuidof (FDMFlashVideoDownloads));

	assert (spFVDownloads != NULL);
	if (spFVDownloads == NULL)
		return NS_OK;

	spFVDownloads->CreateFromUrl (SysAllocString (wstrUrl));

	*_retval = TRUE;

    return NS_OK;
}