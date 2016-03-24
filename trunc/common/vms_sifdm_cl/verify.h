/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include <assert.h>

#ifndef _DEBUG
#define verify(x) (x)
#else
#define verify(x) assert(x)
#endif