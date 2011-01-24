/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Iecooks.h"
#include "FDMIECookiesBHO.h"
#include <mshtml.h>
#include <oleacc.h>
#include "../../vmsVideoSiteHtmlCodeParser.h"
#include "vmsIeTabsHookFitter.h"
#include "HtmlEventSink.h"
#include "vmsFlashWindowsSubclasser.h"
#include "vmsComGIT.h"
#include <shlguid.h>
#include <vector>
using namespace std;

_COM_SMARTPTR_TYPEDEF(IHTMLInputElement, __uuidof(IHTMLInputElement));

extern BOOL _bIsWin9x;              

#pragma data_seg("shdata")

long CFDMIEBHO::_cIEDownloads = 0;
#pragma data_seg()  

#pragma comment(linker, "/section:shdata,rws")  

IHTMLDocument2Ptr doc_from_hwnd (HWND hwnd)
{
	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
	if (hInst == NULL)
		return NULL;

	LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, _T("ObjectFromLresult") );
	if (pfObjectFromLresult == NULL)
		return NULL;

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
	if (spLoc == NULL)
		return FALSE;

	BSTR bstrHost = NULL;
	spLoc->get_hostname (&bstrHost);
	if (bstrHost == NULL)
		return FALSE;

	USES_CONVERSION;

	if (vmsVideoSiteHtmlCodeParser::GetSupportedSiteIndex (W2A (bstrHost)) != -1)
		return TRUE;

	return FALSE;
}

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
        lRet = rk.QueryValue (szBuff, _T("Version"), &dwCount);
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
			char sz [1000] = "";
			GetMenuString (menu, cItems, sz, sizeof (sz), MF_BYPOSITION);

			if (lstrcmp (sz, szItem) == 0) 
			{
				BOOL bRemoveSeparator = GetMenuState (menu, cItems+1, MF_BYPOSITION) & 
						GetMenuState (menu, cItems-1, MF_BYPOSITION) & MF_SEPARATOR;

				if (_dwIeMajorVersion < 7)
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

	return 0;
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
			return hwnd2;

		HWND hwnd3 = find_ie_server (hwnd2);
		if (hwnd3 != NULL)
			return hwnd3;
	}
}

STDMETHODIMP CFDMIEBHO::SetSite(IUnknown *pSite)
{
	if (pSite == NULL)
		vmsIeTabsHookFitter::o ().RemoveTabHook (m_spWB);

	if (m_spWB != NULL)
	{
		Disconnect ();
		vmsComGIT::RevokeInterfaceFromGlobal (m_dwWbCookie);
		m_dwWbCookie = 0;
	}

	#ifndef DISABLE_OLD_MONITORING_HELPER
		if (pSite == NULL)
			DeleteFile ();
	#endif

	m_spWB = pSite;

	if (m_spWB == NULL)
		return pSite == NULL ? S_OK : E_INVALIDARG;

	vmsComGIT::RegisterInterfaceInGlobal (m_spWB, IID_IWebBrowser2, &m_dwWbCookie);

	_bUseUnicode = (GetVersion () & 0x80000000) == 0;
	DWORD dw;
	GetIEVersion (&_dwIeMajorVersion, &dw, &dw, &dw);

	vmsIeTabsHookFitter::o ().SetTabHook (m_spWB);

	m_spWB_CPC = m_spWB;
	if (m_spWB_CPC == NULL)
		return E_INVALIDARG;

	return Connect ();
}

STDMETHODIMP CFDMIEBHO::GetSite(REFIID riid, void **ppvSite)
{
	if (m_spWB == NULL || ppvSite == NULL)
		return E_INVALIDARG;
	
	*ppvSite = NULL;
	return m_spWB.QueryInterface (riid, (IWebBrowser2**)ppvSite);
}

STDMETHODIMP CFDMIEBHO::Connect()
{
	HRESULT hr;

	hr = Disconnect ();
	if (FAILED (hr))
		return hr;

	hr = m_spWB_CPC->FindConnectionPoint (DIID_DWebBrowserEvents2, &m_spWB_CPC_CP);
	if (FAILED (hr))
		return hr;

	IUnknown *pthis = (IUnknown*)(IFDMIEBHO*) this;

	hr = m_spWB_CPC_CP->Advise (pthis, &m_dwCookie);

	return hr;
}

STDMETHODIMP CFDMIEBHO::Disconnect()
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

BOOL CALLBACK FindFlashWindows_EnumProc (HWND hwnd, LPARAM lp)
{
	TCHAR tszClass [1000] = _T ("");
	GetClassName (hwnd, tszClass, 1000);

	if (_tcsicmp (tszClass, _T ("MacromediaFlashPlayerActiveX")) == 0)
	{
		vector <HWND> *pv = (vector <HWND>*)lp;
		pv->push_back (hwnd);
	}
	
	return TRUE;
}

void FindFlashWindows (HWND hwnd, vector <HWND> &v)
{
	EnumChildWindows (hwnd, FindFlashWindows_EnumProc, (LPARAM)&v);
}

LRESULT CALLBACK CFDMIEBHO::_FlashWindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int nWnd = vmsFlashWindowsSubclasser::o ().FindWindow (hwnd);
	if (nWnd == -1)
		return DefWindowProc (hwnd, msg, wp, lp);
	vmsFlashWindowsSubclasser::FlashWindow* wnd = vmsFlashWindowsSubclasser::o ().getWindow (nWnd);
	CFDMIEBHO *pthis = (CFDMIEBHO*)wnd->pvData;

	if (msg == WM_MOUSEMOVE)
	{
		if ((wp & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) == 0 && pthis->m_pGetItBtn == NULL)
		{
			wnd->ptMousePos.x = LOWORD (lp);
			wnd->ptMousePos.y = HIWORD (lp);
			SetTimer (hwnd, 0x123, 500, NULL);
		}
	}

	else if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN)
	{
		KillTimer (hwnd, 0x123);
	}

	else if (msg == WM_TIMER)
	{
		if (wp == 0x123)
		{
			KillTimer (hwnd, wp);
			POINT pt, ptScreen;
			GetCursorPos (&pt);
			ptScreen = pt;
			ScreenToClient (hwnd, &pt);
			if (wnd->ptMousePos.x == pt.x && wnd->ptMousePos.y == pt.y &&
					WindowFromPoint (ptScreen) == wnd->hwnd)
			{
				pthis->m_hwndGetItBtnParent = hwnd;
				pthis->ShowGetItButton (true);
			}
		}
	}

	return CallWindowProc (wnd->pfnOldProc, hwnd, msg, wp, lp);
}

STDMETHODIMP CFDMIEBHO::ProgressChange(long Progress, long ProgressMax)
{
#ifndef DISABLE_OLD_MONITORING_HELPER
	AnalyzePageAndWriteInfo ();
#endif

	QueryWindow ();
	SubclassFlashWindows ();	

	return S_OK;
}

STDMETHODIMP CFDMIEBHO::BeforeNavigate2(IDispatch *pdWB, VARIANT *url, VARIANT *flags, VARIANT *tfn, VARIANT *pd, VARIANT *headers, VARIANT_BOOL *bCancel)
{
#ifndef DISABLE_OLD_MONITORING_HELPER
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
		if (bstr)
		{
			SysFreeString (bstr);
			bstr = NULL;
		}
		
		if (bFound == false)
			continue;
		
		spForm->get_method (&bstr);
		
		if (bstr == NULL || wcsicmp (bstr, L"post"))
			break;
		if (bstr)
		{
			SysFreeString (bstr);
			bstr = NULL;
		}
		
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
	
	m_strUrl = ""; 
	AnalyzePageAndWriteInfo ();
#endif
	
	return S_OK;
}

#ifndef DISABLE_OLD_MONITORING_HELPER

void CFDMIEBHO::SaveInfoToDisk()
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

void CFDMIEBHO::WalkThroughForm(IHTMLElement *pElement, fsString& str)
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
			
			BSTR bstr = NULL, bstr2 = NULL;
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

void CFDMIEBHO::DeleteFile()
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

void CFDMIEBHO::AnalyzePageAndWriteInfo()
{
	USES_CONVERSION;
	
	IDispatchPtr spdDoc;
	m_spWB->get_Document (&spdDoc);
	if (spdDoc == NULL)
		return;
	
	IHTMLDocument2Ptr spDoc (spdDoc);
	if (spDoc == NULL)
		return;
	
	BSTR bstr = NULL;
	spDoc->get_cookie (&bstr);
	
	fsString strCookies, strUrl;
	
	if (bstr)
	{
		strCookies = W2A (bstr);
		SysFreeString (bstr);
		bstr = NULL;
	}
	
	spDoc->get_URL (&bstr);
	
	if (bstr)
	{
		strUrl = W2A (bstr);
		SysFreeString (bstr);
		bstr = NULL;
	}
	
	if (m_strUrl != strUrl || m_strCookies != strCookies || 
		m_strPostData != "")
	{
		m_strUrl = strUrl;
		m_strCookies = strCookies;
		SaveInfoToDisk ();
	}
}

#endif  

STDMETHODIMP CFDMIEBHO::DownloadBegin()
{
	InterlockedIncrement (&_cIEDownloads);
	return S_OK;
}

STDMETHODIMP CFDMIEBHO::DownloadComplete()
{
	InterlockedDecrement (&_cIEDownloads);
	return S_OK;
}

long CFDMIEBHO::get_IEDownloadsCount()
{
	return _cIEDownloads;
}

#include "vmsFlashHelper.h"
#include <mflash.h>

_COM_SMARTPTR_TYPEDEF(IShockwaveFlash, __uuidof(IShockwaveFlash));

STDMETHODIMP CFDMIEBHO::DocumentComplete(IDispatch *pDisp, VARIANT *URL)
{
	QueryWindow ();
	SubclassFlashWindows ();

	

	

	return S_OK;
}

STDMETHODIMP CFDMIEBHO::WindowStateChanged(DWORD dwFlags, DWORD dwValidFlagsMask)
{
	

	return S_OK;
}

STDMETHODIMP CFDMIEBHO::NavigateComplete2(IDispatch *pDisp, VARIANT *URL)
{
	

	IUnknownPtr spUnk1 (m_spWB);
	IUnknownPtr spUnk2 (pDisp);
	if (spUnk1 == spUnk2)
	{
		ATLASSERT (URL != NULL);
		if (URL != NULL)
		{
			ATLASSERT (URL->vt == VT_BSTR);
			if (URL->vt == VT_BSTR && URL->bstrVal != NULL && wcslen (URL->bstrVal) != 0)
				m_wstrPageUrl = URL->bstrVal;
		}
	}

	QueryWindow ();

	if (m_spHtmlEvents == NULL)
	{
		m_spHtmlEvents = CHtmlEventSink <CFDMIEBHO>::CreateHandler (this, OnHtmlEvent, 0);
		IDispatchPtr spDisp;
		m_spWB->get_Document (&spDisp);
		IHTMLDocument2Ptr spDoc (spDisp);
		AtlAdvise (spDoc, m_spHtmlEvents, __uuidof (HTMLDocumentEvents2), &m_dwCookieHtmlEvents);
	}

	return S_OK;
}

void CFDMIEBHO::OnHtmlEvent(DWORD dwSource, DISPID idEvent, VARIANT *pVarResult)
{
	IDispatchPtr spDisp;
	m_spWB->get_Document (&spDisp);
	IHTMLDocument2Ptr spDoc (spDisp); 

	if (spDoc == NULL)
		return;
	
	IHTMLWindow2Ptr spWin;
	IHTMLEventObjPtr spEvent;
	spDoc->get_parentWindow (&spWin);
	spWin->get_event (&spEvent);
	
	if (spEvent != NULL)
	{
		IHTMLElementPtr spElem;
		spEvent->get_srcElement (&spElem);
		if (spElem)
		{
			IShockwaveFlashPtr spFlash (spElem);
			if (spFlash != NULL)
			{
				CComBSTR bsType;
				spEvent->get_type (&bsType);

				if (_wcsicmp (bsType, L"mousemove") == 0)
				{
					

					
					
					m_ticksFlash_MouseLastMove = GetTickCount ();

					if (m_htFlash_MouseTrack == NULL && m_pGetItBtn == NULL)
					{
						
						DWORD dw;
						m_htFlash_MouseTrack = CreateThread (NULL, 0, _threadFlash_MouseTrack,
							this, 0, &dw); 
					}
				}

				else if (_wcsicmp (bsType, L"mouseout") == 0)
				{
					
					m_ticksFlash_MouseLastMove = 0;
					if (m_htFlash_MouseTrack)
					{
						TerminateThread (m_htFlash_MouseTrack, 0);
						CloseHandle (m_htFlash_MouseTrack);
						m_htFlash_MouseTrack = NULL;
					}
					ShowGetItButton (false);
				}

				
			}
		}
	}

	
}

DWORD WINAPI CFDMIEBHO::_threadFlash_MouseTrack(LPVOID lp)
{
	CFDMIEBHO *pthis = (CFDMIEBHO*) lp;

	CoInitialize (NULL);

	while (true)
	{
		if (pthis->m_pGetItBtn == NULL && 
				pthis->m_ticksFlash_MouseLastMove != 0 &&
				GetTickCount () - pthis->m_ticksFlash_MouseLastMove >= 1000)
		{
			
			
			pthis->m_hwndGetItBtnParent = pthis->m_hWndIeServer;
			pthis->ShowGetItButton (true);
		}

		Sleep (200);
		ATLTRACE ("_threadFlash_MouseTrack is running...\n");
	}

	CoUninitialize ();

	return 0;
}

DWORD WINAPI CFDMIEBHO::_threadFlash_GetItButton(LPVOID lp)
{
	CFDMIEBHO *pthis = (CFDMIEBHO*) lp;

	HRESULT hrCoInit = CoInitialize (NULL);

	HWND hwndIE = pthis->m_hWnd;

	if (pthis->m_pGetItBtn)
		delete pthis->m_pGetItBtn;
	pthis->m_pGetItBtn = new vmsGetItButton;
	USES_CONVERSION;
	pthis->m_pGetItBtn->m_strHtmlPageUrl = W2CA (pthis->m_wstrPageUrl.c_str ());
	pthis->m_pGetItBtn->Create (pthis->m_hwndGetItBtnParent);

	MSG msg; int nRes;
	while (IsWindow (pthis->m_pGetItBtn->getHWND ()))
	{
		nRes = GetMessage (&msg, NULL, 0, 0);
		if (nRes == 0 || nRes == -1)
			break;
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	std::string strMsg, strTitle;

	switch (pthis->m_pGetItBtn->getResult ())
	{
	case vmsGetItButton::E_SNIFF_MODULE_NOT_FOUND:
		strMsg = "Flash video monitoring module is not loaded. Make sure FDM is running and you've enabled this option in FDM (see Options | Downloads | FLV)";
		strTitle = "Error";
		break;

	case vmsGetItButton::E_FAILED:
		char sz [300];
		sprintf (sz, "An error occurred while trying to transfer downloads to FDM.\nError: 0x%x.", pthis->m_pGetItBtn->getHResult ());
		strMsg = sz;
		strTitle = "Error";
		break;

	case vmsGetItButton::E_NO_FLV_FOUND:
		strMsg = "There were no flash videos found on this page. Make sure the videos on this page are playing or try to reload it (refresh page or clear browser's cache).";
		strTitle = "Error";
		break;

	case vmsGetItButton::E_URL_NOT_FOUND:
		strMsg = "Failed found information about this web page. Please force your browser to reload it.";
		strTitle = "Error";
		break;		
	}

	vmsGetItButton* p = pthis->m_pGetItBtn;
	pthis->m_pGetItBtn = NULL;
	delete p;

	if (SUCCEEDED (hrCoInit))
		CoUninitialize ();
	
	if (strMsg.empty () == FALSE)
		MessageBox (hwndIE, strMsg.c_str (), strTitle.c_str (), MB_ICONERROR);

	return 0;
}

BOOL CALLBACK FindIeServerWindow_EnumProc (HWND hwnd, LPARAM lp)
{
	TCHAR tszClass [1000] = _T ("");
	GetClassName (hwnd, tszClass, 1000);

	if (_tcsicmp (tszClass, _T ("Internet Explorer_Server")) == 0)
	{
		HWND *phwndIeServer = (HWND*)lp;
		*phwndIeServer = hwnd;
		return FALSE;
	}
	
	return TRUE;
}

void FindIeServerWindow (HWND hwnd, HWND *phwndIeServer)
{
	EnumChildWindows (hwnd, FindIeServerWindow_EnumProc, (LPARAM)phwndIeServer);
}

void CFDMIEBHO::QueryWindow()
{
	if (m_hWnd == NULL)
	{
		IServiceProviderPtr spService (m_spWB);
		IOleWindowPtr spWindow;
		spService->QueryService (SID_SShellBrowser, IID_IOleWindow, (void**)&spWindow);
		if (spWindow != NULL)
			spWindow->GetWindow (&m_hWnd);
	}

	if (m_hWndIeServer == NULL && m_hWnd != NULL)
		FindIeServerWindow (m_hWnd, &m_hWndIeServer);
}

void CFDMIEBHO::ShowGetItButton(bool bShow)
{
	if (bShow)
	{
		if (m_pGetItBtn == NULL)
		{
			CRegKey key;
			if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, 
					"Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\FlvMonitoring", 
					KEY_READ))
			{
				DWORD dw = TRUE;
				key.QueryValue (dw, "ShowGetItBtn");
				if (!dw)
					return;
			}
			
			RetrieveFlashInfo ();
			DWORD dw;
			m_htFlash_GetItButton = CreateThread (NULL, 0, _threadFlash_GetItButton, this, 0, &dw);
		}
	}
	else
	{
		if (m_htFlash_GetItButton && m_pGetItBtn && IsWindow (m_pGetItBtn->getHWND ()))
		{
			TerminateThread (m_htFlash_GetItButton, 0);
			CloseHandle (m_htFlash_GetItButton);
			m_htFlash_GetItButton = NULL;
			if (m_pGetItBtn)
			{
				delete m_pGetItBtn;
				m_pGetItBtn = NULL;
			}
		}
	}
}

void CFDMIEBHO::RetrieveFlashInfo()
{
	POINT pt; GetCursorPos (&pt);
	ScreenToClient (m_hWndIeServer, &pt);
	
	IWebBrowserPtr spWB;
	vmsComGIT::GetInterfaceFromGlobal (m_dwWbCookie, IID_IWebBrowser2, (void**)&spWB);
	if (spWB == NULL)
		return;
	
	IDispatchPtr spDocDisp;
	spWB->get_Document (&spDocDisp);

	IHTMLDocument2Ptr spDoc (spDocDisp);

	
	
	
	
	IHTMLElementPtr spElement;
	spDoc->elementFromPoint (pt.x, pt.y, &spElement);
	
	if (spElement == NULL)
		return;
	
	IShockwaveFlashPtr spFlash (spElement);
	
	if (spFlash == NULL)
		return;

	CComBSTR bstr;
	spFlash->get_Movie (&bstr);

	m_wstrFlashMovie = bstr;
}

void CFDMIEBHO::SubclassFlashWindows()
{
	if (m_hWnd != NULL)
	{
		vector <HWND> vFlashWnds;
		FindFlashWindows (m_hWnd, vFlashWnds);
		for (size_t i = 0; i < vFlashWnds.size (); i++)
			vmsFlashWindowsSubclasser::o ().Subclass (vFlashWnds [i], _FlashWindowProc, this);
	}
}

STDMETHODIMP CFDMIEBHO::StatusTextChange(BSTR bstrText)
{
	QueryWindow ();
	SubclassFlashWindows ();
	return S_OK;
}
