/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#ifndef __COMMON_H_
#define __COMMON_H_

#include "assert.h"

#define CC(cmd) {HRESULT hr = cmd; assert (SUCCEEDED (hr)); if (FAILED (hr)) return NS_ERROR_FAILURE;}

#endif 