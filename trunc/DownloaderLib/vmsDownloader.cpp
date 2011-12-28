/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsDownloader.h"

#define DNP_COPY(a,b) SAFE_DELETE_ARRAY (m_dldr.GetDNP ()->a);\
	m_dldr.GetDNP ()->a = new char [lstrlen (b) + 1];\
	lstrcpy (m_dldr.GetDNP ()->a, b);

vmsDownloader::vmsDownloader() : 
	m_dldr (NULL)
{
	m_dldr.SetEventFunc (_DownloadMgrEvents, this);
	m_dldr.SetEventDescFunc (NULL, 0);
	m_enState = D_S_STOPPED;
}

vmsDownloader::~vmsDownloader()
{

}

DWORD vmsDownloader::_DownloadMgrEvents(fsDownloadMgr *pMgr, fsDownloaderEvent ev, UINT uInfo, LPVOID lp)
{
	vmsDownloader* pthis = (vmsDownloader*) lp;

	switch (ev)
	{
	case DE_STARTINGDOWNLOAD:
	case DE_STARTINGDOWNLOAD_CONNECTING:
	case DE_CONNECTING:
		pthis->m_enState = D_S_CONNECTING;
		break;

	case DE_SECTIONSTARTED:
	case DE_SECTDOWNLOADING:
		if (pMgr->GetDownloader ()->GetRunningSectionCount ())
			pthis->m_enState = D_S_DOWNLOADING;
		break;

	case DE_EXTERROR:
		if (uInfo == DMEE_STOPPEDORDONE)
		{
			if (pMgr->IsDone ())
				pthis->m_enState = D_S_DONE;
			else
				pthis->m_enState = D_S_FAILED;
		}
		break;

	case DE_STARTINGDOWNLOAD_FAILEDTOCONNECT:
	case DE_FAILCONNECT:
	case DE_ERROROCCURED:
	case DE_WRITEERROR:
	case DE_ERRFROMSERVER:
		pthis->m_enState = D_S_ERROR_NOW;
		break;
	}

	return TRUE;
}

fsInternetResult vmsDownloader::Initialize(LPCSTR pszURL, LPCSTR pszOutFile)
{
	fsInternetResult ir = m_dldr.CreateByUrl (pszURL, TRUE);
	if (ir != IR_SUCCESS)
		return ir;

	fsDownload_Properties *dp = m_dldr.GetDP ();

	SAFE_DELETE_ARRAY (dp->pszFileName);

	dp->pszFileName = new char [lstrlen (pszOutFile) + 1];
	lstrcpy (dp->pszFileName, pszOutFile);
	dp->uMaxSections = 1;
	dp->uMaxAttempts = UINT_MAX;
	dp->enAER = AER_REWRITE;
	*dp->pszAdditionalExt = 0;

	return IR_SUCCESS;
}

void vmsDownloader::StartDownloading()
{
	if (m_dldr.IsRunning ())
		return;
	m_enState = D_S_CONNECTING;
	m_dldr.StartDownloading ();
}

void vmsDownloader::StopDownloading()
{
	m_dldr.StopDownloading ();
}

vmsDownloader_State vmsDownloader::get_State()
{
	return m_enState;
}

int vmsDownloader::get_Progress()
{
	return (int)m_dldr.GetDownloader ()->GetPercentDone ();
}

UINT vmsDownloader::get_Speed()
{
	return m_dldr.GetDownloader ()->GetSpeed ();
}

UINT64 vmsDownloader::get_FileSize()
{
	return m_dldr.GetDownloader ()->GetSSFileSize ();
}

UINT64 vmsDownloader::get_DownloadedByteCount()
{
	return m_dldr.GetDownloader ()->GetDownloadedBytesCount ();
}

void vmsDownloader::set_MaxRetryCount(int nMax)
{
	m_dldr.GetDP ()->uMaxAttempts = nMax;
}

void vmsDownloader::set_RetriesTime(int nTime)
{
	m_dldr.GetDP ()->uRetriesTime = nTime;
}

void vmsDownloader::set_Timeout(UINT u)
{
	m_dldr.GetDP ()->uTimeout = u;
}

void vmsDownloader::set_Proxy(LPCSTR pszProxy, LPCSTR pszUserName, LPCSTR pszPassword)
{
	if (pszProxy == NULL)
	{
		m_dldr.GetDNP ()->enAccType = IATE_NOPROXY;
		return;
	}

	if (pszUserName == NULL)
		pszUserName = "";
	if (pszPassword == NULL)
		pszPassword = "";

	DNP_COPY (pszProxyName, pszProxy);
	DNP_COPY (pszProxyUserName, pszUserName);
	DNP_COPY (pszProxyPassword, pszPassword);

	m_dldr.GetDNP ()->enAccType = IATE_MANUALPROXY;
}

void vmsDownloader::set_DisableCookies(BOOL bDisable)
{
	m_dldr.GetDNP ()->bUseCookie = !bDisable;
}

void vmsDownloader::set_UserAgent(LPCSTR pszAgent)
{
	DNP_COPY (pszAgent, pszAgent);
}

void vmsDownloader::set_MaxSections(UINT uiMaxSections)
{
	m_dldr.GetDP()->uMaxSections = uiMaxSections;
}

void vmsDownloader::set_MinSectionSize (UINT uiMinSectionSize)
{
	m_dldr.GetDP()->uSectionMinSize = uiMinSectionSize;
}
