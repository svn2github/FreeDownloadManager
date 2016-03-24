/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcBrowserProxyTaskProcessor.h"

vmsFdmBhIpcBrowserProxyTaskProcessor::vmsFdmBhIpcBrowserProxyTaskProcessor ()
{
}

vmsFdmBhIpcBrowserProxyTaskProcessor::~vmsFdmBhIpcBrowserProxyTaskProcessor ()
{
}

void vmsFdmBhIpcBrowserProxyTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	std::wstring error = L"failure";

	CRegKey keyFDM;
	if (ERROR_SUCCESS == keyFDM.Open (HKEY_CURRENT_USER,
		_T ("Software\\FreeDownloadManager.ORG\\Free Download Manager"), KEY_WRITE))
	{
		CRegKey keyFDMFfStgs;
		if (ERROR_SUCCESS == keyFDMFfStgs.Open (keyFDM, _T ("FirefoxSettings\\Proxy"), KEY_WRITE) ||
			ERROR_SUCCESS == keyFDMFfStgs.Create (keyFDM, _T ("FirefoxSettings\\Proxy")))
		{
			error.clear ();

			const auto &proxy = task->m_browserProxy;

			verify (ERROR_SUCCESS == 
				keyFDMFfStgs.SetDWORDValue (_T("ProxyType"), proxy.m_type));

			verify (ERROR_SUCCESS == keyFDMFfStgs.SetStringValue (
				(proxy.m_protocol + L"_addr").c_str (), proxy.m_addr.c_str ()));

			verify (ERROR_SUCCESS == keyFDMFfStgs.SetDWORDValue (
				(proxy.m_protocol + L"_port").c_str (), proxy.m_port));
		}
	}

	callback (task, construct_response (task, error));
}