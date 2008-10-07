/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "fsDownloadsMgr.h"
#include "inetutil.h"
#include "mfchelp.h"
#include "vmsRegisteredApp.h"

#ifndef FDM_DLDR__RAWCODEONLY
#include "DDRDlg.h"
#include "WaitDlg.h"
#include "ZipPreviewDlg.h"
#include "WaitForConfirmationDlg.h"
#include "WaitForConfirmationDlg.h"
#include "DownloadsWnd.h"
#include "UIThread.h"
#endif

#include "fsScheduleMgr.h"
#include "fsSitesMgr.h"
#include "misc.h"
#include "system.h"
#include "vmsFilesToDelete.h"
#include "Hash\vmsHash.h"
#include "Dlg_CheckFileIntegrity_Result.h"
#include "vmsMetalinkFile.h"
#include "Dlg_Download.h"
#include "MyMessageBox.h"
#include "vmsVideoSiteHtmlCodeParser.h"
#include "FlashVideoDownloadsWnd.h"
#include "TorrentsWnd.h"
#include <Iphlpapi.h>

#ifndef FDM_DLDR__RAWCODEONLY
extern CDownloadsWnd* _pwndDownloads;
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace fsArchive;

const double DLD_HP_START_COEFF = 3.0;        

fsDownloadsMgr::fsDownloadsMgr()
{
	m_pfnEvents = NULL;
	m_cThreadsRunning = 0;

	m_bSkip1Cicle = FALSE;
	m_nID = 0;

	m_bAllowStart = TRUE;
	m_bDisablePD = FALSE;

	InitializeCriticalSection (&m_csQSection);

	InitializeCriticalSection (&m_csSave);
	
	m_bDeletingNow = m_bDeletingDeletedNow = m_bRestoringNow = FALSE;

#ifndef FDM_DLDR__RAWCODEONLY
	m_histmgr.SetEventFunc (_HistoryMgrEvents, this);
#endif

	fsInternetSession::InitializeWinInet ();
}

fsDownloadsMgr::~fsDownloadsMgr()
{
	vmsBtDownloadManager::Shutdown ();
}

UINT fsDownloadsMgr::Add(vmsDownloadSmartPtr dld, BOOL bKeepIDAsIs, bool bPlaceToTop)
{
	
	if (dld->pMgr->GetDownloadMgr () != NULL)
	{
		dld->pMgr->GetDownloadMgr ()->SetEventFunc (_DownloadMgrEvents, this);
		dld->pMgr->GetDownloadMgr ()->SetEventDescFunc (_DownloadMgrEventDesc, this);
		dld->pMgr->GetDownloadMgr ()->set_Download (dld);
	}
	else if (dld->pMgr->GetBtDownloadMgr () != NULL)
	{
		dld->pMgr->GetBtDownloadMgr ()->SetEventsHandler (_BtDownloadManagerEventHandler, this);
	}

	dld->pMgr->UseDetailedLog (m_bDetLog);
	
	if (bKeepIDAsIs == FALSE)
		dld->nID = m_nID++; 
	else
		dld->nID = dld->nID;

	if (dld->pMgr->GetDownloadMgr () != NULL)
		Apply_MirrParameters (dld);

	SYSTEMTIME time;
	GetLocalTime (&time);
	SystemTimeToFileTime (&time, &dld->dateAdded);

	
	if (dld->pMgr->GetDownloadMgr () != NULL)
	{
		fsSiteInfo *site = _SitesMgr.FindSite2 (dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszServerName, 
			fsNPToSiteValidFor (dld->pMgr->GetDownloadMgr ()->GetDNP ()->enProtocol));
		if (site)
			dld->pMgr->GetDownloadMgr ()->GetDNP ()->dwFtpFlags = site->dwFtpFlags;
	}

	if (bPlaceToTop == false)
		DownloadsList_Add (dld);
	else
		DownloadsList_Insert (0, dld);

	m_bSkip1Cicle = TRUE;

	return dld->nID;
}

void fsDownloadsMgr::_DownloadMgrEventDesc(fsDownloadMgr *pMgr, fsDownloadMgr_EventDescType enType, LPCSTR pszEvent, LPVOID lp)
{
	try {

	fsDownloadsMgr* pThis = (fsDownloadsMgr*) lp;
	vmsDownloadSmartPtr dld = pThis->GetDownloadByDownloadMgr (pMgr);
	if (dld == NULL)
		return;

	pThis->OnDownloadDescEventRcvd (dld, enType, pszEvent);

	} 
	
	catch (...) {}	
					
}

vmsDownloadSmartPtr fsDownloadsMgr::GetDownloadByDownloadMgr(fsDownloadMgr *pMgr)
{
	ASSERT (pMgr != NULL);

	try {
		for (size_t i = 0; i < m_vDownloads.size (); i++)
		{
			if (m_vDownloads [i]->pMgr->GetDownloadMgr () == pMgr)
				return m_vDownloads [i];
		}
	}catch (...){}

	return NULL;
}

DWORD fsDownloadsMgr::_DownloadMgrEvents(fsDownloadMgr *pMgr, fsDownloaderEvent enEvent, UINT uInfo, LPVOID lp)
{
	fsDownloadsMgr* pThis = (fsDownloadsMgr*) lp;

	ASSERT (pMgr != NULL);
	vmsDownloadSmartPtr dld = pThis->GetDownloadByDownloadMgr (pMgr);

	if (dld == NULL)
		return TRUE;

	try {

	switch (enEvent)
	{
		case DE_SECTIONDONE:
			pThis->Event (dld, DME_DOWNLOADEREVENTRECEIVED);
			pThis->OnSectionStop (dld);
		break;

		case DE_SECTDOWNLOADING:
			pThis->Event (dld, DME_SECTIONDOWNLOADING);
		break;

		case DE_SECTIONSTOPPED:
			pThis->OnSectionStop (dld);
		break;

		case DE_EXTERROR:	
			switch (uInfo)
			{
				case DMEE_FILEWASDELETED:	
					dld->bAutoStart = FALSE; 
					break;

				case DMEE_STOPPEDORDONE: 
					if (FALSE == pThis->OnDownloadStoppedOrDone (dld))
						return TRUE;
					dld = NULL;	
					break;

				case DMEE_FATALERROR:
				case DMEE_USERSTOP:
					dld->bAutoStart = FALSE;
					break;

				case DMEE_FILEUPDATED:
					pThis->Event (dld, DME_FILEUPDATED);
					dld = NULL;	
					break;

				case DMEE_STARTING:
					if (FALSE == pThis->Event (dld, DME_DLMGRTHREADSTARTING))
						return FALSE;
					pThis->Event (dld, DME_CREATEDLDDIALOG);
					break;
			}
		break;

		case DE_QUERYNEWSECTION:	
			pThis->RebuildServerList ();	
			return pThis->OnQueryNewSection (dld, uInfo);

		case DE_SECTIONSTARTED:
			pThis->ApplyTrafficLimit ();
			pThis->RebuildServerList ();
			pThis->Event (dld, DME_SECTIONSTARTED);
		break;

		case DE_ALLSTOPPEDORDONE:
		break;

		case DE_WRITEERROR:
			dld->bAutoStart = FALSE;
		break;

		case DE_REDIRECTINGOKCONTINUEOPENING:
			pThis->Event (dld, DME_REDIRECTED);
			pThis->Event (dld, DME_UPDATEDLDDIALOG);
			break;

		case DE_NOMIRRFOUND:
			dld->dwFlags |= DLD_DONTUSEMIRRORS; 
			pMgr->GetDownloader ()->Set_SearchForMirrors (FALSE);
			break;

		case DE_FILESIZETOOBIG:
			dld->bAutoStart = FALSE;
			break;

		case DE_NEEDFILE:
			return pThis->OnBeforeDownload (dld);

		case DE_CONFIRMARCHIVEDETECTION:
		{
#ifndef FDM_DLDR__RAWCODEONLY
			UINT nRes;
			UIThread *thr = (UIThread*) RUNTIME_CLASS (UIThread)->CreateObject ();
			thr->set_Thread (_threadConfirmZIP, &nRes);
			thr->CreateThread ();
			WaitForSingleObject (thr->m_hThread, INFINITE);
			return nRes;
#else
			return 0;
#endif
		}

		case DE_ARCHIVEDETECTED:
			return pThis->OnArchiveDetected (dld, (fsArchiveRebuilder*) uInfo);
	}

	}
	catch (...) {}

	if (dld) 
		pThis->Event (dld, DME_DOWNLOADEREVENTRECEIVED);

	return TRUE;
}

void fsDownloadsMgr::ProcessDownloads()
{
	
	DLDS_LIST vDldsToStop, vDldsToStart;

	if (m_bAllowStart == FALSE || m_bDisablePD)
		return;

	LOG ("fsDsM::PD: start" << nl);

	
	m_mxBuildConns.Lock ();

	try {

	RebuildServerList (FALSE);

	UINT cConns = m_aTUM [m_enTUM].uMaxConns;
	UINT cDlds = m_aTUM [m_enTUM].uMaxDlds;
	UINT cMaxCPS = m_aTUM [m_enTUM].uMaxConnsPS;

	
	
	for (size_t i = 0; i < (size_t)_SitesMgr.GetSiteCount (); i++)
		_SitesMgr.GetSite (i)->cConnsNow = min (_SitesMgr.GetSite (i)->cMaxConns, cMaxCPS);

	LOG ("fsDsM::PD: 1" << nl);

	for (i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		if (dld->pMgr->IsDone ())
			continue;

		if (dld->pMgr->IsBittorrent () && 
				dld->pMgr->GetBtDownloadMgr ()->get_State () == BTDSE_QUEUED)
			continue;

		BOOL bAccept = FALSE;	
		
		fsDownloadMgr *pMgr = dld->pMgr->GetDownloadMgr (); 
		LPCSTR pszSiteName = pMgr ? pMgr->GetDNP ()->pszServerName : NULL;
		fsNetworkProtocol enNP = pMgr ? pMgr->GetDNP ()->enProtocol : (fsNetworkProtocol)-1;

		if (cDlds && cConns)	
		{	
			fsSiteInfo* site = pMgr? _SitesMgr.FindSite2 (pszSiteName, fsNPToSiteValidFor (enNP)) : NULL;
			if (site == NULL || site->cConnsNow)	
			{
				UINT cDldConns = 0;	

				if (dld->pMgr->IsRunning ())
				{
					cDldConns = dld->pMgr->IsBittorrent () ? 0 : dld->pMgr->GetConnectionCount ();

					if (pMgr)
					{
						cDldConns += pMgr->GetDownloader ()->GetCreatingNowSectionCount (UINT_MAX);

						
						for (int i = 0; i < pMgr->GetDownloader ()->GetMirrorURLCount (); i++)
						{
							UINT cMirrConns = pMgr->GetDownloader ()->GetCreatingNowSectionCount (i);
							fsDownload_NetworkProperties* mirrDNP = pMgr->GetDownloader ()->MirrorDNP (i);

							fsSiteInfo* mirrSite = _SitesMgr.FindSite2 (mirrDNP->pszServerName, fsNPToSiteValidFor (mirrDNP->enProtocol));

							if (mirrSite->cConnsNow > cMirrConns)
								mirrSite->cConnsNow -= cMirrConns;
							else
								mirrSite->cConnsNow = 0; 
						}
					}

					bAccept = TRUE;
				}
				else
				{
					if (dld->bAutoStart)
					{
						cDldConns = min (cConns, cMaxCPS);
						if (pMgr)
						{
							cDldConns = min (cDldConns, site->cConnsNow);
							cDldConns = min (cDldConns, pMgr->GetDP ()->uMaxSections);
						}
						else
						{
							cDldConns = min (cDldConns, 2);
							
						}
						if (pMgr && cDldConns > 1)
						{
							UINT64 size = pMgr->GetDownloader ()->GetSSFileSize ();
							if (size == _UI64_MAX)
								cDldConns = 1;
							else if (size)
								cDldConns = min (cDldConns, 
									UINT (pMgr->GetDownloader ()->GetBytesLeft () / pMgr->GetDP ()->uSectionMinSize));
						}
						bAccept = TRUE;
					}
				}

				if (bAccept)
				{
					
					if (site)
					{
						if(site->cConnsNow > cDldConns)
							site->cConnsNow -= cDldConns;
						else
							site->cConnsNow = 0;
					}

					if (cConns > cDldConns)
						cConns -= cDldConns;
					else
						cConns = 0;

					cDlds--;
				}
			}
		}

		if (bAccept && dld->pMgr->IsRunning () == FALSE)
			vDldsToStart.push_back (dld);	
		else if (bAccept == FALSE && dld->pMgr->IsRunning ())
		{
			
			if (pMgr == NULL || dld->pMgr->GetNumberOfSections () == 0 || 
					(dld->pMgr->GetSSFileSize () != _UI64_MAX && dld->pMgr->IsResumeSupported () == RST_PRESENT))
				vDldsToStop.push_back (dld); 
		}
	}

	RebuildServerList (TRUE);

	} catch (...) {}

	LOG ("fsDsM::PD: 2" << nl);

	m_mxBuildConns.Unlock (); 

	StopDownloads (vDldsToStop);
	StartDownloads (vDldsToStart);

	LOG ("fsDsM::PD: exit" << nl);
}

void fsDownloadsMgr::StartDownloads(DLDS_LIST &vpDlds, BOOL )
{
	LOG ("fsDsM::SD: start" << nl);

	try {

	UINT cRunning = GetRunningDownloadCount ();

	LOG ("fsDsM::SD: number of running downloads = " << (DWORD)cRunning << nl);

	for (size_t i = 0; i < vpDlds.size (); i++)
	{
		vmsDownloadSmartPtr dld = vpDlds [i];

		if (cRunning < m_aTUM [m_enTUM].uMaxConns && cRunning < m_aTUM [m_enTUM].uMaxDlds && cRunning < 20)	
		{
			if (dld->pMgr->IsDone () == FALSE)
			{
				LOG ("fsDsM::SD: start download (" << dld->pMgr->get_URL () << ")" << nl);

				dld->pMgr->StartDownloading ();
				cRunning ++;
			}
		}
		else
		{
			dld->bAutoStart = TRUE;
		}
	}

	}catch (...) {}

	ApplyTrafficLimit ();

	Event (NULL, DME_DLDSAUTOSTARTMDFD);

	LOG ("fsDsM::SD: exit" << nl);
}

void fsDownloadsMgr::StopDownloads(DLDS_LIST &vDlds, BOOL bByUser)
{
	LOG ("fsDsM::SpD: start" << nl);

	try {

	
	if (bByUser)
	{
		for (size_t i = 0; i < vDlds.size (); i++)
			vDlds [i]->bAutoStart = FALSE;
	}

	for (size_t i = 0; i < vDlds.size (); i++)
		vDlds [i]->pMgr->StopDownloading ();

	}
	catch (...) {}

	ApplyTrafficLimit ();

	if (bByUser)
		Event (NULL, DME_DLDSAUTOSTARTMDFD);

	LOG ("fsDsM::SpD: exit" << nl);
}

void fsDownloadsMgr::SetEventsFunc(fntDownloadsMgrEventFunc pfn, LPVOID lpParam)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lpParam;
}

DWORD fsDownloadsMgr::Event(vmsDownloadSmartPtr dld, fsDownloadsMgrEvent ev)
{
	DWORD dwRet = TRUE; 

	if (dld && dld->pfnDownloadEventsFunc)
		dwRet = dld->pfnDownloadEventsFunc (dld, ev, dld->lpEventsParam);

	if (m_pfnEvents)
		dwRet = m_pfnEvents (dld, ev, m_lpEventsParam);

	return dwRet;
}

vmsDownloadSmartPtr fsDownloadsMgr::GetDownload(size_t iIndex)
{
	try {
	if (iIndex >= m_vDownloads.size ())
		return NULL;

	return m_vDownloads [iIndex];
	}
	catch (...) { return NULL; }
}

BOOL fsDownloadsMgr::LoadDownloads()
{
	if (!m_saver.Load (&m_vDownloads, "downloads", FALSE))
		return FALSE;

	BOOL bFailIfLarge = _App.CheckRecycleBinSize ();
	for (int j = 0; j < 2; j++)
	{
		fsDLLoadResult res = m_saver.Load (&m_vDeletedDownloads, "downloads.del", j == 0 && bFailIfLarge);
		if (res == DLLR_TOOLARGESIZE)
		{
			CWaitForConfirmationDlg dlg;
			dlg.Init (LS (L_RBTOOLARGE), UINT_MAX, TRUE, FALSE, LS (L_DONTCHECKAGAIN));
			dlg.m_pszIcon = IDI_WARNING;
			UINT nRet = dlg.DoModal ();
			if (dlg.m_bDontAsk)
				_App.CheckRecycleBinSize (FALSE);
			if (nRet == IDOK)
				break;
		}
		else 
			break;
	}

#ifndef FDM_DLDR__RAWCODEONLY
	m_histmgr.LoadHistory ();
#endif

	m_nID = 0;

	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		dld->pdlg = NULL;

		dld->pfnDownloadEventsFunc = NULL;

		if (dld->pMgr->GetDownloadMgr ())
		{
			dld->pMgr->GetDownloadMgr ()->SetEventFunc (_DownloadMgrEvents, this);
			dld->pMgr->GetDownloadMgr ()->SetEventDescFunc (_DownloadMgrEventDesc, this);			
			dld->pMgr->GetDownloadMgr ()->set_Download (dld);
		}
		else if (dld->pMgr->GetBtDownloadMgr ())
		{
			dld->pMgr->GetBtDownloadMgr ()->SetEventsHandler (_BtDownloadManagerEventHandler, this);
		}

		dld->pMgr->UseDetailedLog (m_bDetLog);

		m_nID = max (m_nID, dld->nID);

		dld->pGroup->cDownloads++;
	}

	for (i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		UINT id = m_vDeletedDownloads [i]->nID;
		m_nID = max (m_nID, id);
	}

	m_nID++;

	LoadStateInformation ();

	Apply_MirrParameters ();

	if (_BT.is_Initialized ())
	{
		InterlockedIncrement (&m_cThreadsRunning);
		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadStartSeeding, this, 0, &dw));
	}

	return TRUE;
}

BOOL fsDownloadsMgr::Save()
{
	EnterCriticalSection (&m_csSave);

	BOOL b = FALSE;

	try {
		b = m_saver.Save (&m_vDownloads, "downloads");
		if (FALSE == m_saver.Save (&m_vDeletedDownloads, "downloads.del"))
			b = FALSE;
		if (FALSE == m_histmgr.SaveHistory ())
			b = FALSE;
		if (FALSE == SaveStateInformation ())
			b = FALSE;
		if (FALSE == _BT.SaveState ())
			b = FALSE;
	}catch (...){}

	LeaveCriticalSection (&m_csSave);
	return b;
}

int fsDownloadsMgr::GetDownloadIndex(vmsDownloadSmartPtr dld, int nFindStartPos)
{
	ASSERT (nFindStartPos >= 0);

	for (size_t i = nFindStartPos; i < m_vDownloads.size (); i++)
	{
		if (m_vDownloads [i] == dld)
			return i;
	}

	return -1;
}

int fsDownloadsMgr::DeleteDownloads(DLDS_LIST &vDlds, BOOL bByUser, BOOL bDontConfirmFileDeleting)
{
	size_t cMaxDlds = 40;

	try {
		if (vDlds [0]->pfnDownloadEventsFunc)
			cMaxDlds = 20;
	} catch (...) {}
	
	
	if (vDlds.size () < cMaxDlds)
		return DeleteDownloads2 (&vDlds, bByUser, bDontConfirmFileDeleting);

	

	int iDeleted = 0;
	
	CWaitDlg dlg;
	dlg.StartWaiting (LS (L_DELETINGDLDS), _threadWaitDelDlds, TRUE, &vDlds, &bByUser, &bDontConfirmFileDeleting, &iDeleted, this);

	return iDeleted;
}

DWORD WINAPI fsDownloadsMgr::_threadDeleteDownload(LPVOID lp)
{
	
	DLDS_LIST *pvDlds = (DLDS_LIST*) lp;

	try {

	
	for (size_t i = 0; i < pvDlds->size (); i++)
	{
		vmsDownloadMgrSmartPtr mgr = pvDlds->at (i)->pMgr;

		if (mgr->GetDownloadMgr ())
		{
			mgr->GetDownloadMgr ()->SetEventFunc (NULL, 0);
			mgr->GetDownloadMgr ()->SetEventDescFunc (NULL, 0);
		}
		else if (mgr->GetBtDownloadMgr ())
		{
			mgr->GetBtDownloadMgr ()->SetEventsHandler (NULL, 0);
		}

		mgr->StopDownloading ();
	}

	MSG msg;

	
	for (i = 0; i < pvDlds->size (); i++)
	{
		vmsDownloadSmartPtr dld = pvDlds->at (i);
		while (dld->pMgr->IsRunning ())
		{
			while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
				DispatchMessage (&msg);

			Sleep (0);
		}
	}

	while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		DispatchMessage (&msg);

	}
	catch (...) {}

	try {
		delete pvDlds;	
	}catch (...) {}

	return 0;
}

void fsDownloadsMgr::StopGroup(vmsDownloadsGroupSmartPtr pGroup)
{
	DLDS_LIST vDlds;

	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		if (dld->pGroup->nId == pGroup->nId)
			vDlds.push_back (dld);
	}

	StopDownloads (vDlds, TRUE);
}

void fsDownloadsMgr::StartGroup(vmsDownloadsGroupSmartPtr pGroup)
{
	DLDS_LIST vDlds;

	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		if (dld->pGroup->nId == pGroup->nId && 
			  dld->pMgr->IsRunning () == FALSE)
			vDlds.push_back (dld);
	}

	StartDownloads (vDlds, TRUE);
}

DWORD WINAPI fsDownloadsMgr::_threadDownloadsMgr(LPVOID lp)
{
	fsDownloadsMgr *pThis = (fsDownloadsMgr*) lp;
	fsTicksMgr now;
	fsTicksMgr lastSpeedMeasure;	

	pThis->m_bNeedExit = FALSE;

	now.Now ();
	lastSpeedMeasure.Now ();

	pThis->m_vSummSpeed.clear ();

	while (pThis->m_bNeedExit == FALSE)
	{
		if (now - lastSpeedMeasure >= 5000)	
		{
			

			lastSpeedMeasure.Now ();

			
			if (pThis->m_vSummSpeed.size () == MAX_SUMMS)
				pThis->m_vSummSpeed.clear ();

			UINT summ = 0;

			
			try {
			for (int i = pThis->m_vDownloads.size () - 1; i >= 0; i--)
				summ += pThis->m_vDownloads [i]->pMgr->GetSpeed ();
			}catch (...) {}

			
			
			if (summ == 0 && pThis->IsRunning () == FALSE)
				pThis->m_vSummSpeed.clear ();
			else
				pThis->m_vSummSpeed.add (summ);
		}

		if (pThis->m_bSkip1Cicle == FALSE)
		{
			pThis->ManageTraffic ();
		}
		else
			pThis->m_bSkip1Cicle = FALSE;

		Sleep (1000);
		now.Now ();
	}

	InterlockedDecrement (&pThis->m_cThreadsRunning);

	return 0;
}

fsTrafficUsageMode* fsDownloadsMgr::GetTUMs()
{
	return m_aTUM;
}

void fsDownloadsMgr::RebuildServerList(BOOL bUpdateSiteList)
{
	LOG ("fsDsM::RSL: start" << nl);

	m_mxBuildConns.Lock (); 

	try {

	for (int i = 0; i < _SitesMgr.GetSiteCount (); i++)
		_SitesMgr.GetSite (i)->cConnsNow = 0;	

	
	for (i = m_vDownloads.size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		if (dld->pMgr->GetDownloadMgr () == NULL)
			continue;

		fsDownloadMgr *pMgr = dld->pMgr->GetDownloadMgr ();

		int cSects = pMgr->GetDownloader ()->GetNumberOfSections ();
		fs::list <UINT> vMirrsCalculated;

		

		
		for (int j = -1; j < cSects || (cSects == 0 && j == 0); j++)
		{
			fsSection sect;
			bool bSect = j != -1 && cSects != 0;

			if (bSect)
				pMgr->GetDownloader ()->GetSectionInfo (j, &sect);

			int cConns = bSect && (sect.state == SS_DOWNLOADING) ? 1 : 0;

			UINT uSect = UINT_MAX;
			if (bSect)
				uSect = j;

			fsDownload_NetworkProperties *dnp = pMgr->GetDownloader ()->DNP (uSect);

			LPCSTR pszServer = dnp->pszServerName;
			fsNetworkProtocol np = dnp->enProtocol;
			
			fsSiteInfo *site = _SitesMgr.FindSite2 (pszServer, fsNPToSiteValidFor (np));

			
			for (int k = 0; k < vMirrsCalculated.size (); k++)
			{
				if (vMirrsCalculated [k] == sect.nMirrorURL)
					break;
			}

			if (k != vMirrsCalculated.size ())
			{
				cConns += pMgr->GetDownloader ()->GetCreatingNowSectionCount (sect.nMirrorURL);
				vMirrsCalculated.add (sect.nMirrorURL);
			}

			
			if (site == NULL)
			{
				
				fsSiteInfo site;
				site.strName = pszServer;
				site.cConnsNow = cConns;
				site.dwFtpFlags = dnp->dwFtpFlags;
				site.bTemp = TRUE;
				site.cMaxConns = UINT_MAX;
				site.pGroup = dld->pGroup;
				if (_App.SM_KeepPasswords ())
				{
					site.strUser = *dnp->pszUserName ? dnp->pszUserName : NULL;
					site.strPassword = *dnp->pszPassword ? dnp->pszPassword : NULL;
				}
				site.dwValidFor = fsNPToSiteValidFor (np) | SITE_VALIDFOR_SUBDOMAINS;
				_SitesMgr.AddSite (site);
			}
			else
			{
				
				site->cConnsNow += cConns;
			}
		}
	}

	}
	catch (...) { }

	m_mxBuildConns.Unlock (); 

	try {
		
		if (bUpdateSiteList)
		{
			for (int i = 0; i < _SitesMgr.GetSiteCount (); i++)
				_SitesMgr.SiteUpdated (_SitesMgr.GetSite (i));
		}
	}
	catch (...) {}

	LOG ("fsDsM::RSL: done" << nl);
}
BOOL fsDownloadsMgr::OnQueryNewSection(vmsDownloadSmartPtr dld, UINT nUsingMirror)
{
	LOG ("Entering DLSM::OnQueryNewSection..." << nl);

	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);
	
	
	if (dld->pMgr->GetDownloadMgr ()->GetDP ()->bIgnoreRestrictions)
	{
		LOG ("Exit DLSM::OnQueryNewSection" << nl);
		return TRUE;
	}

	EnterCriticalSection (&m_csQSection);
	BOOL b = TRUE; 

	fsDownload_NetworkProperties *dnp;
	if (nUsingMirror != UINT_MAX)
		dnp = dld->pMgr->GetDownloadMgr ()->GetDownloader ()->MirrorDNP (nUsingMirror);
	else
		dnp = dld->pMgr->GetDownloadMgr ()->GetDNP ();

	
	if (IsServerFilled (dnp->pszServerName, fsNPToSiteValidFor (dnp->enProtocol)))
	{
		Event (dld, LS (L_LIMITREACHED), EDT_WARNING);
		b = FALSE;
	}
	else if (IsMaxConnsReached ()) 
	{
		Event (dld, LS (L_TOTALLIMITREACHED), EDT_WARNING);
		b = FALSE;
	}
	
	
	
	
	if (b == FALSE && dld->bAutoStart == FALSE && dld->pMgr->IsDownloading () == FALSE)
		dld->bAutoStart = TRUE;

	LeaveCriticalSection (&m_csQSection);
	LOG ("Exit DLSM::OnQueryNewSection" << nl);
	return b;
}

void fsDownloadsMgr::Event(vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType)
{
	OnDownloadDescEventRcvd (dld, enType, pszEvent);
}

UINT fsDownloadsMgr::GetAmountConnections()
{
	UINT nConns = 0;

	try {

	for (int i = _SitesMgr.GetSiteCount () - 1; i >= 0; i--)
		nConns += _SitesMgr.GetSite (i)->cConnsNow;

	}catch (...) {}

	return nConns;
}

void fsDownloadsMgr::OnSectionStop(vmsDownloadSmartPtr dld)
{
	

	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);

	fsSiteInfo* site = _SitesMgr.FindSite2 (dld->pMgr->GetDownloadMgr ()->GetDNP ()->pszServerName, 
		fsNPToSiteValidFor (dld->pMgr->GetDownloadMgr ()->GetDNP ()->enProtocol));

	if (site)
	{
		site->cConnsNow --;	
		_SitesMgr.SiteUpdated (site);
	}

	ApplyTrafficLimit ();
}

void fsDownloadsMgr::ManageTraffic()
{
	
	
	
	
	
	
	
	
	
	ApplyTrafficLimit ();
}

void fsDownloadsMgr::SetTUM(fsTUM tum)
{
	m_enUserTUM = m_enTUM = tum;
	ResetDldHpManageState ();
	ProcessDownloads ();
	ApplyTrafficLimit ();
}

fsTUM fsDownloadsMgr::GetTUM()
{
	return m_enTUM;
}

void fsDownloadsMgr::ApplyTrafficLimit()
{
	LOG ("fsDsM::ATL: start" << nl);

	
	ASSERT (m_aTUM [TUM_HEAVY].uTrafficLimit == UINT_MAX);
	m_aTUM [TUM_HEAVY].uTrafficLimit = UINT_MAX;

    try {

	if (m_vDownloads.size () == 0)
	{
		LOG ("fsDsM::ATL: exit [0]" << nl);
		return;
	}

	LOG ("fsDsM::ATL: apply bt properties" << nl);	

	if (_BT.is_Initialized ())
	{
		_BT.get_Session ()->SetUploadLimit (
			_App.Bittorrent_UploadTrafficLimit (m_enTUM));
		_BT.get_Session ()->SetMaxUploads (
			_App.Bittorrent_UploadConnectionLimit (m_enTUM));
		if (vmsBtSupport::getBtDllVersion () >= 751)
		{
			_BT.get_Session ()->SetMaxHalfOpenConnections (
				_App.Bittorrent_MaxHalfConnections (m_enTUM));
		}
	}

	if (m_gabInfo.dld == NULL)
	{
		ApplyTrafficLimit_NoHpDld ();
	}
	else
	{
		if (m_gabInfo.dld->pMgr->IsDownloading () == FALSE)
		{
			ResetDldHpManageState ();
			ApplyTrafficLimit_NoHpDld ();
		}
		else
		{
			ApplyTrafficLimit_HasHpDld ();
		}
	}

	}
	catch (...) {}

	LOG ("fsDsM::ATL: exit" << nl);
}

void fsDownloadsMgr::StartAllDownloads(BOOL bByUser)
{
	DLDS_LIST vDlds;

	try {

	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		if (m_vDownloads [i]->pMgr->IsDone () == FALSE)
			vDlds.push_back (m_vDownloads [i]);
	}

	}
	catch (...) {}

	StartDownloads (vDlds, bByUser);

}

void fsDownloadsMgr::StopAllDownloads(BOOL bByUser)
{
	DLDS_LIST vDlds;

	try {

	for (size_t i = 0; i < m_vDownloads.size (); i++)
		vDlds.push_back (m_vDownloads [i]);

	}
	catch (...) {}

	StopDownloads (vDlds, bByUser);
}

BOOL fsDownloadsMgr::IsSummarySpeedWasLessThan(UINT uSpeed, UINT uTimeInterval)
{
	UINT uSize = m_vSummSpeed.size ();	
	UINT uNeed = uTimeInterval / 5;	

	if (uSize == 0)
		return FALSE;

	if (uSize < uNeed)
		return FALSE;

	for (UINT i = uSize - 1; i > uSize - uNeed; i--)
	{
		
		
		if (m_vSummSpeed [i] >= uSpeed)
				return FALSE;
	}

	return TRUE;
}

BOOL fsDownloadsMgr::IsRunning()
{
	try {
	for (int i = m_vDownloads.size () - 1; i >= 0; i--)
		if (m_vDownloads [i]->pMgr->IsRunning ())
			return TRUE;
	}
	catch (...) {}

	return FALSE;
}

void fsDownloadsMgr::CheckNoActiveDownloads()
{
	if (IsRunning () == FALSE)
		Event ((fsDownload*)NULL, DME_NOACTIVEDOWNLOADS);
}

vmsDownloadSmartPtr fsDownloadsMgr::GetDownloadByID(UINT nID)
{
	try{
	for (int i = m_vDownloads.size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];
		if (dld->nID == nID)
			return dld;
	}
	}catch(...){}

	return NULL;
}

void fsDownloadsMgr::AllowStartNewDownloads(BOOL bAllow)
{
	m_bAllowStart = bAllow;
}

void fsDownloadsMgr::LockList()
{
	m_mxDlList.Lock ();
}

void fsDownloadsMgr::UnlockList()
{
	m_mxDlList.Unlock ();
}

void fsDownloadsMgr::SaveSettings()
{
	_App.Set_TUM (m_aTUM);
	_App.TUMManage (m_enTUMManage);
	_App.CurrentTUM (m_enUserTUM);
	_App.DDR (m_enDDR);
	_App.AutoDelCompleted (m_bAutoDelCompleted);
	_App.DetLog (m_bDetLog);

	_App.Avir_Perform (m_bVirCheck);
	_App.Avir_Name (m_strVirName);
	_App.Avir_Args (m_strVirArgs);
	_App.Avir_Exts (m_strVirExts);
}

void fsDownloadsMgr::ApplyConnectionType(fsConnectionType enCT)
{
	switch (enCT)
	{
		case CT_MODEM_14_4:
			m_aTUM [TUM_LIGHT].uMaxConns = 1;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 1;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 200;
			m_aTUM [TUM_LIGHT].uMaxDlds = 1;

			m_aTUM [TUM_MEDIUM].uMaxConns = 1;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 1;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 800;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 1;

			m_aTUM [TUM_HEAVY].uMaxConns = 3;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 3;
			m_aTUM [TUM_HEAVY].uMaxDlds = 1;

			_App.MaxSections (3);
			_App.FileWriteCacheSize (0);
			break;

		case CT_MODEM_28_8:
			m_aTUM [TUM_LIGHT].uMaxConns = 1;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 1;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 400;
			m_aTUM [TUM_LIGHT].uMaxDlds = 1;

			m_aTUM [TUM_MEDIUM].uMaxConns = 2;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 2;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 1600;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 1;

			m_aTUM [TUM_HEAVY].uMaxConns = 4;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 3;
			m_aTUM [TUM_HEAVY].uMaxDlds = 1;

			_App.MaxSections (4);
			_App.FileWriteCacheSize (0);
			break;

		case CT_MODEM_33_6:
			m_aTUM [TUM_LIGHT].uMaxConns = 1;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 1;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 500;
			m_aTUM [TUM_LIGHT].uMaxDlds = 1;

			m_aTUM [TUM_MEDIUM].uMaxConns = 2;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 2;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 1900;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 1;

			m_aTUM [TUM_HEAVY].uMaxConns = 8;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 4;
			m_aTUM [TUM_HEAVY].uMaxDlds = 2;

			_App.MaxSections (4);
			_App.FileWriteCacheSize (0);
			break;

		case CT_MODEM_56_ISDN:
			m_aTUM [TUM_LIGHT].uMaxConns = 1;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 1;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 800;
			m_aTUM [TUM_LIGHT].uMaxDlds = 1;

			m_aTUM [TUM_MEDIUM].uMaxConns = 4;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 2;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 3000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 1;

			m_aTUM [TUM_HEAVY].uMaxConns = 8;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 4;
			m_aTUM [TUM_HEAVY].uMaxDlds = 2;

			_App.MaxSections (4);
			_App.FileWriteCacheSize (0);
			break;

		case CT_DUAL_ISDN:
			m_aTUM [TUM_LIGHT].uMaxConns = 4;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 2;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 3000;
			m_aTUM [TUM_LIGHT].uMaxDlds = 1;

			m_aTUM [TUM_MEDIUM].uMaxConns = 8;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 3;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 7000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 2;

			m_aTUM [TUM_HEAVY].uMaxConns = 16;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 6;
			m_aTUM [TUM_HEAVY].uMaxDlds = 3;

			_App.MaxSections (4);
			_App.FileWriteCacheSize (0);
			break;

		case CT_CABLE_DSL_256:
			m_aTUM [TUM_LIGHT].uMaxConns = 6;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 3;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 6000;
			m_aTUM [TUM_LIGHT].uMaxDlds = 2;

			m_aTUM [TUM_MEDIUM].uMaxConns = 8;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 3;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 20000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 3;

			m_aTUM [TUM_HEAVY].uMaxConns = 10;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 4;
			m_aTUM [TUM_HEAVY].uMaxDlds = 4;

			_App.MaxSections (5);
			_App.FileWriteCacheSize (0);
			break;

		case CT_CABLE_DSL_512:
		case CT_CABLE_DSL_768:
			m_aTUM [TUM_LIGHT].uMaxConns = 6;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 3;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 20000;
			m_aTUM [TUM_LIGHT].uMaxDlds = 2;

			m_aTUM [TUM_MEDIUM].uMaxConns = 8;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 3;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 45000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 3;

			m_aTUM [TUM_HEAVY].uMaxConns = 12;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 4;
			m_aTUM [TUM_HEAVY].uMaxDlds = 4;

			_App.MaxSections (5);
			_App.FileWriteCacheSize (1024*1024);
			break;

		case CT_CABLE_DSL_1M:
		case CT_CABLE_DSL_1_5M:
		case CT_CABLE_DSL_2M:
			m_aTUM [TUM_LIGHT].uMaxConns = 6;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 3;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 50000;
			m_aTUM [TUM_LIGHT].uMaxDlds = 3;

			m_aTUM [TUM_MEDIUM].uMaxConns = 8;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 3;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 150000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 3;

			m_aTUM [TUM_HEAVY].uMaxConns = 20;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 10;
			m_aTUM [TUM_HEAVY].uMaxDlds = 4;

			_App.MaxSections (10);
			_App.FileWriteCacheSize (3*1024*1024);
			break;

		case CT_LAN_10:
			m_aTUM [TUM_LIGHT].uMaxConns = 12;
			m_aTUM [TUM_LIGHT].uMaxConnsPS = 4;
			m_aTUM [TUM_LIGHT].uTrafficLimit = 100000;
			m_aTUM [TUM_LIGHT].uMaxDlds = 3;

			m_aTUM [TUM_MEDIUM].uMaxConns = 20;
			m_aTUM [TUM_MEDIUM].uMaxConnsPS = 5;
			m_aTUM [TUM_MEDIUM].uTrafficLimit = 400000;
			m_aTUM [TUM_MEDIUM].uMaxDlds = 4;

			m_aTUM [TUM_HEAVY].uMaxConns = 60;
			m_aTUM [TUM_HEAVY].uMaxConnsPS = 12;
			m_aTUM [TUM_HEAVY].uMaxDlds = 5;

			_App.MaxSections (12);
			_App.FileWriteCacheSize (3*1024*1024);
			break;
	}

	m_aTUM [TUM_HEAVY].uTrafficLimit = UINT_MAX;

	_App.NOW (enCT);
}

void fsDownloadsMgr::InitTUM()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	
	
	if (GetAdaptersInfo (pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	std::vector <fsConnectionType> venCT;
	
	if ((dwRetVal = GetAdaptersInfo (pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;

		while (pAdapter) 
		{
			switch (pAdapter->Type)
			{
			case MIB_IF_TYPE_ETHERNET: 
			case MIB_IF_TYPE_TOKENRING:
			case MIB_IF_TYPE_FDDI:
				venCT.push_back (CT_LAN_10);
				break;

			case MIB_IF_TYPE_LOOPBACK:
				venCT.push_back (CT_CABLE_DSL_1M);
				break;

			case MIB_IF_TYPE_PPP:
				venCT.push_back (CT_CABLE_DSL_1M);
				break;

			case MIB_IF_TYPE_OTHER:
				venCT.push_back (CT_CABLE_DSL_1M);
				break;				

			case MIB_IF_TYPE_SLIP:
				venCT.push_back (CT_MODEM_56_ISDN);
				break;
			}
			
			pAdapter = pAdapter->Next;
		}
	}

	free (pAdapterInfo);

	fsConnectionType enCT = venCT.size () ? CT_LAN_10 : CT_CABLE_DSL_1M;
	for (int i = 0; i < (int)venCT.size (); i++)
	{
		if (venCT [i] < enCT)
			enCT = venCT [i];
	}

	ApplyConnectionType (enCT);
}

BOOL fsDownloadsMgr::IsServerFilled(LPCSTR pszServer, DWORD dwReqProtocols)
{
	fsSiteInfo *site = _SitesMgr.FindSite2 (pszServer, dwReqProtocols);

	if (site == NULL)
		return FALSE;

	if (site->cConnsNow >= m_aTUM [m_enTUM].uMaxConnsPS || site->cConnsNow >= site->cMaxConns)
		return TRUE;

	return FALSE;
}

BOOL fsDownloadsMgr::IsMaxConnsReached()
{
	if (GetAmountConnections () >= m_aTUM [m_enTUM].uMaxConns)
		return TRUE;

	return FALSE;
}

void fsDownloadsMgr::ReadSettings()
{
	if (FALSE == _App.Get_TUM (m_aTUM))
		InitTUM ();

	m_aTUM [TUM_HEAVY].uTrafficLimit = UINT_MAX;

	m_enUserTUM = m_enTUM = _App.CurrentTUM ();
	m_enTUMManage = _App.TUMManage ();
	m_enDDR = _App.DDR ();
	m_bAutoDelCompleted = _App.AutoDelCompleted ();
	m_bDetLog = _App.DetLog ();

	m_bVirCheck = _App.Avir_Perform ();
	m_strVirName = _App.Avir_Name ();
	m_strVirArgs = _App.Avir_Args ();
	m_strVirExts = _App.Avir_Exts ();

	ReadMirrParameters ();
	ReadDeletedSettings ();

	m_histmgr.ReadSettings ();
}

int fsDownloadsMgr::GetRunningDownloadCount()
{
	int cRunning = 0;

	try {

		for (size_t i = 0; i < m_vDownloads.size (); i++)
			if (m_vDownloads [i]->pMgr->IsRunning ())
				cRunning ++;
	}
	catch (...) {}

	return cRunning;
}

void fsDownloadsMgr::_IECatchMgrEvents(fsIECatchMgrEvent ev, LPVOID lp)
{
	fsDownloadsMgr *pThis = (fsDownloadsMgr*) lp;

	switch (ev)
	{
		case ICME_DLSTARTED:
			switch (pThis->m_enTUMManage)
			{
				case TM_OFF:
				return;

				case TM_DECREASEBYONELEVEL:
					switch (pThis->m_enTUM)
					{
						case TUM_HEAVY:
							pThis->m_enTUM = TUM_MEDIUM;
							break;

						case TUM_MEDIUM:
							pThis->m_enTUM = TUM_LIGHT;
							break;
					}
				break;

				case TM_SETTOLIGHT:
					pThis->m_enTUM = TUM_LIGHT;
				break;
			}
		break;

		case ICME_DLCOMPLETED:
			if (pThis->m_enTUM != pThis->m_enUserTUM)
				pThis->m_enTUM = pThis->m_enUserTUM;
		break;
	}
}

void fsDownloadsMgr::AttachToIECatchMgr(fsIECatchMgr *mgr)
{
	mgr->SetEventsFunc (_IECatchMgrEvents, this);
}

void fsDownloadsMgr::SetTUMManage(fsTUMManage enManage)
{
	m_enTUMManage = enManage;
	if (enManage == TM_OFF) 
	{
		if (m_enTUM != m_enUserTUM)
			m_enTUM = m_enUserTUM; 
	}
}

fsTUMManage fsDownloadsMgr::GetTUMManage()
{
	return m_enTUMManage;
}

BOOL fsDownloadsMgr::IsDeletingNow()
{
	return m_bDeletingNow;
}

void fsDownloadsMgr::UseDetailedLog(BOOL b)
{
	m_bDetLog = b;
	for (int i = m_vDownloads.size () - 1; i >= 0; i--)
		m_vDownloads [i]->pMgr->UseDetailedLog (b);
}

int fsDownloadsMgr::DeleteDownloads2(DLDS_LIST *vDlds, BOOL bByUser, BOOL bDontConfirmFileDeleting, BOOL* pbNeedStop, int* piProgress)
{
	DLDS_LIST *pvDlds;
	fsnew1 (pvDlds, DLDS_LIST);
	UINT i = 0;
	fsDeleteDownloadReaction enDDR = m_enDDR;

	
	if (bDontConfirmFileDeleting && enDDR == DDR_ASK)
		enDDR = DDR_WITHFILE;	

	#ifdef _USELOGGING
	if (IsDeletingNow ())
		LOG ("Someone is deleting downloads already..." << nl);
	#endif

	
	while (IsDeletingNow ())
	{
		if (pbNeedStop && *pbNeedStop)
			break;

		
		MSG msg;
		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage (&msg);

		Sleep (50);
	}

	if (pbNeedStop && *pbNeedStop)
		return 0;

	LOG ("Start deleting..." << nl);

	m_bDeletingNow = TRUE;
	m_bDisablePD = TRUE;
	LockList ();

	LOG ("list locked, going into loop..." << nl);

	vmsDownloadSmartPtr dldFake;
	Download_CreateInstance (dldFake);
	dldFake->pMgr->GetDownloadMgr ()->CreateByUrl ("http://localhost/", TRUE);

	DeletedDownloads_UseTmpList (TRUE);

	try {

	_pwndDownloads->set_DontUpdateTIPO (TRUE);

	
	std::vector <size_t> vIndexes;
	size_t cDlds = vDlds->size ();
	size_t nStart = 0;	

	
	for (i = 0; i < cDlds; i++)
		vIndexes.push_back ((size_t)-1);

	
	for (i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];

		
		for (size_t j = nStart; j < cDlds; j++)
		{
			if (dld == vDlds->at (j))
			{				
				vIndexes [j] = i;

				
				for (size_t k = j; k > nStart && vIndexes [k - 1] != -1; k--)
					; 
				if (k == nStart)
					nStart = j + 1;	
				break;
			}
		}
	}
	

	
	BOOL bBypassDeleted = m_bBypassDeleted || bByUser == FALSE || bDontConfirmFileDeleting;

	for (i = 0; i < (UINT)vDlds->size (); i++)
	{
		
		
		if (pbNeedStop && *pbNeedStop)
			break;

		vmsDownloadSmartPtr dld = vDlds->at (i);

		int iIndex = vIndexes [i];
		if (iIndex == -1) 
			continue;

		BOOL bDone = dld->pMgr->IsDone ();

		if (bBypassDeleted && dld->pMgr->IsFileInitializedOnDisk ())
		{
			
			
			if (bDontConfirmFileDeleting || bDone == FALSE)
				if (FALSE == DeleteDownloadFile (dld, FALSE, enDDR, TRUE))
					break;
		}

		if (dld->pdlg)
			Event (dld, DME_CLOSEDLDDIALOG);

		if (piProgress)
			*piProgress = (int) ((double)i / vDlds->size () * 100);

		if (dld->pMgr->GetDownloadMgr ())
		{
			dld->pMgr->GetDownloadMgr ()->SetEventDescFunc (NULL, 0);
			dld->pMgr->GetDownloadMgr ()->SetEventFunc (NULL, 0);
		}
		else if (dld->pMgr->GetBtDownloadMgr ())
		{
			dld->pMgr->GetBtDownloadMgr ()->SetEventsHandler (NULL, 0);
		}

		Event (dld, DME_DOWNLOADWILLBEDELETED);
		
		m_vDownloads [iIndex] = dldFake;
		dld->pGroup->cDownloads--;

		Event (dld, DME_DOWNLOADWASDELETEDFROMLIST);
		dld->pfnDownloadEventsFunc = NULL;

		if (FALSE == bBypassDeleted && 
			(m_bBypassCopleted == FALSE || bDone == FALSE))
		{
			dld = PutDownloadToDeleted (dld);
			if (dld && dld->pMgr->IsFileInitializedOnDisk ())
				DeleteDownloadFile (dld, FALSE, enDDR, TRUE);
		}
		else
		{
			Event (dld, DME_DLDWILLBEFULLYDELETED);
			if ((dld->dwFlags & DLD_DONTPUTTOHISTORY) == 0 && bDone == FALSE)
				m_histmgr.AddToHistory (dld);
		}
		
		if (dld)
			pvDlds->push_back (dld);
	}

	}
	catch (...) {}

	int cDeleted = i;

	DeletedDownloads_UseTmpList (FALSE);

	

	DLDS_LIST v;
	for (i = 0; i < m_vDownloads.size (); i++)
	{
		if (m_vDownloads [i] != dldFake)
			v.push_back (m_vDownloads [i]);
	}
	m_vDownloads = v;	

	_pwndDownloads->set_DontUpdateTIPO (FALSE);

	LOG ("downloads deleted from list" << nl);

	m_bDeletingNow = FALSE;
	m_bDisablePD = FALSE;

	_pwndDownloads->UpdateTrayIconPlusOthers ();
	_pwndDownloads->UpdateNumbersOfDownloadsInGroups ();

	if (pvDlds->size ())
	{
		DWORD dw;
		CloseHandle (CreateThread (NULL, 0, _threadDeleteDownload, pvDlds, 0, &dw));
	}
	else
	{
		delete pvDlds;
	}

	ProcessDownloads ();

	UnlockList ();

	return cDeleted;	
}

DWORD WINAPI fsDownloadsMgr::_threadWaitDelDlds(LPVOID lp)
{
	ProcWaitInfo *info = (ProcWaitInfo*) lp;
	DLDS_LIST *vDlds =  (DLDS_LIST*) info->lpParam1;
	BOOL bByUser = *((BOOL*) info->lpParam2);
	BOOL bDontConfirmFileDeleting = *((BOOL*) info->lpParam3);
	int *pcDlds = (int*) info->lpParam4;
	fsDownloadsMgr* pThis = (fsDownloadsMgr*) info->lpParam5;

	pThis->Event (NULL, DME_BEGINDELETEMANYDOWNLOADS);
	*pcDlds = pThis->DeleteDownloads2 (vDlds, bByUser, bDontConfirmFileDeleting, &info->bNeedStop, &info->iProgress);
	pThis->Event (NULL, DME_ENDDELETEMANYDOWNLOADS);

	info->bWaitDone = TRUE;

	return 0;
}

void fsDownloadsMgr::Download_CloneSettings(vmsDownloadSmartPtr dst, vmsDownloadSmartPtr src)
{
	dst->bAutoStart = src->bAutoStart;
	dst->dwFlags = src->dwFlags;
	dst->dwReserved = src->dwReserved;
	dst->strComment = src->strComment;
	dst->pGroup = src->pGroup;
	if (dst->pMgr && src->pMgr)
	{
		ASSERT (dst->pMgr->GetDownloadMgr () != NULL && src->pMgr->GetDownloadMgr () != NULL);
		dst->pMgr->GetDownloadMgr ()->CloneSettings (src->pMgr->GetDownloadMgr ());
	}
}

BOOL fsDownloadsMgr::PerformVirusCheck(vmsDownloadSmartPtr dld, BOOL bCheckExtReqs, BOOL bWaitDone)
{
	if (m_strVirName == "")
		return TRUE;

	bool bMultiTorrent = dld->pMgr->GetBtDownloadMgr () != NULL && 
		dld->pMgr->GetBtDownloadMgr ()->get_FileCount () != 1;

	bool bMultiTorrentCheck = bMultiTorrent && bCheckExtReqs;

	int nFiles = bMultiTorrentCheck ? dld->pMgr->GetBtDownloadMgr ()->get_FileCount () : 1;

	fsString strFile;

	for (int i = 0; i < nFiles; i++)
	{
		char szFile [MY_MAX_PATH];
		
		if (bMultiTorrentCheck)
			strFile = dld->pMgr->GetBtDownloadMgr ()->get_OutputFilePathName (i);
		else
			strFile = dld->pMgr->get_OutputFilePathName ();

		fsGetFileName (strFile, szFile);

		if (bCheckExtReqs)
		{
			char *pszExt = strrchr (strFile, '.');

			
			if (pszExt && IsExtInExtsStr (m_strVirExts, pszExt+1))
				break;	
		}
		else
			break;
	}
	if (i == nFiles)
		return TRUE; 

	
	

	Event (dld, LS (L_LAUNCHAVIR), EDT_INQUIRY);
	
	CString strArgs = m_strVirArgs;
	CString str = '"'; str += strFile; str += '"';
	strArgs.Replace ("%file%", str);

	if (bWaitDone == FALSE)
	{
		DWORD dwErr = (DWORD) ShellExecute (HWND_DESKTOP, "open", m_strVirName, strArgs, NULL, SW_SHOW);
		if (dwErr <= 32)
		{
			SetLastError (dwErr);
			goto _lErr;
		}	
	}
	else
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory (&si, sizeof (si));
		si.cb = sizeof (si);
		ZeroMemory (&pi, sizeof (pi));

		fsString str = m_strVirName;
		if (strchr (str, '\\') == NULL && strchr (str, '/') == NULL)
			str = vmsRegisteredApp::GetFullPath (str);

		CString strCmdLine; 
		strCmdLine.Format ("\"%s\" %s", (LPCSTR)str, (LPCSTR)strArgs);

		if (FALSE == CreateProcess (NULL, (LPSTR)(LPCSTR)strCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			goto _lErr;

		WaitForSingleObject (pi.hProcess, INFINITE);
	}

	Event (dld, LS (L_SUCCESS), EDT_RESPONSE_S);
	return TRUE;

_lErr:
	CHAR szErr [1000]; DWORD dwErr = GetLastError ();
	fsErrorToStr (szErr, sizeof (szErr), &dwErr);
	Event (dld, szErr, EDT_RESPONSE_E);
	return FALSE;
}

void fsDownloadsMgr::Apply_MirrParameters()
{
	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];
		if (dld->pMgr->GetDownloadMgr () != NULL)
			Apply_MirrParameters (dld);
	}
}

void fsDownloadsMgr::ReadMirrParameters()
{
	m_bMirrAutoSearch = _App.Mirr_AutoSearch ();
	m_uMirrFileMinSize = _App.Mirr_FileMinSize ();
	m_bMirrCalcSpeed = _App.Mirr_CalcSpeed ();
	m_cMaxMirrs = _App.Mirr_MaxCount ();
	m_uMirrRecalcSpeedTime = _App.Mirr_RecalcSpeedTime ();
	m_bMirrDontSearchIfSizeUnk = _App.Mirr_DontSearchIfSizeUnk ();
}

void fsDownloadsMgr::Apply_MirrParameters(vmsDownloadSmartPtr dld)
{
	ASSERT (dld->pMgr->GetDownloadMgr () != NULL);

	fsInternetDownloader *dldr = dld->pMgr->GetDownloadMgr ()->GetDownloader ();

	if ((dld->dwFlags & DLD_DONTUSEMIRRORS) == 0)
	{
		dldr->Set_SearchForMirrors (m_bMirrAutoSearch);
		dldr->Set_MirrDontSearchIfSizeUnk (m_bMirrDontSearchIfSizeUnk);
		dldr->Set_MirrFileMinSize (m_uMirrFileMinSize);
	}
	else
		dldr->Set_SearchForMirrors (FALSE);

	dldr->Set_MirrCalcSpeed (m_bMirrCalcSpeed);
	dldr->Set_MirrMaxCount (m_cMaxMirrs);
	dld->pMgr->GetDownloadMgr ()->Set_MirrRecalcSpeedTime (m_uMirrRecalcSpeedTime);
}

DWORD fsDownloadsMgr::OnBeforeDownload(vmsDownloadSmartPtr dld)
{
	BOOL bOK = TRUE;

	ASSERT (dld->pMgr->GetDownloadMgr ());

	if (dld->dwFlags & DLD_CTREQ_HTML)
	{
		LPCSTR pszType = dld->pMgr->GetDownloadMgr ()->GetDownloader ()->GetContentType ();

		if(strstr (pszType, "html") == NULL)
			bOK = FALSE;
	}

	if (bOK && dld->pfnDownloadEventsFunc && (dld->dwFlags & DLD_NOTIFYBEFOREDOWNLOAD))
		bOK = dld->pfnDownloadEventsFunc (dld, DME_BEFOREDOWNLOADING, dld->lpEventsParam);

	if (bOK == FALSE)
	{
		DLDS_LIST vpDlds;
		vpDlds.push_back (dld);
		DeleteDownloads (vpDlds, FALSE, TRUE);
	}

	return bOK;
}

int fsDownloadsMgr::Get_DeletedDownloadCount()
{
	return m_vDeletedDownloads.size () ? 
		m_vDeletedDownloads.size () : m_vDeletedDownloads_tmp.size ();
}

vmsDownloadSmartPtr fsDownloadsMgr::Get_DeletedDownload(int iIndex)
{
	if (m_vDeletedDownloads.size ()) 
	{
		return m_vDeletedDownloads [iIndex];
	}
	else
	{
		std::list <vmsDownloadSmartPtr>::iterator iter = m_vDeletedDownloads_tmp.begin ();
		while (iIndex--)
			iter++;
		return *iter;
	}
}

void fsDownloadsMgr::ReadDeletedSettings()
{
	ASSERT (m_vDeletedDownloads_tmp.size () == 0);

	m_bBypassDeleted = _App.Deleted_Bypass ();
	m_cMaxDeleted = _App.Deleted_MaxCount ();
	m_bBypassCopleted = _App.Deleted_BypassCompleted ();

	DLDS_LIST v;

	if (m_bBypassDeleted)
	{
		v = m_vDeletedDownloads;
	}
	else if (UINT (m_vDeletedDownloads.size ()) > m_cMaxDeleted)
	{
		for (size_t i = m_cMaxDeleted; i < m_vDeletedDownloads.size (); i++)
			v.push_back (m_vDeletedDownloads [i]);
	}

	if (v.size ())
		DeleteDeletedDownloads (v, TRUE);
}

vmsDownloadSmartPtr fsDownloadsMgr::PutDownloadToDeleted(vmsDownloadSmartPtr dld)
{
	ASSERT (m_vDeletedDownloads.size () == 0);

	dld->pMgr->StopDownloading ();
	m_vDeletedDownloads_tmp.insert (m_vDeletedDownloads_tmp.begin (), dld);
	Event (dld, DME_DLDADDEDTODELETED);

	vmsDownloadSmartPtr ret;

	UINT sz = m_vDeletedDownloads_tmp.size ();
	if (sz > m_cMaxDeleted)
	{
		std::list <vmsDownloadSmartPtr>::iterator it = m_vDeletedDownloads_tmp.end ();
		it--;
		ret = *it;
		m_vDeletedDownloads_tmp.erase (it);
		Event (ret, DME_DLDREMOVEDFROMDELETED);
		Event (ret, DME_DLDWILLBEFULLYDELETED);
		if ((ret->dwFlags & DLD_DONTPUTTOHISTORY) == 0 &&
				ret->pMgr->IsDone () == FALSE)
			m_histmgr.AddToHistory (ret);
	}

	return ret;
}

BOOL fsDownloadsMgr::DeleteDownloadFile(vmsDownloadSmartPtr dld, BOOL bNoCancel, fsDeleteDownloadReaction &enDoDDR, BOOL bDownloadWillBeDeletedAlso)
{
	fsDeleteDownloadReaction enDDR = enDoDDR;
	
	if (dld->dwFlags & DLD_DELETEFILEALWAYS)
		enDDR = DDR_WITHFILE;

	if (enDDR == DDR_ASK)
	{
		LOG ("asking user permission for deleting this download..." << nl);

		CDDRDlg dlg;
		dlg.m_bNoCancel = bNoCancel;
		if (dld->pMgr->GetDownloadMgr ())
			dlg.m_strUrl = dld->pMgr->get_URL ();
		else if (dld->pMgr->GetBtDownloadMgr ())
			dlg.m_strUrl = dld->pMgr->GetBtDownloadMgr ()->get_TorrentName ();
		if (IDCANCEL == dlg.DoModal ())
		{
			LOG ("user has declined that" << nl);
			return FALSE;
		}

		LOG ("user has approved" << nl);
		
		if (dlg.m_bDontAskAgain) m_enDDR = enDoDDR = dlg.m_enDDR;
		if (dlg.m_bForAll) enDoDDR = dlg.m_enDDR;
		enDDR = dlg.m_enDDR;
	}
		
	if (enDDR == DDR_WITHFILE)
	{
		LOG ("deleting file..." << nl);

		if (dld->pMgr->GetDownloadMgr ())
		{
			dld->pMgr->GetDownloadMgr ()->SetEventDescFunc (NULL, 0);
			dld->pMgr->GetDownloadMgr ()->SetEventFunc (NULL, 0);
			dld->pMgr->GetDownloadMgr ()->CloseFile ();
		}
		else if (dld->pMgr->GetBtDownloadMgr ())
		{
			dld->pMgr->GetBtDownloadMgr ()->SetEventsHandler (NULL, 0);
		}
		
		Event (dld, DME_DOWNLOADFILEWILLBEDELETED);
		if (FALSE == dld->pMgr->DeleteFile ())
		{
			LOG ("can't delete" << nl);
			MessageBox (NULL, LS (L_CANTDELETE), LS (L_ERR), MB_ICONERROR);
		}

		LOG ("file deleted" << nl);
	}
	
	return TRUE;
}

void fsDownloadsMgr::ClearDeleted()
{
	ASSERT (m_vDeletedDownloads_tmp.size () == 0);

	if (m_vDeletedDownloads.size () == 0)
		return;

	DLDS_LIST v;

	v = m_vDeletedDownloads;

	DeleteDeletedDownloads (v, FALSE);
}

int fsDownloadsMgr::DeleteDeletedDownloads(DLDS_LIST &vDlds, BOOL bNoCancel)
{
	size_t cMaxDlds = 40;

	
	
	if (vDlds.size () < cMaxDlds)
		return DeleteDeletedDownloads2 (&vDlds, bNoCancel, NULL, NULL);

	

	int iDeleted = 0;
	
	CWaitDlg dlg;
	dlg.StartWaiting (LS (L_DELETINGDLDS), _threadWaitDelDeletingsDlds, TRUE, &vDlds, &bNoCancel, &iDeleted, this);

	return iDeleted;
}

int fsDownloadsMgr::DeleteDeletedDownloads2(DLDS_LIST *vDlds, BOOL bNoCancel, BOOL* pbNeedStop, int* piProgress)
{
	DLDS_LIST *pvDlds;
	fsnew1 (pvDlds, DLDS_LIST);
	UINT i = 0;
	fsDeleteDownloadReaction enDDR = m_enDDR;

	#ifdef _USELOGGING
	if (IsDeletingDeletedNow ())
		LOG ("Someone is deleting downloads already..." << nl);
	#endif

	
	while (IsDeletingDeletedNow ())
	{
		if (pbNeedStop && *pbNeedStop)
			break;

		
		MSG msg;
		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage (&msg);

		Sleep (50);
	}

	if (pbNeedStop && *pbNeedStop)
		return 0;

	m_bDeletingDeletedNow = TRUE;

	vmsDownloadSmartPtr dldFake;
	Download_CreateInstance (dldFake);
	dldFake->pMgr->GetDownloadMgr ()->CreateByUrl ("http://localhost/", TRUE);

	try {

	ASSERT (m_vDeletedDownloads.size () != 0);

	
	std::vector <size_t> vIndexes;
	size_t cDlds = vDlds->size ();
	size_t nStart = 0;	
	

	for (i = 0; i < cDlds; i++)
		vIndexes.push_back ((size_t)-1);

	for (i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDeletedDownloads [i];

		for (size_t j = nStart; j < cDlds; j++)
		{
			if (dld == vDlds->at (j))
			{				
				vIndexes [j] = i;

				for (size_t k = j; k > nStart && vIndexes [k - 1] != -1; k--)
					;
				if (k == nStart)
					nStart = j + 1;
				break;
			}
		}

	}
	

	for (i = 0; i < (UINT)vDlds->size (); i++)
	{
		
		
		if (pbNeedStop && *pbNeedStop)
			break;

		vmsDownloadSmartPtr dld = vDlds->at (i);

		int iIndex = vIndexes [i];
		if (iIndex == -1)
			continue;

		if (dld->pMgr->IsDone () == FALSE && dld->pMgr->IsFileInitializedOnDisk ())
		{
			if (FALSE == DeleteDownloadFile (dld, bNoCancel, enDDR, TRUE))
				break;
		}

		if (piProgress)
			*piProgress = (int) ((double)i / vDlds->size () * 100);

		Event (dld, DME_DLDWILLBEFULLYDELETED);
		if ((dld->dwFlags & DLD_DONTPUTTOHISTORY) == 0 &&
				dld->pMgr->IsDone () == FALSE)
			m_histmgr.AddToHistory (dld);

		m_vDeletedDownloads [iIndex] = dldFake;

		Event (dld, DME_DLDREMOVEDFROMDELETED);
			
		pvDlds->push_back (dld);
	}

	}
	catch (...) {}

	int cDeleted = i;

	DLDS_LIST v;
	for (i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		if (m_vDeletedDownloads [i] != dldFake)
			v.push_back (m_vDeletedDownloads [i]);
	}
	m_vDeletedDownloads = v;	

	Event (NULL, DME_RECYCLEBINCONTENTCHANGED);

	m_bDeletingDeletedNow = FALSE;

	if (pvDlds->size ())
	{
		DWORD dw;
		CloseHandle (CreateThread (NULL, 0, _threadDeleteDownload, pvDlds, 0, &dw));
	}
	else
	{
		delete pvDlds;
	}

	return cDeleted;	
}

BOOL fsDownloadsMgr::IsDeletingDeletedNow()
{
	return m_bDeletingDeletedNow;
}

int fsDownloadsMgr::GetDeletedDownloadIndex(vmsDownloadSmartPtr dld)
{
	for (size_t i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		if (m_vDeletedDownloads [i] == dld)
			return i;
	}

	return -1;
}

DWORD WINAPI fsDownloadsMgr::_threadWaitDelDeletingsDlds(LPVOID lp)
{
	ProcWaitInfo *info = (ProcWaitInfo*) lp;
	DLDS_LIST *vDlds =  (DLDS_LIST*) info->lpParam1;
	BOOL bNoCancel = *(BOOL*) info->lpParam2;
	int *pcDlds = (int*) info->lpParam3;
	fsDownloadsMgr* pThis = (fsDownloadsMgr*) info->lpParam4;

	*pcDlds = pThis->DeleteDeletedDownloads2 (vDlds, bNoCancel, &info->bNeedStop, &info->iProgress);

	info->bWaitDone = TRUE;

	return 0;
}

int fsDownloadsMgr::RestoreDownloads(DLDS_LIST &vDlds)
{
	size_t cMaxDlds = 40;

	if (vDlds.size () < cMaxDlds)
		return RestoreDownloads2 (&vDlds, NULL, NULL);

	

	int iRestored = 0;
	
	CWaitDlg dlg;
	dlg.StartWaiting (LS (L_RESTORINGDLDS), _threadWaitRestoringDlds, TRUE, &vDlds, &iRestored, this);

	return iRestored;
}

int fsDownloadsMgr::RestoreDownloads2(DLDS_LIST* vDlds, BOOL *pbNeedStop, int *piProgress)
{
	UINT i = 0;

	
	while (IsRestoringNow ())
	{
		if (pbNeedStop && *pbNeedStop)
			break;

		
		MSG msg;
		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage (&msg);

		Sleep (50);
	}

	if (pbNeedStop && *pbNeedStop)
		return 0;

	m_bRestoringNow = TRUE;

	vmsDownloadSmartPtr dldFake;
	Download_CreateInstance (dldFake);
	dldFake->pMgr->GetDownloadMgr ()->CreateByUrl ("http://localhost/", TRUE);

	try {

	ASSERT (m_vDeletedDownloads.size () != 0);

	
	std::vector <size_t> vIndexes;
	size_t cDlds = vDlds->size ();
	size_t nStart = 0;	
	

	for (i = 0; i < cDlds; i++)
		vIndexes.push_back ((size_t)-1);

	for (i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		vmsDownloadSmartPtr dld = m_vDeletedDownloads [i];

		for (size_t j = nStart; j < cDlds; j++)
		{
			if (dld == vDlds->at (j))
			{				
				vIndexes [j] = i;

				for (size_t k = j; k > nStart && vIndexes [k - 1] != -1; k--)
					;
				if (k == nStart)
					nStart = j + 1;
				break;
			}
		}

	}
	

	for (i = 0; i < (UINT)vDlds->size (); i++)
	{
		
		
		if (pbNeedStop && *pbNeedStop)
			break;

		vmsDownloadSmartPtr dld = vDlds->at (i);

		int iIndex = vIndexes [i];
		if (iIndex == -1)
			continue;

		if (dld->pGroup->bAboutToBeDeleted)
			dld->pGroup = _DldsGrps.FindGroup (GRP_OTHER_ID);

		m_vDeletedDownloads [iIndex] = dldFake;
		Event (dld, DME_DLDREMOVEDFROMDELETED);

		Add (dld, TRUE);
		if (dld->dwFlags & DLD_FLASH_VIDEO)
			_pwndFVDownloads->AddDownload (dld, FALSE);
		if (_pwndTorrents && dld->isTorrent ())
			_pwndTorrents->AddDownload (dld, FALSE);

		Event (dld, DME_DLDRESTORED);

		if (piProgress)
			*piProgress = (int) ((double)i / vDlds->size () * 100);
	}

	}
	catch (...) {}

	int cRestored = i;

	DLDS_LIST v;
	for (i = 0; i < m_vDeletedDownloads.size (); i++)
	{
		if (m_vDeletedDownloads [i] != dldFake)
			v.push_back (m_vDeletedDownloads [i]);
	}
	m_vDeletedDownloads = v;	

	Event (NULL, DME_RECYCLEBINCONTENTCHANGED);

	m_bRestoringNow = FALSE;

	if (cRestored)
		ProcessDownloads ();

	return cRestored;	
}

BOOL fsDownloadsMgr::IsRestoringNow()
{
	return m_bRestoringNow;
}

DWORD WINAPI fsDownloadsMgr::_threadWaitRestoringDlds(LPVOID lp)
{
	ProcWaitInfo *info = (ProcWaitInfo*) lp;
	DLDS_LIST *vDlds =  (DLDS_LIST*) info->lpParam1;
	int *pcDlds = (int*) info->lpParam2;
	fsDownloadsMgr* pThis = (fsDownloadsMgr*) info->lpParam3;

	*pcDlds = pThis->RestoreDownloads2 (vDlds, &info->bNeedStop, &info->iProgress);

	info->bWaitDone = TRUE;

	return 0;
}

void fsDownloadsMgr::_HistoryMgrEvents(fsDownloadsHistoryMgrEvent ev, int info, LPVOID lp)
{
	fsDownloadsMgr* pThis = (fsDownloadsMgr*) lp;
	fsDLHistoryRecord* rec = (fsDLHistoryRecord*) info;

	switch (ev)
	{
		case DHME_RECORDADDED:
			pThis->Event (rec, DME_RECADDEDTOHISTORY);
			break;

		case DHME_RECORDDELETED:
			pThis->Event (rec, DME_RECDELETEDFROMHISTORY);
			break;

		case DHME_HISTORYCLEARED:
			pThis->Event (rec, DME_HISTORYCLEARED);
			break;
	}
}

void fsDownloadsMgr::Event(fsDLHistoryRecord *rec, fsDownloadsMgrEvent ev)
{
	if (m_pfnEvents)
		m_pfnEvents ((fsDownload*)rec, ev, m_lpEventsParam);
}

struct _threadArchiveDetectedInfo
{
	fsArchiveRebuilder *ar;
	BOOL* pbRes;
};

DWORD fsDownloadsMgr::OnArchiveDetected(vmsDownloadSmartPtr dld, fsArchiveRebuilder *ar)
{
	BOOL b;
	UIThread *thr = (UIThread*) RUNTIME_CLASS (UIThread)->CreateObject ();
	_threadArchiveDetectedInfo inf;
	inf.ar = ar; inf.pbRes = &b;
	thr->set_Thread (_threadArchiveDetected, &inf);
	thr->CreateThread ();
	WaitForSingleObject (thr->m_hThread, INFINITE);

	if (b == FALSE)
		dld->bAutoStart = FALSE;

	return b;
}

void fsDownloadsMgr::MoveDownloadToEndOfList(vmsDownloadSmartPtr dld)
{
	int iIndex = GetDownloadIndex (dld);
	if (iIndex != -1)
	{
		LockList ();
		DownloadsList_Del (iIndex);
		DownloadsList_Add (dld);
		UnlockList ();
	}
}

DWORD WINAPI fsDownloadsMgr::_threadConfirmZIP(LPVOID lp)
{
	CWaitForConfirmationDlg dlg;
	UINT* pnRes = (UINT*) lp;

	dlg.Init (LS (L_ZIPDETECTED), 30, TRUE, TRUE, LS (L_DONTPREVIEWINFUTURE), LS (L_YOUTURNEDOFFPREVIEW));

	*pnRes = _DlgMgr.DoModal (&dlg) == IDOK;

	if (dlg.m_bDontAsk)
		_App.NewDL_UseZIPPreview (FALSE);

	return 0;
}

DWORD WINAPI fsDownloadsMgr::_threadArchiveDetected(LPVOID lp)
{
	CZipPreviewDlg dlg;
	_threadArchiveDetectedInfo* inf = (_threadArchiveDetectedInfo*) lp;

	dlg.m_ar = inf->ar;
	*inf->pbRes = _DlgMgr.DoModal (&dlg) == IDOK;

	return 0;
}

void fsDownloadsMgr::Initialize()
{
	ReadSettings ();

	InterlockedIncrement (&m_cThreadsRunning);
	
	DWORD dw;
	CloseHandle (
		CreateThread (NULL, 0, _threadDownloadsMgr, this, 0, &dw));
}

void fsDownloadsMgr::LaunchDownload(vmsDownloadSmartPtr dld, UINT nWaitForConfirmation)
{
	if (nWaitForConfirmation != 0)
	{
		CWaitForConfirmationDlg dlg;
		char szFile [MY_MAX_PATH];
		if (dld->pMgr->GetDownloadMgr ())
			fsGetFileName (dld->pMgr->get_OutputFilePathName (), szFile);
		else if (dld->pMgr->GetBtDownloadMgr ())
			lstrcpy (szFile, dld->pMgr->GetBtDownloadMgr ()->get_TorrentName ());
		CString strMsg = "\""; strMsg += szFile; strMsg += "\" ";
		if (dld->strComment != "") {
			strMsg += '('; strMsg += dld->strComment; strMsg += ") ";
		}
		strMsg += LS (L_WASSCHEDULEDTOLAUNCH); strMsg += "\n";
		strMsg += LS (L_DOYOUWANTTOLAUNCHIT);
		dlg.Init (strMsg, nWaitForConfirmation);
		if (IDCANCEL == dlg.DoModal ())
			return;
	}

	CString strFileName = dld->pMgr->get_OutputFilePathName ();
	if (dld->pMgr->IsBittorrent ())
		strFileName += dld->pMgr->GetBtDownloadMgr ()->get_RootFolderName ();

	ShellExecute (::GetDesktopWindow (), "open", strFileName, NULL, NULL, SW_SHOW);
}

void fsDownloadsMgr::Shutdown()
{
	m_bNeedExit = TRUE;
	while (m_cThreadsRunning)
		Sleep (10);

	if (_BT.is_Initialized ())
		_BT.get_Session ()->set_EventsHandler (NULL, NULL);
	
	m_vDownloads.clear ();

	if (_BT.is_Initialized ())
		_BT.Shutdown ();

	DeleteCriticalSection (&m_csQSection);
	
	DeleteCriticalSection (&m_csSave);
}

void fsDownloadsMgr::ApplyAER(fsAlreadyExistReaction enAER)
{
	LockList ();
	for (int i = m_vDownloads.size () - 1; i >= 0; i--)
	{
		if (m_vDownloads [i]->pMgr->GetDownloadMgr () != NULL)
			m_vDownloads [i]->pMgr->GetDownloadMgr ()->GetDP ()->enAER = enAER;
	}
	UnlockList ();
}

BOOL fsDownloadsMgr::GenerateDescriptionFile(vmsDownloadSmartPtr dld)
{
	if (dld->strComment.GetLength () == 0)
		return FALSE;

	CString str = dld->strComment;
	str.Replace ("\r", "");

	str += "\n\n"; str += LS (L_THISFILEWASDLDEDFROM); str += ":\n";
	if (dld->pMgr->GetDownloadMgr ())
	{
		str += dld->pMgr->get_URL ();
	}
	else if (dld->pMgr->GetBtDownloadMgr ())
	{
		str += dld->pMgr->GetBtDownloadMgr ()->get_TorrentName ();
		str += " (";
		str += dld->pMgr->get_URL ();
		str += ")";
	}

	str += "\n\n"; str += LS (L_DATE); str += ": ";
	
	FILETIME time;
	SYSTEMTIME systime;
	GetLocalTime (&systime);
	SystemTimeToFileTime (&systime, &time);

	char szDate [1000], szTime [1000];
	FileTimeToStr (&time, szDate, szTime, FALSE);
	str += szDate; str += ", "; str += szTime;

	CStdioFile file;
	CString strName = dld->pMgr->get_OutputFilePathName ();
	if (dld->pMgr->GetBtDownloadMgr () != NULL && 
			dld->pMgr->GetBtDownloadMgr ()->get_FileCount () != 1)
	{
		strName += "\\"; 
		strName += dld->pMgr->GetBtDownloadMgr ()->get_TorrentName ();
	}
	strName += ".dsc.txt";

	file.Open (strName, CFile::typeText | CFile::modeCreate | CFile::modeWrite);
	file.WriteString (str);
	file.Close ();

	return TRUE;
}

BOOL fsDownloadsMgr::LoadStateInformation()
{
	CString strFileName = fsGetDataFilePath ("dlmgrsi.sav");

	if (GetFileAttributes (strFileName) == DWORD (-1))
		return TRUE;

	HANDLE hFile  = CreateFile (strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;

	fsStateInfoFileHdr hdr;
	if (FALSE == ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	BOOL bOldFormat = FALSE;

	if (strncmp (hdr.szSig, DLMGRFILE_SIG, sizeof (DLMGRFILE_SIG)))
	{
		
		
		bOldFormat = TRUE;
		SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
	}

	UINT u;
	if (FALSE == ReadFile (hFile, &u, sizeof (u), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	m_nID = u;

	if (bOldFormat)
	{
		vmsStringRecentList vFilePathes;
		if (FALSE == vFilePathes.Load (hFile))
		{
			CloseHandle (hFile);
			return FALSE;
		}

		for (int i = 0; i < vFilePathes.get_Count (); i++)
		{
			char sz [MY_MAX_PATH] = "";
			fsGetFileName (vFilePathes.get_String (i), sz);
			m_LastFilesDownloaded.Add (sz, vFilePathes.get_String (i));
		}
	}
	else
	{
		if (FALSE == m_LastFilesDownloaded.Load (hFile))
		{
			CloseHandle (hFile);
			return FALSE;
		}
	}

	CloseHandle (hFile);

	return TRUE;
}

BOOL fsDownloadsMgr::SaveStateInformation()
{
	CString strFileName = fsGetDataFilePath ("dlmgrsi.sav");

	HANDLE hFile = CreateFile (strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw;

	fsStateInfoFileHdr hdr;
	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	if (FALSE == WriteFile (hFile, &m_nID, sizeof (m_nID), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	if (FALSE == m_LastFilesDownloaded.Save (hFile))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	CloseHandle (hFile);

	return TRUE;
}

UINT fsDownloadsMgr::GetNextDownloadID()
{
	return m_nID;
}

vmsFileRecentList* fsDownloadsMgr::get_LastFilesDownloaded()
{
	return &m_LastFilesDownloaded;
}

void fsDownloadsMgr::LastFilesDownloaded_Clear()
{
	m_LastFilesDownloaded.Clear ();
}

void fsDownloadsMgr::MoveDownloads(DLDS_LIST_REF vDlds, vmsDownloadSmartPtr pDldPosition, bool bUp)
{
	LockList ();

	size_t nIndexPos = GetDownloadIndex (pDldPosition);

	for (size_t i = 0; i < vDlds.size (); i++)
	{
		size_t nIndex = GetDownloadIndex (vDlds [i]);
		if (nIndex == -1) 
		{
			vDlds.erase (vDlds.begin () + i--);
			continue;
		}
		DownloadsList_Del (nIndex);
	}

	size_t n = GetDownloadIndex (pDldPosition);
	if (n != -1)
		nIndexPos = n;
	if (bUp == false)
		nIndexPos++;
	if (nIndexPos > m_vDownloads.size ())
		nIndexPos = m_vDownloads.size ();

	for (i = 0; i < vDlds.size (); i++)
		DownloadsList_Insert (nIndexPos++, vDlds [i]);

	UnlockList ();
}

void fsDownloadsMgr::ReorderDownloads(DLDS_LIST_REF vDlds, DLDS_LIST_REF vReqOrder, bool bDldsSorted)
{
	if (vDlds.size () != vReqOrder.size ())
		return;	

	LockList ();

	
	fs::list <int> vIndxs;
	
	
	size_t nPrevIndx = (size_t)-1; 
	for (size_t i = 0; i < vDlds.size (); i++) 
	{
		if (m_vDownloads [nPrevIndx + 1] == vDlds [i]) {
			vIndxs.add (++nPrevIndx);	
		}
		else {
			
			
			
			vIndxs.add (nPrevIndx = GetDownloadIndex (vDlds [i], bDldsSorted ? nPrevIndx + 1 : 0));
			if (nPrevIndx == m_vDownloads.size () - 1)
				nPrevIndx = (size_t)-1;
		}
	}

	

	for (i = 0; i < vReqOrder.size (); i++)
	{
		size_t nNewIndx = vIndxs [i];	
		if (nNewIndx == (size_t)-1)
			continue;

		size_t nOldIndx;	

		

		vmsDownloadSmartPtr dld = vReqOrder [i];
		for (size_t j = 0; j < vDlds.size (); j++){
			if (dld == vDlds [j])
				break;
		}
		if (j == vDlds.size ())
			continue;	
		nOldIndx = vIndxs [j];
		if (nOldIndx == (size_t)-1)
			continue;

		

		vIndxs [i] = (size_t)-1;	
		vIndxs [j] = (size_t)-1;	

		if (nNewIndx != nOldIndx)
		{
			

			vmsDownloadSmartPtr dldTmp = m_vDownloads [nNewIndx];
			m_vDownloads [nNewIndx] = dld;
			m_vDownloads [nOldIndx] = dldTmp;
		}
	}

	UnlockList ();
}

DWORD WINAPI fsDownloadsMgr::_threadIntegrityCheckAndVirCheckAndLaunch(LPVOID lp)
{
	vmsDownloadSmartPtr dld = (fsDownload*) lp;
	dld->Release (); 

	bool bNeedLaunchDld = (dld->dwFlags & DLD_NOAUTOLAUNCH) == 0 && 
		dld->pMgr->IsLaunchWhenDone ();
	bool bNeedIntegrityCheck = dld->pMgr->GetDownloadMgr () != NULL && 
		dld->pMgr->GetDownloadMgr ()->GetDP ()->bCheckIntegrityWhenDone && 
		*dld->pMgr->GetDownloadMgr ()->GetDP ()->pszCheckSum;

	if (bNeedIntegrityCheck)
	{
		_DldsMgr.Event (dld, LS (L_PERFORMINGINTEGRITYCHECK), EDT_INQUIRY);

		fsDownload_Properties *dp = dld->pMgr->GetDownloadMgr ()->GetDP ();

		vmsHash hash;
		hash.set_SHA2Strength ((vmsHash_SHA2Strength) HIWORD (dp->dwIntegrityCheckAlgorithm));
		fsString strHashResult = hash.Hash (dp->pszFileName, (vmsHashAlgorithm) LOWORD (dp->dwIntegrityCheckAlgorithm)).c_str ();
		if (strHashResult == dp->pszCheckSum)
		{
			
			_DldsMgr.Event (dld, LS (L_INTEGRITYCHECKSUCCEEDED), EDT_RESPONSE_S);
		}
		else
		{
			
			_DldsMgr.Event (dld, LS (L_INTEGRITYCHECKFAILED), EDT_RESPONSE_E);

			switch (dp->enICFR)
			{
			case ICFR_ASKUSER:
			{
				CDlg_CheckFileIntegrity_Result dlg;
				dlg.m_bResultOK = FALSE;
				char sz [MY_MAX_PATH];
				fsGetFileName (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName, sz);
				dlg.m_strFileName = sz;
				dlg.m_strUrl = dld->pMgr->get_URL ();
				dlg.m_strValidHash = dp->pszCheckSum;
				dlg.m_strResultHash = strHashResult;
				if (IDC_RESTARTDOWNLOAD == _DlgMgr.DoModal (&dlg))
				{
					_pwndDownloads->RestartDownload (dld, TRUE);
					return 0;
				}
			}
			break;

			case ICFR_RESTART:
				_pwndDownloads->RestartDownload (dld, TRUE);
			return 0;

			case ICFR_IGNORE:
			break;

			default:
				ASSERT (FALSE);
			}
		}
	}

	if (_DldsMgr.m_bVirCheck)
	{
		BOOL bOk = _DldsMgr.PerformVirusCheck (dld, TRUE, bNeedLaunchDld);

		if (bOk == FALSE && bNeedLaunchDld)
		{
			CWaitForConfirmationDlg dlg;
			dlg.Init (LS (L_VIRCHECKFAILEDLAUNCHDLDANYWAY), 60, FALSE, TRUE);
			dlg.m_pszIcon = IDI_WARNING;

			if (IDOK != _DlgMgr.DoModal (&dlg))
				bNeedLaunchDld = false;
		}
	}

	if (dld->dwFlags & DLD_FLASH_VIDEO)
		_pwndFVDownloads->OnDownloadDone (dld);

	if (bNeedLaunchDld)
	{
		dld->AddRef (); 
		_pwndDownloads->PostMessage (WM_DLD_LAUNCH, 
			dld->pMgr->IsDontConfirmLaunchWhenDone () ? 0 : _App.ConfTimeout_LaunchDld (),
			(LPARAM)(fsDownload*)dld);
	}

	_DldsMgr.OnDldDoneCheckIfNeedDel (dld);

	InterlockedDecrement (&_DldsMgr.m_cThreadsRunning);

	return 0;
}

void fsDownloadsMgr::OnDldDoneCheckIfNeedDel(vmsDownloadSmartPtr dld)
{
	if (m_bAutoDelCompleted || (dld->dwFlags & DLD_DELETEWHENDONE) != 0)
	{
		DLDS_LIST vDlds;
		vDlds.push_back (dld);
		DeleteDownloads (vDlds, FALSE, (dld->dwFlags & DLD_DELETEFILEALWAYS) != FALSE);
		Event ((fsDownload*)NULL, DME_DLDAUTODELETED);
	}
}

BOOL fsDownloadsMgr::OnDldDone_CheckDownloadIsMetaLink(vmsDownloadSmartPtr dld)
{
	if (dld->pMgr->GetDownloadMgr () == NULL)
		return FALSE;

	fsString strFile = dld->pMgr->get_OutputFilePathName ();
	if (lstrlen (strFile) > 10 && lstrcmpi (strFile + lstrlen (strFile) - 9, ".metalink") != 0)
		return FALSE; 

	vmsMetalinkFile mf;
	if (FALSE == mf.Parse (strFile))
		return FALSE;

	if (mf.get_FileCount () == 0)
		return FALSE;

	
	BOOL bRecreated = FALSE;
	
	int iWinOSFile = -1;

	for (int iFile = 0; iFile < mf.get_FileCount () && iWinOSFile != -1; iFile++)
	{
		vmsMetalinkFile_File *file = mf.get_File (iFile);

		
		if (file->strOS.GetLength () && strstr (file->strOS, "Windows") == NULL)
			continue;

		iWinOSFile = iFile;
	}

	if (iWinOSFile == -1)
		iWinOSFile = 0;

	vmsMetalinkFile_File *file = mf.get_File (iWinOSFile);
	
	if (file->vMirrors.size () == 0)
		return FALSE;

	
	fsString strBtUrl;

	for (int i = 0; i < file->vMirrors.size (); i++)
	{
		vmsMetalinkFile_File_Url *url = &file->vMirrors [i];

		if (strcmpi (url->strProtocol, "http") && strcmpi (url->strProtocol, "https") &&
				strcmpi (url->strProtocol, "ftp"))
		{
			if (strcmpi (url->strProtocol, "bittorrent") == 0)
				strBtUrl = url->strUrl;
			continue;
		}

		if (bRecreated == FALSE)
		{
			if (IR_SUCCESS != dld->pMgr->GetDownloadMgr ()->CreateByUrl (url->strUrl, TRUE))
				continue;
			dld->pMgr->GetDownloadMgr ()->Reset ();
			bRecreated = TRUE;
		}
		else
		{
			dld->pMgr->GetDownloadMgr ()->GetDownloader ()->AddMirrorURL (url->strUrl);
		}
	}

	if (bRecreated == FALSE)
	{
		if (strBtUrl.IsEmpty ())
			return FALSE;
		
		
		if (IR_SUCCESS != dld->pMgr->GetDownloadMgr ()->CreateByUrl (strBtUrl, TRUE))
			return FALSE;
		dld->pMgr->GetDownloadMgr ()->Reset ();
		bRecreated = TRUE;
	}

	Event (dld, LS (L_METALINK_DETECTED), EDT_INQUIRY);

	for (i = 0; i < file->vHashes.size (); i++)
	{
		vmsMetalinkFile_File_Hash *hash = &file->vHashes [i];
		DWORD dwAlg;
		if (hash->strChecksum.GetLength () == 0)
			continue;
		
		if (lstrcmpi (hash->strAlgorithm, "md5") == 0)
			dwAlg = HA_MD5;

		else if (lstrcmpi (hash->strAlgorithm, "sha1") == 0 || 
				lstrcmpi (hash->strAlgorithm, "sha-1") == 0)
			dwAlg = HA_SHA1;

		else
			continue;

		
		dld->pMgr->GetDownloadMgr ()->GetDP ()->dwIntegrityCheckAlgorithm = dwAlg;
		SAFE_DELETE_ARRAY (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszCheckSum);
		dld->pMgr->GetDownloadMgr ()->GetDP ()->pszCheckSum = new char [hash->strChecksum.GetLength () + 1];
		lstrcpy (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszCheckSum, hash->strChecksum);
	}

	DeleteFile (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName);
	LPSTR psz = strrchr (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName, '\\');
	if (psz == NULL)
		psz = strrchr (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName, '/');
	if (psz)
		*psz = 0;

	char szNewFile [MY_MAX_PATH];
	lstrcpy (szNewFile, dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName);
	lstrcat (szNewFile, "\\");
	if (file->strName.pszString != NULL)
		lstrcat (szNewFile, file->strName);
	SAFE_DELETE_ARRAY (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName);
	dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName = new char [lstrlen (szNewFile) + 1];
	lstrcpy (dld->pMgr->GetDownloadMgr ()->GetDP ()->pszFileName, szNewFile);

	if (dld->strComment.GetLength () != 0)
		dld->strComment += "\r\n";
	dld->strComment += mf.get_Description ();

	dld->pMgr->StartDownloading ();

	return TRUE;
}

void fsDownloadsMgr::DownloadsList_Add(vmsDownloadSmartPtr dld)
{
	m_vDownloads.push_back (dld);
	dld->pGroup->cDownloads++;
}

void fsDownloadsMgr::DownloadsList_Del(size_t nIndex)
{
	m_vDownloads [nIndex]->pGroup->cDownloads--;
	m_vDownloads.erase (m_vDownloads.begin () + nIndex);
}

void fsDownloadsMgr::DownloadsList_Insert(size_t nIndexBefore, vmsDownloadSmartPtr dld)
{
	m_vDownloads.insert (m_vDownloads.begin () + nIndexBefore, dld);
	dld->pGroup->cDownloads++;
}

void fsDownloadsMgr::DeletedDownloads_UseTmpList(BOOL bUse)
{
	if (bUse)
	{
		ASSERT (m_vDeletedDownloads_tmp.size () == 0);
		std::list <vmsDownloadSmartPtr> v;
		for (size_t i = 0; i < m_vDeletedDownloads.size (); i++)
			v.push_back (m_vDeletedDownloads [i]);
		m_vDeletedDownloads_tmp = v;
		m_vDeletedDownloads.clear ();
	}
	else
	{
		ASSERT (m_vDeletedDownloads.size () == 0);
		DLDS_LIST v;
		std::list <vmsDownloadSmartPtr>::iterator iter;
		for (iter = m_vDeletedDownloads_tmp.begin (); iter != m_vDeletedDownloads_tmp.end (); iter++)
			v.push_back (*iter);
		m_vDeletedDownloads = v;
		m_vDeletedDownloads_tmp.clear ();		
	}
}

BOOL fsDownloadsMgr::DeleteGroupsDownloads(vmsDownloadsGroupSmartPtr pGroup, BOOL bInclSubgroups)
{
	std::vector <vmsDownloadsGroupSmartPtr> v;
	if (bInclSubgroups)
		_DldsGrps.GetGroupWithSubgroups (pGroup, v);
	else
		v.push_back (pGroup);

	DLDS_LIST vDlds;

	for (int i = m_vDownloads.size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = m_vDownloads [i];
		
		for (size_t j = 0; j < v.size (); j++)
		{
			if (v [j] == dld->pGroup)
			{
				
				vDlds.push_back (dld);
				break;
			}
		}
	}

	if (vDlds.size () == 0)
		return TRUE;

	size_t cToDelete = vDlds.size ();
	size_t cDeleted = DeleteDownloads (vDlds, TRUE, FALSE);
	if (cDeleted != cToDelete)
		return FALSE;

	return TRUE;
}

void fsDownloadsMgr::GetEventColors(fsDownloadMgr_EventDescType enType, COLORREF &clrText, COLORREF &clrBg, int &iImage)
{
	clrText = RGB (0, 0, 0);

	
	
	switch (enType)
	{
		case EDT_INQUIRY:
			clrBg = CLR_INQUIRY;
			iImage = 0;
			break;

		case EDT_INQUIRY2:
			clrBg = CLR_INQUIRY2;
			iImage = 0;
			break;

		case EDT_DONE:
			clrBg = CLR_DONE;
			iImage = 1;
			break;

		case EDT_RESPONSE_S:
			clrBg = CLR_RESPONSE_S;
			iImage = 2;
			break;

		case EDT_RESPONSE_S2:
			clrBg = CLR_RESPONSE_S2;
			iImage = 2;
			break;

		case EDT_RESPONSE_E:
			clrBg = CLR_RESPONSE_E;
			iImage = 3;
			break;

		case EDT_WARNING:
			clrBg = CLR_WARNING;
			iImage = 5;
			break;

		default:
			ASSERT (4!=4);
			return;
	}
}

BOOL fsDownloadsMgr::OnDldDone_CheckDownloadIsBittorrent(vmsDownloadSmartPtr dld)
{
	if (Is9xME)
		return FALSE;

	if (dld->pMgr->GetDownloadMgr () == NULL)
		return FALSE;

	fsString strFile = dld->pMgr->get_OutputFilePathName ();

	if ((dld->dwFlags & DLD_TORRENT_DOWNLOAD) == 0)
	{
		if (lstrlen (strFile) > 10 && lstrcmpi (strFile + lstrlen (strFile) - 8, ".torrent") != 0)
			return FALSE; 
	}

	if (_App.Bittorrent_Enable () == FALSE)
	{
		if (_App.View_DontAskEnableBittorrent () == FALSE)
		{
			CMyMessageBox dlg (_pwndDownloads);
			dlg.m_hIcon = LoadIcon (NULL, IDI_QUESTION);
			dlg.m_strBtn1Text = LS (L_YES);
			dlg.m_strBtn2Text = LS (L_NO);
			dlg.m_strCheckBoxText = LS (L_DONTASKAGAIN);
			dlg.m_strText = LS (L_ENABLEBTISREQ);
			dlg.m_strTitle = LS (L_CONFIRMATION);
			dlg.m_bChecked = FALSE;
			if (IDC_BTN1 != _DlgMgr.DoModal (&dlg))
			{
				if (dlg.m_bChecked)
					_App.View_DontAskEnableBittorrent (TRUE);
				return FALSE;
			}
			_App.Bittorrent_Enable (TRUE);
		}
		else 
		{
			return FALSE;
		}
	}

	if (_AppMgr.IsBtInstalled () == FALSE)
	{
		_AppMgr.ShowInstallBtMessage ();
		_App.Bittorrent_Enable (FALSE);
		return FALSE;
	}

	char szPath [MY_MAX_PATH];
	fsGetPath (strFile, szPath);

	dld->pMgr->GetDownloadMgr ()->CloseFile ();

	vmsBtDownloadManager *pBtMgr = new vmsBtDownloadManager;

	if (dld->pMgr->GetDownloadMgr ()->GetDP ()->bReserveDiskSpace)
		pBtMgr->enable_Flags (BTDF_RESERVE_DISK_SPACE);
	else
		pBtMgr->disable_Flags (BTDF_RESERVE_DISK_SPACE);

	if (dld->pMgr->IsLaunchWhenDone ())
		pBtMgr->enable_Flags (BTDF_LAUNCH_WHEN_DONE);
	else
		pBtMgr->disable_Flags (BTDF_LAUNCH_WHEN_DONE);

	if (dld->pMgr->IsDescriptionFileShouldBeGenerated ())
		pBtMgr->enable_Flags (BTDF_GENERATE_DESC_FILE);
	else
		pBtMgr->disable_Flags (BTDF_GENERATE_DESC_FILE);

	if (dld->pMgr->GetDownloadMgr ()->GetDP ()->bIgnoreRestrictions)
		pBtMgr->enable_Flags (BTDF_IGNORE_ALL_RESTRICTIONS);
	else
		pBtMgr->disable_Flags (BTDF_IGNORE_ALL_RESTRICTIONS);  

	if (FALSE == pBtMgr->CreateByTorrentFile (strFile, szPath, dld->pMgr->get_URL ()))
		return FALSE;

	fsString strComment = pBtMgr->get_TorrentComment ();
	if (strComment.IsEmpty () == FALSE)
	{
		if (dld->strComment.IsEmpty () == FALSE)
			dld->strComment += "\r\n";
		dld->strComment += strComment;
	}

	vmsDownloadMgrEx *pNewMgr = new vmsDownloadMgrEx;
	pNewMgr->Attach (pBtMgr);

	vmsDownloadMgrEx *pMgr = dld->pMgr;
	pMgr->AddRef ();
	

	dld->pMgr = pNewMgr;
	dld->pMgr->GetBtDownloadMgr ()->SetEventsHandler (_BtDownloadManagerEventHandler, this);

	Event (dld, DME_DLD_CHANGED_TO_BT_TYPE);
	Event (dld, LS (L_BITTORRENT_DETECTED), EDT_INQUIRY);
	Event (dld, DME_BTDLD_STAT_CHANGED);

	if ((dld->dwFlags & DLD_TORRENT_DOWNLOAD) == 0)
	{
		dld->dwFlags |= DLD_TORRENT_DOWNLOAD;
		if (_pwndTorrents)
			_pwndTorrents->AddDownload (dld, FALSE);
	}

	dld->pMgr->StartDownloading ();

	DWORD dw;	
	CloseHandle (
		CreateThread (NULL, 0, _threadDeleteDownloadMgrEx, pMgr, 0, &dw));

	return TRUE;
}

DWORD WINAPI fsDownloadsMgr::_threadDeleteDownloadMgrEx(LPVOID lp)
{
	vmsDownloadMgrEx *pMgr = (vmsDownloadMgrEx*) lp;
	pMgr->Release ();
	return 0;
}

void fsDownloadsMgr::_BtSessionEventsHandler(vmsBtSession *, vmsBtSessionEvent *ev, LPVOID lp)
{
	fsDownloadsMgr *pthis = (fsDownloadsMgr*)lp;

	vmsDownloadSmartPtr dld;
	if (ev->pDownload)
		dld = pthis->FindDownloadByBtDownload (ev->pDownload);
	if (dld == NULL)
		return;

	switch (ev->enType)
	{
	case BTSET_FILE_ERROR:
    case BTSET_BTDLD_WAS_RESET:
		dld->bAutoStart = FALSE;
		dld->pMgr->GetBtDownloadMgr ()->DeleteBtDownload ();
		if (ev->enType == BTSET_FILE_ERROR)
			pthis->Event (dld, LS (L_BT_FILE_ERROR), EDT_RESPONSE_E);
		break;

	case BTSET_TRACKER_ANNOUNCE:
		if (dld->pMgr->IsRunning () == FALSE && dld->pMgr->GetBtDownloadMgr ()->isSeeding () == FALSE)
			return;
		pthis->Event (dld, LS (L_BT_TRACKER_ANNOUNCE), EDT_INQUIRY);
		dld->pMgr->GetBtDownloadMgr ()->m_strTrackerStatus = LS (L_CONNECTING);
		break;

	case BTSET_TRACKER:
		if (dld->pMgr->IsRunning () == FALSE && dld->pMgr->GetBtDownloadMgr ()->isSeeding () == FALSE)
			return;
		pthis->Event (dld, LS (L_BT_TRACKER_ERROR), EDT_RESPONSE_E);
		if (ev->pszMsg)
		{
			LPCSTR pszMsg = strchr (ev->pszMsg, '"');
			if (pszMsg)
				pszMsg = strchr (pszMsg+1, '"');
			if (pszMsg)
				pszMsg += 2;
			if (pszMsg == NULL)
				pszMsg = ev->pszMsg;
			pthis->EventEx (dld, pszMsg, EDT_RESPONSE_E, 70);
			dld->pMgr->GetBtDownloadMgr ()->m_strTrackerStatus = LS (L_FAILED); 
			dld->pMgr->GetBtDownloadMgr ()->m_strTrackerStatus += ": ";
			dld->pMgr->GetBtDownloadMgr ()->m_strTrackerStatus += pszMsg;
		}
		char sz [1000];
		int nNext; nNext = dld->pMgr->GetBtDownloadMgr ()->get_NextAnnounceInterval ();
		if (nNext)
		{
			sprintf (sz, LS (L_NEXT_CONNECT_WITH_TRACKER_IN), nNext);
			pthis->Event (dld, sz, EDT_INQUIRY);
		}
		break;

	case BTSET_TRACKER_REPLY:
		if (dld->pMgr->IsRunning () == FALSE && dld->pMgr->GetBtDownloadMgr ()->isSeeding () == FALSE)
			return;
		dld->pMgr->GetBtDownloadMgr ()->m_strTrackerStatus = "OK"; 
		pthis->Event (dld, LS (L_BT_TRACKER_OK_RESPONSE), EDT_RESPONSE_S);
		break;

	case BTSET_HASH_FAILED:
		if (dld->pMgr->IsRunning () == FALSE && dld->pMgr->GetBtDownloadMgr ()->isSeeding () == FALSE)
			return;
		pthis->Event (dld, LS (L_BT_HASH_FAILED), EDT_WARNING);
		break;
	}

	pthis->Event (dld, DME_DOWNLOAD_STATE_CHANGED);
	pthis->Event (dld, DME_BTDLD_STAT_CHANGED);
}

void fsDownloadsMgr::AttachToBtSession()
{
	_BT.get_Session ()->set_EventsHandler (_BtSessionEventsHandler, this);
}

vmsDownloadSmartPtr fsDownloadsMgr::GetDownloadByBtDownloadMgr(vmsBtDownloadManager *pMgr)
{
	ASSERT (pMgr != NULL);

	try {
		for (size_t i = 0; i < m_vDownloads.size (); i++)
		{
			if (m_vDownloads [i]->pMgr->GetBtDownloadMgr () == pMgr)
				return m_vDownloads [i];
		}
	}catch (...){}

	return NULL;
}

vmsDownloadSmartPtr fsDownloadsMgr::FindDownloadByBtDownload(vmsBtDownload *pDld)
{
	ASSERT (pDld != NULL);

	try {
		for (size_t i = 0; i < m_vDownloads.size (); i++)
		{
			if (m_vDownloads [i]->pMgr->GetBtDownloadMgr () != NULL &&
					m_vDownloads [i]->pMgr->GetBtDownloadMgr ()->get_BtDownload () == pDld)
				return m_vDownloads [i];
		}
	}catch (...){}

	return NULL;
}

DWORD fsDownloadsMgr::_BtDownloadManagerEventHandler(vmsBtDownloadManager *pMgr, vmsBtDownloadManagerEvent ev, DWORD dwInfo, LPVOID lp)
{
	fsDownloadsMgr* pthis = (fsDownloadsMgr*) lp;

	vmsDownloadSmartPtr dld;
	dld = pthis->GetDownloadByBtDownloadMgr  (pMgr);

	if (dld != NULL)
	{
		switch (ev)
		{
		case BTDME_CHECKING_FILES:
		case BTDME_ALLOCATING:
			pthis->Event (dld, LS (L_PREP_FILES_ONDISK), EDT_INQUIRY);
			break;

		case BTDME_ALLOCATION_FAILED:
			CHAR szErr [1000];
			fsErrorToStr (szErr, sizeof (szErr), &dwInfo);
			dld->bAutoStart = FALSE;
			pthis->Event (dld, szErr, EDT_RESPONSE_E);
			break;

		case BTDME_DOWNLOAD_STARTED:
			pthis->Event (dld, DME_DLMGRTHREADSTARTING);
			pthis->Event (dld, DME_CREATEDLDDIALOG);
			pthis->Event (dld, LS (L_STARTING), EDT_RESPONSE_S);
			break;

		case BTDME_DOWNLOAD_STOPPED_OR_DONE:
			pthis->Event (dld, dld->pMgr->IsDone () ? LS (L_DONE) : LS (L_STOPPED), EDT_RESPONSE_S);
			pthis->OnDownloadStoppedOrDone (dld);
			return 0;

		case BTDME_DOWNLOADING:
			pthis->Event (dld, LS (L_DOWNLOADING), EDT_RESPONSE_S);
			break;

		case BTDME_SEEDING:
			pthis->ProcessDownloads ();
			break;

		case BTDME_FATAL_ERROR:
			dld->bAutoStart = FALSE;
			break;

		case BTDME_STOP_SEEDING:
			break;
		}

		pthis->Event (dld, DME_DOWNLOAD_STATE_CHANGED);
	}

	return 0;
}

BOOL fsDownloadsMgr::OnDownloadStoppedOrDone(vmsDownloadSmartPtr dld)
{
	#ifdef _USELOGGING
	LPVOID lp = dld->pMgr->GetDownloadMgr () ? (LPVOID)dld->pMgr->GetDownloadMgr () : (LPVOID)dld->pMgr->GetBtDownloadMgr ();
	#endif
	#define LOG_local(x) LOG("fsDsM::ODSOD(" << (DWORD)lp << "): " << x << nl)

	LOG_local ("1");
	Event (dld, DME_DOWNLOADEREVENTRECEIVED);
	LOG_local ("2");
	Event (dld, DME_DOWNLOADSTOPPEDORDONE);
	LOG_local ("3");
	Event (dld, DME_UPDATEDLDDIALOG);
	LOG_local ("4");

	if (m_gabInfo.dld == dld)
		m_gabInfo.dld = NULL;

	if (dld->pMgr->IsDone ())	
	{
		if (OnDldDone_CheckDownloadIsMetaLink (dld))
			return FALSE;

		if (OnDldDone_CheckDownloadIsBittorrent (dld))
			return FALSE;

		if (OnDldDone_CheckDownloadIsHtmlPageWithVideo (dld))
			return FALSE;

		#ifndef FDM_DLDR__RAWCODEONLY
		if ((dld->dwFlags & DLD_DONTPUTTOHISTORY) == 0)
			m_histmgr.AddToHistory (dld);
		#endif

		if (dld->pMgr->IsDescriptionFileShouldBeGenerated ())
			GenerateDescriptionFile (dld);

		if (FALSE == dld->pMgr->IsHtmlSpiderDownload ())
		{
			if (dld->pMgr->GetBtDownloadMgr () && dld->pMgr->GetBtDownloadMgr ()->get_FileCount () > 1)
			{
				m_LastFilesDownloaded.Add (dld->pMgr->GetBtDownloadMgr ()->get_TorrentName (), 
					dld->pMgr->get_OutputFilePathName ());
			}
			else
			{
				fsString strPath = dld->pMgr->get_OutputFilePathName ();
				char sz [MY_MAX_PATH] = "";
				fsGetFileName (strPath, sz);
				m_LastFilesDownloaded.Add (sz, strPath);
			}
		}

		if (dld->dwFlags & DLD_DELETEFILEATRESTART)
		{
			if (dld->pMgr->GetBtDownloadMgr () && dld->pMgr->GetBtDownloadMgr ()->get_FileCount () > 1)
			{
				for (int i = 0; i < dld->pMgr->GetBtDownloadMgr ()->get_FileCount (); i++)
					vmsDeleteFileAtWinBoot  (dld->pMgr->GetBtDownloadMgr ()->get_OutputFilePathName (i));
			}
			else
			{			
				vmsDeleteFileAtWinBoot  (dld->pMgr->get_OutputFilePathName ());
			}
		}

		dld->bAutoStart = FALSE;

		bool bNeedLaunchDld = (dld->dwFlags & DLD_NOAUTOLAUNCH) == 0 && 
			dld->pMgr->IsLaunchWhenDone ();
		bool bNeedIntegrityCheck = dld->pMgr->GetDownloadMgr () != NULL &&
			dld->pMgr->GetDownloadMgr ()->GetDP ()->bCheckIntegrityWhenDone && 
			*dld->pMgr->GetDownloadMgr ()->GetDP ()->pszCheckSum;
		bool bNeedThread = m_bVirCheck || bNeedLaunchDld || bNeedIntegrityCheck ||
			(dld->dwFlags & DLD_FLASH_VIDEO);

		if (bNeedThread)
		{
			dld->AddRef (); 
			InterlockedIncrement (&m_cThreadsRunning);
			DWORD dw;
			CloseHandle (
				CreateThread (NULL, 0, _threadIntegrityCheckAndVirCheckAndLaunch, dld, 0, &dw));
		}
		else
		{
			OnDldDoneCheckIfNeedDel (dld);
		}

		if (dld->dwFlags & DLD_MF_AUTO_CONVERT)
		{
			dld->AddRef ();
			_pwndDownloads->PostMessage (WM_DLD_CONVERT_MEDIA, MCM_OS_SEARCH_IN_MGR, 
				(LPARAM)(fsDownload*)dld);
		}
	}

	ProcessDownloads ();	
	LOG_local ("5");
	CheckNoActiveDownloads (); 
	LOG_local ("6");

	return TRUE;
}

void fsDownloadsMgr::OnDownloadDescEventRcvd(vmsDownloadSmartPtr dld, fsDownloadMgr_EventDescType enType, LPCSTR pszEvent)
{
	try {

	COLORREF clrBg, clrText;
	int iImage;

	GetEventColors (enType, clrText, clrBg, iImage);

	fsDownloadEvents event;
	SYSTEMTIME time;
	GetLocalTime (&time);

	event.clrBg = clrBg;
	event.clrText = clrText;
	event.strEvent = pszEvent;
	event.iImage = iImage;
	SystemTimeToFileTime (&time, &event.timeEvent);

	dld->vEvents.add (event);	

	
	Event (dld, DME_EVENTDESCRIPRIONRECEIVED);

	}catch (...) {}
}

void fsDownloadsMgr::ResetDldHpManageState()
{
	m_gabInfo.enCurState = _inc_GrantAllBandwidthInfo::CS_Start;
	m_gabInfo.uBandwidth = 0;
	m_gabInfo.uDldMaxSpeed = 0;
}

void fsDownloadsMgr::GetRunningDownloads(DLDS_LIST &v)
{
	for (size_t i = 0; i < m_vDownloads.size (); i++)
	{
		if (m_vDownloads [i]->pMgr->IsRunning ())
			v.push_back (m_vDownloads [i]);
	}
}

void fsDownloadsMgr::ApplyTrafficLimit_NoHpDld()
{
	LOG ("fsDsM::ATL_NHD: start" << nl);

	DLDS_LIST vRunningDlds;
	GetRunningDownloads (vRunningDlds);
	if (vRunningDlds.size () == 0)
	{
		LOG ("fsDsM::ATL_NHD: exit [0]" << nl);
		return;
	}

	
	UINT tlpd = m_enTUM == TUM_HEAVY ? UINT_MAX : 
		m_aTUM [m_enTUM].uTrafficLimit / vRunningDlds.size ();

	m_bSkip1Cicle = TRUE;

	ApplyTrafficLimitForListOfDownloads (vRunningDlds, tlpd);

	LOG ("fsDsM::ATL_NHD: exit" << nl);
}

void fsDownloadsMgr::ApplyTrafficLimit_HasHpDld()
{
	LOG ("fsDsM::ATL_HHD: start" << nl);

	ASSERT (m_gabInfo.dld != NULL);
	ASSERT (m_gabInfo.dld->pMgr->IsRunning ());

	DLDS_LIST vRunningDlds;
	GetRunningDownloads (vRunningDlds);
	if (vRunningDlds.size () == 0)
	{
		LOG ("fsDsM::ATL_HHD: exit [0]" << nl);
		return;
	}

	

	if (vRunningDlds.size () == 1)
	{
		UINT uTraffLimit = m_aTUM [m_enTUM].uTrafficLimit;
		if (m_gabInfo.dld->pMgr->GetDownloadMgr ())
			uTraffLimit = min (uTraffLimit, m_gabInfo.dld->pMgr->GetDownloadMgr ()->GetDP ()->uTrafficRestriction);
		m_gabInfo.dld->pMgr->LimitTraffic (uTraffLimit);
		LOG ("fsDsM::ATL_HHD: exit [1]" << nl);
		return;
	}

	

	for (size_t i = 0; i < vRunningDlds.size (); i++)
	{
		if (vRunningDlds [i] == m_gabInfo.dld)
		{
			vRunningDlds.erase (vRunningDlds.begin () + i);
			break;
		}
	}

	BOOL bWaiting = FALSE;

	if (m_gabInfo.enCurState == _inc_GrantAllBandwidthInfo::CS_Measuring_Bandwidth ||
			m_gabInfo.enCurState == _inc_GrantAllBandwidthInfo::CS_Measuring_DldMaxSpeed)
	{
		fsTicksMgr time;
		UINT nTimeout = m_gabInfo.enCurState == _inc_GrantAllBandwidthInfo::CS_Measuring_Bandwidth ? 15 : 60;
		if (time - m_gabInfo.timeStateChange < nTimeout * 1000)
			bWaiting = TRUE;
	}

	if (bWaiting == FALSE)
	switch (m_gabInfo.enCurState)
	{
	case _inc_GrantAllBandwidthInfo::CS_Start:
		m_gabInfo.uBandwidth = m_aTUM [m_enTUM].uTrafficLimit;
		m_gabInfo.uDldMaxSpeed = 0;
		ApplyTrafficLimit_NoHpDld ();
		if (m_gabInfo.uBandwidth == UINT_MAX)
		{
			m_gabInfo.enCurState = _inc_GrantAllBandwidthInfo::CS_Measuring_Bandwidth;
			m_gabInfo.timeStateChange.Now ();
			break;
		}
		
	case _inc_GrantAllBandwidthInfo::CS_Measuring_Bandwidth:
	{
		if (m_gabInfo.uBandwidth == UINT_MAX)
		{
			m_gabInfo.uBandwidth = m_gabInfo.dld->pMgr->GetSpeed ();
			for (size_t i = 0; i < vRunningDlds.size (); i++)
				m_gabInfo.uBandwidth += vRunningDlds [i]->pMgr->GetSpeed ();
		}
		m_gabInfo.uDldMaxSpeed = m_gabInfo.uBandwidth - vRunningDlds.size ();
		m_gabInfo.enCurState = _inc_GrantAllBandwidthInfo::CS_Measuring_DldMaxSpeed;
		m_gabInfo.dld->pMgr->LimitTraffic (m_aTUM [m_enTUM].uTrafficLimit);
		m_gabInfo.timeStateChange.Now ();
	}
		break;

	case _inc_GrantAllBandwidthInfo::CS_Measuring_DldMaxSpeed:
		m_gabInfo.uDldMaxSpeed = m_gabInfo.dld->pMgr->GetSpeed ();
		m_gabInfo.uBandwidth = max (m_gabInfo.uBandwidth, m_gabInfo.uDldMaxSpeed + vRunningDlds.size ());
		m_gabInfo.enCurState = _inc_GrantAllBandwidthInfo::CS_Done;
		m_gabInfo.timeStateChange.Now ();
		break;

	case _inc_GrantAllBandwidthInfo::CS_Done:
	{
		m_gabInfo.uDldMaxSpeed = max (m_gabInfo.uDldMaxSpeed, m_gabInfo.dld->pMgr->GetSpeed ());

		if (m_aTUM [m_enTUM].uTrafficLimit == UINT_MAX)
		{
			UINT uBandwidth = m_gabInfo.dld->pMgr->GetSpeed ();;
			for (size_t i = 0; i < vRunningDlds.size (); i++)
				uBandwidth += vRunningDlds [i]->pMgr->GetSpeed ();
			m_gabInfo.uBandwidth = max (m_gabInfo.uBandwidth, uBandwidth);
			m_gabInfo.uBandwidth = max (m_gabInfo.uBandwidth, m_gabInfo.uDldMaxSpeed + vRunningDlds.size ());
		}
		
		m_gabInfo.uDldMaxSpeed = min (m_gabInfo.uDldMaxSpeed, m_gabInfo.uBandwidth - vRunningDlds.size ());
	}
		break;
	}

	m_bSkip1Cicle = TRUE;

	if (m_gabInfo.enCurState != _inc_GrantAllBandwidthInfo::CS_Measuring_Bandwidth)
	{
		ApplyTrafficLimitForListOfDownloads (vRunningDlds, 
			(m_gabInfo.uBandwidth - m_gabInfo.uDldMaxSpeed) / vRunningDlds.size ());
	}

	LOG ("fsDsM::ATL_HHD: exit" << nl);
}

void fsDownloadsMgr::ApplyTrafficLimitForListOfDownloads(DLDS_LIST vDlds, UINT nLimit)
{
	LOG ("fsDsM::ATLFLOD: start" << nl);

	UINT uShortage = 0;
	UINT cLimitedWithReqLimit = 0;

	if (nLimit == 0)
		nLimit = 1;

	LOG ("fsDsM::ATLFLOD: number of downloads = " << DWORD (vDlds.size ()) << nl);

	
	for (size_t i = 0; i < vDlds.size (); i++)
	{
		vmsDownloadMgrSmartPtr pMgr = vDlds [i]->pMgr;
		UINT dldLimit = UINT_MAX;
		if (pMgr->GetDownloadMgr () != NULL)
		{
			if (pMgr->GetDownloadMgr ()->GetDP ()->bIgnoreRestrictions)
				continue;
			dldLimit = pMgr->GetDownloadMgr ()->GetDP ()->uTrafficRestriction;
		}

		if (dldLimit >= nLimit)
		{
			pMgr->LimitTraffic (nLimit);
			cLimitedWithReqLimit++;
		}
		else 
		{
			
			
			pMgr->LimitTraffic (dldLimit);
			uShortage += nLimit - dldLimit;	
		}
	}

	LOG ("fsDsM::ATLFLOD: 1" << nl);

	if (cLimitedWithReqLimit)
		uShortage /= cLimitedWithReqLimit;

	for (i = 0; i < vDlds.size (); i++)
	{
		vmsDownloadMgrSmartPtr pMgr = vDlds [i]->pMgr;
		if (pMgr->GetTrafficLimit () == nLimit)
		{
			
			pMgr->LimitTraffic (nLimit + uShortage);
		}
	}

	LOG ("fsDsM::ATLFLOD: exit" << nl);
}

void fsDownloadsMgr::set_HighestPriorityDownload(vmsDownloadSmartPtr dld)
{
	m_gabInfo.dld = dld;
	ResetDldHpManageState ();
	ApplyTrafficLimit ();
}

int fsDownloadsMgr::IsSuchUrlExistsAlready(vmsDownloadSmartPtr dld)
{
	fsURL url1, url2;

	if (dld->pMgr->GetDownloadMgr () == NULL)
		return 0;

	fsDownload_NetworkProperties* dnp0 = dld->pMgr->GetDownloadMgr ()->GetDNP ();

	for (size_t i = 0; i < GetCount (); i++)
	{
		vmsDownloadSmartPtr d = GetDownload (i);
		if (d->pMgr->GetDownloadMgr () == NULL)
			continue;
		fsDownload_NetworkProperties* dnp = d->pMgr->GetDownloadMgr ()->GetDNP ();

		if (dnp0->enProtocol == dnp->enProtocol && 
			dnp0->uServerPort == dnp->uServerPort &&
			fsIsServersEqual (dnp0->pszServerName, dnp->pszServerName, FALSE) && 
			lstrcmp (dnp0->pszPathName, dnp->pszPathName) == 0 &&
			lstrcmp (dnp0->pszUserName, dnp->pszUserName) == 0)
		{
			return i;
		}
	}

	return -1;
}

BOOL fsDownloadsMgr::OnDldDone_CheckDownloadIsHtmlPageWithVideo(vmsDownloadSmartPtr dld)
{
	if (dld->dwFlags & DLD_USEDBYHTMLSPIDER)
		return FALSE;	

	if (dld->pMgr->GetDownloadMgr () == NULL)
		return FALSE;

	fsDownloadMgr *pMgr = dld->pMgr->GetDownloadMgr ();

	if ((dld->dwFlags & DLD_NEED_ONDONE_FOR_FLASH_VIDEO) == 0 &&
			strstr (pMgr->GetDownloader ()->GetContentType (), "html")  == NULL)
		return FALSE;
	
	if (pMgr->GetDownloader ()->GetLDFileSize () > 500000)
		return FALSE;

	HANDLE hFile = CreateFile (pMgr->GetDP ()->pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dw = GetFileSize (hFile, NULL);

	LPSTR pszHtml = new char [dw + 1];
	ReadFile (hFile, pszHtml, dw, &dw, NULL);
	pszHtml [dw] = 0;

	CloseHandle (hFile);

	vmsVideoSiteHtmlCodeParser vshcp;
	BOOL b;

	if (dld->dwFlags & DLD_NEED_ONDONE_FOR_FLASH_VIDEO)
	{
		dld->dwFlags &= ~DLD_NEED_ONDONE_FOR_FLASH_VIDEO;
		b = vshcp.Parse_Further (pMgr->GetDNP ()->pszServerName, pszHtml);
	}
	else
	{
		b = vshcp.Parse (pMgr->GetDNP ()->pszServerName, pszHtml);
	}	

	delete [] pszHtml;

	if (b == FALSE)
		return FALSE;

	if (IR_SUCCESS != dld->pMgr->GetDownloadMgr ()->CreateByUrl (vshcp.get_VideoUrl (), TRUE))
		return FALSE;
	dld->pMgr->GetDownloadMgr ()->Reset ();

	if (vshcp.get_IsVideoUrlDirectLink ())
	{
		if ((dld->dwFlags & DLD_FLASH_VIDEO) == 0)
		{
			dld->dwFlags |= DLD_FLASH_VIDEO;
			_pwndFVDownloads->AddDownload (dld, FALSE);
			Event (dld, LS (L_FVDLD_DETECTED), EDT_INQUIRY);
		}
	}
	else
	{
		dld->dwFlags |= DLD_NEED_ONDONE_FOR_FLASH_VIDEO;
	}

	DeleteFile (pMgr->GetDP ()->pszFileName);
	LPSTR psz = strrchr (pMgr->GetDP ()->pszFileName, '\\');
	if (psz == NULL)
		psz = strrchr (pMgr->GetDP ()->pszFileName, '/');
	if (psz)
		psz [1] = 0;

	char szNewFile [MY_MAX_PATH];
	lstrcpy (szNewFile, pMgr->GetDP ()->pszFileName);
	fsString strFile;
	if (vshcp.get_VideoTitle () != NULL && *vshcp.get_VideoTitle ())
	{
		strFile = vshcp.get_VideoTitle ();
		LPCSTR pszInvChars = "\\/:*?\"<>|";
		LPSTR psz = strFile;
		while (*psz)
		{
			if (strchr (pszInvChars, *psz) != NULL)
				*psz = ' ';
			psz++;
		}

		if (dld->strComment.GetLength () != 0)
			dld->strComment += "\r\n";
		dld->strComment += vshcp.get_VideoTitle ();
	}
	else
	{
		strFile = LS (L_UNKNOWN);
	}
	lstrcat (szNewFile, strFile);
	lstrcat (szNewFile, ".");
	lstrcat (szNewFile, vshcp.get_VideoType ());

	SAFE_DELETE_ARRAY (pMgr->GetDP ()->pszFileName);
	pMgr->GetDP ()->pszFileName = new char [lstrlen (szNewFile) + 1];
	lstrcpy (pMgr->GetDP ()->pszFileName, szNewFile);

	dld->pMgr->StartDownloading ();	

	return TRUE;
}

void fsDownloadsMgr::DownloadStateChanged(vmsDownloadSmartPtr dld)
{
	Event (dld, DME_DOWNLOAD_STATE_CHANGED);
}

void fsDownloadsMgr::AddEvent(vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType)
{
	Event (dld, pszEvent, enType);
}

void fsDownloadsMgr::EventEx(vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType, int nMaxCharsPerLine)
{
	fsString strEvent;
	int n = 0;

	while (*pszEvent)
	{
		if (*pszEvent == ' ' && n >= nMaxCharsPerLine)
		{
			Event (dld, strEvent, enType);
			strEvent = "";
			n = 0;
			pszEvent++;
		}
		else
		{
			strEvent += *pszEvent++;
			if (*pszEvent == '\n' || *pszEvent == '\r')
			{
				n = nMaxCharsPerLine;
				while (*pszEvent == '\n' || *pszEvent == '\r')
					pszEvent++;
				continue;
			}
			else
			{
				n++;

			}
		}
	}

	if (strEvent.IsEmpty () == FALSE)
		Event (dld, strEvent, enType);
}

vmsDownloadSmartPtr fsDownloadsMgr::get_HighestPriorityDownload()
{
	return m_gabInfo.dld;
}

DWORD WINAPI fsDownloadsMgr::_threadStartSeeding(LPVOID lp)
{
	fsDownloadsMgr *pthis = (fsDownloadsMgr*) lp;

	for (int k = 0; k < 2; k++)
	{
		
		for (int i = 0; i < k == 0 ? 15 : 300; i++)
		{
			Sleep (1000);
			if (pthis->m_bNeedExit)
				break;
		}

		if (pthis->m_bNeedExit)
			break;

		vmsBtSession *pS = (vmsBtSession*) _BT.get_Session ();
		for (i = 0; i < pS->get_DownloadCount (); i++)
		{
			vmsBtDownload *dld = pS->get_Download (i);
			if (dld->GetState () == BTDS_SEEDING && dld->IsPaused ())
				dld->Resume ();
		}
	}

	InterlockedDecrement (&pthis->m_cThreadsRunning);
	return 0;
}

BOOL fsDownloadsMgr::AllowStartNewDownloads()
{
	return m_bAllowStart;
}
