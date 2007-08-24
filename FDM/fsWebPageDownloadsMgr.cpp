/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "fsWebPageDownloadsMgr.h"
#include "mfchelp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsWebPageDownloadsMgr::fsWebPageDownloadsMgr()
{
	m_pfnEvents = NULL;
}

fsWebPageDownloadsMgr::~fsWebPageDownloadsMgr()
{
	for (int i = 0; i < m_vWPDs.size (); i++)
		delete m_vWPDs [i];
}

void fsWebPageDownloadsMgr::Add(fsWebPageDownloader *wpd)
{
	wpd->SetEventFunc (_DownloaderEvents, this);
	m_vWPDs.add (wpd);
}

void fsWebPageDownloadsMgr::_DownloaderEvents(fsWebPageDownloader* dldr, fsWPDEvent ev, int info, LPVOID lp)
{
	fsWebPageDownloadsMgr* pThis = (fsWebPageDownloadsMgr*) lp;
	if (pThis->m_pfnEvents)
		pThis->m_pfnEvents (dldr, ev, info, pThis->m_lpEventsParam);
}

void fsWebPageDownloadsMgr::SetEventsFunc(fntWPDEvents pfn, LPVOID lp)
{
	m_pfnEvents = pfn;
	m_lpEventsParam = lp;
}

int fsWebPageDownloadsMgr::GetWPDCount()
{
	return m_vWPDs.size ();
}

fsWebPageDownloader* fsWebPageDownloadsMgr::GetWPD(int iIndex)
{
	return m_vWPDs [iIndex];
}

void fsWebPageDownloadsMgr::Delete(fsWebPageDownloader *wpd)
{
	for (int i = 0; i < m_vWPDs.size (); i++)
	{
		if (m_vWPDs [i] == wpd)
		{
			m_vWPDs.del (i);
			delete wpd;
			return;
		}
	}
}

BOOL fsWebPageDownloadsMgr::Save()
{
	HANDLE hFile = CreateFile (fsGetDataFilePath ("spider.sav"), GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	int cWPD = m_vWPDs.size ();
	DWORD dw;
	fsSpiderFileHdr hdr;

	if (FALSE == WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	if (FALSE == WriteFile (hFile, &cWPD, sizeof (cWPD), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	for (int i = 0; i < cWPD; i++)
	{
		if (FALSE == m_vWPDs [i]->Save (hFile))
		{
			CloseHandle (hFile);
			return FALSE;
		}
	}

	CloseHandle (hFile);
	return TRUE;
}

BOOL fsWebPageDownloadsMgr::Load()
{
	for (int i = 0; i < m_vWPDs.size (); i++)
		delete m_vWPDs [i];

	m_vWPDs.clear ();

	HANDLE hFile = CreateFile (fsGetDataFilePath ("spider.sav"), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (GetLastError () != ERROR_ALREADY_EXISTS)
	{
		CloseHandle (hFile);
		return TRUE;
	}

	int cWPD;
	DWORD dw;
	fsSpiderFileHdr hdr;
	WORD wVer = SPIDERFILE_CURRENT_VERSION;

	if (FALSE == ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
	{
		wVer = 1;
	}
	else
	{
		if (strcmp (hdr.szSig, SPIDERFILE_SIG))
		{
			wVer = 1;
			SetFilePointer (hFile, 0, NULL, FILE_BEGIN);
		}
		else
		{
			wVer = hdr.wVer;
		}
	}

	if (FALSE == ReadFile (hFile, &cWPD, sizeof (cWPD), &dw, NULL))
	{
		CloseHandle (hFile);
		return FALSE;
	}

	for (i = 0; i < cWPD; i++)
	{
		fsWebPageDownloader *wpd;
		fsnew1 (wpd, fsWebPageDownloader);
		if (FALSE == wpd->Load (hFile, wVer))
		{
			delete wpd;
			CloseHandle (hFile);
			return FALSE;
		}
		Add (wpd);
	}

	CloseHandle (hFile);
	return TRUE;
}

BOOL fsWebPageDownloadsMgr::OnDownloadRestored(vmsDownloadSmartPtr dld)
{
	for (int i = m_vWPDs.size () - 1; i >= 0; i--)
	{
		fsWebPageDownloader* wpd = m_vWPDs [i];
		if (fsWebPageDownloader::_DldEvents (dld, DME_DLDRESTORED, wpd))
			return TRUE;
	}

	return FALSE;
}

void fsWebPageDownloadsMgr::StopAll()
{
	for (int i = m_vWPDs.size () - 1; i >= 0; i--)
	{
		fsWebPageDownloader* wpd = m_vWPDs [i];
		wpd->StopDownloading ();
	}
}
