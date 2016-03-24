/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsWinCriticalSection.h"

class vmsTls 
{
public:
	vmsTls()
	{
		setIndex(TlsAlloc());
		if (isTlsAllocatedSuccessfully())
			setValue(NULL);
	}
	virtual ~vmsTls()
	{
		TlsFree(index());
	}

	void setValue(LPVOID lp) 
	{
		TlsSetValue(index(), lp);
	}

	LPVOID getValue() 
	{
		return TlsGetValue(index());
	}
	
	bool isTlsAllocatedSuccessfully() const 
	{
		return index() != TLS_OUT_OF_INDEXES;
	}

private:
	void setIndex(DWORD index)
	{
		m_cs.Lock();
		m_dwIndex = index;
		m_cs.Unlock();
	}

	DWORD index() const
	{
		DWORD ret;

		m_cs.Lock();
		ret = m_dwIndex;
		m_cs.Unlock();

		return ret;
	}

private:
	DWORD m_dwIndex;
	vmsWinCriticalSection m_cs;
};
