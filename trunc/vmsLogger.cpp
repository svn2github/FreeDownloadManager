/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsLogger.h"
#include <fstream>
#include "mfchelp.h"

void vmsLogger::WriteLog(const tstring& message)
{
	tstring logFile = fsGetDataFilePath(_T("fdmlog.txt"));

	tstringstream tss;
	tss << GetTimeStamp() << _T(" ") << message << std::endl;

	std::fstream file(logFile, std::ios_base::app);
	file << stringFromTstring (tss.str ());
	
}

tstring vmsLogger::GetTimeStamp()
{
	time_t t = time(0);   
    struct tm * now = localtime( & t );

	tstringstream bufa;
    bufa << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday;

	return bufa.str();
}

#ifdef UNICODE
void vmsLogger::WriteLog(const std::string& message)
{
	WriteLog (tstringFromString (message));
}
#endif