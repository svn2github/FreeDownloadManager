/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsLockable.h"

class vmsThreadSafe3 : 
	public vmsLockable
{
public:
	vmsThreadSafe3 () {}

protected:
	mutable std::mutex m_objectLock;

public:
	virtual void Lock () const override 
	{
		m_objectLock.lock ();
	}

	virtual void Unlock () const override
	{
		m_objectLock.unlock ();
	}

private:
	vmsThreadSafe3 (const vmsThreadSafe3&) = delete;
	vmsThreadSafe3& operator = (const vmsThreadSafe3&) = delete;
};

#define vmsThreadSafe3Scope auto spLock = LockAuto ()
#define vmsThreadSafe3ScopeUnlock spLock = nullptr