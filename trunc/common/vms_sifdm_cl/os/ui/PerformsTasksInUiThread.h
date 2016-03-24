/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class PerformsTasksInUiThread
{
public:
	virtual void performTasksInUiThread () = 0;
	virtual ~PerformsTasksInUiThread () {}
};