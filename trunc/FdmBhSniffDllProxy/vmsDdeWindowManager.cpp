/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsDdeWindowManager.h"
#include "defs.h"

vmsDdeWindowManager::vmsDdeWindowManager (
	std::shared_ptr <vmsFlvSniffTaskProcessor> taskProcessor) :
	m_taskProcessor (taskProcessor)
{
	m_ddeWindow = std::make_unique <vmsDdeWindow> (
		std::bind (&vmsDdeWindowManager::DdeDataProcessFn, this, std::placeholders::_1, std::placeholders::_2),
		std::bind (&vmsDdeWindowManager::CustomMessageProcessFn, this, std::placeholders::_1, std::placeholders::_2));

	verify (m_ddeWindow->Create (SniffDllProxyDdeWindowTitle));
}

vmsDdeWindowManager::~vmsDdeWindowManager ()
{
}

BOOL vmsDdeWindowManager::DdeDataProcessFn (
	HWND hwndSender, COPYDATASTRUCT *pCopyDataStruct)
{
	if (!pCopyDataStruct->lpData ||
		!pCopyDataStruct->cbData)
	{
		return FALSE;
	}

	auto task = std::make_shared <vmsFlvSniffTask> ();

	auto ss = std::make_shared <std::stringstream> ();
	ss->write (reinterpret_cast <const char*> (pCopyDataStruct->lpData),
		pCopyDataStruct->cbData);

	vmsJson2SerializationInputStream json;
	if (!json.BindToStream (ss) ||
		!task->Serialize (&vmsSerializationIoStream (&json)))
	{
		return FALSE;
	}

	queued_dde_task t;
	t.hwndSender = hwndSender;
	t.dwCdsData = pCopyDataStruct->dwData;
	t.task = std::move (task);
	m_tasks.push (t);
	m_ddeWindow->postCustomMessage ();

	return TRUE;
}

void vmsDdeWindowManager::CustomMessageProcessFn (WPARAM, LPARAM)
{
	auto &t = m_tasks.front ();

	vmsFlvSniffTaskResponse resp;
	verify (m_taskProcessor->process_task (*t.task, resp));

	auto strResponse = vmsJsonSerializeObject (resp);

	COPYDATASTRUCT cds = { 0 };
	cds.dwData = t.dwCdsData;
	cds.lpData = &strResponse.front ();
	cds.cbData = (DWORD)strResponse.length ();

	SendMessage (t.hwndSender, WM_COPYDATA,
		(WPARAM)m_ddeWindow->m_hWnd, (LPARAM)(LPVOID)&cds);

	m_tasks.pop ();
}
