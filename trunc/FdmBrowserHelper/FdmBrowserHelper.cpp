/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBrowserHelperApp.h"
#include "../common/component_crash_rep.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	
#endif

	initializeCrashReporter ();

	vmsFdmBrowserHelperApp app;
	return app.run ();
}

