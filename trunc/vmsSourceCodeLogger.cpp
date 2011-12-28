/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsSourceCodeLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef SCL_ENABLE
vmsSourceCodeLogger _sclgr (SCL_LOGSIZE_PER_THREAD);
#endif

vmsSourceCodeLogger::vmsSourceCodeLogger(int bufSizePerThread)
{
	m_vThreadsLogs.reserve (300);
	m_nBufSizePerThread = bufSizePerThread;

	char sz [MAX_PATH];
	GetModuleFileNameA (NULL, sz, MAX_PATH);
	strcat (sz, ".*.sclgr");
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFileA (sz, &wfd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		char szPath [MAX_PATH];
		strcpy (szPath, sz);
		int l = strlen (szPath);
		while (szPath [l-1] != '\\')
			l--;
		szPath [l] = 0;

		do 
		{
			char sz2 [MAX_PATH];
			strcpy (sz2, szPath);
			strcat (sz2, wfd.cFileName);
			DeleteFileA (sz2);
		}
		while (FindNextFile (hFind, &wfd));

		FindClose (hFind);
	}
}

vmsSourceCodeLogger::~vmsSourceCodeLogger()
{
	for (size_t i = 0; i < m_vThreadsLogs.size (); i++)
	{
		threadCtx *thr = &m_vThreadsLogs [i];
		FlushThreadLogBuffer (thr);
		CloseHandle (thr->hLogFile);
	}
}

void vmsSourceCodeLogger::log(LPCSTR psz, bool bAddNextLine)
{
	threadCtx *thr = getCurrentThreadContext ();
	if (thr == NULL)
		return;
	if (thr->bDisableLog)
		return;

	thr->strLog += psz;
	if (bAddNextLine)
		thr->strLog += "\r\n";

	if (thr->bLogCurrentTime && 
			(thr->strLog [thr->strLog.length ()-1] == '\n' || thr->strLog [thr->strLog.length ()-1] == '\r'))
	{
		SYSTEMTIME time;
		GetLocalTime (&time);
		
		if (time.wSecond != thr->stLastTimeLogged.wSecond ||
			time.wMinute != thr->stLastTimeLogged.wMinute ||
			time.wHour != thr->stLastTimeLogged.wHour)
		{
			char sz [40];
			sprintf (sz, "(time was: %02d:%02d:%02d)\r\n", (int)time.wHour, (int)time.wMinute, (int)time.wSecond);
			thr->strLog += sz;
			thr->stLastTimeLogged = time;
		}
	}
	
	if (thr->strLog.length () > m_nBufSizePerThread)
		FlushThreadLogBuffer (thr);
}

void vmsSourceCodeLogger::logf(LPCSTR pszFormat ...)
{
	threadCtx *thr = getCurrentThreadContext ();
	if (thr == NULL)
		return;
	if (thr->bDisableLog)
		return;
	
	va_list ap;
	char sz [10000];

	va_start (ap, pszFormat);
	vsprintf (sz, pszFormat, ap);
	strcat (sz, "\r\n");
	va_end (ap);

	log (sz);
}

int vmsSourceCodeLogger::findCurrentThreadIndex() const
{
	DWORD dwId = GetCurrentThreadId ();

	for (size_t i = 0; i < m_vThreadsLogs.size (); i++)
	{
		if (m_vThreadsLogs [i].dwThreadId == dwId)
			return i;
	}

	return -1;
}

void vmsSourceCodeLogger::FlushThreadLogBuffer(threadCtx *thr)
{
	DWORD dw;
	if (FALSE == WriteFile (thr->hLogFile, thr->strLog.c_str (), thr->strLog.length (), &dw, NULL))
		return;
	thr->strLog = "";
}

void vmsSourceCodeLogger::FlushBuffers()
{
	
	for (size_t i = 0; i < m_vThreadsLogs.size (); i++)
		FlushThreadLogBuffer (&m_vThreadsLogs [i]);
}

vmsSourceCodeLogger::threadCtx* vmsSourceCodeLogger::CreateCurrentThreadContext()
{
	threadCtx t;
	t.dwThreadId = GetCurrentThreadId ();
	t.bLogCurrentTime = false;
	char sz [MAX_PATH] = "";
	GetModuleFileNameA (NULL, sz, MAX_PATH);
	strcat (sz, ".");
	char sz2 [20] = "";
	itoa (t.dwThreadId, sz2, 16);
	strcat (sz, sz2);
	strcat (sz, ".sclgr");
	t.hLogFile = CreateFileA (sz, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (t.hLogFile == INVALID_HANDLE_VALUE)
		return NULL;
	m_vThreadsLogs.push_back (t);
	threadCtx *thr = &m_vThreadsLogs [findCurrentThreadIndex ()];
	thr->strLog.reserve (m_nBufSizePerThread + 3000);
	return thr;
}

void vmsSourceCodeLogger::setLogCurrentTimeForCurrentThread(bool bLog)
{
	threadCtx *thr = getCurrentThreadContext ();
	if (thr == NULL)
		return;
	thr->bLogCurrentTime = bLog;	
}

vmsSourceCodeLogger::threadCtx* vmsSourceCodeLogger::getCurrentThreadContext()
{
	int n = findCurrentThreadIndex ();
	if (n != -1)
		return &m_vThreadsLogs [n];
	return CreateCurrentThreadContext ();
}

void vmsSourceCodeLogger::DisableLogForCurrentThread(bool bDisable)
{
	threadCtx *thr = getCurrentThreadContext ();
	if (thr == NULL)
		return;
	thr->bDisableLog = bDisable;
}

bool vmsSourceCodeLogger::isLogForCurrentThreadDisabled()
{
	threadCtx *thr = getCurrentThreadContext ();
	if (thr == NULL)
		return false;
	return thr->bDisableLog;
}

void vmsSourceCodeLogger::logSysError(DWORD dw)
{
	if (!dw)
		return;
	logf ("(0x%x - %s)", dw, StringFromError (dw).c_str ());
}

void vmsSourceCodeLogger::logResult(LPCSTR pszDescription, DWORD dwResultCode)
{
	if (dwResultCode == 0)
	{
		std::string str = pszDescription;
		str += ": ok";
		log (str.c_str (), true);
	}
	else
	{
		logf ("%s: 0x%x - %s", pszDescription, dwResultCode, StringFromError (dwResultCode).c_str ());
	}
}

std::string vmsSourceCodeLogger::StringFromError(DWORD dw)
{
	LPSTR psz = NULL;
	
	FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		NULL, dw, 0, (LPSTR)&psz, 0, NULL);
	
	if (psz)
	{
		while (*psz != 0 && (psz [strlen (psz)-1] == '\n' || psz [strlen (psz)-1] == '\r'))
			psz [strlen (psz)-1] = 0;
		if (*psz && psz [strlen (psz)-1] == '.')
			psz [strlen (psz)-1] = 0;
		std::string str = psz;
		LocalFree (psz);
		return str;
	}
	else
	{
		return "unknown error";
	}
}
