/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#if !defined(AFX_VMSSOURCECODELOGGER_H__D585CF28_BB90_4FDD_BC04_6E6B08105495__INCLUDED_)
#define AFX_VMSSOURCECODELOGGER_H__D585CF28_BB90_4FDD_BC04_6E6B08105495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <string>
#include <vector>

#ifdef SCL_ENABLE
extern class vmsSourceCodeLogger _sclgr;
#define LOG	_sclgr.logf
#define LOGs(s) _sclgr.log (s)
#define LOGsnl(s) _sclgr.log (s, true)
#define LOGFN(a) vmsSourceCodeLogger_Function _sclgr_fn (a)
#define LOGERR(e) _sclgr.logSysError (e)
#define LOGRESULT(desc,res) _sclgr.logResult (desc, res)
#define SCL_FLUSH_ALL _sclgr.FlushBuffers ()
#define SCL_DISABLE_LOG_IF(b) vmsSourceCodeLogger_DisableLogIf _sclgr_dlif (b)
#else
#define LOG(a)
#define LOGs(s)
#define LOGsnl(s)
#define LOGFN(a)
#define LOGERR(e)
#define LOGRESULT(desc,res)
#define SCL_FLUSH_ALL
#define SCL_DISABLE_LOG_IF(b)
#pragma warning (disable: 4002)
#endif

class vmsSourceCodeLogger  
{
public:
	void logResult (LPCSTR pszDescription, DWORD dwResultCode);
	void logSysError (DWORD dw);
	bool isLogForCurrentThreadDisabled();
	void DisableLogForCurrentThread(bool bDisable = true);
	void setLogCurrentTimeForCurrentThread(bool bLog = true);
	void FlushBuffers();
	void log (LPCSTR psz, bool bAddNextLine = false);
	void logf (LPCSTR pszFormat ...);
	vmsSourceCodeLogger(int bufSizePerThread = 512*1024);
	virtual ~vmsSourceCodeLogger();

protected:
	std::string StringFromError (DWORD dw);
	struct threadCtx
	{
		DWORD dwThreadId;
		std::string strLog;
		HANDLE hLogFile;
		bool bLogCurrentTime;
		SYSTEMTIME stLastTimeLogged;
		bool bDisableLog;

		threadCtx () {
			ZeroMemory (&stLastTimeLogged, sizeof (stLastTimeLogged));
			hLogFile = INVALID_HANDLE_VALUE;
			bDisableLog = false;
			bLogCurrentTime = false;
		}
	};

	threadCtx* getCurrentThreadContext();
	threadCtx* CreateCurrentThreadContext ();
	void FlushThreadLogBuffer (threadCtx* thr);
	int findCurrentThreadIndex () const;
	size_t m_nBufSizePerThread;
	std::vector <threadCtx> m_vThreadsLogs;
};

#ifdef SCL_ENABLE

class vmsSourceCodeLogger_Function
{
public:
	vmsSourceCodeLogger_Function (LPCSTR pszDesc)
	{
		LOG ("%s started.", pszDesc);
		m_strDesc = pszDesc;
	}

	~vmsSourceCodeLogger_Function ()
	{
		LOG ("%s has finished.", m_strDesc.c_str ());
	}

protected:
	std::string m_strDesc;
};

class vmsSourceCodeLogger_DisableLogIf 
{
public:
	vmsSourceCodeLogger_DisableLogIf (bool b)
	{
		m_bLogWasDisabledAlready = _sclgr.isLogForCurrentThreadDisabled ();
		if (m_bLogWasDisabledAlready == false && b)
			_sclgr.DisableLogForCurrentThread (true);
	}

	~vmsSourceCodeLogger_DisableLogIf ()
	{
		if (m_bLogWasDisabledAlready == false)
			_sclgr.DisableLogForCurrentThread (false);
	}

protected:
	bool m_bLogWasDisabledAlready;
};

#endif 

#endif 
