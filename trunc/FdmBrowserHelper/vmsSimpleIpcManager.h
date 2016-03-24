/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsSimpleIpcItem.h"

class vmsSimpleIpcManager
{
public:
	virtual std::unique_ptr <vmsSimpleIpcItem> receive () = 0;
	virtual bool send (const vmsSimpleIpcItem*) = 0;
	virtual ~vmsSimpleIpcManager () {}
};

template <class TSender, class TReceiver>
class vmsSimpleIpcManagerImpl :
	public vmsSimpleIpcManager,
	public vmsThreadSafe4
{
public:
	vmsSimpleIpcManagerImpl ()
	{
	}

	virtual ~vmsSimpleIpcManagerImpl ()
	{
	}

	virtual std::unique_ptr <vmsSimpleIpcItem> receive () override
	{
		return m_receiver.receive ();
	}

	virtual bool send (const vmsSimpleIpcItem *item) override
	{
		vmsThreadSafe4Scope;
		return m_sender.send (item);
	}

	TReceiver* get_receiver ()
	{
		return &m_receiver;
	}

	TSender* get_sender ()
	{
		return &m_sender;
	}

protected:
	TSender m_sender;
	TReceiver m_receiver;
};

