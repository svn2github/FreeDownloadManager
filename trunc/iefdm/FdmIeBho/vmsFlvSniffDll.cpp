/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "StdAfx.h"

#include "vmsFlvSniffDll.h"
#include <string>
#include "../../common/vmsFdmTranslations.h"

vmsFlvSniffDll::vmsFlvSniffDll()
{
	m_enBR = ButtonResult::NONE;
	m_hrLast = S_OK;
}

vmsFlvSniffDll::~vmsFlvSniffDll()
{

}

void vmsFlvSniffDll::OnDownloadItBtnClicked (const char* pszWebPageUrl, LPCSTR pszFrameUrl, 
											 LPCSTR pszSwfUrl, LPCSTR pszFlashVars,
											 LPCSTR pszOtherSwfUrls, LPCSTR pszOtherFlashVars)
{
	HMODULE hDll = GetModuleHandle (_T("flvsniff.dll"));
	if (!hDll)
	{
		m_enBR = ButtonResult::E_SNIFF_MODULE_NOT_FOUND;
		return;
	}
	typedef HRESULT (WINAPI *FNBTNCLCKD)(LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR);
	FNBTNCLCKD pfn = (FNBTNCLCKD) GetProcAddress (hDll, "onGetItBtnClicked3");
	if (pfn != NULL)
	{
		HRESULT hr = pfn (pszWebPageUrl, pszFrameUrl, pszSwfUrl, pszFlashVars, pszOtherSwfUrls, pszOtherFlashVars);
		if (FAILED (hr))
		{
			if (hr == E_INVALIDARG)
				m_enBR = ButtonResult::E_URL_NOT_FOUND;
			else
				m_enBR = ButtonResult::E_FAILED;
			m_hrLast = hr;
		}
		else if (hr == S_FALSE)
		{
			m_enBR = ButtonResult::E_NO_FLV_FOUND;
		}
	}
}

void vmsFlvSniffDll::OnDownloadItBtnClicked_ShowMsgIfReq(HWND hwndParent) const
{
	tstring strMsg;

	switch (m_enBR)
	{
	case E_SNIFF_MODULE_NOT_FOUND:
		strMsg = vmsFdmTranslations::o ().GetString (L_FLVSNIFFMODULE_NOTFOUND);
		if (strMsg.empty ())
			strMsg = _T("Flash video monitoring module is not loaded. Make sure FDM is running and you've enabled this option in FDM (see Options | Downloads | Flash Video).");
		break;
		
	case E_FAILED:
		strMsg = vmsFdmTranslations::o ().GetString (L_FAILEDTRANSFERDLDSTOFDM);
		if (strMsg.empty ())
			strMsg = _T("An error occurred while trying to transfer downloads to FDM.\nError: 0x%x.");
		TCHAR sz [300]; *sz = 0;
		_stprintf_s (sz, 300, strMsg.c_str (), m_hrLast);
		strMsg = sz;
		break;
		
	case E_NO_FLV_FOUND:
		strMsg = vmsFdmTranslations::o ().GetString (L_NOFLVSFOUND);
		if (strMsg.empty ())
			strMsg = _T("There were no flash videos found on this page. Make sure the videos on this page are playing or try to reload it (refresh page or clear browser's cache).");
		break;
		
	case E_URL_NOT_FOUND:
		strMsg = vmsFdmTranslations::o ().GetString (L_FLVWEBPAGENOTFOUND);
		if (strMsg.empty ())
			strMsg = _T("Failed to found information about this web page. Please force your browser to reload it.");
		break;		
	}

	if (!strMsg.empty ())
		MessageBox (hwndParent, strMsg.c_str (), NULL, MB_ICONERROR);
}

BOOL vmsFlvSniffDll::IsVideoFlash (const char* pszWebPageUrl, const char* pszFrameUrl, const char* pszSwfUrl, const char* pszFlashVars, const char* pszOtherSwfUrls, const char* pszOtherFlashVars)
{
	HMODULE hDll = GetModuleHandle (_T("flvsniff.dll"));
	if (!hDll)
		return FALSE;
	typedef BOOL (WINAPI *FNIVF)(LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR);
	FNIVF pfn = (FNIVF) GetProcAddress (hDll, "isVideoFlash");
	if (pfn != NULL)
		return pfn (pszWebPageUrl, pszFrameUrl, pszSwfUrl, pszFlashVars, pszOtherSwfUrls, pszOtherFlashVars);
	return FALSE;
}
