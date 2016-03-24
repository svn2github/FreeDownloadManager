/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "PerformsTasksInUiThread.h"
class PerformsTasksInUiThreadObjList :
	public PerformsTasksInUiThread
{
public:
	virtual void performTasksInUiThread () override
	{
		for (const auto &obj : m_objects)
			obj->performTasksInUiThread ();
	}

	void add_object (const std::shared_ptr <PerformsTasksInUiThread> &obj)
	{
		m_objects.push_back (obj);
	}

protected:
	std::vector <std::shared_ptr <PerformsTasksInUiThread>> m_objects;
};