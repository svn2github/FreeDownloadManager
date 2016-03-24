/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "../FdmBhSniffDllProxy/vmsFlvSniffTask.h"
#include "../FdmBhSniffDllProxy/vmsFlvSniffTaskResponse.h"
#include "../FdmBhSniffDllProxy/defs.h"
class vmsFdmBhSniffDllProxyManager :
	public vmsCreatesThreads2,
	public vmsThreadSafe4
{
public:
	using task_callback_t = 
		std::function <void(vmsFlvSniffTaskResponse&)>;

public:
	vmsFdmBhSniffDllProxyManager ()
	{
		
	}

	~vmsFdmBhSniffDllProxyManager ()
	{
		if (m_ddeWindow && m_ddeWindow->IsWindow ())
			m_ddeWindow->PostMessage (WM_CLOSE);
		shutdown ();
	}

	bool initialize ()
	{
		m_ddeWindow = std::make_unique <vmsDdeWindow> (
			std::bind (&vmsFdmBhSniffDllProxyManager::process_dde_data, 
				this, std::placeholders::_1, std::placeholders::_2));

		create_thread (
			&vmsFdmBhSniffDllProxyManager::thread_dde_window, this);

		return true;
	}

	bool perform_task (vmsFlvSniffTask &task, 
		task_callback_t callback)
	{
		if (!m_sniffDllProxyWindow)
		{
			if (!findDllProxyDdeWindow ())
				return false;
		}

		{
			vmsThreadSafe4Scope;
			m_tasksInProgress [task.m_id] = callback;
		}

		auto strTask = vmsJsonSerializeObject (task);

		COPYDATASTRUCT cds = { 0 };
		cds.dwData = 0;
		cds.lpData = &strTask.front ();
		cds.cbData = (DWORD)strTask.length ();

		SendMessage (m_sniffDllProxyWindow, WM_COPYDATA,
			(WPARAM)m_ddeWindow->m_hWnd, (LPARAM)(LPVOID)&cds);

		return true;
	}

protected:
	HWND m_sniffDllProxyWindow = nullptr;
	std::unique_ptr <vmsDdeWindow> m_ddeWindow;
	std::map <std::wstring, task_callback_t> m_tasksInProgress;

protected:
	void thread_dde_window ()
	{
		verify (m_ddeWindow->Create (L""));
		MSG msg;
		while (GetMessage (&msg, nullptr, 0, 0))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	LRESULT process_dde_data (HWND hwndSender, COPYDATASTRUCT *cds)
	{
		if (!cds->lpData ||
			!cds->cbData)
		{
			return FALSE;
		}

		vmsFlvSniffTaskResponse resp;

		auto ss = std::make_shared <std::stringstream> ();
		ss->write (reinterpret_cast <const char*> (cds->lpData),
			cds->cbData);

		vmsJson2SerializationInputStream json;
		if (!json.BindToStream (ss) ||
			!resp.Serialize (&vmsSerializationIoStream (&json)))
		{
			return FALSE;
		}

		task_callback_t callback;

		{
			vmsThreadSafe4Scope;
			auto it = m_tasksInProgress.find (resp.m_id);
			assert (it != m_tasksInProgress.end ());
			if (it != m_tasksInProgress.end ())
			{
				callback = it->second;
				m_tasksInProgress.erase (it);
			}
		}

		if (callback)
			callback (resp);

		return TRUE;
	}

	bool findDllProxyDdeWindow ()
	{
		{
			vmsThreadSafe4Scope;
			m_sniffDllProxyWindow = FindWindow (nullptr, 
				SniffDllProxyDdeWindowTitle);
		}
		return m_sniffDllProxyWindow != nullptr;
	}
};

