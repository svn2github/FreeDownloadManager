/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "Data Stretcher.h"
#include "WGUrlReceiver.h"
#include "CreateDownloadDlg.h"
#include "MainFrm.h"
#include "UIThread.h"
#include "inetutil.h"    

#include "DownloadsWnd.h"
#include "SpiderWnd.h"
#include "SpiderAddPageDlg.h"

extern CDownloadsWnd* _pwndDownloads;
extern CSpiderWnd* _pwndSpider;

STDMETHODIMP CWGUrlReceiver::get_Url(BSTR *pVal)
{
	*pVal = m_dlInfo.bstrUrl;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_Url(BSTR newVal)
{
	LOG ("monitor: put_Url" << nl);
	m_dlInfo.bstrUrl = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::ShowAddDownloadDialog()
{
	return AddDownload ();
}

STDMETHODIMP CWGUrlReceiver::AddDownload()
{
	LOG ("monitor: Adddownload" << nl);

	
	if (m_bCheckExt && is_ExtToSkip ())
		return E_NOTIMPL;	

	if (m_bIgnoreSize == FALSE)
	{
		UINT iSize = _App.Monitor_SkipSmaller ();
		if (iSize)
		{
			USES_CONVERSION;
			fsDownloadMgr mgr (NULL);
			mgr.CreateByUrl (W2A (m_dlInfo.bstrUrl), TRUE);
			mgr.QuerySize ();
			UINT64 uSize = mgr.GetDownloader ()->GetSSFileSize ();
			if (iSize >= uSize)
				return E_NOTIMPL;	
		}
	}

	StartJobThread (_threadAddDownload);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::ShowAddPageDownloadDialog()
{
	StartJobThread (_threadAddPage);
	return S_OK;
}

DWORD WINAPI CWGUrlReceiver::_threadAddDownload(LPVOID lp)
{
	_ic_DownloadInfo* dlinfo = (_ic_DownloadInfo*) lp;
	DWORD dw = (DWORD)AddDownload_imp (dlinfo);
	delete dlinfo;
	return dw;
}

DWORD WINAPI CWGUrlReceiver::_threadAddPage(LPVOID lp)
{
	_ic_DownloadInfo* dlinfo = (_ic_DownloadInfo*) lp;

	while (((CDataStretcherApp*)AfxGetApp ())->Is_Starting ())
		Sleep (100);

	USES_CONVERSION;
	HRESULT hr = _pwndSpider->AddWebPage (W2A (dlinfo->bstrUrl), TRUE) ? S_OK : S_FALSE;
	delete dlinfo;
	return hr;
}  

HRESULT CWGUrlReceiver::AddDownload_imp(_ic_DownloadInfo* dlinfo)
{
	USES_CONVERSION;

	LOG ("monitor: adddownload_imp" << nl);
	LOG ("monitor: waiting for frame initializated" << nl);

	while (((CDataStretcherApp*)AfxGetApp ())->Is_Starting ())
		Sleep (100); 

	LOG ("monitor: initialized ok." << nl);
	LOG ("monitor: checking silent flag" << nl);

	UINT res;
	BOOL bSilent = dlinfo->bForceSilent ? TRUE : _App.Monitor_Silent ();

	LOG ("monitor: calling createdownload" << nl);

	vmsDWCD_AdditionalParameters ap;
	ap.dwMask = 0;
	if (dlinfo->bstrCookies.Length () != 0)
	{
		ap.dwMask |= DWCDAP_COOKIES;
		ap.strCookies = dlinfo->bstrCookies;
	}
	if (dlinfo->bstrPostData.Length () != 0)
	{
		ap.dwMask |= DWCDAP_POSTDATA;
		ap.strPostData = dlinfo->bstrPostData;
	}

	if (dlinfo->bstrFileName.Length () != 0)
	{
		ap.dwMask |= DWCDAP_FILENAME;
		ap.strFileName = dlinfo->bstrFileName;
	}

	ap.dwMask |= DWCDAP_FLAGS;

	if (bSilent == FALSE)
		ap.dwFlags = DWDCDAP_F_SETFOCUSTOOKBTN;
	else
		ap.dwFlags = dlinfo->bForceSilent && dlinfo->bNoUIInForceSilentMode ? DWDCDAP_F_NO_UI : 0;

	if (dlinfo->bForceAutoStart)
		ap.dwFlags |= DWDCDAP_F_FORCEAUTOSTART;

	if (dlinfo->bDisableMaliciousChecking)
		ap.dwFlags |= DWDCDAP_F_DISABLEMALICIOUSCHECK;

	if (dlinfo->bDisableURLExistsChecking)
		ap.dwFlags |= DWDCDAP_F_DONTCHECKURLALREADYEXISTS;

	if (dlinfo->bFlashVideoDownload)
	{
		ap.dwFlags |= DWDCDAP_F_FLASHVIDEODOWNLOAD;
		
	}

	BOOL bAdded = _pwndDownloads->CreateDownload (W2A (dlinfo->bstrUrl), TRUE, 
			W2A (dlinfo->bstrComment), W2A (dlinfo->bstrReferer), bSilent, 
			DWCD_NOFORCEAUTOLAUNCH, NULL, &ap, &res);

	LOG ("monitor: create download called ok" << nl);

	if (bAdded && bSilent)
		CMainFrame::ShowTimeoutBalloon (W2A (dlinfo->bstrUrl), "Download added", NIIF_INFO, TRUE);

	LOG ("monitor: do exit from _imp" << nl);

	
	if (res == ID_DLNOTADDED)
		bAdded = TRUE;

	return bAdded ? S_OK : S_FALSE;
}

STDMETHODIMP CWGUrlReceiver::get_Comment(BSTR *pVal)
{
	*pVal = SysAllocString (m_dlInfo.bstrComment);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_Comment(BSTR newVal)
{
	m_dlInfo.bstrComment = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_Referer(BSTR *pVal)
{
	*pVal = SysAllocString (m_dlInfo.bstrReferer);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_Referer(BSTR newVal)
{
	m_dlInfo.bstrReferer = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_CheckExtension(BOOL *pVal)
{
	*pVal = m_bCheckExt;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_CheckExtension(BOOL newVal)
{
	m_bCheckExt = newVal;
	return S_OK;
}

BOOL CWGUrlReceiver::is_ExtToSkip()
{
	USES_CONVERSION;
	fsURL url;

	if (url.Crack (W2A (m_dlInfo.bstrUrl)) != IR_SUCCESS)
		return TRUE;

	char szFile [MY_MAX_PATH];
	fsFileNameFromUrlPath (url.GetPath (), url.GetInternetScheme () == INTERNET_SCHEME_FTP,
		TRUE, szFile, sizeof (szFile));

	if (*szFile == 0)
		return FALSE;

	LPCSTR pszExt = strrchr (szFile, '.');
	if (pszExt == NULL) 
		return FALSE;

	return IsExtInExtsStr (_App.Monitor_SkipExts (), ++pszExt);
}

STDMETHODIMP CWGUrlReceiver::get_IgnoreSize(BOOL *pVal)
{
	*pVal = m_bIgnoreSize;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_IgnoreSize(BOOL newVal)
{
	m_bIgnoreSize = newVal;
	return S_OK;
}

void CWGUrlReceiver::FinalRelease()
{
	if (m_hJobt)
		CloseHandle (m_hJobt);
}

STDMETHODIMP CWGUrlReceiver::get_UIState(BSTR *pVal)
{
	if (m_hJobt == NULL)
	{
		*pVal = NULL;
		return E_FAIL;
	}

	DWORD dw;
	if (0 == GetExitCodeThread (m_hJobt, &dw))
	{
		*pVal = NULL;
		return E_FAIL;
	}

	if (dw == STILL_ACTIVE)
	{
		*pVal = SysAllocString (L"in_progress");
	}
	else
	{
		if (dw != S_OK)
			*pVal = SysAllocString (L"cancelled");
		else
			*pVal = SysAllocString (L"added");
	}

	return S_OK;
}  

void CWGUrlReceiver::StartJobThread(LPTHREAD_START_ROUTINE pfn)
{
	UIThread *thr = (UIThread*) RUNTIME_CLASS (UIThread)->CreateObject ();
	_ic_DownloadInfo* dlinfo = new _ic_DownloadInfo;
	*dlinfo = m_dlInfo;
	thr->set_Thread (pfn, dlinfo);
	thr->CreateThread ();

	if (m_hJobt)
		CloseHandle (m_hJobt);

	DuplicateHandle (GetCurrentProcess (), thr->m_hThread, GetCurrentProcess (),
		&m_hJobt, 0, TRUE, DUPLICATE_SAME_ACCESS);
}  

STDMETHODIMP CWGUrlReceiver::get_ForceSilent(BOOL *pVal)
{
	*pVal = m_dlInfo.bForceSilent;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_ForceSilent(BOOL newVal)
{
	m_dlInfo.bForceSilent = newVal;
	m_dlInfo.bNoUIInForceSilentMode = FALSE;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_Cookies(BSTR *pVal)
{
	*pVal = SysAllocString (m_dlInfo.bstrCookies);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_Cookies(BSTR newVal)
{
	m_dlInfo.bstrCookies = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_PostData(BSTR *pVal)
{
	*pVal = SysAllocString (m_dlInfo.bstrPostData);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_PostData(BSTR newVal)
{
	m_dlInfo.bstrPostData = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_UseSkipExtsList(BOOL *pVal)
{
	*pVal = TRUE;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_UseSkipExtsList(BOOL newVal)
{
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_ForceSilentEx(BOOL *pVal)
{
	*pVal == m_dlInfo.bForceSilent && m_dlInfo.bNoUIInForceSilentMode;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_ForceSilentEx(BOOL newVal)
{
	m_dlInfo.bForceSilent = newVal;
	m_dlInfo.bNoUIInForceSilentMode = newVal;

	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_ForceDownloadAutoStart(BOOL *pVal)
{
	*pVal = m_dlInfo.bForceAutoStart;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_ForceDownloadAutoStart(BOOL newVal)
{
	m_dlInfo.bForceAutoStart = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_DisableMaliciousChecking(BOOL *pVal)
{
	*pVal = m_dlInfo.bDisableMaliciousChecking;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_DisableMaliciousChecking(BOOL newVal)
{
	m_dlInfo.bDisableMaliciousChecking = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_DisableURLExistsCheck(BOOL *pVal)
{
	*pVal = m_dlInfo.bDisableURLExistsChecking;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_DisableURLExistsCheck(BOOL newVal)
{
	m_dlInfo.bDisableURLExistsChecking = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_FileName(BSTR *pVal)
{
	*pVal = SysAllocString (m_dlInfo.bstrFileName);
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_FileName(BSTR newVal)
{
	m_dlInfo.bstrFileName = newVal;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::get_FlashVideoDownload(BOOL *pVal)
{
	*pVal = m_dlInfo.bFlashVideoDownload;
	return S_OK;
}

STDMETHODIMP CWGUrlReceiver::put_FlashVideoDownload(BOOL newVal)
{
	m_dlInfo.bFlashVideoDownload = newVal;
	return S_OK;
}
