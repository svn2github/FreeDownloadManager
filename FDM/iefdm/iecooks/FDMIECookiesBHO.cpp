/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Iecooks.h"
#include "FDMIECookiesBHO.h"
#include <mshtml.h>
#include <oleacc.h>
#include "../../vmsVideoSiteHtmlCodeParser.h"

_COM_SMARTPTR_TYPEDEF(IHTMLInputElement, __uuidof(IHTMLInputElement));

extern BOOL _bIsWin9x;              

#pragma data_seg("shdata")

long CFDMIECookiesBHO::_cIEDownloads = 0;
#pragma data_seg()  

#pragma comment(linker, "/section:shdata,rws")  

IHTMLDocument2Ptr doc_from_hwnd (HWND hwnd)
{
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	if (hInst == NULL)
		return NULL;

	LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, _T("ObjectFromLresult") );

	UINT nMsg = RegisterWindowMessage ("WM_HTML_GETOBJECT");
	
	LRESULT lRes;
	::SendMessageTimeout (hwnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

	IHTMLDocument2Ptr spDoc;
	pfObjectFromLresult (lRes, IID_IHTMLDocument2, 0, (void**)&spDoc);

	FreeLibrary (hInst);

	return spDoc;
}

BOOL is_okpage (HWND hwnd)
{
	IHTMLDocument2Ptr spDoc = doc_from_hwnd (hwnd);
	if (spDoc == NULL)
		return TRUE;

	IHTMLLocationPtr spLoc;
	spDoc->get_location (&spLoc);

	BSTR bstrHost = NULL;
	spLoc->get_hostname (&bstrHost);

	if (bstrHost == NULL)
		return FALSE;

	USES_CONVERSION;

	if (vmsVideoSiteHtmlCodeParser::GetSupportedSiteIndex (W2A (bstrHost)) != -1)
		return TRUE;

	return FALSE;
}

WNDPROC _pfnIeServerOrigProc = NULL;
BOOL _bUseUnicode;
DWORD _dwIeMajorVersion;

BOOL GetIEVersion(DWORD *pMajor, DWORD *pMinor, DWORD *pBuild, DWORD *pSubBuild)
{
    CRegKey rk;
    LONG lRet = rk.Open(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer"), KEY_READ);
    if(lRet == 0)
    {
        TCHAR szBuff[256];
        DWORD dwCount = sizeof(szBuff);
        lRet = rk.QueryValue(szBuff, _T("Version"), &dwCount);
        if(lRet == 0)
        {
            sscanf(szBuff, "%u.%u.%u.%u", pMajor, pMinor, pBuild, pSubBuild);
            return TRUE;
        }
    }

    return FALSE;
}  

BOOL _bMenuWasRemoved = FALSE;
UINT _nRemovedMenuPrevIndex;
char _szRemovedMenuPrevString [100];
UINT _nRemovedMenuID;
char _szRemovedMenuString [100];
BOOL _bSeparatorWasRemoved;  

LRESULT CALLBACK _IeServerWndProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_INITMENUPOPUP && is_okpage (hwnd) == FALSE)
	{
		HMENU menu = (HMENU)wp;

		CRegKey key;
		key.Open (HKEY_CURRENT_USER, 
			"Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\View\\Loc", KEY_READ);
		char szItem [1000] = ""; DWORD dw = sizeof (szItem);
		key.QueryValue (szItem, "dlfvideoiemenu", &dw);
		
		int cItems = GetMenuItemCount (menu);

		while (--cItems)
		{
			char sz [300] = "";
			GetMenuString (menu, cItems, sz, sizeof (sz), MF_BYPOSITION);
			if (lstrcmp (sz, szItem) == 0) 
			{
				BOOL bRemoveSeparator = GetMenuState (menu, cItems+1, MF_BYPOSITION) & 
						GetMenuState (menu, cItems-1, MF_BYPOSITION) & MF_SEPARATOR;

				if (_dwIeMajorVersion != 7)
				{
					_nRemovedMenuPrevIndex = cItems - 1;
					_szRemovedMenuPrevString [0] = 0;
					GetMenuString (menu, _nRemovedMenuPrevIndex, _szRemovedMenuPrevString,
						sizeof (_szRemovedMenuPrevString), MF_BYPOSITION);

					_nRemovedMenuID = GetMenuItemID (menu, cItems);
					GetMenuString (menu, cItems, _szRemovedMenuString,
						sizeof (_szRemovedMenuString), MF_BYPOSITION);

					_bSeparatorWasRemoved = bRemoveSeparator;
					
					_bMenuWasRemoved = TRUE;
				}

				RemoveMenu (menu, cItems, MF_BYPOSITION);
				if (bRemoveSeparator)
					RemoveMenu (menu, cItems, MF_BYPOSITION);
				break;
			}
		}
	}
	else if (msg == WM_UNINITMENUPOPUP && _bMenuWasRemoved)
	{
		HMENU menu = (HMENU)wp;

		char sz [100] = "";
		GetMenuString (menu, _nRemovedMenuPrevIndex, sz, sizeof (sz), MF_BYPOSITION);

		if (lstrcmp (sz, _szRemovedMenuPrevString) == 0)
		{
			_bMenuWasRemoved = FALSE;
			InsertMenu (menu, _nRemovedMenuPrevIndex+1, MF_BYPOSITION, _nRemovedMenuID, _szRemovedMenuString);
			if (_bSeparatorWasRemoved)
				InsertMenu (menu, _nRemovedMenuPrevIndex+2, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		}
	}

	return _bUseUnicode ? CallWindowProcW (_pfnIeServerOrigProc, hwnd, msg, wp, lp) : 
		CallWindowProcA (_pfnIeServerOrigProc, hwnd, msg, wp, lp);
}

HWND find_ie_server (HWND hwnd)
{
	HWND hwnd2 = NULL;
	
	for (;;)
	{
		hwnd2 = FindWindowEx (hwnd, hwnd2, NULL, NULL);
		if (hwnd2 == NULL)
			return NULL;

		char sz [1000];
		GetClassName (hwnd2, sz, sizeof (sz));

		if (lstrcmpi (sz, "Internet Explorer_Server") == 0)
		{
			LONG lProc;
			if (_bUseUnicode)
				lProc = GetWindowLongW (hwnd2, GWL_WNDPROC);
			else
				lProc = GetWindowLongA (hwnd2, GWL_WNDPROC);
			if (_pfnIeServerOrigProc == NULL)
				_pfnIeServerOrigProc = (WNDPROC)lProc;
			if (lProc == (LONG)_pfnIeServerOrigProc)
				return hwnd2;
		}

		HWND hwnd3 = find_ie_server (hwnd2);
		if (hwnd3 != NULL)
			return hwnd3;
	}
}

DWORD WINAPI _threadSubclassIeServerWnd (LPVOID lp)
{
	IWebBrowser2* wb = (IWebBrowser2*) lp;

	_bUseUnicode = (GetVersion () & 0x80000000) == 0;
	DWORD dw;
	GetIEVersion (&_dwIeMajorVersion, &dw, &dw, &dw);

	HWND hwnd;
	wb->get_HWND ((long*)&hwnd);

	HWND hwnd2;
	do {
		Sleep (1000);
		hwnd2 = find_ie_server (hwnd);
	}
	while (hwnd2 == NULL);
	
	if (_bUseUnicode)
		SetWindowLongW (hwnd2, GWL_WNDPROC, (LONG)_IeServerWndProc);
	else
		SetWindowLongA (hwnd2, GWL_WNDPROC, (LONG)_IeServerWndProc);
	
	return 0;
}

STDMETHODIMP CFDMIECookiesBHO::SetSite(IUnknown *pSite)
{
	if (m_spWB != NULL)
		Disconnect ();

	if (pSite == NULL)
		DeleteFile ();

	m_spWB = pSite;

	if (m_spWB == NULL)
		return pSite == NULL ? S_OK : E_INVALIDARG;

	

	m_spWB_CPC = m_spWB;
	if (m_spWB_CPC == NULL)
		return E_INVALIDARG;

	return Connect ();
}

STDMETHODIMP CFDMIECookiesBHO::GetSite(REFIID riid, void **ppvSite)
{
	if (m_spWB == NULL || ppvSite == NULL)
		return E_INVALIDARG;
	
	*ppvSite = NULL;
	return m_spWB.QueryInterface (riid, (IWebBrowser2**)ppvSite);
}

STDMETHODIMP CFDMIECookiesBHO::Connect()
{
	HRESULT hr;

	hr = Disconnect ();
	if (FAILED (hr))
		return hr;

	hr = m_spWB_CPC->FindConnectionPoint (DIID_DWebBrowserEvents2, &m_spWB_CPC_CP);
	if (FAILED (hr))
		return hr;

	IUnknown *pthis = (IUnknown*)(IFDMIECookiesBHO*) this;

	hr = m_spWB_CPC_CP->Advise (pthis, &m_dwCookie);

	return hr;
}

STDMETHODIMP CFDMIECookiesBHO::Disconnect()
{
	if (m_spWB_CPC_CP == NULL)
		return S_FALSE;

	HRESULT hr = m_spWB_CPC_CP->Unadvise (m_dwCookie);
	if (FAILED (hr))
		return hr;

	m_spWB_CPC_CP = NULL;
	m_spWB_CPC = NULL;

	return S_OK;
}

STDMETHODIMP CFDMIECookiesBHO::ProgressChange(long Progress, long ProgressMax)
{
	USES_CONVERSION;

	

	IDispatchPtr spdDoc;
	m_spWB->get_Document (&spdDoc);
	if (spdDoc == NULL)
		return S_OK;

	IHTMLDocument2Ptr spDoc (spdDoc);
	if (spDoc == NULL)
		return S_OK;

	BSTR bstr;
	spDoc->get_cookie (&bstr);

	fsString strCookies, strUrl;

	if (bstr)
	{
		strCookies = W2A (bstr);
		SysFreeString (bstr);
	}

	spDoc->get_URL (&bstr);

	if (bstr)
	{
		strUrl = W2A (bstr);
		SysFreeString (bstr);
	}

	if (m_strUrl != strUrl || m_strCookies != strCookies || 
			m_strPostData != "")
	{
		m_strUrl = strUrl;
		m_strCookies = strCookies;
		SaveInfoToDisk ();
	}

	return S_OK;
}

void CFDMIECookiesBHO::SaveInfoToDisk()
{
	WaitForSingleObject (m_mxFile, INFINITE);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		char szTmpPath [MAX_PATH], szFile [MAX_PATH];
		GetTempPath (MAX_PATH, szTmpPath);
		CreateDirectory (szTmpPath, NULL);
		lstrcat (szTmpPath, "Free Download Manager");
		CreateDirectory (szTmpPath, NULL);
		GetTempFileName (szTmpPath, "tic", 0, szFile);  

		if (_bIsWin9x == FALSE)
		{
			m_hFile = CreateFile (szFile, GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_HIDDEN | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		}
		else
		{
			m_strTmpFile = szFile;
			m_hFile = CreateFile (szFile, GENERIC_WRITE, 
				FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_HIDDEN, NULL);
		}

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			ReleaseMutex (m_mxFile);
			return;
		}
	}

	SetFilePointer (m_hFile, 0, NULL, FILE_BEGIN);

	DWORD dw;

	WriteFile (m_hFile, (LPCSTR)m_strUrl, m_strUrl.GetLength (), &dw, NULL);
	WriteFile (m_hFile, "\r\n", 2, &dw, NULL);
	WriteFile (m_hFile, (LPCSTR)m_strCookies, m_strCookies.GetLength (), &dw, NULL);
	WriteFile (m_hFile, "\r\n", 2, &dw, NULL);
	WriteFile (m_hFile, (LPCSTR)m_strPostData, m_strPostData.GetLength (), &dw, NULL);
	WriteFile (m_hFile, "\r\n", 2, &dw, NULL);
	WriteFile (m_hFile, (LPCSTR)m_strBeforeNavUrl, m_strBeforeNavUrl.GetLength (), &dw, NULL);

	SetEndOfFile (m_hFile);

	ReleaseMutex (m_mxFile);
}

STDMETHODIMP CFDMIECookiesBHO::BeforeNavigate2(IDispatch *pdWB, VARIANT *url, VARIANT *flags, VARIANT *tfn, VARIANT *pd, VARIANT *headers, VARIANT_BOOL *bCancel)
{
	USES_CONVERSION;

	m_strPostData = "";
	m_strBeforeNavUrl = W2A (url->bstrVal);

	IWebBrowser2Ptr spWB (pdWB);
	if (spWB == NULL)
		return S_OK;

	IDispatchPtr spdDoc;
	spWB->get_Document (&spdDoc);
	if (spdDoc == NULL)
		return S_OK;

	IHTMLDocument2Ptr spDoc (spdDoc);
	if (spDoc == NULL)
		return S_OK;

	IHTMLElementCollectionPtr spForms;
	spDoc->get_forms (&spForms);

	long cForms = 0;
	if (spForms != NULL)
		spForms->get_length (&cForms);

	bool bFound = false;

	

	for (long i = 0; bFound == false && i < cForms; i++)
	{
		IDispatchPtr spd;
		spForms->item (CComVariant (i), CComVariant (i), &spd);
		if (spd == NULL)
			continue;

		IHTMLFormElementPtr spForm (spd);
		if (spForm == NULL)
			continue;

		BSTR bstr = NULL;
		spForm->get_action (&bstr);
		
		bFound = bstr != NULL && wcscmp (url->bstrVal, bstr) == 0;
		SysFreeString (bstr);
		if (bFound == false)
			continue;

		bstr = NULL;
		spForm->get_method (&bstr);
		
		if (bstr == NULL || wcsicmp (bstr, L"post"))
			break;
		SysFreeString (bstr);

		IHTMLElementPtr spFormElem (spForm);
		if (spFormElem == NULL)
		{
			bFound = false;
			continue;
		}

		
		WalkThroughForm (spFormElem, m_strPostData);

		
		if (m_strPostData != "" && 
				m_strPostData [m_strPostData.GetLength ()-1] == '&')
			m_strPostData [m_strPostData.GetLength ()-1] = 0;
	}

	return S_OK;
}

void CFDMIECookiesBHO::WalkThroughForm(IHTMLElement *pElement, fsString& str)
{
	USES_CONVERSION;

	IDispatchPtr spd;
	pElement->get_children (&spd);
	if (spd == NULL)
		return;
	IHTMLElementCollectionPtr spels (spd);
	if (spels == NULL)
		return;

	long cElements = 0;
	spels->get_length (&cElements);
	
	for (int j = 0; j < cElements; j++)
	{
		spd = NULL;
		spels->item (CComVariant (j), CComVariant (j), &spd);
		if (spd == NULL)
			continue;

		
		IHTMLInputElementPtr spInp (spd);
		if (spInp != NULL)
		{
			
			BSTR bstr, bstr2;
			spInp->get_name (&bstr);				
			spInp->get_value (&bstr2);
			if (bstr)
			{
				str += W2A (bstr);
				str += "=";
				SysFreeString (bstr);
			}
			if (bstr2)
			{
				str += W2A (bstr2);
				SysFreeString (bstr2);
			}
			if (bstr || bstr2)
				str += "&";
		}
		
		IHTMLElementPtr spel (spd);
		if (spel != NULL)
			WalkThroughForm (spel, str);	
	}
}

STDMETHODIMP CFDMIECookiesBHO::DownloadBegin()
{
	InterlockedIncrement (&_cIEDownloads);
	return S_OK;
}

STDMETHODIMP CFDMIECookiesBHO::DownloadComplete()
{
	InterlockedDecrement (&_cIEDownloads);
	return S_OK;
}

long CFDMIECookiesBHO::get_IEDownloadsCount()
{
	return _cIEDownloads;
}

void CFDMIECookiesBHO::DeleteFile()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle (m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		if (_bIsWin9x)
			::DeleteFile (m_strTmpFile);
	}

	WaitForSingleObject (m_mxFile, INFINITE);
	char szTmpPath [MAX_PATH];
	GetTempPath (MAX_PATH, szTmpPath);
	lstrcat (szTmpPath, "Free Download Manager");
	RemoveDirectory (szTmpPath);
	ReleaseMutex (m_mxFile);
}

#include "vmsFlashHelper.h"
#include <mflash.h>

_COM_SMARTPTR_TYPEDEF(IShockwaveFlash, __uuidof(IShockwaveFlash));

STDMETHODIMP CFDMIECookiesBHO::DocumentComplete(IDispatch *pDisp, VARIANT *URL)
{
	

	return S_OK;
}
