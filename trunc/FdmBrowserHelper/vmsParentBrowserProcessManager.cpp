/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsParentBrowserProcessManager.h"

vmsParentBrowserProcessManager::vmsParentBrowserProcessManager ()
{
	DWORD ppid = vmsGetParentProcessId ();
	assert (ppid);
	if (ppid)
	{
		m_parentProcess = OpenProcess (PROCESS_ALL_ACCESS__XP, FALSE, ppid);
		assert (m_parentProcess);
	}
}

vmsParentBrowserProcessManager::~vmsParentBrowserProcessManager ()
{
	if (m_parentProcess)
		CloseHandle (m_parentProcess);
}

bool vmsParentBrowserProcessManager::injectSniffDllProxy ()
{
	if (m_sniffDllProxyModule)
		return true;

	if (!m_parentProcess)
		return false;

	auto sniffDllProxy = vmsGetModulePath ();
	sniffDllProxy += L"\\FdmBhSniffDllProxy.dll";

	m_sniffDllProxyModule = vmsLoadDllIntoProcess (m_parentProcess, sniffDllProxy.c_str ());

	return m_sniffDllProxyModule != nullptr;
}

bool vmsParentBrowserProcessManager::injectSniffDll ()
{
	if (m_sniffDllModule)
		return true;

	if (!m_parentProcess)
		return false;

	auto sniffDll = vmsGetModulePath ();
	sniffDll += L"\\flvsniff.dll";

	m_sniffDllModule = vmsLoadDllIntoProcess (m_parentProcess, sniffDll.c_str ());

	return m_sniffDllModule != nullptr;
}
