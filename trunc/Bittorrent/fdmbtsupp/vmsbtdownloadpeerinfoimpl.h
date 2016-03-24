/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsbtdownloadpeerinfo.h"
#include "libtorrent/peer_info.hpp"

class vmsBtDownloadPeerInfoImpl :
	public vmsBtDownloadPeerInfo
{
public:
	void get_Ip (LPSTR psz);
	void get_Client (LPSTR psz);
	int get_DownloadSpeed ();
	int get_UploadSpeed ();
	UINT64 get_BytesDownloaded ();
	UINT64 get_BytesUploaded ();
	UINT get_Flags ();

	vmsBtDownloadPeerInfoImpl(void);

	libtorrent::peer_info m_peer;

public:
	virtual ~vmsBtDownloadPeerInfoImpl(void);
};
