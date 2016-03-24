/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcDownloadInfo.h"
class vmsFdmBhIpcTaskCreateDownloads :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskCreateDownloads ()
	{
	}

	~vmsFdmBhIpcTaskCreateDownloads ()
	{
	}

public:
	vmsSerializableUniqueObjVector <vmsFdmBhIpcDownloadInfo> m_downloads;
	
	bool m_catchedDownloads = false;
	
	bool m_waitResponse = false;
	bool m_ignoreSettings = false;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags) override
	{
		return SerializeObject (pStm, &m_downloads, L"downloads") &&
			pStm->SerializeValueS (L"catchedDownloads", m_catchedDownloads, false) &&
			pStm->SerializeValueS (L"waitResponse", m_waitResponse, false) &&
			pStm->SerializeValueS (L"ignoreSettings", m_ignoreSettings, false);
	}
};

