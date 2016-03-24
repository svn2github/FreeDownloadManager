/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
template <typename _Mutex, class _AutoLock = std::unique_lock <_Mutex>>
class vmsLockable2
{
public:
	using auto_lock_t = _AutoLock;

public:
	virtual auto_lock_t LockAuto () const = 0;
};