/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __COMMON_H_
#define __COMMON_H_

#include "assert.h"

#define CC(cmd) {HRESULT hr = cmd; assert (SUCCEEDED (hr)); if (FAILED (hr)) return NS_ERROR_FAILURE;}

#if defined (XUL_SDK_VER) && XUL_SDK_VER >= 22

#include "ivmsfdmff22.h"
#define XULSDK_PRBool	bool

#else

#include "ivmsfdmff.h"
#define XULSDK_PRBool	PRBool

#endif 

#endif 