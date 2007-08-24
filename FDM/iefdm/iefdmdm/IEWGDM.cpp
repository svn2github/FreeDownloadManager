/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Iefdmdm.h"
#include "IEWGDM.h"
#include "../common.cpp"
#include <shlguid.h>
#include "vmsIETmpCookies.h"
#include <fsString.h>        

STDMETHODIMP CIEWGDM::Download(IMoniker *pmk, IBindCtx *pbc, DWORD dwBindVerb, LONG grfBINDF, BINDINFO *pBindInfo, LPCOLESTR pszHeaders, LPCOLESTR pszRedir, UINT uiCP)
{
	USES_CONVERSION;
	LPOLESTR pwszUrl;
	LPMALLOC pMalloc;
	HRESULT hr;

	if (fsIsMonitoringTurnedOn () == FALSE)
		return E_FAIL;

	if (FAILED (hr = pmk->GetDisplayName (pbc, NULL, &pwszUrl)))
		return hr;

	if (FAILED (hr = CoGetMalloc (1, &pMalloc)))
		return hr;

	

	hr = E_FAIL;

	fsString strUrl = W2A (pwszUrl);

	if (fsOnNavigateUrl (strUrl))
	{
		vmsIETmpCookies iec;
		int nIndex = iec.Find (strUrl);
		if (fsUrlToFdm (strUrl, 
				nIndex == -1 ? NULL : iec.get_Referer (nIndex), 
				nIndex == -1 ? NULL : iec.get_Cookies (nIndex),
				nIndex == -1 ? NULL : iec.get_PostData (nIndex), TRUE))
			hr = S_OK;
	}

	pMalloc->Free (pwszUrl);
	pMalloc->Release ();

	return hr;
}
