/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "WindowsTasksManager.h"
class vmsFdmBhIpcTaskWindow :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTaskWindow ()
	{
	}

	~vmsFdmBhIpcTaskWindow ()
	{
	}

public:
	std::shared_ptr <WindowTask> m_task = std::make_shared <WindowTask> ();

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		return m_task->Serialize (pStm, flags);
	}
};

