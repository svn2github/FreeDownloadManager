/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcNetworkRequestNotificationTaskProcessor.h"

vmsFdmBhIpcNetworkRequestNotificationTaskProcessor::vmsFdmBhIpcNetworkRequestNotificationTaskProcessor (
	std::shared_ptr <vmsFdmBhSniffDllProxyManager> sniffDllProxyManager,
	std::shared_ptr <vmsBrowserActivityMonitor> browserActivityMonitor) :
	m_sniffDllProxyManager (sniffDllProxyManager),
	m_browserActivityMonitor (browserActivityMonitor)
{
}

vmsFdmBhIpcNetworkRequestNotificationTaskProcessor::~vmsFdmBhIpcNetworkRequestNotificationTaskProcessor ()
{
	shutdown ();
}

void vmsFdmBhIpcNetworkRequestNotificationTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	if (task->m_netReqNotif.m_type == L"activity")
	{
		m_browserActivityMonitor->onActivity (
			task->m_netReqNotif.m_url.c_str ());
		callback (task, construct_response (task));
	}
	else
	{
		create_thread (&vmsFdmBhIpcNetworkRequestNotificationTaskProcessor::thread_process,
			this, task, callback);
	}
}

void vmsFdmBhIpcNetworkRequestNotificationTaskProcessor::thread_process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	vmsComInitialize comInit;

	auto resp = construct_response (task);
	bool perfResult = false;

	if (task->m_netReqNotif.m_type == L"new")
	{
		vmsFlvSniffTask flvtask;
		flvtask.m_id = task->m_id;
		flvtask.m_callProc.m_name = L"NewRequestNotification";
		flvtask.m_callProc.m_url = task->m_netReqNotif.m_url;
		flvtask.m_callProc.m_srcTabUrl = task->m_netReqNotif.m_srcTabUrl;

		perfResult = m_sniffDllProxyManager->perform_task (flvtask,
			[task, resp, callback] (vmsFlvSniffTaskResponse &flvresp)
		{
			assert (resp->m_id == flvresp.m_id);
			callback (task, resp);
		});
	}
	else if (task->m_netReqNotif.m_type == L"response")
	{
		vmsFlvSniffTask flvtask;
		flvtask.m_id = task->m_id;
		flvtask.m_callProc.m_name = L"RequestResponseNotification";
		flvtask.m_callProc.m_url = task->m_netReqNotif.m_url;
		flvtask.m_callProc.m_requestHeaders = task->m_netReqNotif.m_requestHeaders;
		flvtask.m_callProc.m_responseHeaders = task->m_netReqNotif.m_responseHeaders;
		flvtask.m_callProc.m_requestId = task->m_netReqNotif.m_requestId;

		perfResult = m_sniffDllProxyManager->perform_task (flvtask,
			[task, resp, callback] (vmsFlvSniffTaskResponse &flvresp)
		{
			assert (resp->m_id == flvresp.m_id);
			callback (task, resp);
		});
	}
	else if (task->m_netReqNotif.m_type == L"response_complete")
	{
		vmsFlvSniffTask flvtask;
		flvtask.m_id = task->m_id;
		flvtask.m_callProc.m_name = L"RequestResponseCompleteNotification";
		flvtask.m_callProc.m_requestId = task->m_netReqNotif.m_requestId;

		perfResult = m_sniffDllProxyManager->perform_task (flvtask,
			[task, resp, callback] (vmsFlvSniffTaskResponse &flvresp)
		{
			assert (resp->m_id == flvresp.m_id);
			callback (task, resp);
		});
	}

	assert (perfResult);

	if (!perfResult)
	{
		resp->m_error = L"failed";
		callback (task, resp);
	}

	on_thread_terminating ();
}