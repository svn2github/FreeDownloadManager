/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsLockable.h"

class vmsWinCriticalSection : public vmsLockable
{
public:
	vmsWinCriticalSection ()
	{
		InitializeCriticalSection (&m_cs);
	}

	virtual ~vmsWinCriticalSection ()
	{
		DeleteCriticalSection (&m_cs);
	}

	virtual void Lock () const 
	{
		EnterCriticalSection (&m_cs);
	}

	virtual void Unlock () const 
	{
		LeaveCriticalSection (&m_cs);
	}

protected:
	mutable CRITICAL_SECTION m_cs;
};