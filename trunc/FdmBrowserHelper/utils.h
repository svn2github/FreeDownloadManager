/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <comutil.h>
#include <comdef.h>
#include "../fdm.h"

#ifndef IWGUrlReceiverPtr
	_COM_SMARTPTR_TYPEDEF(IWGUrlReceiver, __uuidof(IWGUrlReceiver));
#endif

#ifndef IWGUrlListReceiverPtr
	_COM_SMARTPTR_TYPEDEF (IWGUrlListReceiver, __uuidof(IWGUrlListReceiver));
#endif

#ifndef IFdmUiWindowPtr
	_COM_SMARTPTR_TYPEDEF (IFdmUiWindow, __uuidof(IFdmUiWindow));
#endif

#ifndef IFDMFlashVideoDownloadsPtr
	_COM_SMARTPTR_TYPEDEF (IFDMFlashVideoDownloads, __uuidof(IFDMFlashVideoDownloads));
#endif

template <class TWgRcvr>
inline HRESULT vmsFdmBhDownloadInfoToWgRcvr (
	const vmsFdmBhIpcDownloadInfo *dlinfo,
	TWgRcvr *rcvr)
{
	assert (dlinfo);
	assert (rcvr);

	HRESULT hr;

	if (FAILED (hr = rcvr->put_Url (_bstr_t (dlinfo->m_url.c_str ()))))
		return hr;

	if (FAILED (hr = rcvr->put_Referer (_bstr_t (dlinfo->m_httpReferer.c_str ()))))
		return hr;

	if (FAILED (hr = rcvr->put_Cookies (_bstr_t (dlinfo->m_httpCookies.c_str ()))))
		return hr;

	return hr;
}

inline HRESULT vmsFdmBhDownloadInfoToWgUrlRcvr (
	const vmsFdmBhIpcDownloadInfo *dlinfo,
	IWGUrlReceiver *rcvr)
{
	HRESULT hr = vmsFdmBhDownloadInfoToWgRcvr (dlinfo, rcvr);
	if (SUCCEEDED (hr))
	{
		if (FAILED (hr = rcvr->put_OriginalURL (_bstr_t (dlinfo->m_originalUrl.c_str ()))))
			return hr;

		if (FAILED (hr = rcvr->put_FileName (_bstr_t (dlinfo->m_suggestedName.c_str ()))))
			return hr;

		if (FAILED (hr = rcvr->put_UserAgent (_bstr_t (dlinfo->m_userAgent.c_str ()))))
			return hr;

		if (FAILED (hr = rcvr->put_PostData (_bstr_t (dlinfo->m_httpPostData.c_str ()))))
			return hr;
	}
	return hr;
}

inline HRESULT vmsFdmShowAddDownloadDialog (
	const vmsFdmBhIpcDownloadInfo *dlinfo,
	bool catchedDownload, 
	bool waitResponse)
{
	IWGUrlReceiverPtr rcvr;
	HRESULT hr = rcvr.CreateInstance (__uuidof (WGUrlReceiver));
	assert (rcvr);
	if (!rcvr)
		return hr;

	hr = vmsFdmBhDownloadInfoToWgUrlRcvr (dlinfo, rcvr);
	if (FAILED (hr))
		return hr;

	if (catchedDownload)
		rcvr->put_IgnoreSize (FALSE);

	hr = rcvr->ShowAddDownloadDialog ();
	if (FAILED (hr))
		return hr;

	if (!waitResponse)
		return hr;

	IServiceProviderPtr servProv (rcvr);
	assert (servProv);
	IFdmUiWindowPtr uiWindow;
	if (servProv)
		servProv->QueryService (__uuidof (IFdmUiWindow), __uuidof (IFdmUiWindow), (void**)&uiWindow);
	assert (uiWindow);
	if (uiWindow)
	{
		uiWindow->WaitForWindowCreation (5000);
		ULONG u = 0;
		uiWindow->get_Handle (&u);
		if (u)
		{
			SetForegroundWindow ((HWND)u);
			BringWindowToTop ((HWND)u);
		}
	}

	BSTR bstrState = nullptr;
	do 
	{
		if (bstrState)
		{
			SysFreeString (bstrState);
			bstrState = nullptr;
		}
		Sleep (5);
		rcvr->get_UIState (&bstrState);
		assert (bstrState);
		if (!bstrState)
			return E_UNEXPECTED;
	} 
	while (_wcsicmp (bstrState, L"in_progress") == 0);

	bool added = !_wcsicmp (bstrState, L"added");
	SysFreeString (bstrState);

	return added ? S_OK : S_FALSE;
}

inline HRESULT vmsFdmBhDownloadInfoToWgUrlListRcvr (
	const vmsFdmBhIpcDownloadInfo *dlinfo,
	IWGUrlListReceiver *rcvr)
{
	HRESULT hr = vmsFdmBhDownloadInfoToWgRcvr (dlinfo, rcvr);
	if (SUCCEEDED (hr))
		hr = rcvr->AddUrlToList ();
	return hr;
}

inline HRESULT vmsFdmShowAddUrlListDialog (
	const std::vector <std::unique_ptr <vmsFdmBhIpcDownloadInfo>> &downloads)
{
	IWGUrlListReceiverPtr rcvr;
	HRESULT hr = rcvr.CreateInstance (__uuidof (WGUrlListReceiver));
	assert (rcvr);
	if (!rcvr)
		return hr;

	for (const auto &dl : downloads)
	{
		hr = vmsFdmBhDownloadInfoToWgUrlListRcvr (dl.get(), rcvr);
		if (FAILED (hr))
			return hr;
	}

	return rcvr->ShowAddUrlListDialog ();
}

inline HRESULT vmsFdmBhCreateFlashVideoDownload (
	const vmsFdmBhIpcDownloadInfo *dlinfo)
{
	IFDMFlashVideoDownloadsPtr fvDownloads;
	HRESULT hr = fvDownloads.CreateInstance (__uuidof (FDMFlashVideoDownloads));
	assert (fvDownloads != nullptr);
	if (!fvDownloads)
		return hr;

	hr = fvDownloads->CreateFromUrl (
		_bstr_t (dlinfo->m_url.c_str ()));

	return hr;
}