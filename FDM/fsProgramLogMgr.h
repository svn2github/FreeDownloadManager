/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#if !defined(AFX_FSPROGRAMLOGMGR_H__DBE5A2AB_BCCB_4AA0_A615_DED3AD7C078E__INCLUDED_)
#define AFX_FSPROGRAMLOGMGR_H__DBE5A2AB_BCCB_4AA0_A615_DED3AD7C078E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

const char* const nl = "\r\n";

class fsProgramLogMgr  
{
public:
	fsProgramLogMgr& fsProgramLogMgr::operator <<(UINT64 u);
	fsProgramLogMgr& fsProgramLogMgr::operator <<(double d);
	fsProgramLogMgr& fsProgramLogMgr::operator <<(DWORD dw);
	fsProgramLogMgr& fsProgramLogMgr::operator <<(long i) {return operator << (int (i));};
	fsProgramLogMgr& fsProgramLogMgr::operator <<(int i);
	fsProgramLogMgr& fsProgramLogMgr::operator <<(LPCSTR psz);
	void AddStringToLog (LPCSTR psz);
	BOOL EraseLog();
	BOOL Initialize (LPCSTR pszLogFileName);
	
	void Set_DoLog (BOOL bDo = TRUE);

	fsProgramLogMgr();
	virtual ~fsProgramLogMgr();

protected:
	BOOL FlushBuffer();
	fsString m_strLogBuffer;
	void UnlockFileOperations();
	void LockFileOperations();
	CRITICAL_SECTION m_csLogFileOp;
	HANDLE m_hLogFile;
	BOOL m_bDoLog;
};

#endif 
