/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsLockable.h"
#include "win/vmsWinCriticalSection.h"

class vmsThreadSafe2 : public vmsLockable
{
public:
	vmsThreadSafe2 () {}
protected:
	vmsWinCriticalSection m_objectLock;
public:
	vmsLockable_ImplementDelegatedTo (m_objectLock);
private:
	vmsThreadSafe2 (const vmsThreadSafe2&);
	vmsThreadSafe2& operator = (const vmsThreadSafe2&);
};

#define vmsThreadSafe2Scope auto spLock = LockAuto ()
#define vmsThreadSafe2ScopeUnlock spLock = nullptr