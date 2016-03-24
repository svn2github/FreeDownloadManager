/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFatalExceptionProviderImpl.h"
class vmsUnhandledExceptionProvider :
	public vmsFatalExceptionProviderImpl <vmsUnhandledExceptionProvider>
{
public:
	~vmsUnhandledExceptionProvider ()
	{
		if (m_registered)
			unregister_provider ();
	}

protected:
	LPTOP_LEVEL_EXCEPTION_FILTER m_prevFilter = nullptr;
	bool m_registered = false;

protected:
	virtual void register_provider () override
	{
		assert (!m_registered);
		m_prevFilter = SetUnhandledExceptionFilter (
			MyUnhandledExceptionFilter);
		m_registered = true;
	}

	virtual void unregister_provider () override
	{
		assert (m_registered);
		SetUnhandledExceptionFilter (m_prevFilter);
		m_prevFilter = nullptr;
		m_registered = false;
	}

	static LONG WINAPI MyUnhandledExceptionFilter (PEXCEPTION_POINTERS pEP)
	{
		assert (ms_this->m_callback);
		return ms_this->m_callback (pEP);
	}
};
