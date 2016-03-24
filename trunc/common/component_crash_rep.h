/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

#include "vms_sifdm_cl/base.h"
#include "vms_sifdm_cl/exceptions/unh_exc_handler/vmsAppCrashReporter.h"

std::unique_ptr <vmsAppCrashReporter> g_crashReporter;

void initializeCrashReporter (const tstring &crashReportingExeRelPath = _T ("fdm.exe"))
{
	const auto mod_handle = vmsGetThisModuleHandle ();
	const auto module_file = vmsGetModuleFileName (mod_handle);
	const auto module_path = vmsGetModulePath (mod_handle);
	LPCTSTR module_file_name = _tcsrchr (module_file.c_str (), _T('\\'));
	assert (module_file_name);
	if (!module_file_name++)
		return;
	const bool exeModule = !_tcsicmp (module_file_name + _tcslen (module_file_name) - 4, _T(".exe"));

	vmsBinaryFileVersionInfo bfi;
	bfi.Retrieve (module_file.c_str ());

	g_crashReporter = std::make_unique <vmsAppCrashReporter> (
		bfi.m_tstrProductName, module_file_name,
		bfi.m_tstrFileVersion, L"", L"");
	g_crashReporter->setCrashReportingExe (module_path + L"\\" + crashReportingExeRelPath);

	uint32_t flags = exeModule ? vmsCrashCatcher::LegacyMode : 
		(vmsCrashCatcher::UseVectoredExceptionsHandler |
		vmsCrashCatcher::CatchCrtExceptions | 
		vmsCrashCatcher::CatchThisModuleExceptionsOnly);

	CRegKey key;
	if (ERROR_SUCCESS == key.Open (HKEY_CURRENT_USER, 
		L"Software\\FreeDownloadManager.ORG\\Free Download Manager",
		KEY_READ))
	{
		DWORD val = 0;
		key.QueryDWORDValue (L"DumpWithFullMemory", val);
		if (val)
			flags |= vmsCrashCatcher::DumpWithFullMemory;
	}

	g_crashReporter->InitializeCrashCatcher (flags);
}

void makeSureCrashReporterInitialized (const tstring &crashReportingExeRelPath = _T ("fdm.exe"))
{
	if (!g_crashReporter)
		initializeCrashReporter (crashReportingExeRelPath);
}