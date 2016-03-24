/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBrowserMenuSettings.h"
#include "vmsFdmBrowserMonitorSettings.h"
class vmsFdmBrowserSettings :
	public vmsSerializable
{
public:
	vmsFdmBrowserSettings ()
	{
	}

	~vmsFdmBrowserSettings ()
	{
	}

public:
	vmsFdmBrowserMenuSettings m_menu;
	vmsFdmBrowserMonitorSettings m_monitor;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		{
			auto node = pStm->SelectOrCreateNode (L"menu");
			assert (node);
			if (!node)
				return false;
			if (!m_menu.Serialize (node.get (), flags))
				return false;
		}

		{
			auto node = pStm->SelectOrCreateNode (L"monitor");
			assert (node);
			if (!node)
				return false;
			if (!m_monitor.Serialize (node.get (), flags))
				return false;
		}

		return true;
	}
};

