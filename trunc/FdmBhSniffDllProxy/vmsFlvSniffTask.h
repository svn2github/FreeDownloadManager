/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFlvSniffCallProcTask.h"
class vmsFlvSniffTask :
	public vmsSerializable
{
public:
	vmsFlvSniffTask ()
	{
	}

	~vmsFlvSniffTask ()
	{
	}

public:
	std::wstring m_id;
	vmsFlvSniffCallProcTask m_callProc;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return pStm->SerializeValue (L"id", m_id) &&
			m_callProc.Serialize (pStm, flags);
	}
};

