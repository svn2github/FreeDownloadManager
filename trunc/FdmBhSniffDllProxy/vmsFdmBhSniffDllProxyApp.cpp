/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhSniffDllProxyApp.h"

vmsFdmBhSniffDllProxyApp::vmsFdmBhSniffDllProxyApp ()
{
}

vmsFdmBhSniffDllProxyApp::~vmsFdmBhSniffDllProxyApp ()
{
}

int vmsFdmBhSniffDllProxyApp::Run ()
{
	vmsComInitialize comInit;

	m_taskProcessor = std::make_shared <vmsFlvSniffTaskProcessor> ();
	
	m_ddeWindowManager = std::make_unique <vmsDdeWindowManager> (
		m_taskProcessor);

	MSG msg;
	while (GetMessage (&msg, nullptr, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return 0;
}
