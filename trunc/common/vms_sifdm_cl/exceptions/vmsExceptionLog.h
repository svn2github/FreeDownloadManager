/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsExceptionLog
{
public:
	static void LogException (std::ostream &os, PEXCEPTION_POINTERS pEP)
	{
		LogExceptionRecord (os, pEP->ExceptionRecord);
	}

	static void LogExceptionRecord (std::ostream &os, PEXCEPTION_RECORD pER)
	{
		std::ios::fmtflags fmtf = os.flags ();

		os << std::hex << "<EXCEPTION Address=\"" << pER->ExceptionAddress << 
			"\" Code=\"" << pER->ExceptionCode << 
			"\" Flags=\"" << pER->ExceptionFlags;

		if (!pER->ExceptionRecord)
		{
			os << "\" />" << std::endl;
		}
		else 
		{
			os << "\" >" << std::endl;
			LogExceptionRecord (os, pER->ExceptionRecord);
			os << "</EXCEPTION>" << std::endl;
		}

		os.flags (fmtf);
	}
};