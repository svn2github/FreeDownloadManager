/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../FdmBhSniffDllProxy/vmsFlvSniffCallProcTask.h"
class vmsFdmBhIpcTaskVideoSniffer :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskVideoSniffer ()
	{
	}

	~vmsFdmBhIpcTaskVideoSniffer ()
	{
	}

public:
	vmsFlvSniffCallProcTask m_callProc;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return m_callProc.Serialize (pStm, flags);
	}
};

