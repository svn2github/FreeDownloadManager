/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFindFlvDownloadsResultsCache.h"
#include "vmsDownloadForTrafficCollector.h"
#include "vmsHttpFlvTrafficAnalyzer5.h"

vmsFindFlvDownloadsResultsCache::vmsFindFlvDownloadsResultsCache(vmsHttpTrafficCollector *pHttpTraffic, vmsWinSockHttpDlgTree *pDlgTree)
{
	assert (pHttpTraffic != NULL && pDlgTree != NULL);
	m_pHttpTraffic = pHttpTraffic;
	m_pDlgTree = pDlgTree;
	m_cFlvsInSession = 0;
}

vmsFindFlvDownloadsResultsCache::~vmsFindFlvDownloadsResultsCache()
{

}

HRESULT vmsFindFlvDownloadsResultsCache::FindFlvDownloads(LPCSTR pszUrl, LPCSTR pszFrameUrl, LPCSTR pszSwfUrl, LPCSTR pszFlashVars, LPCSTR pszOtherSwfUrls, LPCSTR pszOtherFlashVars, ResultPtr &result)
{
	LOGFN ("vmsFindFlvDownloadsResultsCache::FindFlvDownloads");

	vmsCriticalSectionAutoLock _csal (&m_csResults);

#ifdef LOG_WEBFILES_TREE
	extern LONG _cInOnGetItBtnClicked;
	static DWORD _dwLastTicks = 0;
	if ((GetKeyState (VK_SHIFT) & GetKeyState (VK_CONTROL) & 0x8000) &&
		(GetTickCount () - _dwLastTicks > 10000))
	{
		m_vResults.clear ();
		_dwLastTicks = GetTickCount ();
	}
#endif

	if (m_cFlvsInSession != m_pDlgTree->getFlvsInSessionCount ())
	{
		m_cFlvsInSession = m_pDlgTree->getFlvsInSessionCount ();
		m_vResults.clear ();
	}

	result = NULL;

	if (!pszFrameUrl)
		pszFrameUrl = "";
	if (!pszSwfUrl)
		pszSwfUrl = "";
	if (!pszFlashVars)
		pszFlashVars = "";

#ifdef LOG_WEBFILES_TREE
	if (_cInOnGetItBtnClicked)
		goto _lSkipCacheSearch;
#endif

	size_t i;
	for (i = 0; i < m_vResults.size (); i++)
	{
		Result *res = m_vResults [i];
		if (res->strWebPageUrl == pszUrl && res->strFrameUrl == pszFrameUrl &&
				res->strSwfUrl == pszSwfUrl && res->strFlashVars == pszFlashVars)
		{
			result = res;
			return result->pTa->get_FlvDownloadCount () ? S_OK : S_FALSE;
		}
	}

#ifdef LOG_WEBFILES_TREE
_lSkipCacheSearch:
	extern void log_webfiles_init2 ();
	log_webfiles_init2 ();
#endif

	if (!m_pDlgTree->FindTreeItem (pszUrl, vmsHttpTrafficCollector::HttpDialog::HTML))
		vmsDownloadForTrafficCollector::MakeDownload (pszUrl);

	if (pszFrameUrl != NULL && strcmp (pszUrl, pszFrameUrl) == 0)
		pszFrameUrl = NULL;

	vector <string> vOtherSwfs, vOtherFlashVars;
	if (pszOtherSwfUrls && pszOtherFlashVars)
	{
		ExtractUrls (pszOtherSwfUrls, vOtherSwfs);
		ExtractUrls (pszOtherFlashVars, vOtherFlashVars);
		assert (vOtherSwfs.size () == vOtherFlashVars.size ());
		if (vOtherSwfs.size () != vOtherFlashVars.size ())
		{
			vOtherSwfs.clear ();
			vOtherFlashVars.clear ();
		}
	}

	m_pHttpTraffic->LockRemoveOldDialogs (true);
	vmsWinSockHttpDlgTree::o ().LockRemoveOldDialogs (true);

	vmsHttpFlvTrafficAnalyzer *pTA = NULL;
	HRESULT hr;

	vmsHttpFlvTrafficAnalyzer5 ta5;
	if (!pTA)
	{
		hr = ta5.FindFlvDownloads (pszUrl, pszFrameUrl, pszSwfUrl, pszFlashVars, 
			vOtherSwfs, vOtherFlashVars, &vmsWinSockHttpDlgTree::o ());
		if (ta5.get_FlvDownloadCount ())
			pTA = &ta5;
	}

	
	
	vmsWinSockHttpDlgTree::o ().LockRemoveOldDialogs (false);
	m_pHttpTraffic->LockRemoveOldDialogs (false);

	if (FAILED (hr))
		return hr;

	result.CreateInstance ();

	result->strWebPageUrl = pszUrl;

	result->strFrameUrl = pszFrameUrl;

	if (pszSwfUrl)
		result->strSwfUrl = pszSwfUrl;

	if (pszFlashVars)
		result->strFlashVars = pszFlashVars;

	result->pTa = new vmsHttpFlvTrafficAnalyzer;

	if (pTA)	
		result->pTa->AddFlvDownloads (*pTA);

	m_vResults.push_back (result);

	return result->pTa->get_FlvDownloadCount () ? S_OK : S_FALSE;
}

void vmsFindFlvDownloadsResultsCache::ExtractUrls(LPCSTR pszUrls, vector <string> &vResult)
{
	

	vResult.clear ();
	if (pszUrls == NULL || *pszUrls == 0)
		return;
	while (pszUrls && *pszUrls)
	{
		string strUrl;
		LPCSTR psz = strchr (pszUrls, '\n');
		if (psz)
			strUrl.assign (pszUrls, psz-pszUrls);
		else
			strUrl = pszUrls;
		pszUrls = psz;
		if (pszUrls)
			pszUrls++; 
		vResult.push_back (strUrl);
	}
}
