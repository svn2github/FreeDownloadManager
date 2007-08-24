/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSDOWNLOADMGR_H__DE09A9F2_AF9D_41C0_A7D5_DEBF2CFDBA03__INCLUDED_)
#define AFX_FSDOWNLOADMGR_H__DE09A9F2_AF9D_41C0_A7D5_DEBF2CFDBA03__INCLUDED_

#include "fsInternetDownloader.h"	
#include "DownloadProperties.h"	
#include "fsTicksMgr.h"	
#include "vmsCriticalSection.h"	

#if _MSC_VER > 1000
#pragma once
#endif   

typedef DWORD fsDownloadState;
#define DS_STOPPED			1	
#define DS_DOWNLOADING		2	
#define DS_NEEDSTART		4	
#define DS_NEEDSTOP			8	
#define DS_DONE				16	
#define DS_NEEDADDSECTION	32	
#define DS_NEEDRESTARTFROM	64	
#define DS_QUERINGSIZE		128	
#define DS_NEEDADDSECTION2	256	
#define DS_RESERVINGSPACE	512	    

enum fsDownloadMgr_ExtError 
{
	DMEE_FILEWASDELETED,	
	DMEE_STOPPEDORDONE,		
	
	
	DMEE_FATALERROR,	
	DMEE_FILEUPDATED,		
	DMEE_USERSTOP,			
	DMEE_STARTING,			
};    

enum fsDownloadMgr_EventDescType
{
	EDT_INQUIRY,			
	EDT_RESPONSE_S,			
	EDT_RESPONSE_E,			
	EDT_DONE,				
	EDT_WARNING,			
	EDT_INQUIRY2,			
	EDT_RESPONSE_S2,		
};  

typedef DWORD (*fntDownloadMgrEventFunc)(class fsDownloadMgr *pMgr, fsDownloaderEvent, UINT, LPVOID);

typedef void (*fntEventDescFunc)(fsDownloadMgr *pMgr, fsDownloadMgr_EventDescType enType, LPCSTR pszDesc, LPVOID lp);  

#define DFF_NEED_INIT_FILE			1
#define DFF_USE_PORTABLE_DRIVE		2

class fsDownloadMgr
{
public:
	fsDownloadState get_State();
	int get_ReservingDiskSpaceProgress();
	void DoRapidshareSupport();
	
	void Reset();
	
	fsString get_URL();
	
	BOOL MoveToFolder (LPCSTR pszFolder);
	BOOL MoveFile (LPCSTR pszNewFileName);
	
	static void set_GlobalOffline(BOOL bOffline);
	static BOOL is_GlobalOffline();
	
	void set_Download (struct fsDownload* dld);
	
	void RemoveHiddenAttribute();
	
	void MeasureMirrorsSpeed();
	
	fsInternetResult SetToRestartState ();
	
	fsInternetResult FindMirrors();
	
	void Set_MirrRecalcSpeedTime(UINT u);
	
	fsInternetResult StartDownloading();
	void StopDownloading();
	
	void CreateOneMoreSection();
	
	BOOL IsCantStart();
	
	void StopQuering();
	
	
	
	fsInternetResult QuerySize(BOOL bCheckPoss = TRUE);
	
	
	
	void QuerySize2 ();
	
	BOOL IsQueringSize();
	
	void StopSection();
	
	
	fsInternetResult RestartDownloading();
	
	BOOL IsSectionCanBeAdded();
	
	void CloseFile();
	
	BOOL IsFileInit();
	
	
	BOOL InitFile (BOOL bCreateOnDisk = FALSE, LPCSTR pszSetExt = NULL);
	
	fsInternetResult GetLastError();
	
	BOOL DeleteFile ();
	
	void ApplyProperties();
	
	BOOL IsRunning();
	
	BOOL IsDone();
	
	
	
	
	
	BOOL SaveState (LPVOID lpBuffer, LPDWORD pdwSize);
	
	
	BOOL LoadState (LPVOID lpBuffer, LPDWORD pdwSize, WORD wVer);
	
	void CloneSettings (fsDownloadMgr* src);
	
	void SetEventDescFunc (fntEventDescFunc pfn, LPVOID lpParam);
	void SetEventFunc (fntDownloadMgrEventFunc pfnEvents, LPVOID lpParam);
	
	void SetOutputFileName (LPCSTR pszName);
	
	
	
	fsInternetResult CreateByUrl (LPCSTR pszUrl, BOOL bAcceptHTMLPathes = FALSE);
	fsInternetDownloader* GetDownloader();
	
	fsDownload_Properties* GetDP();
	
	fsDownload_NetworkProperties* GetDNP();

	fsDownloadMgr (struct fsDownload* dld = NULL);
	virtual ~fsDownloadMgr();

protected:
	int m_iReservingDiskSpaceProgress;
	DWORD m_dwDownloadFileFlags;
	bool m_bRSsupportDone;
	
	void ProcessFilePathMacroses (CString &str);
	
	void InitFile_ProcessMacroses();
        #ifndef FDM_DLDR__RAWCODEONLY
	class vmsMaliciousDownloadChecker *m_mdc;
	
	BOOL CheckIfMalicious ();
	#endif
	
	static vmsCriticalSection m_csRenameFile;
	
	void SetFileTime (HANDLE hFile);
	
	
	
	BOOL m_bRename_CheckIfRenamed;
	
	
	BOOL m_bNeedStartAgain;
	
	
	void AppendCommentToFileName (BOOL bMoveFile = TRUE);
	
	void CheckDstFileExists();
	
	BOOL OnNeedFile_FinalInit();
	static DWORD WINAPI _threadCalcMirrSpeed (LPVOID lp);
	
	
	void CheckMirrSpeedRecalcRequired();
	
	fsTicksMgr m_tikLastMirrMeasureTime;
	
	UINT m_uMirrRecalcSpeedTime;	
	
	
	void AddSection(BOOL bCheckAdm = TRUE);
	BOOL m_bCantStart;	
	static DWORD WINAPI _threadQSize (LPVOID lp);
	BOOL m_bFatalError;	
	int m_iThread;	
	CString m_strExtError;	
	
	void OnDone();
	
	void DescribeAPIError(DWORD* pdwLastError = NULL);
	
	DWORD ProcessSCR (fsSizeChangeReaction scr, BOOL bFirstCall = TRUE);
	
	DWORD OnSCR();
	
	void ApplyAdditionalExt();
	
	BOOL ReserveDiskSpace();
	
	
	BOOL BuildFileName(LPCSTR pszSetExt = NULL);
	
	void Event (LPCSTR pszEvent, fsDownloadMgr_EventDescType enType = EDT_INQUIRY);
	
	DWORD Event (fsDownloaderEvent ev, UINT uInfo);
	
	
	
	BOOL OpenFile(BOOL bFailIfDeleted = TRUE, BOOL bDisableEvents = FALSE);
	
	
	
	
	void RenameFile(BOOL bFormat1 = TRUE);
	
	BOOL OnNeedFile();
	BOOL m_uNeedStartFrom; 
	
	
	void OnSectionStopped();
	
	BOOL SleepInterval ();
	fntEventDescFunc m_pfnEventDesc;	
	LPVOID m_lpEventDescParam;			
	fsTicksMgr m_ticksStart;			
	HANDLE m_hOutFile;					
	
	void StopDownload();
	fsInternetResult m_lastError;	
	
	fsInternetResult StartDownload();
	
	static DWORD _DownloaderEvents (fsDownloaderEvent enEvent, UINT uInfo, LPVOID lp);
	fsDownloadState m_state;	
	BOOL m_bThreadRunning;	
	
	static DWORD WINAPI _threadDownloadMgr (LPVOID lp);
	
	fsInternetResult CreateInternetSession();
	fsDownload_Properties m_dp;		
	fsInternetDownloader m_dldr;	
	fntDownloadMgrEventFunc m_pfnEvents;	
	LPVOID m_lpParamEvents;		
private:
	
	
	
	BOOL ApplyAER (fsAlreadyExistReaction enAER);
	fsDownload* m_dld; 
};

#endif 
