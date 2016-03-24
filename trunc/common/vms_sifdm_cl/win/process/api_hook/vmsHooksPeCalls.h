/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsPeFnHook.h"
class vmsHooksPeCalls
{
public:
	vmsHooksPeCalls (std::shared_ptr <vmsPeFnHook> hooker) :
		m_hooker (hooker) {}

	virtual bool hook(bool skipWindowsCompatibleModules = false)
	{
		bool result = true;

		for (auto& fn : m_hook_functions)
		{
			if (!m_hooker->HookFunction(fn.importingModuleName, fn.targetFuncName,
				fn.hookFunction, skipWindowsCompatibleModules))
			{
				result = false;
			}
		}

		return result;
	}

	virtual bool unhook ()
	{
		assert (!"implemented");
		return false;
	}

	virtual ~vmsHooksPeCalls () {}

protected:
	std::shared_ptr <vmsPeFnHook> m_hooker;

protected:
	struct hook_fn_info
	{
		LPCSTR importingModuleName;
		LPCSTR targetFuncName;
		FARPROC hookFunction;

		hook_fn_info (LPCSTR importingModuleName_, LPCSTR targetFuncName_, FARPROC hookFunction_) :
			importingModuleName (importingModuleName_), 
			targetFuncName (targetFuncName_),
			hookFunction (hookFunction_) {}
	};

	std::vector <hook_fn_info> m_hook_functions;
};