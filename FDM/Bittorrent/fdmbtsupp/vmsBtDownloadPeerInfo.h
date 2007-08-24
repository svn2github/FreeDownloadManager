/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#pragma once

class vmsBtDownloadPeerInfo
{
public:
	virtual void get_Ip (LPSTR psz) = NULL;
	virtual void get_Client (LPSTR psz) = NULL;
	virtual int get_DownloadSpeed () = NULL;
	virtual int get_UploadSpeed () = NULL;
	virtual UINT64 get_BytesDownloaded () = NULL;
	virtual UINT64 get_BytesUploaded () = NULL;
};
