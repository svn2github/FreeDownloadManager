/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <fstream>
#include "../vmsExceptionLog.h"
#include "exc_provider/vmsUnhandledExceptionProvider.h"
#include "exc_provider/vmsCrtFatalExceptionProvider.h"
#include "exc_provider/vmsVectoredFatalExceptionProvider.h"
#include "../../win/process/util.h"
#pragma comment (lib, "dbghelp.lib")
class vmsCrashCatcher
{
public:
	enum 
	{
		OverrideTopLevelExceptionFilter		= 1,
		CatchCrtExceptions					= 1 << 1,
		UseVectoredExceptionsHandler		= 1 << 2,
		CatchThisModuleExceptionsOnly		= 1 << 3,
		DumpWithFullMemory					= 1 << 4,
	};

	enum
	{
		LegacyMode = (OverrideTopLevelExceptionFilter | CatchCrtExceptions)
	};

public:
	vmsCrashCatcher () : 
		m_dwpFaultModuleCrashAddress (NULL), 
		m_dwGettingFaultModuleNameError (0)
	{
	}

	virtual ~vmsCrashCatcher(void)
	{
	}

	void Initialize ()
	{
		assert (m_excProviders.empty ());
		if (!m_excProviders.empty ())
			return;

		if (m_flags & OverrideTopLevelExceptionFilter)
			m_excProviders.push_back (std::make_unique <vmsUnhandledExceptionProvider> ());

		if (m_flags & CatchCrtExceptions)
			m_excProviders.push_back (std::make_unique <vmsCrtFatalExceptionProvider> ());

		if (m_flags & UseVectoredExceptionsHandler)
			m_excProviders.push_back (std::make_unique <vmsVectoredFatalExceptionProvider> ());

		auto fe_callback = [this](PEXCEPTION_POINTERS ep){return on_fatal_exception (ep);};

		for (const auto &prov : m_excProviders)
			prov->set_callback (fe_callback);
	}

	void Stop ()
	{
		m_excProviders.clear ();
	}

	void setLogExceptions (bool bSet = true)
	{
		try {
			if (!bSet)
			{
				m_LogFile.close ();
				return;
			}
			TCHAR tszPath [MAX_PATH] = _T ("");
			GetTempPath (_countof (tszPath), tszPath);
			_tcscat (tszPath, _T ("\\CrashCatcher"));
			CreateDirectory (tszPath, NULL);
			TCHAR tszModule [MAX_PATH] = _T ("");
			GetModuleFileName (NULL, tszModule, _countof (tszModule));
			LPTSTR ptszExeName = _tcsrchr (tszModule, '\\');
			if (ptszExeName)
			{
				ptszExeName++;
				_tcscat (tszPath, _T ("\\"));
				_tcscat (tszPath, ptszExeName);
				_tcscat (tszPath, _T (".xml"));
				m_LogFile.open (tszPath, std::ios_base::out | std::ios_base::trunc);
			}
		}catch (...){}
	}

protected:
	std::vector <std::unique_ptr <vmsFatalExceptionProvider>> m_excProviders;
	tstring m_tstrDumpFile;
	tstring m_tstrFaultModuleName;
	DWORD_PTR m_dwpFaultModuleCrashAddress;
	DWORD m_dwGettingFaultModuleNameError;
	uint32_t m_flags = LegacyMode;

protected:
	ULONG on_fatal_exception (PEXCEPTION_POINTERS pEP)
	{
		if (m_flags & CatchThisModuleExceptionsOnly)
		{
			if (vmsGetThisModuleHandle () != 
				vmsModuleFromAddress (pEP->ExceptionRecord->ExceptionAddress))
			{
				return EXCEPTION_CONTINUE_SEARCH;
			}
		}

		auto params = new _threadProcessExceptionParams;
		params->pthis = this;
		params->pEP = pEP;
		params->thread_id = GetCurrentThreadId ();

		WaitForSingleObject (
			CreateThread (NULL, 0, _threadProcessException, params, 0, NULL), INFINITE);

		return EXCEPTION_CONTINUE_SEARCH;
	}

	tstring CreateDump (PEXCEPTION_POINTERS pEP, DWORD thread_id) const
	{
		

		MINIDUMP_EXCEPTION_INFORMATION eInfo;
		eInfo.ThreadId = thread_id;
		eInfo.ExceptionPointers = pEP;
		eInfo.ClientPointers = FALSE;

		TCHAR tszTmpFile [MAX_PATH] = _T ("");
		TCHAR tszTmpPath [MAX_PATH] = _T ("");
		GetTempPath (MAX_PATH, tszTmpPath);
		GetTempFileName (tszTmpPath, _T ("tmp"), 0, tszTmpFile);

		HANDLE hFile = CreateFile (tszTmpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return 0;

		DWORD dump_type = 
			MiniDumpScanMemory | 
			MiniDumpWithIndirectlyReferencedMemory | 
			MiniDumpWithDataSegs | 
			MiniDumpWithProcessThreadData;

		if (m_flags & DumpWithFullMemory)
			dump_type |= MiniDumpWithFullMemory;

		BOOL bDumpCreated = MiniDumpWriteDump (GetCurrentProcess(), 
			GetCurrentProcessId(), hFile, (MINIDUMP_TYPE) dump_type, 
			&eInfo, NULL, NULL);

		CloseHandle (hFile);

		if (!bDumpCreated)
			DeleteFile (tszTmpFile);
		
		return bDumpCreated ? tszTmpFile : _T ("");
	}

	void ProcessException (PEXCEPTION_POINTERS pEP, DWORD thread_id)
	{
		vmsAUTOLOCKSECTION (m_csExceptionHandler);

		LogException (pEP);

		m_tstrDumpFile = CreateDump (pEP, thread_id);

		

		MEMORY_BASIC_INFORMATION mbi;
		SIZE_T nSize = VirtualQuery (pEP->ExceptionRecord->ExceptionAddress, &mbi, sizeof(mbi));
		if (nSize)
		{
			m_dwpFaultModuleCrashAddress = (DWORD_PTR)pEP->ExceptionRecord->ExceptionAddress - (DWORD_PTR)mbi.AllocationBase;
			TCHAR tszModule [MAX_PATH] = _T ("");
			GetModuleFileName ((HMODULE)mbi.AllocationBase, tszModule, _countof (tszModule));
			if (*tszModule)
				m_tstrFaultModuleName = _tcsrchr (tszModule, '\\') ? _tcsrchr (tszModule, '\\') + 1 : tszModule;			
			else
				m_dwGettingFaultModuleNameError = GetLastError ();
		}

		if (!m_tstrDumpFile.empty ())
			onCrashDumpCreated ();

		TerminateProcess (GetCurrentProcess (), ERROR_UNHANDLED_EXCEPTION);
	}

protected:
	struct _threadProcessExceptionParams
	{
		vmsCrashCatcher *pthis;
		PEXCEPTION_POINTERS pEP;
		DWORD thread_id;
	};

	static DWORD WINAPI _threadProcessException (LPVOID lp)
	{
		std::unique_ptr <_threadProcessExceptionParams> params (
			reinterpret_cast <_threadProcessExceptionParams*> (lp));
		params->pthis->ProcessException (params->pEP, params->thread_id);
		return 0;
	}

protected:
	virtual void onCrashDumpCreated () = NULL;

protected:
	inline void LogException (PEXCEPTION_POINTERS pEP)
	{
		if (m_LogFile)
		{
			vmsExceptionLog::LogException (m_LogFile, pEP);
			m_LogFile.flush ();
		}
	}

	std::ofstream m_LogFile;
	vmsCriticalSection m_csExceptionHandler;
};
