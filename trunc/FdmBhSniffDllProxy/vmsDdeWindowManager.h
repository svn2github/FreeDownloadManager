/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFlvSniffTaskProcessor.h"
class vmsDdeWindowManager
{
public:
	vmsDdeWindowManager (
		std::shared_ptr <vmsFlvSniffTaskProcessor> taskProcessor);
	~vmsDdeWindowManager ();

protected:
	std::unique_ptr <vmsDdeWindow> m_ddeWindow;
	std::shared_ptr <vmsFlvSniffTaskProcessor> m_taskProcessor;

protected:
	struct queued_dde_task
	{
		HWND hwndSender;
		DWORD dwCdsData;
		std::shared_ptr <vmsFlvSniffTask> task;
	};
	std::queue <queued_dde_task> m_tasks;

protected:
	BOOL DdeDataProcessFn (HWND hwndSender, COPYDATASTRUCT *pCopyDataStruct);
	void CustomMessageProcessFn (WPARAM, LPARAM);
};

