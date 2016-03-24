/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskResponse.h"
#include "../FdmBhSniffDllProxy/vmsFlvSniffCallProcTaskResponse.h"
class vmsFdmBhIpcVideoSnifferTaskResponse :
	public vmsFdmBhIpcTaskResponse
{
public:
	vmsFdmBhIpcVideoSnifferTaskResponse ()
	{
	}

	~vmsFdmBhIpcVideoSnifferTaskResponse ()
	{
	}

public:
	vmsFlvSniffCallProcTaskResponse m_videoSniffer;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, 
		unsigned flags = 0) override
	{
		auto node = pStm->SelectOrCreateNode (L"videoSniffer");
		if (!node)
			return false;
		if (!m_videoSniffer.Serialize (node.get (), flags))
			return false;
		return vmsFdmBhIpcTaskResponse::Serialize (pStm, flags);
	}
};

