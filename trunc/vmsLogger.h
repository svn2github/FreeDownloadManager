/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "StdAfx.h"
#include "tstring.h"
#include <fstream>

class vmsLogger
{
	static tstring GetTimeStamp();
public:
	static void WriteLog(const tstring& message);
#ifdef UNICODE
	static void WriteLog(const std::string& message);
#endif
};

