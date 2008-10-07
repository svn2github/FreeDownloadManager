/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#if !defined(AFX_FSDOWNLOADSMGR_H__49230334_5704_46DF_95BA_2F7535E427DE__INCLUDED_)
#define AFX_FSDOWNLOADSMGR_H__49230334_5704_46DF_95BA_2F7535E427DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "list.h"
#include "array.h"
#include "fsDownloadMgr.h"
#include "vmsObject.h"
#include "fsDownloadRegSaver.h"	
#include "vmsFileRecentList.h"

#ifndef FDM_DLDR__RAWCODEONLY
#endif

#ifndef FDM_DLDR__RAWCODEONLY
#include "fsIECatchMgr.h"
#include "fsDownloadsHistoryMgr.h"	
#endif
#include "fsArchiveRebuilder.h"
#include "vmsStringRecentList.h"	
#include "fsDownload.h"  

enum fsDownloadsMgrEvent
{
	DME_DOWNLOAD_STATE_CHANGED,		
	DME_DOWNLOADEREVENTRECEIVED,	
	DME_EVENTDESCRIPRIONRECEIVED,	
	DME_DOWNLOADWILLBEDELETED,		
	DME_DOWNLOADWASDELETEDFROMLIST,	
	DME_DOWNLOADFILEWILLBEDELETED,	
	DME_NOACTIVEDOWNLOADS,			
	DME_REDIRECTED,					
	DME_DLDSAUTOSTARTMDFD,				
	DME_FILEUPDATED,				
	DME_DOWNLOADSTOPPEDORDONE,		
	DME_SECTIONSTARTED,				
	DME_SECTIONDOWNLOADING,			
	DME_DLMGRTHREADSTARTING,		
	DME_BEFOREDOWNLOADING,			
	DME_DLDADDEDTODELETED,			
	DME_DLDREMOVEDFROMDELETED,		
	DME_DLDWILLBEFULLYDELETED,		
	DME_DLDRESTORED,				
	DME_RECADDEDTOHISTORY,			
	DME_RECDELETEDFROMHISTORY,		
	DME_HISTORYCLEARED,				
	DME_CREATEDLDDIALOG,	
	DME_CLOSEDLDDIALOG,		
	DME_UPDATEDLDDIALOG, 
	DME_DLDAUTODELETED,	
	DME_BEGINDELETEMANYDOWNLOADS, 
	DME_ENDDELETEMANYDOWNLOADS,	
	DME_RECYCLEBINCONTENTCHANGED,	
	DME_BTDLD_STAT_CHANGED,		
	DME_DLD_CHANGED_TO_BT_TYPE,
};  

enum fsTUM
{
	TUM_LIGHT		= 0,	
	TUM_MEDIUM		= 1,	
	TUM_HEAVY		= 2		
};  

struct fsTrafficUsageMode
{
	UINT uMaxConns;		
	UINT uMaxConnsPS;	
	UINT uMaxDlds;		
	UINT uTrafficLimit;	
};  

enum fsDeleteDownloadReaction
{
	DDR_ASK				= 0,	
	DDR_FROMLISTONLY	= 1,	
	DDR_WITHFILE		= 2		
};  

struct fsServerConnections
{
	CString strName;	
	UINT cConns;		
};      

const UINT MAX_SUMMS = 24*60*(60/5);  

enum fsConnectionType
{
	CT_MODEM_14_4,		
	CT_MODEM_28_8,		
	CT_MODEM_33_6,		
	CT_MODEM_56_ISDN,	
	CT_DUAL_ISDN,		
	CT_CABLE_DSL_256,
	CT_CABLE_DSL_512,
	CT_CABLE_DSL_768,
	CT_CABLE_DSL_1M,
	CT_CABLE_DSL_1_5M,
	CT_CABLE_DSL_2M,
	CT_LAN_10,
	CT_UNKNOWN,			
};    

enum fsTUMManage
{
	TM_OFF,						
	TM_DECREASEBYONELEVEL,		
	TM_SETTOLIGHT				
};

#define DLMGRFILE_CURRENT_VERSION	(1)
#define DLMGRFILE_SIG		"FDM Dl Mgr SI"
struct fsStateInfoFileHdr
{
	char szSig [sizeof (DLMGRFILE_SIG) + 1];
	WORD wVer;

	fsStateInfoFileHdr ()
	{
		strcpy (szSig, DLMGRFILE_SIG);
		wVer = DLMGRFILE_CURRENT_VERSION;
	}
};

class fsDownloadsMgr  
{
	friend class CDownloaderProperties_ListPage;
	friend class CDlg_Options_Downloads;
	friend class CPrg_Data; 
public:
	BOOL AllowStartNewDownloads();
	vmsDownloadSmartPtr get_HighestPriorityDownload();
	void AddEvent (vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType = EDT_RESPONSE_S);
	void DownloadStateChanged (vmsDownloadSmartPtr dld);
	
	
	int IsSuchUrlExistsAlready(vmsDownloadSmartPtr dld);
	
	
	
	
	
	void set_HighestPriorityDownload (vmsDownloadSmartPtr dld);
	
	vmsDownloadSmartPtr FindDownloadByBtDownload (vmsBtDownload* pDld);
	vmsDownloadSmartPtr GetDownloadByBtDownloadMgr (vmsBtDownloadManager *pMgr);
	
	void AttachToBtSession();
	
	static void GetEventColors (fsDownloadMgr_EventDescType enType,  COLORREF  &clrText, COLORREF &clrBg, int &iImage);
	
	
	BOOL DeleteGroupsDownloads (vmsDownloadsGroupSmartPtr pGroup, BOOL bInclSubgroups);
	
	
	
	
	
	
	void ReorderDownloads (DLDS_LIST_REF vDlds, DLDS_LIST_REF vReqOrder, bool bDldsSorted = false);
	
	
	
	void MoveDownloads (DLDS_LIST_REF vDlds, vmsDownloadSmartPtr pDldPosition, bool bUp);
	
	void LastFilesDownloaded_Clear();
	
	vmsFileRecentList* get_LastFilesDownloaded ();
	
	UINT GetNextDownloadID();
	
	BOOL GenerateDescriptionFile (vmsDownloadSmartPtr dld);
	
	void ApplyAER (fsAlreadyExistReaction enAER);
	
	void Shutdown();
	
	
	
	void LaunchDownload (vmsDownloadSmartPtr dld, UINT nWaitForConfirmation = 0);
	
	void Initialize();
	
	
	void MoveDownloadToEndOfList (vmsDownloadSmartPtr dld);
#ifndef FDM_DLDR__RAWCODEONLY
	fsDownloadsHistoryMgr m_histmgr;	
#endif
	
	BOOL IsRestoringNow();
	
	
	
	BOOL PerformVirusCheck (vmsDownloadSmartPtr dld, BOOL bCheckExtReqs, BOOL bWaitDone);
	
	int RestoreDownloads (DLDS_LIST &vDlds);
	
	int GetDeletedDownloadIndex (vmsDownloadSmartPtr dld);
	
	BOOL IsDeletingDeletedNow();
	
	
	int DeleteDeletedDownloads (DLDS_LIST &vDlds, BOOL bNoCancel = FALSE);
	
	void ClearDeleted();
	
	void ReadDeletedSettings();
	
	vmsDownloadSmartPtr Get_DeletedDownload (int iIndex);
	
	int Get_DeletedDownloadCount();
	
	void Apply_MirrParameters (vmsDownloadSmartPtr dld);
	void ReadMirrParameters();
	
	void Apply_MirrParameters();
	
	
	void StartDownloads(DLDS_LIST &vpDlds, BOOL bByUser = FALSE);
	
	void StopDownloads(DLDS_LIST &vDlds, BOOL bByUser = FALSE);
	
	
	
	
	void ProcessDownloads();
	
	static void Download_CloneSettings (vmsDownloadSmartPtr dst, vmsDownloadSmartPtr src);
	
	void UseDetailedLog (BOOL b);
	
	BOOL IsDeletingNow();
	fsTUMManage GetTUMManage();
	void SetTUMManage (fsTUMManage enManage);
#ifndef FDM_DLDR__RAWCODEONLY
	
	void AttachToIECatchMgr (fsIECatchMgr* mgr);
#endif
	
	int GetRunningDownloadCount();
	
	void ApplyTrafficLimit();
	
	void ReadSettings();
	
	void ApplyConnectionType(fsConnectionType enCT);
	
	void SaveSettings();
	
	void UnlockList();
	void LockList ();
	
	void RebuildServerList(BOOL bUpdateSiteList = TRUE);
	
	void AllowStartNewDownloads (BOOL bAllow);
	
	vmsDownloadSmartPtr GetDownloadByID (UINT nID);
	
	BOOL IsRunning();
	
	
	BOOL IsSummarySpeedWasLessThan (UINT uSpeed, UINT uTimeInterval);
	
	
	void StopAllDownloads(BOOL bByUser);
	void StartAllDownloads(BOOL bByUser);
	fsTUM GetTUM();
	void SetTUM (fsTUM tum);
	
	UINT GetAmountConnections ();
	
	
	fsTrafficUsageMode* GetTUMs ();
	
	
	void StartGroup (vmsDownloadsGroupSmartPtr pGroup);
	void StopGroup (vmsDownloadsGroupSmartPtr pGroup);
	
	
	
	int  DeleteDownloads (DLDS_LIST &vDlds, BOOL bByUser, BOOL bDontConfirmFileDeleting);
	
	int GetDownloadIndex (vmsDownloadSmartPtr dld, int nFindStartPos = 0);
	
	BOOL Save ();
	BOOL LoadDownloads();
	
	
	size_t GetCount() {return m_vDownloads.size ();};
	
	vmsDownloadSmartPtr GetDownload (size_t iIndex);
	void SetEventsFunc (fntDownloadsMgrEventFunc pfn, LPVOID lpParam);
	
	vmsDownloadSmartPtr GetDownloadByDownloadMgr(fsDownloadMgr *pMgr);
	
	
	
	UINT Add (vmsDownloadSmartPtr dld, BOOL bKeepIDAsIs = FALSE, bool bPlaceToTop = false);
	

	fsDownloadsMgr();
	virtual ~fsDownloadsMgr();

protected:
	static DWORD WINAPI _threadStartSeeding (LPVOID lp);
	void EventEx (vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType, int nMaxCharsPerLine);
	BOOL OnDldDone_CheckDownloadIsHtmlPageWithVideo(vmsDownloadSmartPtr dld);
	
	void ApplyTrafficLimitForListOfDownloads (DLDS_LIST vDlds, UINT nLimit);
	
	void ApplyTrafficLimit_HasHpDld();
	
	void ApplyTrafficLimit_NoHpDld();
	
	void GetRunningDownloads (DLDS_LIST &v);
	
	void ResetDldHpManageState();

	struct _inc_GrantAllBandwidthInfo {
		enum CurState {
			CS_Start,
			CS_Measuring_Bandwidth,
			CS_Measuring_DldMaxSpeed,
			CS_Done,
		} enCurState;
		vmsDownloadSmartPtr dld;
		UINT uBandwidth;
		UINT uDldMaxSpeed;
		fsTicksMgr timeStateChange;
	} m_gabInfo;
	
	void OnDownloadDescEventRcvd (vmsDownloadSmartPtr dld, fsDownloadMgr_EventDescType enType, LPCSTR pszEvent);
	
	BOOL OnDownloadStoppedOrDone (vmsDownloadSmartPtr dld);
	
	static DWORD _BtDownloadManagerEventHandler (vmsBtDownloadManager* pMgr, vmsBtDownloadManagerEvent ev, DWORD dwInfo, LPVOID lp);
	
	static void _BtSessionEventsHandler (class vmsBtSession*, struct vmsBtSessionEvent*, LPVOID lp);
	
	static DWORD WINAPI _threadDeleteDownloadMgrEx (LPVOID lp);
	
	
	BOOL OnDldDone_CheckDownloadIsBittorrent (vmsDownloadSmartPtr dld);
	
	
	
	
	
	void DeletedDownloads_UseTmpList (BOOL bUse);
	
	void DownloadsList_Insert (size_t nIndexBefore, vmsDownloadSmartPtr dld);
	void DownloadsList_Del (size_t nIndex);
	void DownloadsList_Add (vmsDownloadSmartPtr dld);
	
	
	
	BOOL OnDldDone_CheckDownloadIsMetaLink (vmsDownloadSmartPtr dld);
	
	long m_cThreadsRunning;
	
	void OnDldDoneCheckIfNeedDel (vmsDownloadSmartPtr dld);
	
	static DWORD WINAPI _threadIntegrityCheckAndVirCheckAndLaunch (LPVOID lp);
	
	vmsFileRecentList m_LastFilesDownloaded;
	
	BOOL SaveStateInformation();
	BOOL LoadStateInformation();
	
	CRITICAL_SECTION m_csSave;
	
	static DWORD WINAPI _threadArchiveDetected (LPVOID lp);
	
	static DWORD WINAPI _threadConfirmZIP (LPVOID lp);
	
	DWORD OnArchiveDetected (vmsDownloadSmartPtr dld, fsArchive::fsArchiveRebuilder* ar);
#ifndef FDM_DLDR__RAWCODEONLY
	
	void Event (fsDLHistoryRecord* rec, fsDownloadsMgrEvent ev);
	
	static void _HistoryMgrEvents (fsDownloadsHistoryMgrEvent ev, int info, LPVOID);
#endif
	
	static DWORD WINAPI _threadWaitRestoringDlds (LPVOID lp);
	BOOL m_bRestoringNow;	
	
	int RestoreDownloads2(DLDS_LIST* vDlds, BOOL* pbNeedStop, int* piProgress);
	
	static DWORD WINAPI _threadWaitDelDeletingsDlds (LPVOID lp);
	
	int DeleteDeletedDownloads2(DLDS_LIST *vDlds, BOOL bNoCancel, BOOL* pbNeedStop, int* piProgress);
	
	
	BOOL DeleteDownloadFile (vmsDownloadSmartPtr dld, BOOL bNoCancel, fsDeleteDownloadReaction &enDDR, BOOL bDownloadWillBeDeletedAlso);
	
	
	
	vmsDownloadSmartPtr PutDownloadToDeleted (vmsDownloadSmartPtr dld);
	
	
	
	DWORD OnBeforeDownload (vmsDownloadSmartPtr dld);
	
	BOOL m_bMirrDontSearchIfSizeUnk;
	UINT m_uMirrRecalcSpeedTime;
	UINT m_cMaxMirrs;
	BOOL m_bMirrCalcSpeed;
	UINT m_uMirrFileMinSize;
	BOOL m_bMirrAutoSearch;
	
	static DWORD WINAPI _threadWaitDelDlds (LPVOID lp);
	
	int DeleteDownloads2 (DLDS_LIST *vDlds, BOOL bByUser, BOOL bDontConfirmFileDeleting, BOOL* pbNeedStop = NULL, int* piProgress = NULL);
	BOOL m_bVirCheck;			
	fsString m_strVirName, m_strVirArgs, m_strVirExts;	
	BOOL m_bDetLog;		
	BOOL m_bDeletingNow, m_bDeletingDeletedNow;		
	fsTUMManage m_enTUMManage;	
#ifndef FDM_DLDR__RAWCODEONLY
	
	static void _IECatchMgrEvents (fsIECatchMgrEvent ev, LPVOID lp);
#endif
	
	BOOL IsMaxConnsReached();
	
	
	BOOL IsServerFilled (LPCSTR pszServer, DWORD dwReqProtocols);
	
	void InitTUM();
	fsMutex m_mxDlList;	
	CRITICAL_SECTION m_csQSection;	
	
	fsMutex m_mxBuildConns;
	BOOL m_bAllowStart;				
	BOOL m_bDisablePD;	
	
	
	void CheckNoActiveDownloads();
	
	fs::array <UINT, MAX_SUMMS> m_vSummSpeed;
	UINT m_nID;	
	BOOL m_bSkip1Cicle;	
	
	void ManageTraffic();
	
	void OnSectionStop (vmsDownloadSmartPtr dld);
	void Event (vmsDownloadSmartPtr dld, LPCSTR pszEvent, fsDownloadMgr_EventDescType enType = EDT_RESPONSE_S);
	
	
	BOOL OnQueryNewSection (vmsDownloadSmartPtr dld, UINT nUsingMirror);
	BOOL m_bAutoDelCompleted;	
	fsDeleteDownloadReaction m_enDDR;	
	fsTUM m_enTUM;				
	fsTUM m_enUserTUM;			
	BOOL m_bNeedExit;			
	static DWORD WINAPI _threadDownloadsMgr (LPVOID lp);
	
	static DWORD WINAPI _threadDeleteDownload(LPVOID lp);
	fsDownloadRegSaver m_saver;	
	DWORD Event (vmsDownloadSmartPtr dld, fsDownloadsMgrEvent ev);
	
	static void _DownloadMgrEventDesc (fsDownloadMgr *pMgr, fsDownloadMgr_EventDescType enType, LPCSTR pszEvent, LPVOID lp);
	static DWORD _DownloadMgrEvents (fsDownloadMgr *pMgr, fsDownloaderEvent enEvent, UINT uInfo, LPVOID lp);
	DLDS_LIST m_vDownloads;	
	DLDS_LIST m_vDeletedDownloads;	
	
	std::list <vmsDownloadSmartPtr> m_vDeletedDownloads_tmp;
	BOOL m_bBypassDeleted;	
	BOOL m_bBypassCopleted;	
	UINT m_cMaxDeleted;		
	fntDownloadsMgrEventFunc m_pfnEvents;
	LPVOID m_lpEventsParam;
	fsTrafficUsageMode m_aTUM [3];	
};

#endif 
