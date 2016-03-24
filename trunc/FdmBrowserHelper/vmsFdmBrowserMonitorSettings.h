/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmVideoMonitoringSettings.h"
class vmsFdmBrowserMonitorSettings :
	public vmsSerializable
{
public:
	vmsFdmBrowserMonitorSettings ()
	{
	}

	~vmsFdmBrowserMonitorSettings ()
	{
	}

public:
	bool m_enable = true;
	bool m_need_alt = false;
	
	bool m_allowDownload = true;
	std::wstring m_skipExtensions;
	std::wstring m_skipServers;
	vmsFdmVideoMonitoringSettings m_video;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		auto node = pStm->SelectOrCreateNode (L"video");
		assert (node);
		if (!node)
			return false;
		return m_video.Serialize (node.get (), flags) &&
			pStm->SerializeValueS (L"enable", m_enable) &&
			pStm->SerializeValueS (L"needAlt", m_need_alt) &&
			pStm->SerializeValueS (L"allowDownload", m_allowDownload);
	}
};

