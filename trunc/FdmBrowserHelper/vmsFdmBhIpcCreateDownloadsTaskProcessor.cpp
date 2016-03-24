/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcCreateDownloadsTaskProcessor.h"
#include "utils.h"
#include "utils2.h"

vmsFdmBhIpcCreateDownloadsTaskProcessor::vmsFdmBhIpcCreateDownloadsTaskProcessor (
	std::weak_ptr <vmsFdmSettingsManager> settingsManager) :
	m_settingsManager (settingsManager)
{
}

vmsFdmBhIpcCreateDownloadsTaskProcessor::~vmsFdmBhIpcCreateDownloadsTaskProcessor ()
{
}

void vmsFdmBhIpcCreateDownloadsTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto &&downloads = task->m_createDownloads.m_downloads.container ();
	
	if (downloads.size () == 1)
	{
		const auto dlinfo = downloads.front ().get ();

		auto fn_process = [] (
			std::shared_ptr <vmsFdmSettingsManager> settingsManager,
			std::shared_ptr <vmsFdmBhIpcTask> task,
			process_callback_fn_t callback,
			const vmsFdmBhIpcDownloadInfo *dlinfo)
		{
			bool ignore_download = false;

			if (task->m_createDownloads.m_catchedDownloads &&
				!task->m_createDownloads.m_ignoreSettings)
			{
				assert (settingsManager);
				settingsManager->ReadSettings (); 
				auto lock = settingsManager->LockAuto ();
				ignore_download = vmsIsCatchedDownloadShoudBeIgnored (
					dlinfo, settingsManager->settings (lock));
			}

			auto resp = vmsFdmBhIpcTaskProcessor::construct_response (task);

			if (!ignore_download)
			{
				vmsComInitialize comInit;
				HRESULT hr;
				if (FAILED (hr = vmsFdmShowAddDownloadDialog (dlinfo,
					task->m_createDownloads.m_catchedDownloads,
					task->m_createDownloads.m_waitResponse)))
				{
					resp->m_error = L"failed";
				}
				else
				{
					if (task->m_createDownloads.m_waitResponse)
						resp->m_result = hr == S_OK ? L"1" : L"0";
				}
			}
			else
			{
				resp->m_result = L"0";
			}
			
			callback (task, resp);
		};

		if (m_osVersion.isVistaOrHigher ())
		{
			std::async (std::launch::async, fn_process,
				m_settingsManager.lock (), task, callback, dlinfo);
		}
		else
		{
			fn_process (m_settingsManager.lock (), task, callback, dlinfo);
		}
	}
	else
	{
		auto fn_process = [] (
			std::shared_ptr <vmsFdmBhIpcTask> task,
			process_callback_fn_t callback)
		{
			vmsComInitialize comInit;
			std::wstring error;
			auto &&downloads = task->m_createDownloads.m_downloads.container ();
			if (FAILED (vmsFdmShowAddUrlListDialog (downloads)))
				error = L"failed";
			callback (task,
				vmsFdmBhIpcTaskProcessor::construct_response (task, error));
		};

		if (m_osVersion.isVistaOrHigher ())
		{
			std::async (std::launch::async, fn_process, task, callback);
		}
		else
		{
			fn_process (task, callback);
		}
	}
}