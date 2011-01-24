/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSBINDSTATUSCALLBACK_H__CC76B3A9_8772_4121_ABA6_BF1213528E52__INCLUDED_)
#define AFX_VMSBINDSTATUSCALLBACK_H__CC76B3A9_8772_4121_ABA6_BF1213528E52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <comdef.h>
#include <shlguid.h>
#include <wininet.h>
#include <fsString.h>

class vmsBindStatusCallback : public IBindStatusCallback,
	public IHttpNegotiate, public IServiceProvider
{
public:

	

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject)
	{
		*ppvObject = NULL;
		
		if (IsEqualGUID (riid, IID_IUnknown))
			*ppvObject = static_cast <IBindStatusCallback*> (this);

		else if (IsEqualGUID (riid, IID_IServiceProvider))
			*ppvObject = static_cast <IServiceProvider*> (this);
		
		else if (IsEqualGUID (riid, IID_IBindStatusCallback))
			*ppvObject = static_cast<IBindStatusCallback*>(this);

		else if (IsEqualGUID (riid, IID_IHttpNegotiate))
			*ppvObject = static_cast<IHttpNegotiate*>(this);

		else 
			return E_NOINTERFACE;
		
		AddRef ();
		return S_OK;	
	}

	DWORD __stdcall AddRef()
	{
		return InterlockedIncrement (&m_cRef);
	}
	
	DWORD __stdcall Release()
	{
		return InterlockedDecrement (&m_cRef);
	}

protected:
	LONG m_cRef;

public:
	
	

	STDMETHOD (QueryService) (REFGUID guidService, REFIID riid, void **ppvObject)
	{
		return QueryInterface (riid, ppvObject);
	}

public:

	

	STDMETHOD (GetBindInfo) (DWORD *grfBINDF, BINDINFO *pbindinfo)
	{
		*grfBINDF = m_grfBINDF;

		*grfBINDF |= BINDF_ASYNCHRONOUS | BINDF_RESYNCHRONIZE | 
			BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE | BINDF_PRAGMA_NO_CACHE;

		pbindinfo->dwBindVerb = m_dwBindVerb;

		

		return S_OK;
	}

	STDMETHOD (GetPriority) (LONG *pnPriority)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnDataAvailable) (DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnLowResource) (DWORD reserved)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnObjectAvailable) (REFIID riid, IUnknown *punk)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnProgress) (ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnStartBinding) (DWORD dwReserved, IBinding *pib)
	{
		if (pib == NULL)
			return E_INVALIDARG;

		IWinInetHttpInfoPtr spHttpInfo (pib);
		if (spHttpInfo == NULL)
			return E_INVALIDARG;

		char sz [30000] = "";
		DWORD dw = sizeof (sz);
		DWORD dwFlags = 0;

		HRESULT hr = spHttpInfo->QueryInfo (
			HTTP_QUERY_RAW_HEADERS_CRLF | HTTP_QUERY_FLAG_REQUEST_HEADERS, sz, &dw, &dwFlags, NULL);

		LPDWORD pdw = (LPDWORD)sz;
		dw = *pdw;

		if (S_OK == hr)
		{
			m_strCookies = "";
			m_strReferer = "";

			LPCSTR pszReferer = strstr (sz, "Referer: ");		
			if (pszReferer)
			{
				pszReferer += strlen ("Referer: ");
				while (*pszReferer != 0 && *pszReferer != '\r' && *pszReferer != '\n')
					m_strReferer += *pszReferer++;
			}

			LPCSTR pszCookie = strstr (sz, "Cookie: ");		
			if (pszCookie)
			{
				pszCookie += strlen ("Cookie: ");
				while (*pszCookie != 0 && *pszCookie != '\r' && *pszCookie != '\n')
					m_strCookies += *pszCookie++;
			}

			ATLASSERT (m_strCookies.IsEmpty () == FALSE);
				
		}
		
		return E_FAIL;
	}

	STDMETHOD (OnStopBinding) (HRESULT hresult, LPCWSTR szError)
	{
		return E_NOTIMPL;
	}  

	

	STDMETHOD (BeginningTransaction) (LPCWSTR szURL, LPCWSTR szHeaders, DWORD dwReserved, LPWSTR *pszAdditionalHeaders)
	{
		return E_NOTIMPL;
	}

	STDMETHOD (OnResponse) (DWORD dwResponseCode, LPCWSTR szResponseHeaders, LPCWSTR szRequestHeaders, LPWSTR *pszAdditionalRequestHeaders)
	{
		return E_NOTIMPL;
	}

public:
	fsString m_strCookies, m_strReferer;
	BINDINFO* m_pBindInfo;
	DWORD m_grfBINDF;
	DWORD m_dwBindVerb;
	vmsBindStatusCallback();
	virtual ~vmsBindStatusCallback();

};

#endif 
