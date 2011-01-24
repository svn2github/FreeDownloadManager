/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsBtDownload.h"
#include "vmsBtDownloadPeerInfoListImpl.h"
#include "vmsBtFileImpl.h"    

class vmsBtDownloadImpl : public vmsBtDownload
{
public:
	void check_handle_is_valid (); 
	vmsBtDownloadState GetState ();
	BOOL IsPaused ();
	void Pause ();
	void Resume ();
	vmsBtFile* get_Torrent ();
	void get_SavePath (LPSTR pszRes);
	float get_PercentDone ();
	int get_PiecesProgressMap (bool* pbPieces, int *pnCompletedPieces);
	bool is_PieceCompleted (int nIndex);
	int get_DownloadConnectionCount ();
	UINT64 get_TotalDownloadedBytesCount ();
	UINT GetDownloadSpeed ();
	UINT GetUploadSpeed ();
	void SetDownloadLimit (int limit);
	int  GetDownloadLimit ();
	int get_ConnectionCount ();
	void set_ConnectionLimit (int limit);
	BOOL get_FastResumeData (LPBYTE pbRes, DWORD dwSize, DWORD *pdwDataSize);
	void set_TrackerLogin (LPCSTR pszUser, LPCSTR pszPassword);
	BOOL MoveToFolder (LPCSTR pszNewFolder);
	int get_NextAnnounceInterval ();
	void get_CurrentTracker (LPSTR pszRes);
	UINT64 get_TotalUploadedByteCount ();
	void get_PeersStat (int *pnPeersConnected, int *pnSeedsTotal, int *pnLeechersTotal, int *pnSeedsConnected);
	UINT64 get_WastedByteCount ();
	double get_ShareRating ();
	vmsBtDownloadPeerInfoList* get_PeerInfoList ();
	void get_FileProgress (float *p);
	BOOL is_HandleValid ();
	void OnTrackerAlert (LPCSTR pszMsg);
	int get_CurrentTaskProgress ();
	void PrioritizeFiles (int* piPriorities);
	void get_CurrentTracker2 (LPSTR pszRes, DWORD dwBuffSize);

	void AddRef () {InterlockedIncrement (&m_cRefs);}
	void Release () {if (0 == InterlockedDecrement (&m_cRefs)) delete this;}

	vmsBtDownloadImpl(void);
protected:
	virtual ~vmsBtDownloadImpl(void);

public:
	torrent_handle m_handle; 
	
	
	
	vmsBtFileImpl *m_pTorrent;
	std::string m_strOutputPath;

protected:
	bool m_bOnTrackerAlert_IsSeed;
	DWORD m_dwAnnounceInterval;
	std::vector <DWORD> m_vTicksTrackersConnected;
	vmsBtDownloadPeerInfoListImpl m_peerList;
	long m_cRefs;
	torrent_status& status ();
	torrent_status m_status; DWORD m_dwDob_status;

	
	
	int m_iDownloadSpeedLimit;
};
