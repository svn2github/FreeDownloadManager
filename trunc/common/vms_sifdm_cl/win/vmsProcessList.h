/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include <tlhelp32.h>
#include "vmsWinUtil.h"

struct vmsProcessListItem
{
	tstring tstrExeName;
	vmsCommandLine cmdline;
	DWORD dwProcessId;
	DWORD dwParentProcessId;
};

class vmsProcessList  
{
public:
	enum GenerateListMethod
	{
		GetExeNamesOnly,
		GetFullPathToExes,
		GetFullCommandLine,
	};

public:
	void GenerateList(GenerateListMethod enMethod = GetExeNamesOnly)
	{
		m_vItems.clear ();

		HANDLE hSnapshot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
		if (hSnapshot == INVALID_HANDLE_VALUE) 
			return;

		PROCESSENTRY32 pe = {sizeof (PROCESSENTRY32)}; 

		if (!Process32First (hSnapshot, &pe))
			return;

		while (Process32Next (hSnapshot, &pe))
		{
			vmsProcessListItem item;
			item.tstrExeName = pe.szExeFile;
			item.dwProcessId = pe.th32ProcessID;
			item.dwParentProcessId = pe.th32ParentProcessID;

			switch (enMethod)
			{
			case GetExeNamesOnly:
				break;

			case GetFullPathToExes:
				HANDLE hProcess; hProcess = OpenProcess (PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
					FALSE, item.dwProcessId);
				if (hProcess != NULL)
				{
					TCHAR tsz [MAX_PATH] = _T ("\"");
					if (vmsWinUtil::GetModuleFileNameEx (hProcess, NULL, tsz+1, MAX_PATH-2))
					{
						_tcscat (tsz, _T ("\""));
						item.cmdline.fromString (tsz);
					}
					CloseHandle (hProcess);
				}
				break;

			case GetFullCommandLine:
				TCHAR tsz [MAX_PATH]; UINT nSize; nSize = MAX_PATH;
				if (vmsWinUtil::GetProcessCommandLine (item.dwProcessId, tsz, &nSize))
					item.cmdline.fromString (tsz);
				break;
			}

			m_vItems.push_back (item);
		}

		CloseHandle (hSnapshot);
	}

	int findProcessIndex (LPCTSTR ptszExeName) const
	{
		if (ptszExeName == NULL)
			return -1;

		for (size_t i = 0; i < m_vItems.size (); i++)
		{
			if (_tcsicmp (m_vItems [i].tstrExeName.c_str (), ptszExeName) == 0)
				return (int)i;
		}

		return -1;
	}

	int findProcessIndex (DWORD dwProcessId) const
	{
		for (size_t i = 0; i < m_vItems.size (); i++)
		{
			if (m_vItems [i].dwProcessId == dwProcessId)
				return (int)i;
		}

		return -1;
	}

	int findProcessIndex (const vmsCommandLine &cl, bool bAtLeastExeMatchOnly = false) const
	{
		for (size_t i = 0; i < m_vItems.size (); i++)
		{
			if (m_vItems [i].cmdline == cl)
				return (int)i;
		}

		if (bAtLeastExeMatchOnly)
		{
			for (size_t i = 0; i < m_vItems.size (); i++)
			{
				if (_tcsicmp (m_vItems [i].cmdline.getExe (), cl.getExe ()) == 0)
					return (int)i;
			}
		}

		return -1;
	}

	const vmsProcessListItem* getProcess (int nIndex) const
	{
		return &m_vItems [nIndex];
	}

	int getProcessCount () const
	{
		return (int)m_vItems.size ();
	}

	vmsProcessList()
	{

	}

	virtual ~vmsProcessList()
	{

	}

protected:
	std::vector <vmsProcessListItem> m_vItems;

};

