/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/    

#ifndef __FDMIEBHO_H_
#define __FDMIEBHO_H_

#include "resource.h"       
#include <exdisp.h>
#include <comdef.h>
#include <fsString.h>
#include "vmsGetItButton.h"
#include <string>

#define DISABLE_OLD_MONITORING_HELPER    

class ATL_NO_VTABLE CFDMIEBHO : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFDMIEBHO, &CLSID_FDMIEBHO>,
	public IObjectWithSiteImpl<CFDMIEBHO>,
	public IDispatchImpl<IFDMIEBHO, &IID_IFDMIEBHO, &LIBID_IEFDMBHOLib>
{
public:
	CFDMIEBHO()
	{
		#ifndef DISABLE_OLD_MONITORING_HELPER
			m_hFile = INVALID_HANDLE_VALUE;		
			m_mxFile = CreateMutex (NULL, FALSE, "vmsMX::fdmietmpcookiesfileops");
		#endif
		m_htFlash_MouseTrack = NULL;
		m_ticksFlash_MouseLastMove = 0;
		m_hWnd = NULL; m_hWndIeServer = NULL; m_hwndGetItBtnParent = NULL;
		m_pGetItBtn = NULL;
		m_htFlash_GetItButton = NULL;
		m_dwWbCookie = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FDMIEBHO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFDMIEBHO)
	COM_INTERFACE_ENTRY(IFDMIEBHO)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()  

public:
	STDMETHOD(StatusTextChange)( BSTR bstrText);
	STDMETHOD(NavigateComplete2)( IDispatch *pDisp,  VARIANT *URL);
	STDMETHOD(WindowStateChanged)(DWORD dwFlags, DWORD dwValidFlagsMask);
	STDMETHOD(DocumentComplete)( IDispatch *pDisp,  VARIANT *URL);
	static long get_IEDownloadsCount();
	STDMETHOD(DownloadComplete)();
	STDMETHOD(DownloadBegin)();
	STDMETHOD(BeforeNavigate2)(IDispatch*, VARIANT* url, VARIANT* flags, VARIANT* tfn, VARIANT* pd, VARIANT* headers, VARIANT_BOOL* bCancel);
	STDMETHOD(ProgressChange)(long Progress, long ProgressMax);
	STDMETHOD(GetSite)(REFIID riid, void **ppvSite);
	STDMETHOD(SetSite )(IUnknown *pSite);
protected:
	void SubclassFlashWindows ();
	std::wstring m_wstrFlashMovie;
	std::wstring m_wstrPageUrl;
	void RetrieveFlashInfo();
	DWORD m_dwWbCookie;
	HANDLE m_htFlash_GetItButton;
	void ShowGetItButton (bool bShow);
	vmsGetItButton* m_pGetItBtn;
	HWND m_hwndGetItBtnParent;
	void QueryWindow();
	HWND m_hWnd, m_hWndIeServer; 
	static DWORD WINAPI _threadFlash_GetItButton (LPVOID lp);
	static DWORD WINAPI _threadFlash_MouseTrack (LPVOID lp);
	static LRESULT CALLBACK _FlashWindowProc (HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	HANDLE m_htFlash_MouseTrack;
	DWORD m_ticksFlash_MouseLastMove;
	DWORD m_dwCookieHtmlEvents;
	void OnHtmlEvent (DWORD dwSource, DISPID idEvent, VARIANT *pVarResult);
	IDispatchPtr m_spHtmlEvents;
#ifndef DISABLE_OLD_MONITORING_HELPER
	void AnalyzePageAndWriteInfo();
	void DeleteFile();
	void WalkThroughForm (IHTMLElement* pElement, fsString& str);
	HANDLE m_mxFile;
	void SaveInfoToDisk();
	fsString m_strCookies;
	fsString m_strUrl;
	fsString m_strPostData;
	fsString m_strBeforeNavUrl;
	fsString m_strTmpFile;	
	HANDLE m_hFile;
#endif
	static long _cIEDownloads;
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)();

	IConnectionPointContainerPtr m_spWB_CPC;
	IWebBrowser2Ptr m_spWB;
	DWORD m_dwCookie;
	IConnectionPointPtr m_spWB_CPC_CP;
};

#endif 
