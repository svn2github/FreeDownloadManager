/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsElevatedFdm.h"
#include "DlgElevateRequired.h"

vmsElevatedFdm::vmsElevatedFdm(void)
	: m_bRunning(false)
{
}

vmsElevatedFdm::~vmsElevatedFdm(void)
{
}

bool vmsElevatedFdm::Run(LPCTSTR ptszAdditionalArgs)
{
	if (m_bRunning)
		return false;

	TCHAR tsz [MAX_PATH];
	GetModuleFileName (NULL, tsz, _countof (tsz));
	
	SHELLEXECUTEINFO sei;
	ZeroMemory (&sei, sizeof (sei));
	sei.cbSize = sizeof (sei);
	sei.fMask = SEE_MASK_FLAG_NO_UI;
	sei.lpVerb = _T ("runas");
	sei.lpFile = tsz;
	tstring tstr = _T ("-runelevated");
	if (ptszAdditionalArgs && *ptszAdditionalArgs)
	{
		tstr += ' ';
		tstr += ptszAdditionalArgs;
	}
	sei.lpParameters = tstr.c_str ();
	sei.nShow = SW_HIDE;
	
	if (ShellExecuteEx (&sei))
		m_bRunning = true;

	return m_bRunning;
}

vmsElevatedFdm& vmsElevatedFdm::o(void)
{
	static vmsElevatedFdm _o;
	return _o;
}

bool vmsElevatedFdm::InstallIeIntegration(bool bShowPreUacDlg)
{
	if (m_bRunning)
		return false;

	if (bShowPreUacDlg)
	{
		CDlgElevateRequired dlg;
		dlg.m_strMsg = LS (L_IE_INTEGRATION_REQ_ADMIN);
		if (IDOK != dlg.DoModal ())
			return false;
	}

	return Run (_T ("-ie"));
}
