/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#if !defined(AFX_FSFDMCMDLINEPARSER_H__3DD882CC_AADC_457D_91DE_228723758761__INCLUDED_)
#define AFX_FSFDMCMDLINEPARSER_H__3DD882CC_AADC_457D_91DE_228723758761__INCLUDED_

#include "fsCommandLineParser.h"	
#if _MSC_VER > 1000
#pragma once
#endif 

class fsFDMCmdLineParser  
{
public:
	bool isNeedExit () const {return m_bNeedExit;}
	BOOL is_ForceSilentSpecified();
	
	void Parse();
	fsFDMCmdLineParser();
	virtual ~fsFDMCmdLineParser();

protected:
	void AddTorrentFile (LPCSTR pszFile);
	BOOL m_bAnotherFDMStarted;	
	fsCommandLineParser m_parser;	
	BOOL m_bForceSilent;		
	bool m_bNeedExit;
};

#endif 
