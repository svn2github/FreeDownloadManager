/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmCrashReporter.h"
#include "Include.Add/vmsCommandLineParser.h"
#include "vmsCommandLine.h"
#include "DlgUnhExc2.h"
#include "vmsAppMutex.h"

vmsFdmCrashReporter::vmsFdmCrashReporter(void)
{
}

vmsFdmCrashReporter::~vmsFdmCrashReporter(void)
{
}

void vmsFdmCrashReporter::onCrashDumpCreated ()
{
	vmsCommandLine cl;
	TCHAR tsz [MAX_PATH] = _T ("");
	GetModuleFileName (NULL, tsz, _countof (tsz));
	cl.setExe (tsz);
	tstring tstrArgs = _T ("-submitDump=\"");
	tstrArgs += m_tstrDumpFile;
	tstrArgs += '"';
	cl.setArgs (tstrArgs.c_str ());
	cl.Execute ();
	
	TerminateProcess (GetCurrentProcess (), 1);
}

bool vmsFdmCrashReporter::CheckIfSubmitDumpIsRequestedByCommandLine(bool& bContinue)
{
	vmsCommandLineParser cp;

	const vmsCommandLineParser::Argument *pArg = cp.findArgument (_T ("submitDump"));
	if (!pArg)
		return false;

	bContinue = true;

	if (GetFileAttributes (pArg->second.c_str ()) == DWORD (-1))
		return true;

	bContinue = false;

	extern vmsAppMutex _appMutex;
	_appMutex.CloseMutex ();

	CoInitialize (NULL);

	CDlgUnhExc2 dlg;
	UINT nRes = dlg.DoModal ();

	if (dlg.m_bRestartApp)
	{
		vmsCommandLine cl;
		TCHAR tsz [MAX_PATH] = _T ("");
		GetModuleFileName (NULL, tsz, _countof (tsz));
		cl.setExe (tsz);
		cl.Execute ();
	}

	if (nRes == IDOK)
	{
		std::string strXml;
		vmsCrashReporter::GenerateXml (_T ("Free Download Manager"), vmsFdmAppMgr::getVersion ()->m_tstrFileVersion.c_str (), 
			dlg.m_strDescription, NULL, strXml);

		SubmitDumpToServer (_T ("freedownloadmanager.org"), _T ("/dump.php"), pArg->second.c_str (), strXml.c_str ());
	}

	DeleteFile (pArg->second.c_str ());

	CoUninitialize ();

	return true;
}
