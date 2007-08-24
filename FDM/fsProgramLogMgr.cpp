/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "fsProgramLogMgr.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif        

fsProgramLogMgr::fsProgramLogMgr()
{
	Set_DoLog (TRUE);
	InitializeCriticalSection (&m_csLogFileOp);
	m_hLogFile = INVALID_HANDLE_VALUE;
}

fsProgramLogMgr::~fsProgramLogMgr()
{
	DeleteCriticalSection (&m_csLogFileOp);

	if (m_hLogFile != INVALID_HANDLE_VALUE)
		CloseHandle (m_hLogFile);
}

void fsProgramLogMgr::Set_DoLog(BOOL bDo)
{
	m_bDoLog = bDo;
}

BOOL fsProgramLogMgr::Initialize(LPCSTR pszLogFileName)
{
	m_hLogFile = CreateFile (pszLogFileName, GENERIC_WRITE, FILE_SHARE_READ, 
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hLogFile == INVALID_HANDLE_VALUE)
		return FALSE;

	SetFilePointer (m_hLogFile, 0, NULL, FILE_END);

	return TRUE;
}

BOOL fsProgramLogMgr::EraseLog()
{
	LockFileOperations ();

	SetFilePointer (m_hLogFile, 0, NULL, FILE_BEGIN);
	BOOL b = SetEndOfFile (m_hLogFile);

	UnlockFileOperations ();

	return b;
}

void fsProgramLogMgr::LockFileOperations()
{
	EnterCriticalSection (&m_csLogFileOp);
}

void fsProgramLogMgr::UnlockFileOperations()
{
	LeaveCriticalSection (&m_csLogFileOp);
}

BOOL fsProgramLogMgr::FlushBuffer()
{
	LockFileOperations ();

	DWORD dwWritten = 0;
	DWORD dwLen = m_strLogBuffer.Length ();

	if (dwLen > 0)	
	{
		WriteFile (m_hLogFile, m_strLogBuffer, dwLen, &dwWritten, NULL);
		m_strLogBuffer = "";
	}
	else
	{
		dwWritten = dwLen;
	}

	UnlockFileOperations ();

	return dwWritten == dwLen;
}

void fsProgramLogMgr::AddStringToLog(LPCSTR psz)
{
	if (m_bDoLog == FALSE)
		return;

	LockFileOperations ();
	m_strLogBuffer += psz;
	FlushBuffer ();
	UnlockFileOperations ();
}

fsProgramLogMgr& fsProgramLogMgr::operator <<(LPCSTR psz)
{
	if (m_bDoLog == FALSE)
		return *this;

	LockFileOperations ();
	if (psz == nl)
	{
		SYSTEMTIME time;
		GetLocalTime (&time);
		operator << (" (");
		operator << (time.wHour);
		operator << (":");
		operator << (time.wMinute);
		operator << (":");
		operator << (time.wSecond);
                operator << (")");
	}
	AddStringToLog (psz);
	UnlockFileOperations ();

	return *this;
}

fsProgramLogMgr& fsProgramLogMgr::operator <<(int i)
{
	if (m_bDoLog == FALSE)
		return *this;

	char sz [100];
	itoa (i, sz, 10);
	return operator << (sz);
}

fsProgramLogMgr& fsProgramLogMgr::operator <<(DWORD dw)
{
	if (m_bDoLog == FALSE)
		return *this;

	char sz [100];
	sprintf (sz, "%u", dw);
	return operator << (sz);
}

fsProgramLogMgr& fsProgramLogMgr::operator <<(double d)
{
	if (m_bDoLog == FALSE)
		return *this;

	char sz [100];
	sprintf (sz, "%.*g", FLT_DIG, d);
	return operator << (sz);
}

fsProgramLogMgr& fsProgramLogMgr::operator <<(UINT64 u)
{
	if (m_bDoLog == FALSE)
		return *this;

	char sz [100];
	_i64toa (u, sz, 10);
	return operator << (sz);
}
