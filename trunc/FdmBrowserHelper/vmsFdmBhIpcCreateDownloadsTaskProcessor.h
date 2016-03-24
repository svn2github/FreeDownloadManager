/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskProcessor.h"
#include "vmsFdmSettingsManager.h"
class vmsFdmBhIpcCreateDownloadsTaskProcessor :
	public vmsFdmBhIpcTaskProcessor
{
public:
	vmsFdmBhIpcCreateDownloadsTaskProcessor (
		std::weak_ptr <vmsFdmSettingsManager> settingsManager);
	~vmsFdmBhIpcCreateDownloadsTaskProcessor ();

	virtual void process (std::shared_ptr <vmsFdmBhIpcTask> task,
		process_callback_fn_t callback) override;

protected:
	std::weak_ptr <vmsFdmSettingsManager> m_settingsManager;
	
	
	vmsWinOsVersion m_osVersion;
};

