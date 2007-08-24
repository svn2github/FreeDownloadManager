/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef __FDMIECOOKIESBHO_H_
#define __FDMIECOOKIESBHO_H_

#include "resource.h"       
#include <exdisp.h>
#include <comdef.h>
#include <fsString.h>    

class ATL_NO_VTABLE CFDMIECookiesBHO : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFDMIECookiesBHO, &CLSID_FDMIECookiesBHO>,
	public IObjectWithSiteImpl<CFDMIECookiesBHO>,
	public IDispatchImpl<IFDMIECookiesBHO, &IID_IFDMIECookiesBHO, &LIBID_IECOOKSLib>
{
public:
	CFDMIECookiesBHO()
	{
		m_hFile = INVALID_HANDLE_VALUE;
		m_mxFile = CreateMutex (NULL, FALSE, "vmsMX::fdmietmpcookiesfileops");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FDMIECOOKIESBHO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFDMIECookiesBHO)
	COM_INTERFACE_ENTRY(IFDMIECookiesBHO)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()  

public:
	static long get_IEDownloadsCount();
	STDMETHOD(DownloadComplete)();
	STDMETHOD(DownloadBegin)();
	STDMETHOD(BeforeNavigate2)(IDispatch*, VARIANT* url, VARIANT* flags, VARIANT* tfn, VARIANT* pd, VARIANT* headers, VARIANT_BOOL* bCancel);
	STDMETHOD(ProgressChange)(long Progress, long ProgressMax);
	STDMETHOD(GetSite)(REFIID riid, void **ppvSite);
	STDMETHOD(SetSite )(IUnknown *pSite);
protected:
	void DeleteFile();
	static long _cIEDownloads;
	void WalkThroughForm (IHTMLElement* pElement, fsString& str);
	HANDLE m_mxFile;
	void SaveInfoToDisk();
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)();

	IConnectionPointContainerPtr m_spWB_CPC;
	IWebBrowser2Ptr m_spWB;
	DWORD m_dwCookie;
	IConnectionPointPtr m_spWB_CPC_CP;
	fsString m_strCookies;
	fsString m_strUrl;
	fsString m_strPostData;
	fsString m_strBeforeNavUrl;
	fsString m_strTmpFile;	
	HANDLE m_hFile;
};

#endif 
