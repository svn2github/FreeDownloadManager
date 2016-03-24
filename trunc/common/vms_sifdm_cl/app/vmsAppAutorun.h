/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsAppAutorunBase
{
public:
	virtual bool autorun_enabled () const = 0;
	virtual bool enable_autorun (bool enable) = 0;
	virtual ~vmsAppAutorunBase () {}
};

#ifdef _WIN32
#include "win/vmsWinAppAutorun.h"
typedef vmsWinAppAutorun vmsAppAutorun;
#endif