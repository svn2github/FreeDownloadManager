/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsLockable2.h"
class vmsThreadSafe4 :
	public vmsLockable2 <std::mutex>
{
public:
	vmsThreadSafe4 () {}

protected:
	mutable std::mutex m_objectLock;

public:
	virtual auto_lock_t LockAuto () const override
	{
		return auto_lock_t (m_objectLock);
	}

private:
	vmsThreadSafe4 (const vmsThreadSafe4&) = delete;
	vmsThreadSafe4& operator = (const vmsThreadSafe4&) = delete;
};

#define vmsThreadSafe4Scope auto autoLock = vmsThreadSafe4::LockAuto ()
#define vmsThreadSafe4ScopeUnlock autoLock.unlock ()