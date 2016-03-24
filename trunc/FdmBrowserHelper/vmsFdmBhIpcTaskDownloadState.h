/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFdmBhIpcTaskDownloadState :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskDownloadState ()
	{
	}

	~vmsFdmBhIpcTaskDownloadState ()
	{
	}

public:
	std::wstring m_url;
	std::wstring m_state;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, 
		unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"url", m_url) &&
			pStm->SerializeValue (L"state", m_state);
	}
};

