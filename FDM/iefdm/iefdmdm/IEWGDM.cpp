/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Iefdmdm.h"
#include "IEWGDM.h"
#include "../common.cpp"
#include <shlguid.h>
#include "vmsIETmpCookies.h"
#include <fsString.h>
#include "vmsBindStatusCallback.h"      

STDMETHODIMP CIEWGDM::Download(IMoniker *pmk, IBindCtx *pbc, 
	DWORD dwBindVerb, LONG grfBINDF, BINDINFO *pBindInfo, 
	LPCOLESTR pszHeaders, LPCOLESTR pszRedir, UINT uiCP)
{
	if (fsIsMonitoringTurnedOn () == FALSE)
		return E_FAIL;

	HRESULT hr;
	USES_CONVERSION;
	LPOLESTR pwszUrl;
	LPMALLOC pMalloc;
	
	if (FAILED (hr = pmk->GetDisplayName (pbc, NULL, &pwszUrl)))
		return hr;
	
	if (FAILED (hr = CoGetMalloc (1, &pMalloc)))
		return hr;
		
	fsString strUrl = W2A (pwszUrl);

	if (!fsOnNavigateUrl (strUrl))
		return E_FAIL;

	vmsBindStatusCallback *pBSC = new vmsBindStatusCallback;
	pBSC->m_pBindInfo = pBindInfo;
	pBSC->m_grfBINDF = grfBINDF;
	pBSC->m_dwBindVerb = dwBindVerb;

	fsString strPostData;
	if (pBindInfo && pBindInfo->dwBindVerb == BINDVERB_POST)
	{
		if (pBindInfo->stgmedData.tymed == TYMED_HGLOBAL)
		{
			DWORD dwSize = GlobalSize (pBindInfo->stgmedData.hGlobal);
			if (dwSize < 30000)
			{
				LPVOID pData = GlobalLock (pBindInfo->stgmedData.hGlobal);
				for (DWORD i = 0; i < dwSize; i++)
					strPostData += ((char*)pData) [i];
				GlobalUnlock (pBindInfo->stgmedData.hGlobal);
			}
		}
	}

	IBindStatusCallbackPtr spPrevCallback;
	hr = RegisterBindStatusCallback (pbc, pBSC, &spPrevCallback, 0);
	if (FAILED (hr))
		return hr;

	IStreamPtr spStream;

	LPOLESTR pwszUrl2;
	if (FAILED (hr = pmk->GetDisplayName (pbc, NULL, &pwszUrl2)))
		return hr;

	hr = pmk->BindToStorage (pbc, NULL, IID_IStream, (void**)&spStream);
	

	hr = E_FAIL;
	
	if (fsUrlToFdm (strUrl, pBSC->m_strReferer,  pBSC->m_strCookies, strPostData, TRUE))
		hr = S_OK;
	
	pMalloc->Free (pwszUrl);
	pMalloc->Release ();
	
	return hr;
	

	
}
