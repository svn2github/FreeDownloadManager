/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../vmsProcessList.h"

inline DWORD vmsGetParentProcessId ()
{
	vmsProcessList pl;
	pl.GenerateList ();
	auto index = pl.findProcessIndex (GetCurrentProcessId ());
	assert (index != -1);
	return index != -1 ? pl.getProcess (index)->dwParentProcessId : 0;
}
