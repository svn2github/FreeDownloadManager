/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsIsAppRunningAlreadyBase
{
public:
	vmsIsAppRunningAlreadyBase (const std::wstring& wstrAppName) :
	  m_wstrAppName (wstrAppName)
	{
	}

	virtual bool isAnotherInstanceRunning () = NULL;
	
	virtual void inidicateMeRunning () = NULL;
	
	virtual void inidicateMeNotRunning () = NULL;
	
	
	
	virtual bool incicateMeRunningIfSingle () = 0;
	
	virtual bool isInstanceRegistered () = 0;

protected:
	std::wstring m_wstrAppName;
};

#ifdef _WIN32
#include "win/vmsIsWinAppRunningAlready.h"
typedef vmsIsWinAppRunningAlready vmsIsAppRunningAlready;
#endif