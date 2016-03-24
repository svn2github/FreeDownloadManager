/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsFirefoxExtensionUpdateMgr.h"

extern std::tr1::shared_ptr <vmsFirefoxExtensionUpdateMgr> _spFfExtUpdateMgr;

class vmsAppGlobalObjects
{
public:

	vmsAppGlobalObjects(void)
	{
	}

	virtual ~vmsAppGlobalObjects(void)
	{
	}

	
	static void Create2 (bool currentBuildFirstRun);
};

