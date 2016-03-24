/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "../serialization/vmsSerializable.h"
#include "../error/vmsError.h"
#include "windows_error.h"
#include "vmsFileUtil.h"
#include <TlHelp32.h>
#include <ShellAPI.h>

class vmsCommandLine :
	public vmsSerializable
{
public:
	bool haveArgs () const
	{
		return !m_tstrArgs.empty ();
	}

	int CompareEXEs (const vmsCommandLine &cl) const
	{
		LPCTSTR ptsz1 = getExe ();
		LPCTSTR ptsz2 = cl.getExe ();

		if (ptsz1 && ptsz2)
			return _tcsicmp (ptsz1, ptsz2);

		if (ptsz1 == NULL && ptsz2 == NULL)
			return 0;

		if (!ptsz1)
			return -1;

		return 1;
	}

	LPCTSTR getExeName () const
	{
		LPCTSTR ptsz = _tcsrchr (getExe (), '\\');
		if (!ptsz)
			ptsz = _tcsrchr (getExe (), '/');
		if (ptsz)
			ptsz++;
		return ptsz ? ptsz : getExe ();
	}

	bool is_EXE_CmdLine () const
	{
		if (m_tstrExe.empty ())
			return false;
		if (m_tstrExe.length () < 5)
			return false;

		return _tcsicmp (m_tstrExe.c_str () + m_tstrExe.length () - 4, _T (".exe")) == 0;
	}

	void setArgs (LPCTSTR ptsz)
	{
		while (*ptsz == ' ')
			ptsz++;
		m_tstrArgs = ptsz;
		RemoveTrailingSpaces ();
		setDirty ();
	}

	void setExe (LPCTSTR ptsz)
	{
		m_tstrExe = ptsz;
		setDirty ();
	}

	void Clear()
	{
		m_tstrExe = m_tstrArgs = _T ("");
		setDirty ();
	}

	LPCTSTR getArgs () const
	{
		return m_tstrArgs.c_str ();
	}

	LPCTSTR getExe () const
	{
		return m_tstrExe.c_str ();
	}

	const tstring& getExeAsTstring () const {return m_tstrExe;}

	bool isEmpty () const
	{
		return m_tstrExe.empty ();
	}

	tstring toString() const
	{
		if (m_tstrArgs.empty ())
			return m_tstrExe;

		if (m_tstrExe.empty ())
			return _T ("");

		TCHAR tsz [32*1024] = _T ("");
		_sntprintf (tsz, 32*1024, _T ("\"%s\" %s"), m_tstrExe.c_str (), m_tstrArgs.c_str ());
		return tsz;
	}

	void fromString (LPCTSTR ptsz, bool bConvertToLongPath = false)
	{
		Clear ();

		if (ptsz == NULL || *ptsz == 0)
			return;

		TCHAR tszTmp [MAX_PATH] = _T ("");
		if (_tcschr (ptsz, '%'))
		{
			ExpandEnvironmentStrings (ptsz, tszTmp, MAX_PATH);
			ptsz = tszTmp;
		}

		if (*ptsz == '"')
		{
			ptsz++;
			while (*ptsz && *ptsz != '"')
				m_tstrExe += *ptsz++;
			if (*ptsz == 0)
			{
				m_tstrExe = _T ("");
				return;
			}

			ptsz++;
		}
		else
		{
			

			while (*ptsz == ' ')
				ptsz++;

			do 
			{
				while (*ptsz && *ptsz != ' ')
					m_tstrExe += *ptsz++;

				if (m_tstrExe.length () > 1 && m_tstrExe [1] != ':')
				{
					if (vmsFileUtil::SearchPath (m_tstrExe.c_str (), _T (".exe"), &m_tstrExe))
					{
						bConvertToLongPath = false; 
						break;
					}
				}

				if (*ptsz)
				{
					DWORD dw = GetFileAttributes (m_tstrExe.c_str ());
					if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY) == 0)
						break;

					tstring tstr = m_tstrExe; 

					tstr += _T (".exe");
					dw = GetFileAttributes (tstr.c_str ());
					if (dw != DWORD (-1) && (dw & FILE_ATTRIBUTE_DIRECTORY) == 0)
						break;

					if (m_tstrExe.length () > 4 && 
							_tcsicmp (m_tstrExe.c_str ()+m_tstrExe.length ()-4, _T (".exe")) == 0)
						break; 

					while (*ptsz == ' ')
						m_tstrExe += *ptsz++;
				}

			} while (*ptsz);
		}

		if (bConvertToLongPath)
			m_tstrExe = vmsFileUtil::getLongPathName (m_tstrExe.c_str ());

		while (*ptsz == ' ')
			ptsz++;
	
		m_tstrArgs = ptsz ? ptsz : _T ("");

		RemoveTrailingSpaces ();
	}

	bool operator == (const vmsCommandLine &cl) const 
	{
		return _tcsicmp (m_tstrExe.c_str (), cl.m_tstrExe.c_str ()) == 0 && 
			_tcsicmp (m_tstrArgs.c_str (), cl.m_tstrArgs.c_str ()) == 0;
	}

	vmsCommandLine()
	{

	}

	virtual ~vmsCommandLine()
	{

	}

	vmsCommandLine (LPCTSTR ptszCommandLine) {fromString (ptszCommandLine);}
	vmsCommandLine (const tstring &tstrCommandLine) {fromString (tstrCommandLine.c_str ());}
	vmsCommandLine (const tstring& exe, const tstring& args) : m_tstrExe (exe), m_tstrArgs (args) {setDirty ();}

public:
	enum ExecutionFlags
	{
		RunElevatedIfRequired		= 1,
		WaitForCompletion			= 1 << 1,
		
		SearchPathIfRequired		= 1 << 2,
		WaitForChildCompletion		= 1 << 3, 
		RunAssociatedAppIfNotExe	= 1 << 4,
		RunElevated					= 1 << 5,
	};
	
	bool Execute(DWORD dwFlags = 0, LPDWORD pdwProcessExitCode = nullptr, 
		HANDLE *phProcess = nullptr, vmsError *error = nullptr,
		std::function <bool()> abort_waiting = std::function <bool()> ()) const
	{
		if (isEmpty ())
		{
			if (error)
				*error = std::make_error_code (std::errc::invalid_argument);
			return false;
		}

		HANDLE hProcess = nullptr;
		DWORD dwProcessId = 0;
		BOOL bOK;
		tstring tstrExe = m_tstrExe;

		if (dwFlags & RunElevated)
			bOK = RunProcessElevated (tstrExe, m_tstrArgs, hProcess, dwProcessId);
		else
			bOK = RunProcessNormal (tstrExe, m_tstrArgs, hProcess, dwProcessId);

		if (!bOK)
		{
			DWORD dwError = GetLastError ();
			if (dwError == ERROR_NOT_FOUND || dwError == ERROR_PATH_NOT_FOUND)
			{
				if (dwFlags & SearchPathIfRequired)
				{
					LPTSTR ptszTmp;
					TCHAR tszExe [MAX_PATH] = _T ("");
					SearchPath (NULL, m_tstrExe.c_str (), _T (".exe"), MAX_PATH, tszExe, &ptszTmp);
					tstrExe = tszExe;

					if (dwFlags & RunElevated)
						bOK = RunProcessElevated (tstrExe, m_tstrArgs, hProcess, dwProcessId);
					else
						bOK = RunProcessNormal (tstrExe, m_tstrArgs, hProcess, dwProcessId);
				}
				else
				{
					if (error)
						*error = windows_error::last_error ();
					return false;
				}
			}		
		}

		if (!bOK)
		{
			if (GetLastError () == ERROR_ELEVATION_REQUIRED)
			{
				if (dwFlags & RunElevatedIfRequired)
				{
					bOK = RunProcessElevated (tstrExe, m_tstrArgs, hProcess, dwProcessId);
				}
				else
				{
					if (error)
						*error = windows_error::last_error ();
					return false;
				}
			}
			else if (GetLastError () == ERROR_BAD_EXE_FORMAT)
			{
				SHELLEXECUTEINFO sei = {0};
				sei.cbSize = sizeof (sei);
				sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
				sei.lpVerb = _T ("open");
				sei.lpFile = tstrExe.c_str ();
				sei.nShow = SW_SHOWNORMAL;
				bOK = ShellExecuteEx (&sei);
				if (bOK)
				{
					hProcess = sei.hProcess;
					dwProcessId = GetProcessId (hProcess);
				}
			}
		}

		if (!bOK)
		{
			if (error)
				*error = windows_error::last_error ();
			return false;
		}

		if (phProcess)
			*phProcess = hProcess;

		if ((dwFlags & WaitForCompletion) || pdwProcessExitCode)
		{
			if (!abort_waiting)
			{
				WaitForSingleObject (hProcess, INFINITE);
			}
			else
			{
				while (WaitForSingleObject (hProcess, 100) == WAIT_TIMEOUT)
				{
					if (abort_waiting ())
					{
						CloseHandle (hProcess);
						return true;
					}
				}					
			}

			if (dwFlags & WaitForChildCompletion)
			{
				HANDLE hSnapshot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);
				if (hSnapshot != INVALID_HANDLE_VALUE) 
				{
					PROCESSENTRY32 pe = {sizeof (PROCESSENTRY32)}; 
					if (Process32First (hSnapshot, &pe)) 
					while (Process32Next (hSnapshot, &pe))
					{
						if (pe.th32ParentProcessID == dwProcessId)
						{
							HANDLE h = OpenProcess (SYNCHRONIZE, FALSE, pe.th32ProcessID);
							if (h)
							{
								if (!abort_waiting)
								{
									WaitForSingleObject (h, INFINITE);
								}
								else
								{
									while (WaitForSingleObject (h, 100) == WAIT_TIMEOUT)
									{
										if (abort_waiting ())
										{
											CloseHandle (hProcess);
											CloseHandle (h);
											return true;
										}
									}
								}
								CloseHandle (h);
							}
							break;
						}
					}
					CloseHandle (hSnapshot);
				}
			}

			if (pdwProcessExitCode)
			{
				*pdwProcessExitCode = 0;
				if (FALSE == GetExitCodeProcess (hProcess, pdwProcessExitCode))
				{
					if (error)
						*error = windows_error::last_error ();
					if (!phProcess)
						CloseHandle (hProcess);
					return false;
				}
			}
		}

		if (!phProcess)
			CloseHandle (hProcess);

		return true;
	}

	
	bool process_wait_including_children (HANDLE process, HANDLE abort_waiting_event)
	{
		assert (process);
		assert (abort_waiting_event);

		auto process_id = GetProcessId (process);

		HANDLE ahToWait [] = {abort_waiting_event, process};

		if (WAIT_OBJECT_0 == WaitForMultipleObjects (_countof (ahToWait), ahToWait, FALSE, INFINITE))
			return false;

		bool aborted = false;

		HANDLE hSnapshot = CreateToolhelp32Snapshot (TH32CS_SNAPALL, NULL);

		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 pe = { sizeof (PROCESSENTRY32) };

			if (Process32First (hSnapshot, &pe))
			{
				while (Process32Next (hSnapshot, &pe))
				{
					if (pe.th32ParentProcessID == process_id)
					{
						HANDLE h = OpenProcess (SYNCHRONIZE, FALSE, pe.th32ProcessID);
						if (h)
						{
							ahToWait [1] = h;
							aborted = WAIT_OBJECT_0 == WaitForMultipleObjects (_countof (ahToWait), ahToWait, FALSE, INFINITE);
							CloseHandle (h);
							if (aborted)
								break;
						}
						break;
					}
				}
			}

			CloseHandle (hSnapshot);
		}

		return !aborted;
	}

protected:
	void RemoveTrailingSpaces ()
	{
		while (m_tstrArgs.empty () == false && m_tstrArgs [m_tstrArgs.length () - 1] == ' ')
			m_tstrArgs.erase (m_tstrArgs.begin () + m_tstrArgs.length () - 1);
	}

	tstring m_tstrArgs;
	tstring m_tstrExe;

protected:
	static bool RunProcessElevated (const tstring& exe, const tstring& args, 
		HANDLE& hProcess, DWORD &dwProcessId)
	{
		SHELLEXECUTEINFO sei = {0};
		sei.cbSize = sizeof (sei);
		sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = _T ("runas");
		sei.lpFile = exe.c_str ();
		sei.lpParameters = args.c_str ();
		sei.nShow = SW_SHOWNORMAL;
		if (!ShellExecuteEx (&sei))
			return false;
		hProcess = sei.hProcess;
		dwProcessId = GetProcessId (hProcess);
		return true;
	}

	static bool RunProcessNormal (const tstring& exe, const tstring& args, 
		HANDLE& hProcess, DWORD &dwProcessId)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory (&si, sizeof (si));
		si.cb = sizeof (si);
		ZeroMemory (&pi, sizeof (pi));

		tstringstream tss;
		tss << _T ("\"") << exe << _T ("\"");
		if (!args.empty ())
			tss << _T (" ") << args;

		auto tss_s = tss.str (); tss_s.push_back (0);

		if (!CreateProcess (nullptr, &tss_s.front (),
			NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi))
		{
			return false;
		}

		hProcess = pi.hProcess;
		dwProcessId = pi.dwProcessId;

		CloseHandle (pi.hThread);

		return true;
	}

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags ) override
	{
		return pStm->SerializeValue (L"cmdlineEXE", m_tstrExe) &&
			pStm->SerializeValue (L"cmdlineArgs", m_tstrArgs);
	}
};

