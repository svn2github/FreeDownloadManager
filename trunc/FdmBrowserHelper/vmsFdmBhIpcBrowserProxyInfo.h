/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBhIpcBrowserProxyInfo :
	public vmsSerializable
{
public:
	vmsFdmBhIpcBrowserProxyInfo ()
	{
	}

	~vmsFdmBhIpcBrowserProxyInfo ()
	{
	}

public:
	uint32_t m_type;
	std::wstring m_protocol;
	std::wstring m_addr;
	uint32_t m_port;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValueS (L"type", m_type) &&
			pStm->SerializeValue (L"protocol", m_protocol) &&
			pStm->SerializeValue (L"addr", m_addr) &&
			pStm->SerializeValueS (L"port", m_port);
	}
};

