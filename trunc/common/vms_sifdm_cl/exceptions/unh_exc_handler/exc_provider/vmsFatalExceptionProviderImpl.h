/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFatalExceptionProvider.h"
template <class TDerived>
class vmsFatalExceptionProviderImpl :
	public vmsFatalExceptionProvider
{
public:
	vmsFatalExceptionProviderImpl ()
	{
		make_sure (!ms_this);
		ms_this = static_cast <TDerived*> (this);
	}

	~vmsFatalExceptionProviderImpl ()
	{
		ms_this = nullptr;
	}

	virtual void set_callback (callback_fn_t fn) override
	{
		if (!fn)
			unregister_provider ();
		m_callback = fn;
		if (m_callback)
			register_provider ();
	}

protected:
	static TDerived *ms_this;
	callback_fn_t m_callback;

protected:
	virtual void register_provider () = 0;
	virtual void unregister_provider () = 0;
};

template <class T> 
__declspec(selectany) T* vmsFatalExceptionProviderImpl<T>::ms_this = nullptr;