/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsIsAppRunningAlready.h"
class vmsIsWinAppRunningAlready : public vmsIsAppRunningAlreadyBase
{
public:
	vmsIsWinAppRunningAlready (const std::wstring& wstrAppName) :
	  vmsIsAppRunningAlreadyBase (wstrAppName)
	{
		assert (!m_wstrAppName.empty ());
	}

	virtual bool isAnotherInstanceRunning ()
	{
		assert (!m_spAppMutex); 
		HANDLE h = OpenMutex (SYNCHRONIZE, FALSE, m_wstrAppName.c_str ());
		if (h)
			CloseHandle (h);
		return h != NULL;
	}

	virtual void inidicateMeRunning ()
	{
		m_spAppMutex = createAppMutex ();
	}

	virtual void inidicateMeNotRunning ()
	{
		m_spAppMutex = NULL;
	}

	virtual bool incicateMeRunningIfSingle () override
	{
		assert (!m_spAppMutex);
		auto mutex = createAppMutex ();
		assert (mutex && mutex->getHandle ());
		if (mutex && mutex->getHandle () && GetLastError () == ERROR_ALREADY_EXISTS)
			return false;
		m_spAppMutex = mutex;
		return true;
	}

	virtual bool isInstanceRegistered () override
	{
		return m_spAppMutex != nullptr;
	}

protected:
	vmsWinHandle::tSP m_spAppMutex;

	vmsWinHandle::tSP createAppMutex ()
	{
		auto handle = CreateMutex (NULL, FALSE, m_wstrAppName.c_str ());
		auto last_err = GetLastError ();
		auto result = std::make_shared <vmsWinHandle> (handle);
		SetLastError (last_err);
		return result;
	}
};