/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsSimpleIpcItem.h"
template <class TTask>
class vmsSimpleIpcTaskItem :
	public vmsSimpleIpcItem
{
public:
	vmsSimpleIpcTaskItem (
		std::shared_ptr <TTask> task = std::make_shared <TTask> ()) :
		m_task (task)
	{
	}

	~vmsSimpleIpcTaskItem ()
	{
	}

	virtual std::string content () const override
	{
		return vmsJsonSerializeObject (
			*const_cast <vmsSimpleIpcTaskItem*> (this)->m_task);
	}

	virtual void content (const std::string &s) override
	{
		vmsJson2SerializationInputStream json;
		if (!json.BindToStream(std::make_shared <std::stringstream>(s)))
			return;
		verify (m_task->Serialize (&vmsSerializationIoStream (&json)));
	}

	std::shared_ptr <TTask> task () const
	{
		return m_task;
	}

protected:
	std::shared_ptr <TTask> m_task;
};

