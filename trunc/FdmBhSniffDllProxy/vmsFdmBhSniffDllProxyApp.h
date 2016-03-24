/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsDdeWindowManager.h"
class vmsFdmBhSniffDllProxyApp
{
public:
	vmsFdmBhSniffDllProxyApp ();
	~vmsFdmBhSniffDllProxyApp ();
	
public:
	int Run ();

protected:
	std::unique_ptr <vmsDdeWindowManager> m_ddeWindowManager;
	std::shared_ptr <vmsFlvSniffTaskProcessor> m_taskProcessor;
};

