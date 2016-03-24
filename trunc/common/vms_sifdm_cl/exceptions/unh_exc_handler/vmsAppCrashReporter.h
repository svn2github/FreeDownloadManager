/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsCrashCatcher.h"
#include "vmsCrashReporter.h"
#include "DlgUnhandledException.h"
class vmsAppCrashReporter :
	protected vmsCrashReporter,
	public vmsCrashCatcher
{
public:
	enum 
	{
		DontTerminateProcess	= 1,
		NoRestartAppOption		= 1 << 1,
	};

public:
	vmsAppCrashReporter(void) {}

	vmsAppCrashReporter (
		const std::wstring& wstrAppName, 
		const std::wstring &wstrAppPartName,
		const std::wstring& wstrAppVersion,
		const std::wstring& targetHost, const std::wstring& targetPath) :
		m_wstrAppName (wstrAppName), 
		m_wstrAppPartName (wstrAppPartName),
		m_wstrAppVersion (wstrAppVersion),
		m_targetHost (targetHost),
		m_targetPath (targetPath) 
	{
	}

	~vmsAppCrashReporter(void) 
	{
	}

protected:
	enum
	{
		SubmitReportFlagsMask = NoRestartAppOption
	};

	uint32_t m_flags = 0;

protected:
	virtual void onCrashDumpCreated () override
	{
		TCHAR tsz [1000];
		vmsCommandLine cl;
		cl.setExe (!m_tstrCrashReportingExe.empty () ? 
			m_tstrCrashReportingExe.c_str () : vmsGetModuleFileName ().c_str ());
		tstring tstrArgs = _T ("-submitDump=\"");
		tstrArgs += m_tstrDumpFile;
		tstrArgs += _T ("\" -faultModule=\"");
		tstrArgs += m_tstrFaultModuleName;
		if (m_tstrFaultModuleName.empty ())
		{
			tstrArgs += _T ("\" -gettingFaultModuleNameError=\"");
			tstrArgs += _itot (m_dwGettingFaultModuleNameError, tsz, 10);
		}
		tstrArgs += _T ("\" -faultModuleCrashAddr=");
		_stprintf_s <_countof (tsz)> (tsz, _T ("%I64x"), (UINT64)m_dwpFaultModuleCrashAddress);
		tstrArgs += tsz;
		if (!m_tstrAppRestartExe.empty ())
		{
			tstrArgs += _T (" -appRestartExe=\"");
			tstrArgs += m_tstrAppRestartExe;
			tstrArgs += _T ("\"");
		}
		if (!m_tstrAppRestartArgs.empty ())
		{
			tstrArgs += _T (" -appRestartArgs=\"");
			tstrArgs += m_tstrAppRestartArgs;
			tstrArgs += _T ("\"");
		}
		tstrArgs += _T (" -appName=\""); 
		tstrArgs += m_wstrAppName;
		tstrArgs += L"\"";
		tstrArgs += _T (" -appPartName=\"");
		tstrArgs += m_wstrAppPartName;
		tstrArgs += L"\"";
		tstrArgs += _T (" -appVersion=\"");
		tstrArgs += m_wstrAppVersion;
		tstrArgs += L"\"";
		auto flags = m_flags & SubmitReportFlagsMask;
		tstrArgs += L" -flags=";
		tstrArgs += _ultow (flags, tsz, 10);
		cl.setArgs (tstrArgs.c_str ());
		cl.Execute ();
		
		if (!(m_flags & DontTerminateProcess))
			TerminateProcess (GetCurrentProcess (), 1);
		else
			vmsCrashCatcher::Stop (); 
	}

public:
	bool CheckIfSubmitDumpIsRequestedByCommandLine(bool& bContinue)
	{
		vmsCommandLineParser cp;

		const vmsCommandLineParser::Argument *pArgDump = cp.findArgument (_T ("submitDump"));
		if (!pArgDump)
			return false;

		const vmsCommandLineParser::Argument *pArgFaultModule = cp.findArgument (_T ("faultModule"));
		const vmsCommandLineParser::Argument *pArgGettingFaultModuleNameError = cp.findArgument (_T ("gettingFaultModuleNameError"));
		const vmsCommandLineParser::Argument *pArgFaultModuleCrashAddr = cp.findArgument (_T ("faultModuleCrashAddr"));
		const vmsCommandLineParser::Argument *pArgAppRestartExe = cp.findArgument (_T ("appRestartExe"));
		const vmsCommandLineParser::Argument *pArgAppRestartArgs = cp.findArgument (_T ("appRestartArgs"));
		const vmsCommandLineParser::Argument *pAppName = cp.findArgument (_T ("appName"));
		const vmsCommandLineParser::Argument *pAppPartName = cp.findArgument (_T ("appPartName"));
		const vmsCommandLineParser::Argument *pAppVersion = cp.findArgument (_T ("appVersion"));
		const vmsCommandLineParser::Argument *pFlags = cp.findArgument (_T ("flags"));

		auto appName = pAppName && !pAppName->second.empty () ? pAppName->second : m_wstrAppName;
		auto appPartName = pAppPartName && !pAppPartName->second.empty () ? pAppPartName->second : m_wstrAppPartName;
		auto appVersion = pAppVersion && !pAppVersion->second.empty () ? pAppVersion->second : m_wstrAppVersion;

		if (pArgAppRestartExe)
			setAppRestartExe (pArgAppRestartArgs->second);

		if (pArgAppRestartArgs)
			setAppRestartArgs (pArgAppRestartArgs->second.c_str ());

		if (pFlags)
		{
			uint32_t flags = wcstoul (pFlags->second.c_str (), nullptr, 10);
			m_flags &= ~SubmitReportFlagsMask;
			m_flags |= flags;
		}

		bContinue = true;

		if (GetFileAttributes (pArgDump->second.c_str ()) == DWORD (-1))
			return true;

		bContinue = false;

		CoInitialize (NULL);

		bool bSendDumpToServer, restart_app;
		std::wstring user_description;
		show_on_crashed_ui (appName, appPartName, restart_app, 
			bSendDumpToServer, user_description);

		if (restart_app)
		{
			vmsCommandLine cl;
			cl.setExe (!m_tstrAppRestartExe.empty () ? 
				m_tstrAppRestartExe.c_str () : vmsGetModuleFileName ().c_str ());
			cl.setArgs (m_tstrAppRestartArgs.c_str ());
			cl.Execute ();
		}

		if (bSendDumpToServer)
		{
			std::string strXml;
			UINT64 uCrashAddr = 0;
			if (pArgFaultModuleCrashAddr)
				_stscanf (pArgFaultModuleCrashAddr->second.c_str (), _T ("%I64x"), &uCrashAddr);
			assert (!appName.empty () && !appVersion.empty ());
			vmsCrashReporter::GenerateXml (appName.c_str (), appPartName.c_str (), appVersion.c_str (),
				user_description.c_str (), pArgFaultModule ? pArgFaultModule->second.c_str () : NULL, 
				pArgGettingFaultModuleNameError ? (DWORD)_ttoi (pArgGettingFaultModuleNameError->second.c_str ()) : 0, 
				(DWORD_PTR)uCrashAddr, NULL, strXml);

			SubmitDumpToServer (m_targetHost.c_str (), m_targetPath.c_str (), pArgDump->second.c_str (), strXml.c_str ());
		}

		DeleteFile (pArgDump->second.c_str ());

		CoUninitialize ();

		return true;
	}

	void setCrashReportingExe (const tstring &exe)
	{
		m_tstrCrashReportingExe = exe;
		m_flags |= NoRestartAppOption;
	}

	void setAppRestartExe (const tstring &exe)
	{
		m_tstrAppRestartExe = exe;
		m_flags &= ~NoRestartAppOption;
	}

	void setAppRestartArgs (LPCTSTR ptszArgs) 
	{
		m_tstrAppRestartArgs = ptszArgs;
	}

	void InitializeCrashCatcher (uint32_t crashCatcherFlags = vmsCrashCatcher::LegacyMode)
	{
		vmsCrashCatcher::m_flags = crashCatcherFlags;
		vmsCrashCatcher::Initialize ();
	}

	void setAppNameAndVersion (const std::wstring& wstrAppName, const std::wstring& wstrAppVersion)
	{
		m_wstrAppName = wstrAppName;
		m_wstrAppVersion = wstrAppVersion;
	}

	void setTargetHostAndPath (const std::wstring& targetHost, const std::wstring& targetPath)
	{
		m_targetHost = targetHost;
		m_targetPath = targetPath;
	}

protected:
	tstring m_tstrCrashReportingExe;
	tstring m_tstrAppRestartExe;
	tstring m_tstrAppRestartArgs;
	std::wstring m_wstrAppName;
	std::wstring m_wstrAppPartName;
	std::wstring m_wstrAppVersion;
	std::wstring m_targetHost, m_targetPath;

protected:
	virtual void show_on_crashed_ui (const std::wstring &appName, 
		const std::wstring &appPartName, 
		bool& restart_app, 
		bool& send_crash_report, std::wstring& user_description)
	{
		const bool no_restart = (m_flags & NoRestartAppOption) != 0;
		CDlgUnhandledException dlgUnhExc (appName, appPartName, no_restart);
		send_crash_report = IDOK == dlgUnhExc.DoModal ();
		restart_app = no_restart ? false : dlgUnhExc.m_bRestartApp;
		user_description = dlgUnhExc.m_wstrDescription;
	}
};

