/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "vmsDownloadMgrEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

vmsDownloadMgrEx::vmsDownloadMgrEx()
{
	m_pMgr = NULL;
	m_pBtMgr = NULL;
}

vmsDownloadMgrEx::~vmsDownloadMgrEx()
{
	SAFE_DELETE (m_pMgr);
	SAFE_DELETE (m_pBtMgr);
}

void vmsDownloadMgrEx::Attach(fsDownloadMgr *pMgr)
{
	SAFE_DELETE (m_pMgr);
	SAFE_DELETE (m_pBtMgr);
	m_pMgr = pMgr;
}

void vmsDownloadMgrEx::Attach(vmsBtDownloadManager *pBtMgr)
{
	SAFE_DELETE (m_pMgr);
	SAFE_DELETE (m_pBtMgr);
	m_pBtMgr = pBtMgr;
}

fsDownloadMgr* vmsDownloadMgrEx::GetDownloadMgr()
{
	return m_pMgr;
}

vmsBtDownloadManager* vmsDownloadMgrEx::GetBtDownloadMgr()
{
	return m_pBtMgr;
}

fsString vmsDownloadMgrEx::get_OutputFilePathName()
{
	if (m_pMgr)
		return m_pMgr->GetDP ()->pszFileName;
	else
		return m_pBtMgr->get_OutputFilePathName ();
}

float vmsDownloadMgrEx::GetPercentDone()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetPercentDone ();
	else
		return m_pBtMgr->GetPercentDone ();
}

BOOL vmsDownloadMgrEx::IsLaunchWhenDone()
{
	if (m_pMgr)
		return m_pMgr->GetDP ()->dwFlags & DPF_STARTWHENDONE;
	else
		return m_pBtMgr->get_Flags () & BTDF_LAUNCH_WHEN_DONE;
}

fsResumeSupportType vmsDownloadMgrEx::IsResumeSupported()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->IsResumeSupported ();
	
	return RST_PRESENT; 
}

BOOL vmsDownloadMgrEx::IsDone()
{
	if (m_pMgr)
		return m_pMgr->IsDone ();
	else
		return m_pBtMgr->IsDone ();
}

BOOL vmsDownloadMgrEx::IsRunning()
{
	if (m_pMgr)
		return m_pMgr->IsRunning ();
	else
		return m_pBtMgr->IsRunning ();
}

BOOL vmsDownloadMgrEx::IsFileInitializedOnDisk()
{
	if (m_pMgr)
		return m_pMgr->IsFileInit ();
	else
		return TRUE;
}

void vmsDownloadMgrEx::set_LaunchWhenDone(BOOL b)
{
	if (m_pMgr)
	{
		if (b)
			m_pMgr->GetDP ()->dwFlags |= DPF_STARTWHENDONE;
		else
			m_pMgr->GetDP ()->dwFlags &= ~DPF_STARTWHENDONE;
	}
	else
	{
		if (b)
			m_pBtMgr->enable_Flags (BTDF_LAUNCH_WHEN_DONE);
		else
			m_pBtMgr->disable_Flags (BTDF_LAUNCH_WHEN_DONE);
	}
}

fsString vmsDownloadMgrEx::get_URL(BOOL bInclAuth)
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->get_URL (bInclAuth);
	else
		return m_pBtMgr->get_URL ();
}

int vmsDownloadMgrEx::GetNumberOfSections()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetNumberOfSections ();
	else
		return m_pBtMgr->GetNumberOfSections ();
}

UINT64 vmsDownloadMgrEx::GetLDFileSize()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetLDFileSize ();
	else
		return m_pBtMgr->GetTotalFilesSize ();
}

UINT64 vmsDownloadMgrEx::GetDownloadedBytesCount()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetDownloadedBytesCount ();
	else
		return m_pBtMgr->GetDownloadedBytesCount ();
}

BOOL vmsDownloadMgrEx::IsDownloading()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->IsDownloading ();
	else
		return m_pBtMgr->IsDownloading ();
}

UINT64 vmsDownloadMgrEx::GetSSFileSize()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetSSFileSize ();
	else
		return m_pBtMgr->GetTotalFilesSize ();
}

void vmsDownloadMgrEx::GetSectionInfo(int nIndex, vmsSectionInfo *sect, BOOL bNoCacheAccounting)
{
	if (m_pMgr)
	{
		fsSection s;
		m_pMgr->GetDownloader ()->GetSectionInfo (nIndex, &s, bNoCacheAccounting);
		sect->uDStart = s.uDStart;
		sect->uDCurrent = s.uDCurrent;
		sect->uDEnd = s.uDEnd;
	}
	else
	{
		m_pBtMgr->GetSectionInfo (nIndex, sect);
	}
}

int vmsDownloadMgrEx::GetDownloadingSectionCount()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetDownloadingSectionCount ();
	else
		return m_pBtMgr->GetDownloadingSectionCount ();
}

void vmsDownloadMgrEx::CreateOneMoreSection()
{
	if (m_pMgr)
		m_pMgr->CreateOneMoreSection ();

	ASSERT (GetBtDownloadMgr () == NULL);
}

void vmsDownloadMgrEx::StopSection()
{
	if (m_pMgr)
		m_pMgr->StopSection ();

	ASSERT (GetBtDownloadMgr () == NULL);
}

UINT64 vmsDownloadMgrEx::GetBytesLeft()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetBytesLeft ();
	else
		return m_pBtMgr->GetTotalFilesSize () - m_pBtMgr->GetDownloadedBytesCount ();
}

UINT vmsDownloadMgrEx::GetSpeed()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetSpeed ();
	else
		return m_pBtMgr->GetSpeed ();
}

BOOL vmsDownloadMgrEx::MoveToFolder(LPCSTR pszPath)
{
	if (m_pMgr)
		return m_pMgr->MoveToFolder (pszPath);
	else
		return m_pBtMgr->MoveToFolder (pszPath);
}

BOOL vmsDownloadMgrEx::IsStoppedByUser()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->IsStoppedByUser ();
	else
		return m_pBtMgr->IsStoppedByUser ();
}

BOOL vmsDownloadMgrEx::IsCantStart()
{
	if (m_pMgr)
		return m_pMgr->IsCantStart ();
	else
		return m_pBtMgr->IsCantStart ();
}

fsInternetResult vmsDownloadMgrEx::RestartDownloading()
{
	if (m_pMgr)
		return m_pMgr->RestartDownloading ();
	else
		return m_pBtMgr->RestartDownloading ();
}

BOOL vmsDownloadMgrEx::IsDescriptionFileShouldBeGenerated()
{
	if (m_pMgr)
		return m_pMgr->GetDP ()->dwFlags & DPF_GENERATEDESCFILE;
	else
		return m_pBtMgr->get_Flags () & BTDF_GENERATE_DESC_FILE;
}

BOOL vmsDownloadMgrEx::IsHtmlSpiderDownload()
{
	if (m_pMgr)
		return m_pMgr->GetDP ()->dwFlags & DLD_USEDBYHTMLSPIDER;
	else
		return FALSE;	
}

fsInternetResult vmsDownloadMgrEx::StartDownloading()
{
	if (m_pMgr)
		return m_pMgr->StartDownloading ();
	else
		return m_pBtMgr->StartDownloading ();
}

void vmsDownloadMgrEx::StopDownloading()
{
	if (m_pMgr)
		m_pMgr->StopDownloading ();
	else
		m_pBtMgr->StopDownloading ();
}

void vmsDownloadMgrEx::LimitTraffic(UINT uLimit)
{
	if (m_pMgr)
		m_pMgr->GetDownloader ()->LimitTraffic (uLimit);
	else
		m_pBtMgr->LimitTraffic (uLimit);
}

UINT vmsDownloadMgrEx::GetTrafficLimit()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetTrafficLimit ();
	else
		return m_pBtMgr->GetTrafficLimit ();
}

void vmsDownloadMgrEx::UseDetailedLog(BOOL b)
{
	if (m_pMgr)
		m_pMgr->GetDownloader ()->UseDetailedLog (b);
	else
		m_pBtMgr->UseDetailedLog (b);
}

BOOL vmsDownloadMgrEx::DeleteFile()
{
	if (m_pMgr)
		return m_pMgr->DeleteFile ();
	else
		return m_pBtMgr->DeleteFile ();
}

BOOL vmsDownloadMgrEx::IsDontConfirmLaunchWhenDone()
{
	if (m_pMgr)
		return m_pMgr->GetDP ()->dwFlags & DPF_STARTWHENDONE_NOCONF;
	else
		return m_pBtMgr->get_Flags () & BTDF_LAUNCH_WHEN_DONE_NO_CONFIRM;
}

fsInternetResult vmsDownloadMgrEx::SetToRestartState()
{
	if (m_pMgr)
		return m_pMgr->SetToRestartState ();
	else
		return m_pBtMgr->SetToRestartState ();
}

int vmsDownloadMgrEx::GetConnectionCount()
{
	if (m_pMgr)
		return m_pMgr->GetDownloader ()->GetRunningSectionCount ();
	else
		return m_pBtMgr->get_ConnectionCount ();
}

BOOL vmsDownloadMgrEx::SaveState(LPBYTE pb, DWORD *pdwSize)
{
	if (pb)
		(*pdwSize)--;

	BOOL bOk = m_pMgr ? m_pMgr->SaveState (pb ? pb + 1 : NULL, pdwSize) :
		m_pBtMgr->SaveState (pb ? pb + 1 : NULL, pdwSize);

	(*pdwSize)++;

	if (bOk == FALSE)
		return FALSE;

	if (pb)
		*pb = m_pMgr == NULL;

	return bOk;
}

BOOL vmsDownloadMgrEx::LoadState(LPBYTE lpBuffer, LPDWORD lpdwSize, WORD wVer)
{
#define CHECK_BOUNDS(need) if (need < 0 || need > int(*lpdwSize) - (pB - LPBYTE (lpBuffer))) return FALSE;
	LPBYTE pB = LPBYTE (lpBuffer);

	BYTE bBitTorrent = FALSE;
	DWORD dwMgrStateSize = *lpdwSize;
	DWORD dwConsumed = 0;

	if (wVer > 9)	
	{
		CHECK_BOUNDS (sizeof (BYTE));
		bBitTorrent = *lpBuffer++;
		dwConsumed++;
		dwMgrStateSize--;
	}

	BOOL bOk;

	if (bBitTorrent)
	{
		Attach (new vmsBtDownloadManager);
		bOk = m_pBtMgr->LoadState (lpBuffer, &dwMgrStateSize, wVer);
	}
	else
	{
		Attach (new fsDownloadMgr);	
		bOk = m_pMgr->LoadState (lpBuffer, &dwMgrStateSize, wVer);
	}

	if (bOk == FALSE)
		return FALSE;

	dwConsumed += dwMgrStateSize;
	*lpdwSize = dwConsumed;

	return TRUE;
}

BOOL vmsDownloadMgrEx::IsBittorrent()
{
	return m_pBtMgr != NULL;
}

void vmsDownloadMgrEx::GetSplittedSectionsList(std::vector <vmsSectionInfo> &v)
{
	try{

		std::vector <vmsSectionInfo> vBtSects;
		if (m_pBtMgr)
			m_pBtMgr->GetSectionsInfo (vBtSects);
		
		v.clear ();
		size_t num = GetNumberOfSections ();
		vmsSectionInfo *sectLast = NULL;
		
		for (size_t i = 0; i < num; i++)
		{
			vmsSectionInfo sect;
			if (m_pBtMgr)
				sect = vBtSects [i];
			else
				GetSectionInfo (i, &sect);
			if (sectLast != NULL && 
				(sectLast->uDCurrent == sect.uDStart || sectLast->uDCurrent+1 == sect.uDStart))
			{
				sectLast->uDEnd = sect.uDEnd;
				sectLast->uDCurrent = sect.uDCurrent;
			}
			else
			{
				v.push_back (sect);
				sectLast = v.end () - 1;
			}
		}
		
	}catch (...) {v.clear ();}	
	
	
}

BOOL vmsDownloadMgrEx::IsReservingDiskSpace()
{
	if (m_pMgr)
		return (m_pMgr->get_State () & DS_RESERVINGSPACE) != 0;
	else
		return m_pBtMgr->get_State () == BTDSE_ALLOCATING;
}

int vmsDownloadMgrEx::get_ReservingDiskSpaceProgress()
{
	if (m_pMgr)
		return m_pMgr->get_ReservingDiskSpaceProgress ();
	else
		return m_pBtMgr->get_CurrentTaskProgress ();
}

void vmsDownloadMgrEx::Do_OpenFolder()
{
	if (m_pMgr && m_pMgr->IsFileInit () == FALSE)
	{
		ShellExecute (NULL, "explore", get_OutputFilePathName (), NULL, NULL, SW_SHOW);
	}
	else
	{
		CString strFileName = get_OutputFilePathName ();

		if (m_pBtMgr && m_pBtMgr->get_FileCount () > 1)
			strFileName += m_pBtMgr->get_RootFolderName ();

		if (GetFileAttributes (strFileName) == DWORD (-1))
		{
			char szPath [MY_MAX_PATH];
			
			fsGetPath (strFileName, szPath);
			ShellExecute (NULL, "explore", szPath, NULL, NULL, SW_SHOW);
		}
		else
		{
			CString strCmd;
			strCmd.Format ("/select,\"%s\"", strFileName);
				ShellExecute (NULL, "open", "explorer.exe", strCmd, NULL, SW_SHOW);
		}
	}
}
