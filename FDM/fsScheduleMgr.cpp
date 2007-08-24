/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#include "stdafx.h"
#include "data stretcher.h"
#include "fsScheduleMgr.h"
#include "DownloadsWnd.h"
#include "fsPluginMgr.h"
#include "DialDlg.h"
#include "fsRASMgr.h"
#include "ShedulerWnd.h"
#include "WaitForConfirmationDlg.h"
#include "SpiderWnd.h"
#include "MainFrm.h"

extern CSpiderWnd *_pwndSpider;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDownloadsWnd *_pwndDownloads;
extern fsPluginMgr _PluginMgr;
extern CShedulerWnd* _pwndScheduler;        

fsScheduleMgr::fsScheduleMgr()
{
	m_bNeedExit = FALSE;

	m_pfnEvents = NULL;
	m_pfnEventDesc = NULL;

	m_uID = 0;
}

fsScheduleMgr::~fsScheduleMgr()
{
	FreeTasks ();
}

DWORD WINAPI fsScheduleMgr::_threadScheduleMgr(LPVOID lp)
{
	fsScheduleMgr *pThis = (fsScheduleMgr*) lp;

	while (pThis->m_bNeedExit == FALSE)
	{
		pThis->ManageNotEvents ();
		pThis->ManageHalfEvents ();
		
		int iSleep = 1000;
		while (iSleep && pThis->m_bNeedExit == FALSE)
		{
			iSleep -= 200;
			Sleep (200);
		}
	}

	LOG ("threadScheduleMgr shutted down" << nl);

	pThis->m_bNeedExit = FALSE;

	return 0;
}

void fsScheduleMgr::ManageNotEvents()
{
	UpdateCurrentTime ();

	
	fs::list <fsSchedule*> vTasks; 

	for (int i = m_vTasks.size () - 1; i >= 0; i--)
	{
		fsSchedule *task = m_vTasks [i];

		if ((task->dwFlags & SCHEDULE_ENABLED) == 0)
			continue;

		if (IsTimeBased (task))
		{
			if (IsNeedToStart (task))
			{
				
				

				if (task->wts.enType == WTS_SHUTDOWN)
					vTasks.insert (0, task);  
				else if (task->wts.enType == WTS_EXIT)
					vTasks.add (task);		
				else
					StartTask (task);
			}
		}
	}

	
	for (i = 0; i < vTasks.size (); i++)
		StartTask (vTasks [i]);
}

BOOL fsScheduleMgr::IsNeedToStart(fsSchedule *task)
{
	SYSTEMTIME time1, time2;

	FileTimeToSystemTime (&m_curTime, &time1);
	FileTimeToSystemTime (&task->hts.next, &time2);

	
	if (time1.wYear == time2.wYear && time1.wMonth == time2.wMonth &&
		time1.wDay  == time2.wDay  && time1.wHour  == time2.wHour  &&
		time1.wMinute == time2.wMinute)
	{
		
		
		if (CompareFileTime (&task->hts.last, &task->hts.next))
			return TRUE;	
	}

	return FALSE;	
}

void fsScheduleMgr::StartTask(fsSchedule *task)
{
	if ((task->dwFlags & SCHEDULE_ENABLED) == 0)
		return;

	if (IsTimeBased (task))
	{
		task->hts.last = task->hts.next;
		
		GetNextTime (task);
	}
	else
		task->hts.last = m_curTime;

	if (task->wts.enType == WTS_HANGUP)
	{
		fsRASMgr mgr;

		if (FALSE == mgr.EnumConnections ())
			return;

		if (mgr.GetConnectionCount () == 0)
			return; 
	}

	
	OnTaskUpdated (task);

	if (task->uWaitForConfirmation)
	{
		

		CWaitForConfirmationDlg dlg;
		CString strMsg;
		CString strTask;

		strTask = WTSToStr (task);
		strMsg.Format (LS (L_STARTIT), strTask);
		dlg.Init (strMsg, task->uWaitForConfirmation);

  		if (_DlgMgr.DoModal (&dlg) != IDOK)
			return;
	}

	
	Event (task, SME_TASK_STARTED);

	switch (task->wts.enType)
	{
		case WTS_PROGRAM:
			LaunchProgram (task);
		break;

		case WTS_STARTDOWNLOAD:
			StartDownloads (task);
		break;

		case WTS_STOPDOWNLOAD:
			StopDownloads (task);
		break;

		case WTS_STARTALLDOWNLOADS:
			Event (LS (L_STARTALLDLDS));
			_DldsMgr.StartAllDownloads (TRUE);
		break;

		case WTS_STOPALLDOWNLOADS:
			Event (LS (L_STOPALLDLDS));
			_DldsMgr.StopAllDownloads (TRUE);
		break;

		case WTS_DIAL:
			Dial (task);
		break;

		case WTS_HANGUP:
			Hangup (task);
		break;
	
		case WTS_CHANGE_TUM:
		{
			LPCSTR ppszTUM [] = {LS (L_LIGHT), LS (L_MEDIUM), LS (L_HEAVY)};
			CString str;
			str.Format (LS (L_CHANGINGTUM), ppszTUM [task->wts.enTUM]);
			Event (str);
			_pwndDownloads->SetTUM (task->wts.enTUM);
		}
		break;

		case WTS_RESTRAINALLDLDS:
		{
			Event (WTSToStr (task));
			BOOL bEn = fsInternetDownloader::is_PauseMode ();
			BOOL bShould;
			if (task->wts.enRAD == RAD_ENABLE)
				bShould = TRUE;
			else if (task->wts.enRAD == RAD_DISABLE)
				bShould = FALSE;
			else
				bShould = !bEn;
			if (!bEn != !bShould)
				fsInternetDownloader::set_PauseMode (bShould);
		}
		break;

		case WTS_EXIT:
			_DlgMgr.EndAllDialogs ();
			Event (LS (L_EXIT));
			AfxGetApp ()->m_pMainWnd->PostMessage (WM_COMMAND, ID_NEED_EXIT);
		break;

		case WTS_SHUTDOWN:
			_DlgMgr.EndAllDialogs ();
			Shutdown (task);
		break;
	}

	if (task->dwFlags & SCHEDULE_AUTODEL)
		DeleteTask (task);
	else if (task->dwFlags & SCHEDULE_AUTODIS)
	{
		task->dwFlags &= ~ SCHEDULE_ENABLED;
		OnTaskUpdated (task);
	}
}

BOOL fsScheduleMgr::IsTimeUnknown(FILETIME &time)
{
	return time.dwHighDateTime == UINT_MAX && time.dwLowDateTime == UINT_MAX;
}

void fsScheduleMgr::RepairNextTime(fsSchedule *task)
{
	SYSTEMTIME time;
	SYSTEMTIME time2;

	if (IsTimeBased (task) == FALSE || task->hts.enType == HTS_ONCE)
		return;

	
	if (CompareFileTime (&m_curTime, &task->hts.next) != 1)
		return; 

	
	
	
	if (CompareFileTime (&task->hts.next, &task->hts.last) == 0)
		return;	
	
	

	FileTimeToSystemTime (&m_curTime, &time);
	FileTimeToSystemTime (&task->hts.next, &time2);

	
	
	if (task->hts.enType == HTS_DAILY || 
		(task->hts.enType == HTS_CONTINUOUSLY && task->hts.interval >= 24*60) )
	{
		
		
		time.wHour = time2.wHour;
		time.wMinute = time2.wMinute;
		time.wSecond = time2.wSecond;
	}

	
	SystemTimeToFileTime (&time, &task->hts.next);

	
	
	GetNextTime (task);
}

void fsScheduleMgr::GetNextTime(fsSchedule *task)
{
	if (IsTimeBased (task) == FALSE)
		return;
	
	
	ULARGE_INTEGER next;

	next.LowPart  = task->hts.next.dwLowDateTime;
	next.HighPart = task->hts.next.dwHighDateTime;

	
	switch (task->hts.enType)
	{
		case HTS_DAILY:
		{
			SYSTEMTIME time;
			FileTimeToSystemTime (&m_curTime, &time);

			

			for (int i = time.wDayOfWeek+1; TRUE; i++)
			{
				next.QuadPart += (ULONGLONG) 24 * 60 * 60 * 10000000;

				if (i == 7)
					i = 0;

				if (task->hts.aWeeklyDays [i])	
					break;
			}
		}
		break;

		case HTS_CONTINUOUSLY:	
			
			next.QuadPart += (ULONGLONG) task->hts.interval * 60 * 10000000;
			break;
	}

	

	task->hts.next.dwLowDateTime = next.LowPart;
	task->hts.next.dwHighDateTime = next.HighPart;
}

BOOL fsScheduleMgr::IsTimeBased(fsSchedule *task)
{
	
	return task->hts.enType == HTS_ONCE || task->hts.enType == HTS_DAILY || task->hts.enType == HTS_CONTINUOUSLY;
}

void fsScheduleMgr::CalculateStartTime(fsSchedule *task)
{
	if (!IsTimeBased (task) || task->hts.enType == HTS_ONCE)
		return;

	SYSTEMTIME timeNow, timeStart, time;
	GetLocalTime (&timeNow);

	timeStart = timeNow;

	FileTimeToSystemTime (&task->hts.next, &time);

	
	timeStart.wHour   = time.wHour;
	timeStart.wMinute = time.wMinute;
	timeStart.wSecond = time.wSecond;

	
	SystemTimeToFileTime (&timeStart, &task->hts.next);

	

	switch (task->hts.enType)
	{
		case HTS_DAILY:
			int iStart, i;

			
			if (CompareFileTime (&m_curTime, &task->hts.next) == -1)
				iStart = timeStart.wDayOfWeek;	
			else
			{
				
				
				iStart = timeStart.wDayOfWeek+1;
				((ULARGE_INTEGER*) &task->hts.next)->QuadPart += (ULONGLONG) 24 * 60 * 60 * 10000000;
			}

			
			for (i = iStart; TRUE; i++)
			{
				if (i == 7)
					i = 0;

				if (task->hts.aWeeklyDays [i])	
					break;

				((ULARGE_INTEGER*) &task->hts.next)->QuadPart += (ULONGLONG) 24 * 60 * 60 * 10000000;
			}
		break;

		case HTS_CONTINUOUSLY:
			
			if (CompareFileTime (&m_curTime, &task->hts.next) != -1)
				
				((ULARGE_INTEGER*) &task->hts.next)->QuadPart += (ULONGLONG) 24 * 60 * 60 * 10000000;
		break;
	}

	OnTaskUpdated (task);
}

int fsScheduleMgr::AddTask(fsSchedule *task)
{
	task->hts.last.dwHighDateTime =  task->hts.last.dwLowDateTime = UINT (-1);
	fsSchedule *t = new fsSchedule;
	*t = *task;
	m_vTasks.add (t);

	if (task->wts.enType == WTS_STARTDOWNLOAD)
	{
		UpdateTaskDownloads (task);
		_pwndSpider->UpdateTasksIcons ();
	}

	return m_vTasks.size () - 1;
}

CString fsScheduleMgr::WTSToStr(fsSchedule *task)
{
	CString str;

	switch (task->wts.enType)
	{
		case WTS_PROGRAM:
			str = LS (L_START); str += ' ';
			str += task->wts.prog.pszName;
		break;
	
		case WTS_STARTDOWNLOAD:
			str = LS (L_STARTDLDS);
		break;

		case WTS_STOPDOWNLOAD:
			str = LS (L_STOPDLDS);
		break;

		case WTS_STARTALLDOWNLOADS:
			str = LS (L_STARTALLDLDS);
		break;

		case WTS_STOPALLDOWNLOADS:
			str = LS (L_STOPALLDLDS);
		break;

		case WTS_DIAL:
			str = LS (L_DIAL); str += ' ';
			str += task->wts.dial.pszConnection;
		break;

		case WTS_HANGUP:
			str += LS (L_HANGUP); str += ' ';
			if (task->wts.pszHangupConnection)
			{
				str += '"';
				str += task->wts.pszHangupConnection;
				str += '"';
			}
			else
				str += LS (L_ALLCONNS);
		break;

		case WTS_CHANGE_TUM:
		{
			LPCSTR pszTum [] = {LS (L_LIGHT), LS (L_MEDIUM), LS (L_HEAVY)};
			str = LS (L_CHANGETUMTO); str += ' '; str += pszTum [task->wts.enTUM];
		}
		break;

		case WTS_EXIT:
			str = LS (L_EXIT);
		break;

		case WTS_SHUTDOWN:
		{
			LPCSTR pszSh [] = {LS (L_SHUTDOWN), LS (L_RESTARTCOMP), LS (L_LOGOFF), LS (L_HIBERNATE), LS (L_STANDBY)};
			str = pszSh [task->wts.shutdown.enShutdown];
		}
		break;

		case WTS_RESTRAINALLDLDS:
		{
			LPCSTR psz [] = {LS (L_ENABLE), LS (L_DISABLE), LS (L_SWITCH)};
			str = LS (L_PAUSEALLDOWNLOADS);
			str += " ("; str += psz [task->wts.enRAD]; str += ')';
		}
		break;

		default:
			ASSERT (FALSE);
	}

	return str;
}

fsSchedule* fsScheduleMgr::GetTask(int i)
{
	return m_vTasks [i];
}

CString fsScheduleMgr::HTSToStr(fsSchedule *task)
{
	CString str;

	switch (task->hts.enType)
	{
		case HTS_ONCE:
			str = LS (L_ONCE);
		break;

		case HTS_DAILY:
		{
			LPCSTR pszDays [] = {LS (L_SUN), LS (L_MON), LS (L_TUE), LS (L_WED), LS (L_THU), LS (L_FRI), LS (L_SAT)};
			str = LS (L_ONDAYS); str += ' ';
			for (int i = 0; i < 7; i++)
			{
				if (task->hts.aWeeklyDays [i])
				{
					str += '"';
					str += pszDays [i];
					str += '"';
					str += ' ';
				}
			}
		}
		break;

		case HTS_CONTINUOUSLY:
			str = LS (L_EVERY); str += ' ';
			if (task->hts.interval % (24*60) == 0)
			{
				CString s;
				s.Format ("%d %s", task->hts.interval / 24 / 60, LS (L_DAYS));
				str += s;
			}
			else if (task->hts.interval % 60 == 0)
			{
				CString s;
				s.Format ("%d %s", task->hts.interval / 60, LS (L_HOURS));
				str += s;
			}
			else
			{
				CString s;
				s.Format ("%d %s", task->hts.interval, LS (L_MIN_SMALL));
				str += s;
			}
		break;

		case HTS_WHENISTART:
			str = LS (L_ATSTARTUP);
		break;

		case HTS_WHENDIALINGSUCCESS:
			str = LS (L_AFTERDIALOK);
		break;

		case HTS_WHENDIALINGFAILED:
			str = LS (L_DIALWASFAILED);
		break;

		case HTS_WHENNODOWNLOADS:
			str = LS (L_WHENNOACTIVEDLDS);
		break;

		case HTS_WHENSPEEDLOW:
		{
			CString str1;
			str1.Format (LS (L_WHENSPEEDLESS), task->hts.speedLow.uLowSpeed, task->hts.speedLow.uTimeInterval);
			str.Format ("%s %s", str1, LS (L_MIN_SMALL));
		}
		break;

		case HTS_NOTHINGRECEIVED:
			str.Format ("%s %d %s", LS (L_WHENALLDLDSINERR), task->hts.uTimeNothingReceived, LS (L_MIN_SMALL));
		break;

		case HTS_WHENDONE:
			str = LS (L_WHENDONE);
		break;
	}

	return str;
}

CString fsScheduleMgr::NextTimeToStr(fsSchedule *task)
{
	CString str;

	switch (task->hts.enType)
	{
		case HTS_ONCE:
			SYSTEMTIME time;
			FILETIME tm;
			GetLocalTime (&time);
			SystemTimeToFileTime (&time, &tm);
			if (CompareFileTime (&tm, &task->hts.next) >= 0)
			{
				str = LS (L_NEVER);
				break;
			}

		case HTS_DAILY:
		case HTS_CONTINUOUSLY:
		{
			SYSTEMTIME time;
			CHAR strTime [1000], strDate [1000];

			FileTimeToSystemTime (&task->hts.next, &time);

			SystemTimeToStr (&time, strDate, strTime, FALSE);

			str += strTime;
			str += " ";
			str += strDate;
		}
		break;

		default:
			str = "";	
	}

	return str;
}

CString fsScheduleMgr::LastTimeToStr(fsSchedule *task)
{
	if (IsTimeUnknown (task->hts.last))
		return LS (L_NEVER);

	SYSTEMTIME time;
	CHAR strTime [1000], strDate [1000];
	CString str;

	FileTimeToSystemTime (&task->hts.last, &time);

	SystemTimeToStr (&time, strDate, strTime, FALSE);

	str += strTime;
	str += " ";
	str += strDate;

	return str;
}

void fsScheduleMgr::SetEventFunc(fntScheduleMgrEvents pfn, LPVOID lp)
{
	m_pfnEvents = pfn;
	m_lpEvents  = lp;
}

void fsScheduleMgr::Event(fsSchedule* task, fsScheduleMgrEvent ev)
{
	if (m_pfnEvents)
		m_pfnEvents (task, ev, m_lpEvents);
}

BOOL fsScheduleMgr::SaveStateToFile(HANDLE hFile)
{
	int cTasks = m_vTasks.size ();

	DWORD dw;

	if (!WriteFile (hFile, &cTasks, sizeof (cTasks), &dw, NULL))
		return FALSE;
	
	for (int i = 0; i < cTasks; i++)
	{
		fsSchedule *task = m_vTasks [i];

		if (!WriteFile (hFile, task, sizeof (fsSchedule), &dw, NULL))
			return FALSE;

		
		switch (task->wts.enType)
		{
			case WTS_PROGRAM:
				if (!fsSaveStrToFile (task->wts.prog.pszName, hFile))
					return FALSE;

				if (!fsSaveStrToFile (task->wts.prog.pszArgs, hFile))
					return FALSE;
			break;

			case WTS_STARTDOWNLOAD:
			case WTS_STOPDOWNLOAD:
				if (!task->wts.pvIDs->save (hFile))
					return FALSE;
			break;

			case WTS_DIAL:
				if (!fsSaveStrToFile (task->wts.dial.pszConnection, hFile))
					return FALSE;
			break;

			case WTS_HANGUP:
				if (!fsSaveStrToFile (task->wts.pszHangupConnection, hFile))
					return FALSE;
			break;
		}
	}

	return TRUE;
}

BOOL fsScheduleMgr::LoadStateFromFile(HANDLE hFile)
{
	FreeTasks ();
	m_uID = 0;

	int cTasks;

	DWORD dw;

	if (!ReadFile (hFile, &cTasks, sizeof (cTasks), &dw, NULL) || dw != sizeof (cTasks))
		return FALSE;

	UpdateCurrentTime ();

	while (cTasks--)
	{
		fsSchedule *task = new fsSchedule;

		if (!ReadFile (hFile, task, sizeof (fsSchedule), &dw, NULL) || dw != sizeof (fsSchedule))
			return FALSE;

		switch (task->wts.enType)
		{
			case WTS_PROGRAM:
				if (!fsReadStrFromFile (&task->wts.prog.pszName, hFile))
					return FALSE;

				if (!fsReadStrFromFile (&task->wts.prog.pszArgs, hFile))
					return FALSE;
			break;

			case WTS_STARTDOWNLOAD:
			case WTS_STOPDOWNLOAD:
				fsnew1 (task->wts.pvIDs, fs::list <UINT>);
				if (!task->wts.pvIDs->load (hFile))
					return FALSE;
			break;

			case WTS_DIAL:
				if (!fsReadStrFromFile (&task->wts.dial.pszConnection, hFile))
					return FALSE;
			break;

			case WTS_HANGUP:
				if (!fsReadStrFromFile (&task->wts.pszHangupConnection, hFile))
					return FALSE;
			break;
		}

		RepairNextTime (task);
		m_vTasks.add (task);
	}

	return TRUE;
}

void fsScheduleMgr::FreeTask(fsSchedule *task)
{
	switch (task->wts.enType)
	{
		case WTS_PROGRAM:
			SAFE_DELETE_ARRAY (task->wts.prog.pszName);
			SAFE_DELETE_ARRAY (task->wts.prog.pszArgs);
		break;

		case WTS_STARTDOWNLOAD:
		case WTS_STOPDOWNLOAD:
			SAFE_DELETE (task->wts.pvIDs);
		break;

		case WTS_DIAL:
			SAFE_DELETE_ARRAY (task->wts.dial.pszConnection);
		break;

		case WTS_HANGUP:
			SAFE_DELETE_ARRAY (task->wts.pszHangupConnection);
		break;
	}
}

void fsScheduleMgr::FreeTasks()
{
	for (int i = m_vTasks.size () - 1; i >= 0; i--)
	{
		FreeTask (m_vTasks [i]);
		delete m_vTasks [i];
	}

	m_vTasks.clear ();
}

int fsScheduleMgr::GetTaskCount()
{
	return m_vTasks.size ();
}

void fsScheduleMgr::DeleteTask(fsSchedule *task)
{
	int index = FindTask (task);

	if (index == -1)
		return;

	Event (task, SME_TASK_WILLBEDELETED);
	m_vTasks.del (index);

	if (task->wts.enType == WTS_STARTDOWNLOAD)
		UpdateTaskDownloads (task);
}

int fsScheduleMgr::FindTask(fsSchedule *task)
{
	for (int i = m_vTasks.size () - 1; i >= 0; i--)
		if (task == m_vTasks [i])
			return i;

	return -1;
}

void fsScheduleMgr::StartDownloads(fsSchedule *task)
{
	DLDS_LIST vDldsToStart;

	for (int i = task->wts.pvIDs->size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownloadByID (task->wts.pvIDs->at (i));

		if (dld != NULL && dld->pMgr->IsRunning () == FALSE)
		{
			BOOL bDone = dld->pMgr->IsDone ();
			BOOL bStart = TRUE;

			if (bDone)
			{
				if (task->hts.enType == HTS_DAILY || task->hts.enType == HTS_CONTINUOUSLY)
					dld->pMgr->SetToRestartState ();
				else
					bStart = FALSE;
			}

			if (bStart)
			{	
				CString str;

				str.Format ("%s \"%s\"", bDone ? LS (L_RESTART) : LS (L_START), dld->pMgr->get_URL ());
				
				Event (str);

				vDldsToStart.push_back (dld);
			}
		}
		else if (dld == NULL)
		{
			task->wts.pvIDs->del (i);
		}
	}

	_DldsMgr.StartDownloads (vDldsToStart);
}

void fsScheduleMgr::StopDownloads(fsSchedule *task)
{
	DLDS_LIST vDlds;

	for (int i = task->wts.pvIDs->size () - 1; i >= 0; i--)
	{
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownloadByID (task->wts.pvIDs->at (i));

		if (dld != NULL && dld->pMgr->IsRunning ())
		{		
			CString str;
			str.Format ("%s \"%s\"", LS (L_STOP), dld->pMgr->get_URL ());
			Event (str);

			vDlds.push_back (dld);
		}
		else if (dld == NULL)
		{
			task->wts.pvIDs->del (i);
		}
	}

	_DldsMgr.StopDownloads (vDlds);
}

void fsScheduleMgr::Dial(fsSchedule *task)
{
	DWORD dw;
	CloseHandle (CreateThread (NULL, 0, _threadDial, task, 0, &dw));
}

void fsScheduleMgr::Hangup(fsSchedule *task)
{
	fsRASMgr mgr;

	if (FALSE == mgr.EnumConnections ())
		return;

	for (int i = mgr.GetConnectionCount () - 1; i >= 0; i--)
	{
		LPRASCONN conn = mgr.GetConnection (i);
		char szEv [1000];
		sprintf (szEv, "%s \"%s\"", LS (L_HANGUP), conn->szEntryName);

		if (task->wts.pszHangupConnection)
		{
			if (stricmp (task->wts.pszHangupConnection, conn->szEntryName) == 0)
			{
				Event (szEv);
				fsRasHangUp (conn->hrasconn);
				break;
			}
		}
		else
		{
			Event (szEv);
			fsRasHangUp (conn->hrasconn);
		}
	}
}

void fsScheduleMgr::Shutdown(fsSchedule *task)
{
	HANDLE hProc = GetCurrentProcess ();
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	
	OpenProcessToken (hProc, TOKEN_ADJUST_PRIVILEGES, &hToken);

	LookupPrivilegeValue (NULL, SE_SHUTDOWN_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges [0].Luid = luid;
	tp.Privileges [0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges (hToken, FALSE, &tp, 0, NULL, 0);
	

	UINT uFlags = 0;
	switch (task->wts.shutdown.enShutdown)
	{
		case ST_SHUTDOWN:
			uFlags = EWX_POWEROFF;
		break;

		case ST_RESTART:
			uFlags = EWX_REBOOT;
		break;

		case ST_LOGOFF:
			uFlags = EWX_LOGOFF;
		break;

		case ST_HIBERNATE:
			uFlags = 1 << 16;
		break;

		case ST_STANDBY:
			uFlags = 1 << 17;
		break;
	}

	if (task->wts.shutdown.bForce)
		uFlags |= EWX_FORCE;

	LPCSTR szEv [] = { LS (L_SHUTDOWN), LS (L_RESTARTCOMP), LS (L_LOGOFF), 
		LS (L_HIBERNATE), LS (L_STANDBY) };
	Event (szEv [task->wts.shutdown.enShutdown]);

	DWORD dw;
	CloseHandle (CreateThread (NULL, 0, _threadShutdown, (LPVOID) uFlags, 0, &dw));
}

DWORD WINAPI fsScheduleMgr::_threadDial(LPVOID lp)
{
	fsSchedule *task = (fsSchedule*) lp;

	fsDialInfo dial = task->wts.dial;

	fsnew (dial.pszConnection, char, strlen (dial.pszConnection) + 1);
	strcpy (dial.pszConnection, task->wts.dial.pszConnection);

	CDialDlg dlg (NULL);

	dlg.Init (&dial);

	fsScheduleMgr *pThis = _pwndScheduler->GetMgr ();

	CString str = LS (L_DIAL); str += " \"";
	str += dial.pszConnection;
	str += '"';

	pThis->Event (str, SMET_INPROGRESS);

    _DlgMgr.OnDoModal (&dlg);

	if (IDOK == dlg.DoModal ())
	{
		
		str.Format (LS (L_SUCCCONNECTED), dial.pszConnection);
		pThis->Event (str, SMET_S);
	}
	else
	{
		str.Format (LS (L_FAILEDCONNECTTO), dial.pszConnection);
		pThis->Event (str, SMET_E);
	}

    _DlgMgr.OnEndDialog (&dlg);

	delete [] dial.pszConnection;

	return 0;
}

void fsScheduleMgr::SetEventDescFunc(fntScheduleMgrEventDesc pfn, LPVOID lp)
{
	m_pfnEventDesc = pfn;
	m_lpEventDesc = lp;
}

void fsScheduleMgr::Event(LPCSTR pszEvent, fsScheduleMgrEventType enType)
{
	if (m_pfnEventDesc)
		m_pfnEventDesc (pszEvent, enType, m_lpEventDesc);
}

void fsScheduleMgr::LaunchProgram(fsSchedule *task)
{
	DWORD dwRet = (DWORD) ShellExecute (NULL, "open", task->wts.prog.pszName, task->wts.prog.pszArgs, 
										NULL, SW_SHOW);
	if (dwRet > 32)
	{
		

		CString str = '"';
		str += task->wts.prog.pszName;
		str += "\" ";
		str += LS (L_LAUNCHEDSUCC);
		Event (str);
	}
	else
	{
		

		CString str;
		char szErr [200];
		SetLastError (dwRet);
		if (dwRet)
			fsErrorToStr (szErr, sizeof (szErr));
		else
			strcpy (szErr, LS (L_OUTOFMEMORY));
		str.Format ("%s %s [%s]", LS (L_CANTLAUNCH), task->wts.prog.pszName, szErr); 
		Event (str, SMET_E);
	}
}

void fsScheduleMgr::ExciteEvent(fsExternalEvent ev)
{
	
	fsWhenToStartType hts = ExternalEventToHTS (ev);

	if (ev == EV_STARTUP)
	{
		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadScheduleMgr, this, 0, &dw));
	}
	
	
	fs::list <fsSchedule*> vTasks; 

	for (int i = 0; i < m_vTasks.size (); i++)
	{
		fsSchedule *task = m_vTasks [i];

		if ((task->dwFlags & SCHEDULE_ENABLED) == 0)
			continue;

		fsWhenToStartType htsTask = task->hts.enType;
		if (htsTask == HTS_WHENDONE)
			htsTask = HTS_WHENNODOWNLOADS;

		if (hts == htsTask)
		{
			

			if (ev == EV_STARTUP &&
				(task->wts.enType == WTS_SHUTDOWN || task->wts.enType == WTS_EXIT))
			{
				MessageBox (0, LS (L_SCHEDULERESTOEXIT), LS (L_SCHEDULER), MB_ICONEXCLAMATION);
				task->dwFlags &= ~ SCHEDULE_ENABLED;
				continue;
			}

			if (task->wts.enType == WTS_SHUTDOWN)
				vTasks.insert (0, task);  
			else if (task->wts.enType == WTS_EXIT)
				vTasks.add (task);		
			else
				StartTask (task);
		}
	}

	for (i = 0; i < vTasks.size (); i++)
		StartTask (vTasks [i]);
}

fsWhenToStartType fsScheduleMgr::ExternalEventToHTS(fsExternalEvent ev)
{
	switch (ev)
	{
		case EV_STARTUP:
			return HTS_WHENISTART;

		case EV_DIALINGSUCCESS:
			return HTS_WHENDIALINGSUCCESS;

		case EV_DIALINGFAILED:
			return HTS_WHENDIALINGFAILED;

		case EV_NODOWNLOADS:
			return HTS_WHENNODOWNLOADS;

		default:
			ASSERT (FALSE);
			return HTS_ONCE;
	}
}

void fsScheduleMgr::ManageHalfEvents()
{
	fs::list <fsSchedule*> vTasks;

	
	
	if (_DldsMgr.IsRunning () == FALSE)
		return;

	for (int i = m_vTasks.size () - 1; i >= 0; i--)
	{
		fsSchedule *task = m_vTasks [i];

		if ((task->dwFlags & SCHEDULE_ENABLED) == 0)
			continue;

		UINT uSpeed = task->hts.speedLow.uLowSpeed;
		UINT uInterval = task->hts.speedLow.uTimeInterval;

		switch (task->hts.enType)
		{
			case HTS_NOTHINGRECEIVED:
			case HTS_WHENDONE:
				uSpeed = 1;	
				uInterval = task->hts.uTimeNothingReceived;
			case HTS_WHENSPEEDLOW:
				
				
				if (IsTimeUnknown (task->hts.last) || 
					 fsGetTimeDelta (&m_curTime, &task->hts.last) >= uInterval*60)
				{
					
					if (_DldsMgr.IsSummarySpeedWasLessThan (uSpeed+1, uInterval*60))
					{
						

						switch (task->wts.enType)
						{
							case WTS_EXIT:
								vTasks.add (task);
								break;

							case WTS_SHUTDOWN:
								vTasks.insert (0, task);
								break;

							default:
								StartTask (task);
						}
					}
				}
		}
	}

	for (i = 0; i < vTasks.size (); i++)
		StartTask (vTasks [i]);
}

void fsScheduleMgr::Stop()
{
	m_bNeedExit = TRUE;
	while (m_bNeedExit)
		Sleep (5);
}

DWORD WINAPI fsScheduleMgr::_threadShutdown(LPVOID uFlags)
{
	Sleep (1000);

	if ((DWORD)uFlags & ((1 << 16) | (1 << 17)))
	{
		BOOL bHibernate = ((DWORD)uFlags & (1 << 16)) != 0;
		vmsSetSuspendState (bHibernate, ((DWORD)uFlags & EWX_FORCE) != 0, FALSE);
		return 0;
	}

	LOG ("Shutting down computer..." << nl);
	LOG ("goto OnAppExit..." << nl);

	_PluginMgr.OnAppExit (FALSE);

	LOG ("saving history..." << nl);

	((CDataStretcherApp*)AfxGetApp ())->SaveSettings ();
	((CDataStretcherApp*)AfxGetApp ())->SaveHistory ();

	LOG ("saving state..." << nl);

	((CMainFrame*) AfxGetApp ()->m_pMainWnd)->SaveState ();

	
	

	LOG ("performing shutdown..." << nl);

	ExitWindowsEx ((UINT) uFlags, 0);

	LOG ("did shutdown" << nl);

	

	return 0;
}

BOOL fsScheduleMgr::IsDownloadScheduled(vmsDownloadSmartPtr dld)
{
	for (int i = m_vTasks.size () - 1; i >= 0; i--)
	{
		fsSchedule *task = m_vTasks [i];
		
		if (task->wts.enType == WTS_STARTDOWNLOAD && (task->dwFlags & SCHEDULE_ENABLED))
		{
			
			for (int j = task->wts.pvIDs->size () - 1; j >= 0; j--)
			{
				if (task->wts.pvIDs->at (j) == dld->nID)
				{
					

					if (task->hts.enType == HTS_ONCE)
					{
						
						if (IsTimeUnknown (task->hts.next) == FALSE && CompareFileTime (&task->hts.next, &m_curTime) == 1)
							return TRUE;	
					}
					else
						return TRUE;	
				}
			}
		}
	}

	return FALSE;
}

fsSchedule* fsScheduleMgr::GetScheduleDLTask(DLDS_LIST &vDlds, BOOL bStartDL)
{
	fsWhatToStartType enWTS = bStartDL ? WTS_STARTDOWNLOAD : WTS_STOPDOWNLOAD;

	int cDlds = vDlds.size ();
	ASSERT (cDlds);

	for (int i = m_vTasks.size () - 1; i >= 0; i--)
	{
		fsSchedule *task = m_vTasks [i];
		if (task->wts.enType == enWTS)
		{
			if (task->wts.pvIDs->size () == cDlds)
			{
				for (int j = 0; j < cDlds; j++)  
				{
					for (int k = 0; k < cDlds; k++)   
						if (vDlds [j]->nID == task->wts.pvIDs->at (k))
							break;

					if (k == cDlds) 
						break;
				}

				if (j == cDlds) 
					return task;
			}
		}
	}

	return NULL;
}

void fsScheduleMgr::UpdateCurrentTime()
{
	SYSTEMTIME time;
	GetLocalTime (&time);
	SystemTimeToFileTime (&time, &m_curTime);
}

int fsScheduleMgr::FindTask(fsWhatToStartType enType, int iStartPos)
{
	for (int i = iStartPos+1; i < m_vTasks.size (); i++)
	{
		if (m_vTasks [i]->wts.enType == enType)
			return i;
	}

	return -1;
}

void fsScheduleMgr::UpdateTaskDownloads(fsSchedule *task)
{
	DLDS_LIST vDlds;

	for (int j = 0; j < task->wts.pvIDs->size ();  j++)
	{
		vmsDownloadSmartPtr dld = _DldsMgr.GetDownloadByID (task->wts.pvIDs->at (j));
		if (dld)
			_pwndDownloads->UpdateDownload (dld);
	}
}

void fsScheduleMgr::OnTaskUpdated(fsSchedule *task)
{
	if (task->wts.enType == WTS_STARTDOWNLOAD)
	{
		UpdateTaskDownloads (task);
		_pwndSpider->UpdateTasksIcons ();
	}

	Event (task, SME_TASK_UPDATED);
}

void fsScheduleMgr::RepairTasksNextTimes()
{
	for (int i = 0; i < m_vTasks.size (); i++)
		RepairNextTime (m_vTasks [i]);
}
