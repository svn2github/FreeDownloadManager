/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#pragma once

#include "vmsBtDownload.h"
#include "vmsBtFile.h"

enum vmsBtSessionEventType
{
	
	
	BTSET_LISTEN_FAILED,
	
	
	BTSET_FILE_ERROR,
	
	
	BTSET_TRACKER_ANNOUNCE,
	
	
	BTSET_TRACKER,
	
	
	BTSET_TRACKER_REPLY,
	
	
	
	
	BTSET_TRACKER_WARNING,
	
	BTSET_URL_SEED,
	
	
	
	BTSET_HASH_FAILED,
	
	
	BTSET_PEER_BAN,
	
	
	
	BTSET_PEER_ERROR,
	
	
	BTSET_TORRENT_FINISHED,
	
	
	BTSET_BTDLD_WAS_RESET,
};

struct vmsBtSessionEvent
{
	vmsBtSessionEventType enType;
	LPCSTR pszMsg;
	vmsBtDownload *pDownload;

	union {
	
	int times_in_row;
    int status_code;

	
	LPCSTR pszUrl;

	
	int piece_index;

	
	LPCSTR pszIp;
	};
};

typedef void (*fntBtSessionEventsHandler)(class vmsBtSession*, vmsBtSessionEvent*, LPVOID);

class vmsBtSession
{
public:
	
	
	virtual void ListenOn (int portFrom, int portTo) = NULL;
	
	virtual BOOL IsListening () = NULL;
	
	virtual USHORT get_ListenPort () = NULL;
	
	virtual vmsBtDownload* CreateDownload (vmsBtFile *torrent, LPCSTR pszOutputPath, LPBYTE pbFastResumeData, DWORD dwFRDataSize, BOOL bCompactMode) = NULL;
	
	virtual void SetDownloadLimit (int limit) = NULL;
	virtual void SetUploadLimit (int limit) = NULL;
	
	virtual void SetMaxUploads (int limit) = NULL;
	
	virtual void DeleteDownload (vmsBtDownload*) = NULL;
	
	virtual void DHT_start (LPBYTE pbState, DWORD dwStateSize) = NULL;
	virtual void DHT_stop () = NULL;
	virtual BOOL DHT_getState (LPBYTE pbBuffer, DWORD dwBufferSize, LPDWORD pdwDataSize) = NULL;
	virtual BOOL DHT_isStarted () = NULL;
	
	virtual void set_EventsHandler (fntBtSessionEventsHandler pfn, LPVOID pData) = NULL;
	virtual void SetProxySettings (LPCSTR pszIp, int nPort, LPCSTR pszUser, LPCSTR pszPwd) = NULL;
	virtual int get_TotalDownloadConnectionCount () = NULL;
	virtual int get_DownloadCount () = NULL;
	virtual vmsBtDownload* get_Download (int nIndex) = NULL;
	virtual int get_TotalDownloadSpeed () = NULL;
	virtual int get_TotalUploadSpeed () = NULL;
	virtual UINT64 get_TotalDownloadedByteCount () = NULL;
	virtual UINT64 get_TotalUploadedByteCount () = NULL;
	virtual void SetUserAgent (LPCSTR pszUA) = NULL;
};
