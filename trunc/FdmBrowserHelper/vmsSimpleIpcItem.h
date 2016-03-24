/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class vmsSimpleIpcItem
{
public:
	virtual std::string content () const = 0;
	virtual void content (const std::string &s) = 0;
	virtual ~vmsSimpleIpcItem () {}
};

