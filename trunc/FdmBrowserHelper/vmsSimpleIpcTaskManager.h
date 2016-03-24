/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsSimpleIpcItemReceiver.h"
#include "vmsSimpleIpcItemSender.h"
#include "vmsSimpleIpcManager.h"
#include "vmsSimpleIpcTaskItem.h"

template <class TTask>
class vmsSimpleIpcTaskItemReceiver :
	public vmsSimpleIpcItemReceiver
{
protected:
	virtual std::unique_ptr <vmsSimpleIpcItem> createIpcItem (
		const std::string &content) override
	{
		auto result = std::make_unique <vmsSimpleIpcTaskItem<TTask>> ();
		result->content (content);
		return std::move (result);
	}
};

template <class TTask>
class vmsSimpleIpcTaskManager :
	public vmsSimpleIpcManagerImpl <vmsSimpleIpcItemSender, vmsSimpleIpcTaskItemReceiver<TTask>> 
{
public:
	vmsSimpleIpcTaskManager ()
	{
	}

	~vmsSimpleIpcTaskManager ()
	{
	}

	std::unique_ptr <vmsSimpleIpcTaskItem<TTask>> receive_task ()
	{
		std::unique_ptr <vmsSimpleIpcTaskItem<TTask>> result;
		auto r = vmsSimpleIpcManagerImpl::receive ();
		if (r)
		{
			auto p = dynamic_cast <vmsSimpleIpcTaskItem<TTask>*> (r.get ());
			assert (p);
			if (p)
			{
				result.reset (p);
				r.release ();
			}
		}
		return result;
	}

	bool send_task (const vmsSimpleIpcTaskItem<TTask> *item)
	{
		return vmsSimpleIpcManagerImpl::send (item);
	}
};

