/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "WindowTask.h"
class WindowsTasksManager :
	public vmsThreadSafe4
{
public:
	struct task
	{
		std::shared_ptr <WindowTask> t;
		bool need_cancel = false;
	};

public:
	WindowsTasksManager ()
	{
	}

	~WindowsTasksManager ()
	{
	}

	void perform_task (const std::shared_ptr <WindowTask> &t)
	{
		{
			vmsThreadSafe4Scope;
			auto it = m_tasks.find (t->m_window);
			if (it != m_tasks.end ())
			{
				if (it->second->t->m_action == t->m_action)
					return;
				
				it->second->need_cancel = true;
				vmsThreadSafe4ScopeUnlock;
				
				for (;;)
				{
					Sleep (10);
					vmsThreadSafe4Scope;
					it = m_tasks.find (t->m_window);
					if (it == m_tasks.end ())
						break;
				}
			}
		}

		auto new_t = std::make_shared <task> ();
		new_t->t = t;

		{
			vmsThreadSafe4Scope;
			m_tasks [t->m_window] = new_t;
		}

		switch (t->m_action)
		{
		case WindowTaskType::show:
			if (!IsWindowVisible (t->m_window))
				ShowWindow (t->m_window, SW_SHOW);
			break;

		case WindowTaskType::hide:
			if (t->m_needWait)
			{
				while (!IsWindowVisible (t->m_window) && !new_t->need_cancel)
					Sleep (10);
			}
			if (!new_t->need_cancel && IsWindowVisible (t->m_window))
				ShowWindow (t->m_window, SW_HIDE);
			break;
		}

		{
			vmsThreadSafe4Scope;
			auto it = m_tasks.find (t->m_window);
			assert (it != m_tasks.end ());
			if (it != m_tasks.end ())
				m_tasks.erase (it);
		}
	}

protected:
	std::map <HWND, std::shared_ptr <task>> m_tasks;
};

