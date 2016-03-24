/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fsCommandLineParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

fsCommandLineParser::fsCommandLineParser()
{
	Parse ();
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

	try{

	while (*pszCmdLine)
	{
		TCHAR szParam [10000], szValue [10000];
		*szParam = *szValue = 0;
		bool bHasValue = true;

		
		while (*pszCmdLine && (*pszCmdLine == _T(' ') || *pszCmdLine == _T('\r') || *pszCmdLine == _T('\n')))
			pszCmdLine++;

		if (*pszCmdLine == _T('/') || *pszCmdLine == _T('-'))
		{
			int i = 0;
			while (*++pszCmdLine && *pszCmdLine != _T(' ') && *pszCmdLine != _T('='))
				szParam [i++] = *pszCmdLine;

			szParam [i] = 0;

			while (*pszCmdLine == ' ')
				pszCmdLine++;

			
			if (*pszCmdLine == _T('='))
			{
				pszCmdLine++;
				while (*pszCmdLine == _T(' '))
					pszCmdLine++;
			}
			else
				bHasValue = false;
		}

		if (bHasValue)
		{
			TCHAR cSp = _T(' ');	
			TCHAR cSp1 = _T('\n'), cSp2 = _T('\r');
			
			if (*pszCmdLine == '"' || *pszCmdLine == _T('\''))
			{
				cSp = *pszCmdLine++;
				cSp1 = cSp2 = 0;
			}

			
			if (*pszCmdLine != _T('/') && *pszCmdLine != _T('-'))
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
	catch (...) {}

	return TRUE;
}

int fsCommandLineParser::Get_ParameterCount()
{
	return m_vPars.size ();
}

LPCTSTR fsCommandLineParser::Get_Parameter(int iIndex)
{
	return m_vPars [iIndex].strParam.c_str ();
}

LPCTSTR fsCommandLineParser::Get_ParameterValue(int iIndex)
{
	return m_vPars [iIndex].strValue.c_str ();
}
