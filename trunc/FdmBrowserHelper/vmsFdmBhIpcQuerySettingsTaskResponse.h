/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmSettingsManager.h"
class vmsFdmBhIpcQuerySettingsTaskResponse :
	public vmsFdmBhIpcTaskResponse
{
public:
	vmsFdmBhIpcQuerySettingsTaskResponse (
		std::shared_ptr <vmsFdmSettingsManager> settingsManager) :
		m_settingsManager (settingsManager)
	{
	}

	~vmsFdmBhIpcQuerySettingsTaskResponse ()
	{
	}

protected:
	std::shared_ptr <vmsFdmSettingsManager> m_settingsManager;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		assert (pStm->isOutputStream ());
		if (!pStm->isOutputStream ())
			return false;
		
		{
			auto lock = m_settingsManager->LockAuto ();
			auto&& settings = m_settingsManager->settings (lock);
			auto node = pStm->SelectOrCreateNode (L"settings");
			if (!settings.Serialize (node.get (), flags))
				return false;
		}

		return vmsFdmBhIpcTaskResponse::Serialize (pStm, flags);
	}
};

