/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsHooksPeCalls.h"
#include "../../../threadsafe/win/vmsTls.h"

class vmsHooksPeCallsForLoadingModules :
	public vmsHooksPeCalls
{
public:
	enum 
	{
		DontHookGetProcAddress		= 1
	};
public:
	vmsHooksPeCallsForLoadingModules (std::shared_ptr <vmsPeFnHook> hooker,
		uint32_t flags = 0) :
		vmsHooksPeCalls (hooker) 
	{
		assert (!ms_hooker); 
		ms_hooker = hooker;
		
		if (!(flags & DontHookGetProcAddress))
		{
			m_hook_functions.push_back (
				hook_fn_info ("kernel32.dll", "GetProcAddress", (FARPROC)myGetProcAddress));
		}
		m_hook_functions.push_back (
			hook_fn_info ("kernel32.dll", "LoadLibraryA", (FARPROC)myLoadLibraryA));
		m_hook_functions.push_back (
			hook_fn_info ("kernel32.dll", "LoadLibraryW", (FARPROC)myLoadLibraryW));
		m_hook_functions.push_back (
			hook_fn_info ("kernel32.dll", "LoadLibraryExA", (FARPROC)myLoadLibraryExA));
		m_hook_functions.push_back (
			hook_fn_info ("kernel32.dll", "LoadLibraryExW", (FARPROC)myLoadLibraryExW));
	}

	~vmsHooksPeCallsForLoadingModules ()
	{
		ms_hooker = nullptr;
	}

protected:
	static std::shared_ptr <vmsPeFnHook> ms_hooker;
	static vmsTls m_tls;

protected:
	static HMODULE WINAPI myLoadLibraryA(LPCSTR lpFileName)
	{
		return onLoadLibrary (lpFileName, false);
	}

	static HMODULE WINAPI myLoadLibraryW(LPCWSTR lpFileName)
	{
		return onLoadLibrary (lpFileName, true);
	}

	static HMODULE onLoadLibrary(LPCVOID lpFileName, bool bUnicode)
	{
		typedef HMODULE (WINAPI *FNLoadLibrary)(LPCVOID lpFileName);

		assert (ms_hooker);
		FNLoadLibrary pfn = (FNLoadLibrary)ms_hooker->getOriginalFunction (bUnicode ? (FARPROC)myLoadLibraryW : (FARPROC)myLoadLibraryA);
		assert (pfn != NULL);

		vmsPeFnHook::loaded_modules_data_t modulesWas;
		ms_hooker->onBeforeNewModuleLoaded (modulesWas);

		HMODULE hMod = pfn (lpFileName);
		if (!hMod)
			return NULL;

		ms_hooker->onAfterNewModuleLoaded (modulesWas);

		return hMod;
	}

	static HMODULE WINAPI myLoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags)
	{
		return onLoadLibraryEx (lpFileName, hFile, dwFlags, false);
	}

	static HMODULE WINAPI myLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)
	{
		return onLoadLibraryEx (lpFileName, hFile, dwFlags, true);
	}

	static HMODULE onLoadLibraryEx(LPCVOID lpFileName, HANDLE hFile, DWORD dwFlags, bool bUnicode)
	{
		typedef HMODULE (WINAPI *FNLoadLibraryEx)(LPCVOID lpFileName, HANDLE hFile, DWORD dwFlags);
		assert (ms_hooker);
		FNLoadLibraryEx pfn = (FNLoadLibraryEx)ms_hooker->getOriginalFunction (bUnicode ? (FARPROC)myLoadLibraryExW : (FARPROC)myLoadLibraryExA);
		assert (pfn != NULL);

		const bool needHook = 0 == (dwFlags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE));

		vmsPeFnHook::loaded_modules_data_t modulesWas;
		if (needHook)
			ms_hooker->onBeforeNewModuleLoaded (modulesWas);

		HMODULE hMod = pfn (lpFileName, hFile, dwFlags);
		if (!hMod)
			return NULL;

		if (needHook)
			ms_hooker->onAfterNewModuleLoaded (modulesWas);

		return hMod;
	}

	static FARPROC WINAPI myGetProcAddress (HMODULE hModule, LPCSTR pszProcName)
	{
		typedef FARPROC(WINAPI *FNGetProcAddress)(HMODULE hModule, LPCSTR pszProcName);

		uintptr_t preventRecursiveCallsCounter = (m_tls.isTlsAllocatedSuccessfully()) ? (uintptr_t)m_tls.getValue() : 0;

		assert (ms_hooker);
		FARPROC pfn = ms_hooker->onGetProcAddress (hModule, pszProcName);
		if (pfn)
		{
			
			
			
			if (!(stricmp(pszProcName, "GetProcAddress") == 0 && hModule == GetModuleHandle(L"kernel32.dll")))
				return pfn;
		}

		FNGetProcAddress pfnGA = NULL;
		if (preventRecursiveCallsCounter)
			pfnGA = (FNGetProcAddress)ms_hooker->getTargetFunction((FARPROC)myGetProcAddress);
		else
			pfnGA = (FNGetProcAddress)ms_hooker->getOriginalFunction((FARPROC)myGetProcAddress);
		assert(pfnGA != NULL);
		
		if (m_tls.isTlsAllocatedSuccessfully())
			m_tls.setValue((LPVOID)++preventRecursiveCallsCounter);

		pfn = pfnGA(hModule, pszProcName);

		if (m_tls.isTlsAllocatedSuccessfully())
			m_tls.setValue((LPVOID)--preventRecursiveCallsCounter);

		return pfn;
	}
};

__declspec (selectany) std::shared_ptr <vmsPeFnHook> vmsHooksPeCallsForLoadingModules::ms_hooker;
__declspec (selectany) vmsTls vmsHooksPeCallsForLoadingModules::m_tls;
