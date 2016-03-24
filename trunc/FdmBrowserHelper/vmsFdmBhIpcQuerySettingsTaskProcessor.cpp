/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcQuerySettingsTaskProcessor.h"
#include "vmsFdmBhIpcQuerySettingsTaskResponse.h"

vmsFdmBhIpcQuerySettingsTaskProcessor::vmsFdmBhIpcQuerySettingsTaskProcessor (
	std::weak_ptr <vmsFdmSettingsManager> settingsManager) :
	m_settingsManager (settingsManager)
{
}

vmsFdmBhIpcQuerySettingsTaskProcessor::~vmsFdmBhIpcQuerySettingsTaskProcessor ()
{
}

void vmsFdmBhIpcQuerySettingsTaskProcessor::process (
	std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto settingsManager = m_settingsManager.lock ();
	assert (settingsManager);

	settingsManager->ReadSettings ();

	auto resp = std::make_shared <vmsFdmBhIpcQuerySettingsTaskResponse> (
		settingsManager);
	initialize_response (task, *resp);

	callback (task, resp);
}