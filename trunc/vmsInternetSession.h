/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#if !defined(AFX_VMSINTERNETSESSION_H__D02A0038_60AB_46ED_8961_C277CA35463C__INCLUDED_)
#define AFX_VMSINTERNETSESSION_H__D02A0038_60AB_46ED_8961_C277CA35463C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "downloadproperties.h"

class vmsInternetSession : public fsInternetSession
{
public:
	fsInternetResult Create (LPCTSTR pszAgent, fsInternetAccessTypeEx enAT, LPCTSTR pszProxyName, fsNetworkProtocol enProtocol);

	vmsInternetSession();
	virtual ~vmsInternetSession();

};

#endif 
