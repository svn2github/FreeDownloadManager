/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsSimpleIpcItemSender.h"
#include "vmsSimpleIpcItemReceiver.h"
#include "vmsFdmSettingsManager.h"
#include "vmsParentBrowserProcessManager.h"
class vmsFdmBhIpcHandshakeTaskProcessor :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcHandshakeTaskProcessor (
		vmsSimpleIpcItemSender *ipc_sender,
		vmsSimpleIpcItemReceiver *ipc_rcvr,
		std::weak_ptr <vmsFdmSettingsManager> settingsManager,
		std::weak_ptr <vmsParentBrowserProcessManager> parentBrowserProcessManager);
	~vmsFdmBhIpcHandshakeTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	vmsSimpleIpcItemSender *m_ipc_sender;
	vmsSimpleIpcItemReceiver *m_ipc_rcvr;
	std::weak_ptr <vmsFdmSettingsManager> m_settingsManager;
	std::weak_ptr <vmsParentBrowserProcessManager> m_parentBrowserProcessManager;
};

