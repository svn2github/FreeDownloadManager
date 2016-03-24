/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcVideoSnifferTaskProcessor.h"
#include "vmsFdmBhIpcVideoSnifferTaskResponse.h"
#include "../common/util.h"
#include "utils.h"

vmsFdmBhIpcVideoSnifferTaskProcessor::vmsFdmBhIpcVideoSnifferTaskProcessor (
	std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager) :
	m_sniffDllProxyManager (sniffDllProxyManager)
{
	
}

vmsFdmBhIpcVideoSnifferTaskProcessor::~vmsFdmBhIpcVideoSnifferTaskProcessor ()
{
	shutdown ();
}

void vmsFdmBhIpcVideoSnifferTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	create_thread (&vmsFdmBhIpcVideoSnifferTaskProcessor::thread_process,
		this, task, callback);
}

void vmsFdmBhIpcVideoSnifferTaskProcessor::thread_process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	vmsComInitialize comInit;

	if (task->m_videoSniffer.m_callProc.m_name == L"CreateVideoDownloadFromUrl")
	{
		if (process_CreateVideoDownloadFromUrl (task, callback))
			return;
	}

	vmsFlvSniffTask flvtask;
	flvtask.m_id = task->m_id;
	flvtask.m_callProc = task->m_videoSniffer.m_callProc;

	auto resp = construct_response <vmsFdmBhIpcVideoSnifferTaskResponse> (task);

	auto perfResult = m_sniffDllProxyManager->perform_task (flvtask, 
		[task, resp, callback] (vmsFlvSniffTaskResponse &flvresp)
	{
		assert (resp->m_id == flvresp.m_id);
		resp->m_videoSniffer = flvresp.m_callProc;
		callback (task, resp);
	});
	
	assert (perfResult);

	if (!perfResult)
	{
		resp->m_error = L"failed";
		callback (task, resp);
	}

	on_thread_terminating ();
}

bool vmsFdmBhIpcVideoSnifferTaskProcessor::process_CreateVideoDownloadFromUrl (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	if (vmsIsYouTubeVideoPage (task->m_videoSniffer.m_callProc.m_webPageUrl))
	{
		vmsFdmBhIpcDownloadInfo dlinfo;
		dlinfo.m_url = task->m_videoSniffer.m_callProc.m_webPageUrl;

		if (SUCCEEDED (vmsFdmBhCreateFlashVideoDownload (&dlinfo)))
		{
			auto resp = construct_response <vmsFdmBhIpcVideoSnifferTaskResponse> (task);
			callback (task, resp);
			return true;
		}
	}

	return false;
}