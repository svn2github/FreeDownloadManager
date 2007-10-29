/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "FdmApp.h"
#include "fsCmdHistoryMgr.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsCmdHistoryMgr::fsCmdHistoryMgr()
{
	m_cMaxDays = 15;
	m_cMaxRecords = 30;
	GetLocalTime (&m_curday);
}

fsCmdHistoryMgr::~fsCmdHistoryMgr()
{

}

BOOL fsCmdHistoryMgr::SaveToFile(HANDLE hFile)
{
	fsHistFileHdr hdr;
	DWORD dw;

	if (!WriteFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		return FALSE;

	int cDays = m_vRecs.size ();

	if (!WriteFile (hFile, &cDays, sizeof (cDays), &dw, NULL))
		return FALSE;

	for (int i = 0; i < m_vRecs.size (); i++)
	{
		if (!WriteFile (hFile, &m_vRecs [i].day, sizeof (FILETIME), &dw, NULL))
			return FALSE;

		int cRecs = m_vRecs [i].vRecs.size ();

		if (!WriteFile (hFile, &cRecs, sizeof (cRecs), &dw, NULL))
			return FALSE;

		for (int j = 0; j < cRecs; j++)
		{
			if (!fsSaveStrToFile (m_vRecs [i].vRecs [j], hFile))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL fsCmdHistoryMgr::ReadFromFile(HANDLE hFile)
{
	DWORD dw;
	fsHistFileHdr hdr;
	int cDays;

	if (!ReadFile (hFile, &hdr, sizeof (hdr), &dw, NULL))
		return FALSE;

	if (strnicmp (hdr.szSig, HISTFILE_SIG, strlen (HISTFILE_SIG)))
		return FALSE;

	if (hdr.wVer != HISTFILE_CURRENT_VERSION)
		return FALSE;

	if (!ReadFile (hFile, &cDays, sizeof (cDays), &dw, NULL))
		return FALSE;

	for (int i = 0; i < cDays; i++)
	{
		fs1DayRecords rec;

		if (!ReadFile (hFile, &rec.day, sizeof (rec.day), &dw, NULL))
			return FALSE;

		int cRecs;
		if (!ReadFile (hFile, &cRecs, sizeof (cRecs), &dw, NULL))
			return FALSE;

		while (cRecs--)
		{
			char *pszRec;

			if (!fsReadStrFromFile (&pszRec, hFile))
				return FALSE;
		
			rec.vRecs.add (pszRec);
			delete [] pszRec;
		}

		m_vRecs.add (rec);

		if (i == cDays-1)
			FileTimeToSystemTime (&rec.day, &m_curday);
	}

	Set_MaxRecordCount (m_cMaxRecords);
	Set_MaxDaysCount (m_cMaxDays);

	return TRUE;
}

int fsCmdHistoryMgr::GetRecordCount()
{
	int cRecords = 0;
	for (int i = 0; i < m_vRecs.size (); i++)
		cRecords += m_vRecs [i].vRecs.size ();
	return cRecords;
}

void fsCmdHistoryMgr::AddRecord(LPCSTR pszRecord)
{
	if (m_bNoHistory)
		return;

	if (m_vRecs.size () == 0)
	{
		fs1DayRecords rec;
		GetLocalTime (&m_curday);
		SystemTimeToFileTime (&m_curday, &rec.day);
		rec.vRecs.add (pszRecord);
		m_vRecs.add (rec);
		return;
	}

	SYSTEMTIME time;
	GetLocalTime (&time);
	if (time.wDay != m_curday.wDay)
	{
		
		fs1DayRecords rec;
		m_curday = time;
		SystemTimeToFileTime (&m_curday, &rec.day);
		m_vRecs.add (rec); 
	}
	
	

	for (int i = 0; i < m_vRecs.size (); i++)
	{
		fs::list <CString>* recs = &m_vRecs [i].vRecs;

		for (int j = 0; j < recs->size (); j++)
		{
			if (recs->at (j).CompareNoCase (pszRecord) == 0)
			{
				recs->del (j);
				break;
			}
		}

		
		if (j != recs->size ())
			break; 
	}

	

	fs::list <CString>* recs = &m_vRecs [m_vRecs.size () - 1].vRecs;

	recs->insert (0, pszRecord);

	if (m_vRecs.size () > m_cMaxDays)
		m_vRecs.del (0);

	

	while (GetRecordCount () > m_cMaxRecords)
	{
		while (m_vRecs [0].vRecs.size () == 0)
			m_vRecs.del (0);

		m_vRecs [0].vRecs.del (m_vRecs [0].vRecs.size ()-1);
		
		if (m_vRecs [0].vRecs.size () == 0)
			m_vRecs.del (0);
	}
}

LPCSTR fsCmdHistoryMgr::GetRecord(int iRec)
{
	
	
	for (int i = m_vRecs.size () - 1; i >= 0, iRec >= 0; i--)
	{
		if (m_vRecs [i].vRecs.size () <= iRec)
		{
			iRec -= m_vRecs [i].vRecs.size ();
			continue; 
		}

		
		return m_vRecs [i].vRecs [iRec];
	}

	return NULL; 
}

void fsCmdHistoryMgr::Set_MaxRecordCount(int iMax)
{
	m_cMaxRecords = iMax;
	int nExcess = GetRecordCount () - m_cMaxRecords;
	while (nExcess > 0)
	{
		
		int sz = m_vRecs [0].vRecs.size ();
		if (sz <= nExcess)
		{
			
			m_vRecs.del (0);
			nExcess -= sz;
		}
		else
		{
			
			while (nExcess-- > 0)
			{
				m_vRecs [0].vRecs.del (--sz);
			}
		}
	}
}

void fsCmdHistoryMgr::ClearHistory()
{
	m_vRecs.clear ();
}

void fsCmdHistoryMgr::Set_NoHistory(BOOL b)
{
	m_bNoHistory = b;
	if (m_bNoHistory)
		ClearHistory ();
}

void fsCmdHistoryMgr::Set_MaxDaysCount(int cMax)
{
	m_cMaxDays = cMax;
	while (m_vRecs.size () > m_cMaxDays)
		m_vRecs.del (0);
}
