/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsPeTools.h"
class vmsPeFnHook :
	public vmsCreatesThreads
{
public:
	vmsPeFnHook ()
	{
	}

	~vmsPeFnHook ()
	{
		Shutdown (true);
	}

protected:
	struct FunctionInfo
	{
		std::string strImportingModuleName;
		HMODULE hImportingModule = nullptr;
		std::string strFunctionName;
		FARPROC pfnTarget = nullptr; 
		FARPROC pfnHook = nullptr; 
		FARPROC pfnOriginal = nullptr; 
	};

public:
	bool HookFunction(LPCSTR pszImportingModuleName, LPCSTR pszTargetFuncName, FARPROC pfnNew)
	{
		assert (pszImportingModuleName);
		assert (pszTargetFuncName);
		assert (pfnNew);

		if (findFunctionIndexByName (pszImportingModuleName, pszTargetFuncName) != -1)
			return true;

		HMODULE hMainModule = GetModuleHandle (NULL);
		HMODULE hThisModule = vmsGetThisModuleHandle ();

		vmsAUTOLOCKSECTION (m_csFunctions); 

		{
			FunctionInfo fi;
			fi.strImportingModuleName = pszImportingModuleName;
			fi.hImportingModule = GetModuleHandleA (pszImportingModuleName);
			assert (fi.hImportingModule != NULL);
			if (!fi.hImportingModule)
				return false;
			fi.strFunctionName = pszTargetFuncName;
			fi.pfnTarget = vmsPeTools::GetProcAddressFromExportTable (
				fi.hImportingModule, pszTargetFuncName);
			fi.pfnHook = pfnNew;

			m_vFunctions.push_back (fi);
		}

		FunctionInfo &r_fi = m_vFunctions [m_vFunctions.size () - 1];

		bool replaced = HookFunctionInModule (hMainModule, r_fi, true);
		if (!r_fi.pfnOriginal)
			r_fi.pfnOriginal = r_fi.pfnTarget;

		vmsAUTOLOCKSECTION_UNLOCK (m_csFunctions);

		
		CToolhelp th (TH32CS_SNAPMODULE, GetCurrentProcessId ());

		MODULEENTRY32 me = { sizeof (me) };
		for (BOOL bOk = th.ModuleFirst (&me); bOk; bOk = th.ModuleNext (&me))
		{
			if (me.hModule != hThisModule && me.hModule != hMainModule)
			{
				if (!vmsCheckModuleHandleValid (me.hModule))
					continue; 
				if (HookFunctionInModule (me.hModule, r_fi))
					replaced = true;
			}
		}

		return replaced;
	}

protected:
	bool HookFunctionInModule (HMODULE mod, FunctionInfo &fi, 
		bool updateOriginalFunction = false)
	{
		vmsAUTOLOCKSECTION (m_csFunctions);
		bool result = false;
		FARPROC pfnOriginal = nullptr;
		if (m_petools.ReplaceIATfunc (mod, fi.strImportingModuleName.c_str (), 
			fi.strFunctionName.c_str (), fi.pfnTarget, fi.pfnHook, &pfnOriginal))
		{
			
			result = true;
		}
		if (m_petools.ReplaceDIATfunc (mod, fi.strImportingModuleName.c_str (),
			fi.strFunctionName.c_str (), fi.pfnTarget, fi.pfnHook, 
			pfnOriginal ? nullptr : &pfnOriginal))
		{
			
			result = true;
		}
		if (result)
		{
			if (!fi.pfnOriginal && updateOriginalFunction)
				fi.pfnOriginal = pfnOriginal;
		}
		return result;
	}

	bool UnHookFunctionInModule (HMODULE mod, const FunctionInfo &fi)
	{
		vmsAUTOLOCKSECTION (m_csFunctions);
		bool result = false;
		if (m_petools.ReplaceIATfunc (mod, fi.strImportingModuleName.c_str (),
			fi.strFunctionName.c_str (), fi.pfnHook, fi.pfnOriginal, nullptr))
		{
			result = true;
		}
		if (m_petools.ReplaceDIATfunc (mod, fi.strImportingModuleName.c_str (),
			fi.strFunctionName.c_str (), fi.pfnHook, fi.pfnOriginal, nullptr))
		{
			result = true;
		}
		return result;
	}

protected:
	std::vector <FunctionInfo> m_vFunctions;
	vmsCriticalSection m_csFunctions;

protected:
	SSIZE_T findFunctionIndexByName(LPCSTR pszImportingModuleName, LPCSTR pszFuncName)
	{
		assert (pszImportingModuleName != NULL && pszFuncName != NULL);
		if (!pszImportingModuleName || !pszFuncName)
			return -1;

		for (size_t i = 0; i < m_vFunctions.size (); i++)
		{
			FunctionInfo &fi = m_vFunctions [i];
			if (!_stricmp (pszImportingModuleName, fi.strImportingModuleName.c_str ()) &&
				!_stricmp (pszFuncName, fi.strFunctionName.c_str ()))
				return i;
		}

		return -1;
	}

	SSIZE_T findFunctionIndexByName(HMODULE hImportingModule, LPCSTR pszFuncName)
	{
		assert (hImportingModule != NULL && pszFuncName != NULL);
		if (!hImportingModule || !pszFuncName)
			return -1;

		for (size_t i = 0; i < m_vFunctions.size (); i++)
		{
			FunctionInfo &fi = m_vFunctions [i];
			if (hImportingModule == fi.hImportingModule && !_stricmp (pszFuncName, fi.strFunctionName.c_str ()))
				return i;
		}

		return -1;
	}

	SSIZE_T findFunctionIndexByHookFnAddr(FARPROC pfnHook)
	{
		for (size_t i = 0; i < m_vFunctions.size (); i++)
		{
			if (m_vFunctions [i].pfnHook == pfnHook)
				return i;
		}

		return -1;
	}

protected:
	vmsPeTools m_petools;

public:
	FARPROC getOriginalFunction(FARPROC pfnHook)
	{
		vmsAUTOLOCKSECTION (m_csFunctions);
		SSIZE_T nIndex = findFunctionIndexByHookFnAddr (pfnHook);
		if (nIndex == -1)
			return NULL;
		return m_vFunctions [nIndex].pfnOriginal;
	}

	
	void onNewModuleLoaded(HMODULE hModule)
	{
		vmsAUTOLOCKSECTION (m_csFunctions);
		std::vector <FunctionInfo> vFunctions = m_vFunctions;
		vmsAUTOLOCKSECTION_UNLOCK (m_csFunctions);

		for (size_t i = 0; i < vFunctions.size (); i++)
			HookFunctionInModule (hModule, vFunctions [i]);
	}

	typedef std::set <HMODULE> loaded_modules_data_t;

	
	

	void onBeforeNewModuleLoaded (loaded_modules_data_t &data)
	{
		get_loaded_modules_data (data);
	}

	void waitForModuleBeLoaded (HMODULE mod)
	{
		auto timeNow = time (nullptr);
		while (std::difftime (time (nullptr), timeNow) < 10)
		{
			if (!vmsCheckModuleHandleValid (mod))
			{
				if (WaitForShutdownEvent (30))
					break;
				continue;
			}
			onNewModuleLoaded (mod);
			break;
		}
	}

	struct threadWaitForModuleBeLoadedParam
	{
		vmsPeFnHook *pthis;
		HMODULE hTarget;
		threadWaitForModuleBeLoadedParam (vmsPeFnHook *pthis_, HMODULE hTarget_) :
			pthis (pthis_), hTarget (hTarget_) {}
	};
	static unsigned __stdcall _threadWaitForModuleBeLoaded (LPVOID lp)
	{
		std::unique_ptr <threadWaitForModuleBeLoadedParam> data (
			reinterpret_cast <threadWaitForModuleBeLoadedParam*> (lp));
		data->pthis->waitForModuleBeLoaded (data->hTarget);
		return 0;
	}

	void onAfterNewModuleLoaded (const loaded_modules_data_t &data)
	{
		loaded_modules_data_t dataNow;
		get_loaded_modules_data (dataNow);

		for (auto it = dataNow.begin (); it != dataNow.end (); ++it)
		{
			if (data.find (*it) == data.end ())
			{
				auto mod = *it;
				if (vmsCheckModuleHandleValid (mod))
				{
					onNewModuleLoaded (mod);
				}
				else
				{
					CreateThread (_threadWaitForModuleBeLoaded, 
						new threadWaitForModuleBeLoadedParam (this, mod));
				}
			}
		}
	}

	
	FARPROC onGetProcAddress(HMODULE hModule, LPCSTR pszFuncName)
	{
		if (DWORD_PTR (pszFuncName) <= _UI16_MAX)
			return NULL;
		vmsAUTOLOCKSECTION (m_csFunctions);
		auto nIndex = findFunctionIndexByName (hModule, pszFuncName);
		return nIndex != -1 ? m_vFunctions [nIndex].pfnHook : NULL;
	}

	void RemoveAllHooks(void)
	{
		vmsAUTOLOCKSECTION (m_csFunctions);
		for (size_t i = 0; i < m_vFunctions.size (); i++)
			RemoveHook (m_vFunctions [i]);
		m_vFunctions.clear ();
	}

protected:
	void RemoveHook(FunctionInfo& fi)
	{
		HMODULE hThisModule = vmsGetThisModuleHandle ();

		
		CToolhelp th (TH32CS_SNAPMODULE, GetCurrentProcessId ());

		MODULEENTRY32 me = { sizeof (me) };
		for (BOOL bOk = th.ModuleFirst (&me); bOk; bOk = th.ModuleNext (&me))
		{
			if (me.hModule != hThisModule)
				UnHookFunctionInModule (me.hModule, fi);
		}
	}

	void get_loaded_modules_data (loaded_modules_data_t &data)
	{
		data.clear ();
		CToolhelp th (TH32CS_SNAPMODULE, GetCurrentProcessId ());
		MODULEENTRY32 me = { sizeof (me) };
		for (BOOL bOk = th.ModuleFirst (&me); bOk; bOk = th.ModuleNext (&me)) 
			data.insert (me.hModule);
	}
};

