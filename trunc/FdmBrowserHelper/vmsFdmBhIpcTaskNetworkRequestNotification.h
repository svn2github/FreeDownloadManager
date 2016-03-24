/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBhIpcTaskNetworkRequestNotification :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskNetworkRequestNotification ()
	{
	}

	~vmsFdmBhIpcTaskNetworkRequestNotification ()
	{
	}

public:
	std::wstring m_type;
	std::wstring m_url;
	std::wstring m_srcTabUrl;
	std::wstring m_requestId;
	std::wstring m_requestHeaders;
	std::wstring m_responseHeaders;
	std::wstring m_data; 

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, 
		unsigned flags = 0) override
	{
		pStm->SerializeValue (L"url", m_url);
		pStm->SerializeValue (L"srcTabUrl", m_srcTabUrl);
		pStm->SerializeValue (L"requestId", m_requestId);
		pStm->SerializeValue (L"requestHeaders", m_requestHeaders);
		pStm->SerializeValue (L"responseHeaders", m_responseHeaders);
		pStm->SerializeValue (L"data", m_data);
		return pStm->SerializeValue (L"type", m_type);
	}
};

