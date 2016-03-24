/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBrowserSettings.h"
class vmsFdmSettings :
	public vmsSerializable
{
public:
	vmsFdmSettings ()
	{
	}

	~vmsFdmSettings ()
	{
	}

public:
	vmsFdmBrowserSettings m_browser;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override
	{
		{
			auto node = pStm->SelectOrCreateNode (L"browser");
			assert (node);
			if (!node)
				return false;
			if (!m_browser.Serialize (node.get (), flags))
				return false;
		}

		return true;
	}
};

