/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#ifndef tstring

#include <string>

#if defined (UNICODE) || defined (_UNICODE)
#define tstring std::wstring
#else
#define tstring std::string
#endif

#endif 

#ifndef tstringstream

#include <sstream>

#if defined (UNICODE) || defined (_UNICODE)
#define tstringstream std::wstringstream
#else
#define tstringstream std::stringstream
#endif

#endif 

#ifdef WIN32
using xstring = std::wstring;
using xstringstream = std::wstringstream;
#else
using xstring = std::string; 
using xstringstream = std::stringstream;
#endif

