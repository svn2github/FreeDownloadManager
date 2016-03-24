/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "fdm.h"
#include "vmsProcessesInjector.h"
#include "common\vms_sifdm_cl\win\vmsProcessList.h"
#include "common\vms_sifdm_cl\win\process\util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vmsProcessesInjector::vmsProcessesInjector()
{
	InitializeCriticalSection (&m_csProcessList);
	m_bNeverFreeInjectedDlls = false;
}

vmsProcessesInjector::~vmsProcessesInjector()
{
	DeleteCriticalSection (&m_csProcessList);
}

void vmsProcessesInjector::DoJob()
{
	if (m_vtstrNamesOfProcesesToMonitor.empty ())
		return;

	vmsProcessList pl;
	pl.GenerateList ();

	std::vector <DWORD> vdwPIDs;

	EnterCriticalSection(&m_csProcessList);

	for (int i = 0; i < pl.getProcessCount (); i++)
	{
		const vmsProcessListItem *process = pl.getProcess (i);

		size_t j = 0;
		for (j = 0; j < m_vtstrNamesOfProcesesToMonitor.size (); j++)
		{
			if (_tcsicmp (m_vtstrNamesOfProcesesToMonitor [j].c_str (), process->tstrExeName.c_str ()) == 0)
				break;
		}

		if (j == m_vtstrNamesOfProcesesToMonitor.size ())
			continue;

		vdwPIDs.push_back (process->dwProcessId);
	}

	std::vector <ProcessedProcess> vProcesses;

	size_t j = 0;
	for (j = 0; j < vdwPIDs.size (); j++)
	{
		DWORD dwPID = vdwPIDs [j];

		size_t k = 0;
		for (k = 0; k < m_vProcesses.size (); k++)
		{
			if (m_vProcesses [k].dwPID == dwPID)
				break;
		}

		if (k == m_vProcesses.size ())
		{
			ProcessedProcess pp;
			pp.dwPID = dwPID;
			InjectIntoProcess (&pp);
			vProcesses.push_back (pp);
		}
		else
		{
			vProcesses.push_back (m_vProcesses [k]);
		}	
	}

	for (j = 0; j < m_vProcesses.size (); j++)
	{
		DWORD dwPID = m_vProcesses [j].dwPID;
		size_t k = 0;
		for (k = 0; k < vProcesses.size (); k++)
		{
			if (dwPID == vProcesses [k].dwPID)
				break;
		}

		if (k == vProcesses.size ())
		{
			if (!m_bNeverFreeInjectedDlls)
				FreeDllsFromProcess (&m_vProcesses [j]);
		}
	}

	m_vProcesses = vProcesses;

	LeaveCriticalSection (&m_csProcessList);
}

void vmsProcessesInjector::InjectIntoProcess(ProcessedProcess *pp)
{
	HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS__XP, FALSE, pp->dwPID);
	ASSERT (hProcess != NULL);

	pp->vhInjectedDlls.clear ();

	if (!hProcess)
		return;

	for (size_t i = 0; i < m_vtstrDllsToInject.size (); i++)
	{
		HMODULE h = vmsLoadDllIntoProcess (hProcess, 
			m_vtstrDllsToInject [i].c_str ());
		if (h)
			pp->vhInjectedDlls.push_back (h);
	}

	CloseHandle (hProcess);
}

void vmsProcessesInjector::FreeDllsFromProcesses()
{
	EnterCriticalSection(&m_csProcessList);

	for (size_t i = 0; i < m_vProcesses.size (); i++)
		FreeDllsFromProcess (&m_vProcesses [i]);

	m_vProcesses.clear ();

	LeaveCriticalSection (&m_csProcessList);
}

void vmsProcessesInjector::FreeDllsFromProcess(ProcessedProcess *pp)
{
	HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS__XP, FALSE, pp->dwPID);
	if (!hProcess)
		return;
	
	for (size_t j = 0; j < pp->vhInjectedDlls.size (); j++)
		vmsFreeDllFromProcess (hProcess, pp->vhInjectedDlls [j]);

	CloseHandle (hProcess);
}
