/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/            

#if !defined(AFX_VMSDOWNLOADMGREX_H__A5D098FD_2C7B_4051_B405_8B1F25F3C3FA__INCLUDED_)
#define AFX_VMSDOWNLOADMGREX_H__A5D098FD_2C7B_4051_B405_8B1F25F3C3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "vmsBtDownloadManager.h"
#include "vmsObject.h"
#include "vmsObjectSmartPtr.h"  

struct vmsSectionInfo
{
	
	
	UINT64 uDStart, uDEnd, uDCurrent;
};

class vmsDownloadMgrEx : public vmsObject
{
public:
	int get_ReservingDiskSpaceProgress();
	BOOL IsReservingDiskSpace();
	void GetSplittedSectionsList (std::vector <vmsSectionInfo> &v);
	
	BOOL IsBittorrent();
	
	
	BOOL IsFileNotInitOrIsMultiFile();
	
	
	
	BOOL LoadState (LPBYTE pb, LPDWORD pdwSize, WORD wVer);
	BOOL SaveState (LPBYTE pb, DWORD *pdwSize);
	int GetConnectionCount();
	fsInternetResult SetToRestartState();
	BOOL IsDontConfirmLaunchWhenDone();
	BOOL DeleteFile();
	void UseDetailedLog (BOOL b);
	UINT GetTrafficLimit();
	void LimitTraffic (UINT uLimit);
	void StopDownloading();
	fsInternetResult StartDownloading();
	BOOL IsHtmlSpiderDownload();
	BOOL IsDescriptionFileShouldBeGenerated();
	fsInternetResult RestartDownloading();
	BOOL IsCantStart();
	BOOL IsStoppedByUser();
	BOOL MoveToFolder (LPCSTR pszPath);
	UINT GetSpeed();
	UINT64 GetBytesLeft();
	void StopSection();
	void CreateOneMoreSection();
	int GetDownloadingSectionCount();
	void GetSectionInfo (int nIndex, vmsSectionInfo *sect);
	UINT64 GetSSFileSize();
	BOOL IsDownloading();
	UINT64 GetDownloadedBytesCount();
	UINT64 GetLDFileSize();
	int GetNumberOfSections();
	fsString get_URL(BOOL bInclAuth = FALSE);
	void set_LaunchWhenDone (BOOL b);
	BOOL IsFileInitializedOnDisk();
	BOOL IsRunning();
	BOOL IsDone();
	fsResumeSupportType IsResumeSupported();
	BOOL IsLaunchWhenDone();
	float GetPercentDone();
	fsString get_OutputFilePathName();
	vmsBtDownloadManager* GetBtDownloadMgr();
	class fsDownloadMgr* GetDownloadMgr();
	
	void Attach (vmsBtDownloadManager* pBtMgr);
	void Attach (fsDownloadMgr *pMgr);
	vmsDownloadMgrEx();
	virtual ~vmsDownloadMgrEx();

protected:
	vmsBtDownloadManager* m_pBtMgr;
	fsDownloadMgr* m_pMgr;
};

typedef vmsObjectSmartPtr <vmsDownloadMgrEx> vmsDownloadMgrSmartPtr;

#endif 
