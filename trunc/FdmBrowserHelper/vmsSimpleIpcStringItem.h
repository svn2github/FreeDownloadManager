/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsSimpleIpcItem.h"
class vmsSimpleIpcStringItem :
	public vmsSimpleIpcItem
{
public:
	vmsSimpleIpcStringItem ()
	{
	}

	std::string content () const override
	{
		return m_content;
	}

	void content (const std::string &s) override
	{
		m_content = s;
	}

protected:
	std::string m_content;
};
