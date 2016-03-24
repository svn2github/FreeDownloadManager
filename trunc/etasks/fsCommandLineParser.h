/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSCOMMANDLINEPARSER_H__83B66E37_1776_4D30_A255_1BC65A140AFD__INCLUDED_)
#define AFX_FSCOMMANDLINEPARSER_H__83B66E37_1776_4D30_A255_1BC65A140AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class fsCommandLineParser  
{
public:
	
	BOOL Parse();
	
	int Get_ParameterCount();
	
	LPCTSTR Get_Parameter(int iIndex);
	
	LPCTSTR Get_ParameterValue (int iIndex);
	
	fsCommandLineParser();
	virtual ~fsCommandLineParser();

protected:
	struct fsCmdLineParameter
	{
		tstring strParam;	
		tstring strValue;	
	};
	
	vector <fsCmdLineParameter> m_vPars;
};

#endif 
