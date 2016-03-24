/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             

#include <windows.h>
#include <tchar.h>
#ifndef tstring

#include <string>

#if defined (UNICODE) || defined (_UNICODE)
#define tstring std::wstring
#else
#define tstring std::string
#endif

#endif 

