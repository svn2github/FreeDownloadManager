/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <csignal>
#include "vmsFatalExceptionProviderImpl.h"
#include "../../getexcptrs.h"
class vmsCrtFatalExceptionProvider :
	public vmsFatalExceptionProviderImpl <vmsCrtFatalExceptionProvider>
{
public:
	~vmsCrtFatalExceptionProvider ()
	{
		if (m_registered)
			unregister_provider ();
	}

protected:
	_invalid_parameter_handler m_prevInvParamHandler = nullptr;
	typedef void (__cdecl *FNSignalHandler)(int);
	FNSignalHandler m_prevAbortHandler = nullptr;
	bool m_registered = false;

protected:
	virtual void register_provider () override
	{
		assert (!m_registered);
		m_prevInvParamHandler = _set_invalid_parameter_handler (
			invalid_parameter_handler);
		m_prevAbortHandler = signal (SIGABRT, signal_handler);
		m_registered = true;
	}

	virtual void unregister_provider () override
	{
		assert (m_registered);
		_set_invalid_parameter_handler (m_prevInvParamHandler);
		signal (SIGABRT, m_prevAbortHandler);
		m_registered = false;
	}

protected:
	static void invalid_parameter_handler (const wchar_t* expression, 
		const wchar_t* function, const wchar_t* file, unsigned int line, 
		uintptr_t pReserved)
	{
		on_crt_fatal_error ();
	}

	static void signal_handler (int)
	{
		on_crt_fatal_error ();
	}

	static void on_crt_fatal_error ()
	{
		PEXCEPTION_POINTERS pEP = NULL;
		GetExceptionPointers (0, &pEP);
		assert (pEP != NULL);
		if (pEP)
		{
			assert (ms_this->m_callback);
			ms_this->m_callback (pEP);
		}
		else
		{
			make_sure (0); 
		}
	}
};