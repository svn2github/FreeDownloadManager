/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFlvSniffCallProcTaskResponse.h"
class vmsFlvSniffTaskResponse :
	public vmsSerializable
{
public:
	vmsFlvSniffTaskResponse ()
	{
	}

	~vmsFlvSniffTaskResponse ()
	{
	}

public:
	std::wstring m_id;
	vmsFlvSniffCallProcTaskResponse m_callProc;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"id", m_id) &&
			m_callProc.Serialize (pStm, flags);
	}
};

