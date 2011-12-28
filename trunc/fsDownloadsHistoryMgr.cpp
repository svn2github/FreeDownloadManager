/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsDownloadsHistoryMgr.h"
#include "mfchelp.h"
#include "WaitForConfirmationDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsDownloadsHistoryMgr::fsDownloadsHistoryMgr()
{
	m_pfnEventFunc = NULL;
}

fsDownloadsHistoryMgr::~fsDownloadsHistoryMgr()
{

}

void fsDownloadsHistoryMgr::ReadSettings()
{
	m_bUse = _App.History_Downloads_Use ();
	m_cDaysMax = _App.History_Downloads_KeepDays ();
	m_bCompOnly = _App.History_Downloads_CompletedOnly ();

	ApplySettings ();
}

void fsDownloadsHistoryMgr::ApplySettings()
{
	if (m_bUse == FALSE)
	{
		ClearHistory ();
		return;
	}

	SYSTEMTIME stime;
	GetLocalTime (&stime);
	FILETIME time;
	SystemTimeToFileTime (&stime, &time);

	vmsAUTOLOCKSECTION (m_csRecords);

	for (int i = m_vRecords.size () - 1; i >= 0; i--)
	{
		fsDLHistoryRecord* r = m_vRecords [i];
		int cDays = fsGetFTimeDaysDelta (time, r->dateRecordAdded);

		
		if (UINT (cDays) < m_cDaysMax)
			break;	

		Event (DHME_RECORDDELETED, r);

		m_vRecords.erase (m_vRecords.begin () + i);
		_DldsMgr.QueryStoringHistory();
	}
}

void fsDownloadsHistoryMgr::ClearHistory()
{
	vmsAUTOLOCKSECTION (m_csRecords);
	m_vRecords.clear ();
	vmsAUTOLOCKSECTION_UNLOCK (m_csRecords);
	_DldsMgr.QueryStoringHistory();
	Event (DHME_HISTORYCLEARED);
}

void fsDownloadsHistoryMgr::SetEventFunc(fntDHMEEventFunc pfn, LPVOID lpParam)
{
	m_pfnEventFunc = pfn;
	m_lpEvParam = lpParam;
}

void fsDownloadsHistoryMgr::Event(fsDownloadsHistoryMgrEvent ev, fsDLHistoryRecord *rec)
{
	if (m_pfnEventFunc)
		m_pfnEventFunc (ev, rec, m_lpEvParam);
}

void fsDownloadsHistoryMgr::AddToHistory(vmsDownloadSmartPtr dld)
{
	if (m_bUse == FALSE)
		return;

	if (m_bCompOnly && dld->pMgr->IsDone () == FALSE)
		return;

	fsDLHistoryRecordPtr r;
	r.CreateInstance ();
	
	r->strURL = dld->pMgr->get_URL ();
	r->dateAdded = dld->dateAdded;
	
	SYSTEMTIME t;
	FILETIME time;
	GetLocalTime (&t);
	SystemTimeToFileTime (&t, &time);

	r->dateRecordAdded = time;
	if (dld->pMgr->IsDone ())
		r->dateDownloaded = time;
	else
		ZeroMemory (&r->dateDownloaded, sizeof (FILETIME));

	char sz [MY_MAX_PATH];
	CDownloads_Tasks::GetFileName (dld, sz);
	r->strFileName = sz;

	r->strSavedTo = dld->pMgr->get_OutputFilePathName ();

	r->uFileSize = dld->pMgr->GetLDFileSize ();
	if (r->uFileSize == 0 && dld->pMgr->GetNumberOfSections () == 0)
		r->uFileSize = _UI64_MAX;

	r->strComment = dld->strComment;

	vmsAUTOLOCKSECTION (m_csRecords);
	m_vRecords.insert (m_vRecords.begin (), r);
	vmsAUTOLOCKSECTION_UNLOCK (m_csRecords);

	_DldsMgr.QueryStoringHistory();

	Event (DHME_RECORDADDED, m_vRecords [0]);
}

int fsDownloadsHistoryMgr::GetRecordCount()
{
	vmsAUTOLOCKSECTION (m_csRecords);
	return m_vRecords.size ();
}

BOOL fsDownloadsHistoryMgr::SaveHistory()
{
	HANDLE hFile = CreateFile (fsGetDataFilePath ("downloads.his.sav"), GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	fsDownloadsHistMgrFileHdr hdr;
	DWORD dw;

	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	int cRecs = GetRecordCount ();
	if (FALSE == WriteFile (hFile, &cRecs, sizeof (int), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	int i = 0;
	for (i = 0; i < cRecs; i++)
	{
		fsDLHistoryRecord* rec = GetRecord (i);
		if (FALSE == fsSaveStrToFile (rec->strFileName, hFile))
			break;

		if (FALSE == fsSaveStrToFile (rec->strSavedTo, hFile))
			break;

		if (FALSE == fsSaveStrToFile (rec->strURL, hFile))
			break;

		if (FALSE == fsSaveStrToFile (rec->strComment, hFile))
			break;

		if (FALSE == WriteFile (hFile, &rec->dateAdded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == WriteFile (hFile, &rec->dateDownloaded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == WriteFile (hFile, &rec->dateRecordAdded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == WriteFile (hFile, &rec->uFileSize, sizeof (UINT64), &dw, NULL))
			break;
	}

	CloseHandle (hFile);

	if (i != cRecs)
		return FALSE;

	return TRUE;
}

BOOL fsDownloadsHistoryMgr::LoadHistory()
{
	vmsAUTOLOCKSECTION (m_csRecords);

	if (GetFileAttributes (fsGetDataFilePath ("downloads.his.sav")) == UINT (-1))
		return TRUE;

	HANDLE hFile = CreateFile (fsGetDataFilePath ("downloads.his.sav"), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (_App.CheckHistorySize () && GetFileSize (hFile, NULL) > 1*1024*1024)
	{
		CWaitForConfirmationDlg dlg;
		dlg.Init (LS (L_HISTTOOLARGE), UINT_MAX, TRUE, FALSE, LS (L_DONTCHECKAGAIN));
		dlg.m_pszIcon = IDI_WARNING;
		UINT nRet = dlg.DoModal ();
		if (dlg.m_bDontAsk)
			_App.CheckHistorySize (FALSE);
		if (nRet == IDOK)
		{
			CloseHandle (hFile);
			return TRUE;
		}
	}

	fsDownloadsHistMgrFileHdr hdr;
	DWORD dw;

	if (FALSE == ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	if (hdr.wVer != DLHISTFILE_CURRENT_VERSION ||
			strnicmp (hdr.szSig, DLHISTFILE_SIG, strlen (DLHISTFILE_SIG)))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	int cRecs;
	if (FALSE == ReadFile (hFile, &cRecs, sizeof (int), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	int i = 0;
	for (i = 0; i < cRecs; i++)
	{
		fsDLHistoryRecordPtr rec; rec.CreateInstance ();

		if (FALSE == fsReadStrFromFile (&rec->strFileName.pszString, hFile))
			break;

		if (FALSE == fsReadStrFromFile (&rec->strSavedTo.pszString, hFile))
			break;

		if (FALSE == fsReadStrFromFile (&rec->strURL.pszString, hFile))
			break;

		if (FALSE == fsReadStrFromFile (&rec->strComment.pszString, hFile))
			break;

		if (FALSE == ReadFile (hFile, &rec->dateAdded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == ReadFile (hFile, &rec->dateDownloaded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == ReadFile (hFile, &rec->dateRecordAdded, sizeof (FILETIME), &dw, NULL))
			break;

		if (FALSE == ReadFile (hFile, &rec->uFileSize, sizeof (UINT64), &dw, NULL))
			break;

		m_vRecords.push_back (rec);
	}

	CloseHandle (hFile);

	if (i != cRecs)
		return FALSE;

	return TRUE;
}

fsDLHistoryRecord* fsDownloadsHistoryMgr::GetRecord(int iIndex)
{
	vmsAUTOLOCKSECTION (m_csRecords);
	return m_vRecords [iIndex];
}

void fsDownloadsHistoryMgr::DeleteRecord(fsDLHistoryRecord *rec)
{
	vmsAUTOLOCKSECTION (m_csRecords);
	int i = FindIndex (rec);
	if (i != -1)
	{
		Event (DHME_RECORDDELETED, rec);
		m_vRecords.erase (m_vRecords.begin () + i);
		_DldsMgr.QueryStoringHistory();
	}
}

int fsDownloadsHistoryMgr::FindIndex(fsDLHistoryRecord *rec)
{
	for (size_t i = 0; i < m_vRecords.size (); i++)
	{
		if (m_vRecords [i] == rec)
			return (int)i;
	}

	return -1;
}
