/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmVideoMonitoringSettings :
	public vmsSerializable
{
public:
	vmsFdmVideoMonitoringSettings ()
	{
	}

	~vmsFdmVideoMonitoringSettings ()
	{
	}

public:
	bool m_enable = false;
	bool m_enableSnifferDll = false;
	bool m_showButton = true;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, 
		unsigned flags = 0) override
	{
		return pStm->SerializeValueS (L"enable", m_enable) &&
			pStm->SerializeValueS (L"enableSnifferDll", m_enableSnifferDll) &&
			pStm->SerializeValueS (L"showButton", m_showButton);
	}
};

