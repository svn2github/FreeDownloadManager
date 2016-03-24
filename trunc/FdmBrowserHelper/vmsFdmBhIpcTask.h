/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#pragma once
#include "vmsFdmBhIpcTaskHandshake.h"
#include "vmsFdmBhIpcTaskCreateDownloads.h"
#include "fdm_ipc_task_type.h"
#include "vmsFdmBhIpcBrowserProxyInfo.h"
#include "vmsFdmBhIpcTaskWindow.h"
#include "vmsFdmBhIpcTaskVideoSniffer.h"
#include "vmsFdmBhIpcTaskNetworkRequestNotification.h"
#include "vmsFdmBhIpcTaskDownloadState.h"
class vmsFdmBhIpcTask :
	public vmsSerializable
{
public:
	vmsFdmBhIpcTask ();
	~vmsFdmBhIpcTask ();

	fdm_ipc_task_type get_type () const;

public:
	std::wstring m_id;
	std::wstring m_type;
	vmsFdmBhIpcTaskHandshake m_handshake;
	vmsFdmBhIpcTaskCreateDownloads m_createDownloads;
	vmsFdmBhIpcBrowserProxyInfo m_browserProxy;
	vmsFdmBhIpcTaskWindow m_window;
	vmsFdmBhIpcTaskVideoSniffer m_videoSniffer;
	vmsFdmBhIpcTaskNetworkRequestNotification m_netReqNotif;
	vmsFdmBhIpcTaskDownloadState m_downloadState;

public:
	virtual bool Serialize (vmsSerializationIoStream *pStm, unsigned flags = 0) override;
};

