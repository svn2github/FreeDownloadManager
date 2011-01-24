/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSCMDHISTORYMGR_H__A0BE2A45_DC68_4ED6_9724_A64D2419D197__INCLUDED_)
#define AFX_FSCMDHISTORYMGR_H__A0BE2A45_DC68_4ED6_9724_A64D2419D197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"  

#define HISTFILE_CURRENT_VERSION	(1)

#define HISTFILE_SIG "FDM History"

struct fsHistFileHdr
{
	char szSig [sizeof (HISTFILE_SIG) + 1];
	WORD wVer;

	fsHistFileHdr ()
	{
		strcpy (szSig, HISTFILE_SIG);
		wVer = HISTFILE_CURRENT_VERSION;
	}
};

class fsCmdHistoryMgr  
{
public:
	
	void Set_MaxDaysCount (int cMax);
	
	void Set_NoHistory (BOOL b);
	
	void ClearHistory();
	
	void Set_MaxRecordCount (int iMax);
	
	void AddRecord (LPCSTR pszRecord);
	
	LPCSTR GetRecord (int iRec);
	
	int GetRecordCount();
	
	BOOL SaveToFile (HANDLE hFile);
	
	BOOL ReadFromFile (HANDLE hFile);

	fsCmdHistoryMgr();
	virtual ~fsCmdHistoryMgr();

protected:
	
	struct fs1DayRecords
	{
		FILETIME day;	
		
		
		
		fs::list <CString> vRecs;
	};

	
	
	
	fs::list <fs1DayRecords> m_vRecs;
	int m_cMaxRecords;			
	SYSTEMTIME m_curday;	
	BOOL m_bNoHistory;	
	int m_cMaxDays;	
};

#endif 
