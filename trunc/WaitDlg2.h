/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once

class CWaitDlg2 : 
	public CDialog,
	public vmsCreatesThreads,
	public vmsOperationProgressCallback
{
	DECLARE_DYNAMIC(CWaitDlg2)

public:
	
	
	typedef std::function <bool(int)> sleep_function_t;
	
	typedef std::function <void(sleep_function_t, vmsOperationProgressCallback*)> job_function_t;
	
	typedef std::function <void()> cancel_function_t;

public:
	CWaitDlg2(CWnd* pParent, CString message, job_function_t jobfn,
		vmsOperationProgressBase *job_progress,
		bool enable_cancel = false, cancel_function_t cancelfn = cancel_function_t ());   
	virtual ~CWaitDlg2();

	bool cancelledByUser () const
	{
		return m_cancelled;
	}

	enum { IDD = IDD_WAITER2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progressCtrl;
	virtual BOOL OnInitDialog();

protected:
	job_function_t m_jobfn;
	bool m_enableCancel;
	cancel_function_t m_cancelfn;
	CString m_message;
	vmsOperationProgressBase *m_job_progress;
	bool m_cancelled;
	int m_progressByCallback;

protected:
	static unsigned _stdcall _threadJob (vmsCreatesThreads*);
	bool sleep_function (int sleep_interval);

	virtual bool onOperationProgressPercentage (double) override;

public:
	afx_msg void OnBnClickedCancel();

protected:
	enum timer_id {TimerIdUpdateProgress = 1};
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
