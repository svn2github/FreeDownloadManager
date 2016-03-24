/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFatalExceptionProviderImpl.h"
class vmsVectoredFatalExceptionProvider :
	public vmsFatalExceptionProviderImpl <vmsVectoredFatalExceptionProvider>
{
public:
	~vmsVectoredFatalExceptionProvider ()
	{
		if (m_handler)
			unregister_provider ();
	}

protected:
	PVOID m_handler = nullptr;

protected:
	virtual void register_provider () override
	{
		assert (!m_handler);
		m_handler = AddVectoredExceptionHandler (TRUE, 
			MyVectoredExceptionHandler);
	}

	virtual void unregister_provider () override
	{
		assert (m_handler);
		RemoveVectoredExceptionHandler (m_handler);
		m_handler = nullptr;
	}

	static LONG WINAPI MyVectoredExceptionHandler (PEXCEPTION_POINTERS pEP)
	{
		if (fatal_exception (pEP))
		{
			assert (ms_this->m_callback);
			return ms_this->m_callback (pEP);
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	static bool fatal_exception (PEXCEPTION_POINTERS pEP)
	{
		switch (pEP->ExceptionRecord->ExceptionCode)
		{
		case STATUS_HEAP_CORRUPTION:
		case EXCEPTION_ACCESS_VIOLATION:
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		case EXCEPTION_DATATYPE_MISALIGNMENT:
		case EXCEPTION_FLT_DENORMAL_OPERAND:
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		case EXCEPTION_FLT_INEXACT_RESULT:
		case EXCEPTION_FLT_INVALID_OPERATION:
		case EXCEPTION_ILLEGAL_INSTRUCTION:
		case EXCEPTION_IN_PAGE_ERROR:
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		case EXCEPTION_STACK_OVERFLOW:
			return true;

		default:
			return false;
		}
	}	
};