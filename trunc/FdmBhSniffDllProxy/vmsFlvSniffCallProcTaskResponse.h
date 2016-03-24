/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
class vmsFlvSniffCallProcTaskResponse :
	public vmsSerializable
{
public:
	vmsFlvSniffCallProcTaskResponse ()
	{
	}

	~vmsFlvSniffCallProcTaskResponse ()
	{
	}

public:
	std::wstring m_result; 
	std::wstring m_error;
	std::wstring m_error2;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"result", m_result) &&
			pStm->SerializeValue (L"error", m_error) &&
			pStm->SerializeValue (L"error2", m_error2);
	}
};

