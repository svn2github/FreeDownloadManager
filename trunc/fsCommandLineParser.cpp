/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "FdmApp.h"
#include "fsCommandLineParser.h"
#include "vmsLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsCommandLineParser::fsCommandLineParser()
{

}

fsCommandLineParser::~fsCommandLineParser()
{

}

BOOL fsCommandLineParser::Parse()
{
	m_vPars.clear ();

	LPCTSTR pszCmdLine = GetCommandLine ();

	if (*pszCmdLine == _T('"'))
	{
		pszCmdLine = _tcschr (pszCmdLine+1, _T('"'));
		if (pszCmdLine)
			pszCmdLine++;
		else
			return FALSE;
	}
	else
	{
		while (*pszCmdLine && *pszCmdLine != _T(' '))
			pszCmdLine++;
	}

	try
	{
		while (*pszCmdLine)
		{
			TCHAR szParam [10000], szValue [10000];
			*szParam = *szValue = 0;
			bool bHasValue = true;

			
			while (*pszCmdLine && (*pszCmdLine == ' ' || *pszCmdLine == '\r' || *pszCmdLine == '\n'))
				pszCmdLine++;

			if (*pszCmdLine == '/' || *pszCmdLine == '-')
			{
				int i = 0;
				while (*++pszCmdLine && *pszCmdLine != ' ' && *pszCmdLine != '=')
					szParam [i++] = *pszCmdLine;

				szParam [i] = 0;

				while (*pszCmdLine == _T(' '))
					pszCmdLine++;

				
				if (*pszCmdLine == '=')
				{
					pszCmdLine++;
					while (*pszCmdLine == ' ')
						pszCmdLine++;
				}
				else
					bHasValue = false;
			}

			if (bHasValue)
			{
				char cSp = ' ';	
				char cSp1 = '\n', cSp2 = '\r';
				
				if (*pszCmdLine == '"' || *pszCmdLine == '\'')
				{
					cSp = *pszCmdLine++;
					cSp1 = cSp2 = 0;
				}

				
				if (*pszCmdLine != '/' && *pszCmdLine != '-')
				{
					int i = 0;
					while (*pszCmdLine && *pszCmdLine != cSp && *pszCmdLine != cSp1 && *pszCmdLine != cSp2)
						szValue [i++] = *pszCmdLine++;

					szValue [i] = 0;

					while (*pszCmdLine && (*pszCmdLine == cSp || *pszCmdLine == cSp1 || *pszCmdLine == cSp2))
						pszCmdLine++;
				}
			}

			if (*szParam || *szValue)
			{
				fsCmdLineParameter par;
				par.strParam = szParam;
				par.strValue = szValue;
				m_vPars.push_back (par);
			}
		}
	}
	catch (const std::exception& ex)
	{
		ASSERT (FALSE);
		vmsLogger::WriteLog("fsCommandLineParser::Parse " + std::string(ex.what()));
	}
	catch (...)
	{
		ASSERT (FALSE);
		vmsLogger::WriteLog("fsCommandLineParser::Parse unknown exception");
	}

	return TRUE;
}

int fsCommandLineParser::Get_ParameterCount()
{
	return m_vPars.size ();
}

LPCTSTR fsCommandLineParser::Get_Parameter(int iIndex)
{
	return m_vPars [iIndex].strParam;
}

LPCTSTR fsCommandLineParser::Get_ParameterValue(int iIndex)
{
	return m_vPars [iIndex].strValue;
}
