/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "WaitDlg2.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CWaitDlg2, CDialog)

CWaitDlg2::CWaitDlg2(CWnd* pParent, CString message, job_function_t jobfn,
	vmsOperationProgressBase *job_progress,
	bool enable_cancel, cancel_function_t cancelfn)
	: CDialog(CWaitDlg2::IDD, pParent),
	m_message (message),
	m_jobfn (jobfn),
	m_job_progress (job_progress),
	m_enableCancel (enable_cancel),
	m_cancelfn (cancelfn),
	m_cancelled (false),
	m_progressByCallback (0)

{

}

CWaitDlg2::~CWaitDlg2()
{
	Shutdown (true);
}

void CWaitDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressCtrl);
}

BEGIN_MESSAGE_MAP(CWaitDlg2, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CWaitDlg2::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CWaitDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText (IDC_MESSAGE, m_message);

	GetDlgItem (IDCANCEL)->EnableWindow (m_enableCancel);

	if (true)
	{
		m_progressCtrl.SetRange (0, 100);
		SetTimer (TimerIdUpdateProgress, 500, NULL);
	}
	else
	{
		m_progressCtrl.ShowWindow (SW_HIDE);
	}

	SetWindowText (LS (L_PLEASEWAIT));
	GetDlgItem (IDCANCEL)->SetWindowText (LS (L_CANCEL));

	CreateThread (_threadJob);

	return TRUE;  
	
}

void CWaitDlg2::OnBnClickedCancel()
{
	m_cancelled = true;

	GetDlgItem (IDCANCEL)->EnableWindow (FALSE);
	
	if (m_cancelfn)
		m_cancelfn ();

	
}

void CWaitDlg2::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TimerIdUpdateProgress)
	{
		auto progress = m_job_progress ? (int) m_job_progress->getProgressCurrentInPercentage () : 
			m_progressByCallback;
		if (m_progressCtrl.GetPos () != progress)
			m_progressCtrl.SetPos (progress);
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

unsigned CWaitDlg2::_threadJob (vmsCreatesThreads *pthis2)
{
	CWaitDlg2 *pthis = static_cast <CWaitDlg2*> (pthis2);

	sleep_function_t sleepfn = std::bind (std::mem_fn (&CWaitDlg2::sleep_function), 
		pthis, std::placeholders::_1);
	
	pthis->m_jobfn (sleepfn, pthis);

	pthis->PostMessage (WM_COMMAND, IDOK);

	return 0;
}

bool CWaitDlg2::sleep_function (int sleep_interval)
{
	return WaitForShutdownEvent ((unsigned)sleep_interval) == FALSE;
}

bool CWaitDlg2::onOperationProgressPercentage (double progress)
{
	m_progressByCallback = (int)progress;
	return !m_cancelled;
}