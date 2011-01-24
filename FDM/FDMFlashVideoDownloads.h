/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/    

#ifndef __FDMFLASHVIDEODOWNLOADS_H_
#define __FDMFLASHVIDEODOWNLOADS_H_

#include "resource.h"           

class ATL_NO_VTABLE CFDMFlashVideoDownloads : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFDMFlashVideoDownloads, &CLSID_FDMFlashVideoDownloads>,
	public IDispatchImpl<IFDMFlashVideoDownloads, &IID_IFDMFlashVideoDownloads, &LIBID_FdmLib>
{
public:
	CFDMFlashVideoDownloads()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FDMFLASHVIDEODOWNLOADS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFDMFlashVideoDownloads)
	COM_INTERFACE_ENTRY(IFDMFlashVideoDownloads)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()  

public:
	STDMETHOD(CreateFromUrl)( BSTR bstrUrl);
	STDMETHOD(ProcessHtml)( BSTR bstrHost,  BSTR bstrHtml);
	STDMETHOD(ProcessIeDocument)( IDispatch *pDispatch);
protected:
	static DWORD WINAPI _threadCreateDownload (LPVOID lp);
	void ProcessHtml (LPCSTR pszHost, LPCSTR pszHtml);
};

#endif 
