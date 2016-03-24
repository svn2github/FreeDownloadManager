/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsFdmSettingsManager.h"
class vmsFdmBhIpcQuerySettingsTaskProcessor :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcQuerySettingsTaskProcessor (
		std::weak_ptr <vmsFdmSettingsManager> settingsManager);
	~vmsFdmBhIpcQuerySettingsTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::weak_ptr <vmsFdmSettingsManager> m_settingsManager;
};

