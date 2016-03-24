/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcHandshakeTaskProcessor.h"

vmsFdmBhIpcHandshakeTaskProcessor::vmsFdmBhIpcHandshakeTaskProcessor (
	vmsSimpleIpcItemSender *ipc_sender,
	vmsSimpleIpcItemReceiver *ipc_rcvr,
	std::weak_ptr <vmsFdmSettingsManager> settingsManager,
	std::weak_ptr <vmsParentBrowserProcessManager> parentBrowserProcessManager) :
	m_ipc_sender (ipc_sender),
	m_ipc_rcvr (ipc_rcvr),
	m_settingsManager (settingsManager),
	m_parentBrowserProcessManager (parentBrowserProcessManager)
{
}

vmsFdmBhIpcHandshakeTaskProcessor::~vmsFdmBhIpcHandshakeTaskProcessor ()
{
}

void vmsFdmBhIpcHandshakeTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	m_ipc_sender->set_flags (vmsSimpleIpcItemSender::send_crlf,
		task->m_handshake.m_response_add_crlf);
	m_ipc_sender->set_flags (vmsSimpleIpcItemSender::send_data_len_as_string,
		task->m_handshake.m_response_send_data_len_as_string);
	
	m_ipc_sender->set_flags (vmsSimpleIpcItemSender::send_uri_encoded,
		task->m_handshake.m_uri_encoding_for_send);
	m_ipc_rcvr->set_flags (vmsSimpleIpcItemReceiver::rcv_uri_encoded,
		task->m_handshake.m_uri_encoding_for_rcv);

	m_ipc_sender->set_flags (vmsSimpleIpcItemSender::send_base64_encoded,
		task->m_handshake.m_base64_encoding_for_send);
	m_ipc_rcvr->set_flags (vmsSimpleIpcItemReceiver::rcv_base64_encoded,
		task->m_handshake.m_base64_encoding_for_rcv);

	auto settingsManager = m_settingsManager.lock ();
	settingsManager->set_browser (task->m_handshake.m_browser);
	settingsManager->ReadSettings ();

	{
		auto lock = settingsManager->LockAuto ();
		auto &&settings = settingsManager->settings (lock);
		if (settings.m_browser.m_monitor.m_video.m_enableSnifferDll)
		{
			auto pbpm = m_parentBrowserProcessManager.lock ();
			assert (pbpm);
			if (pbpm)
				pbpm->injectSniffDll ();
		}
	}

	callback (task, construct_response (task));
}