/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "UpdateHlpr.h"

void turnOffAutoUpdates ()
{
	const auto key_name = L"Software\\FreeDownloadManager.ORG\\Free Download Manager\\Settings\\Update";
	CRegKey key;
	if (ERROR_SUCCESS != key.Open (HKEY_CURRENT_USER, key_name) &&
		ERROR_SUCCESS != key.Create (HKEY_CURRENT_USER, key_name, nullptr, 0, KEY_WRITE))
	{
		return;
	}

	key.SetDWORDValue (L"AutoUpdateType", 2); 
}

void onTooOldVersion ()
{
	turnOffAutoUpdates ();

	MessageBox (nullptr, 
		L"OS you use is no longer supported by newer versions of FDM.\n\
Minimum required OS version is Windows XP Service Pack 3.\n\
Automatic updates for FDM has been disabled.",
		L"Update failed", MB_ICONERROR | MB_SETFOREGROUND);
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	vmsWinOsVersion osver;

	if (!osver.isVistaOrHigher ())
	{
		if (osver.getRawVersion ()->wServicePackMajor < 3)
		{
			onTooOldVersion ();
			return 2;
		}
	}

	vmsExeProgramData exedata (
		vmsExeProgramData::construct_label ("{2F06C730-4E17-42C9-8CB6-982BBA6A1F5F}"));

	assert (!exedata.m_data.empty ());
	if (exedata.m_data.empty ())
		return 1;

	WCHAR wsz [MAX_PATH] = L"";
	GetTempPath (MAX_PATH, wsz);
	_tcscat (wsz, L"\\fdminst.exe");
	HANDLE file = CreateFile (wsz, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		nullptr);
	assert (file != INVALID_HANDLE_VALUE);
	if (file == INVALID_HANDLE_VALUE)
		return 3;
	DWORD dw;
	WriteFile (file, &exedata.m_data.front (), (DWORD)exedata.m_data.size (), &dw, nullptr);
	CloseHandle (file);

	vmsCommandLine cl (wsz, lpCmdLine);
	if (!cl.Execute (vmsCommandLine::RunElevatedIfRequired | vmsCommandLine::WaitForCompletion))
		return 4;

	return 0;
}
