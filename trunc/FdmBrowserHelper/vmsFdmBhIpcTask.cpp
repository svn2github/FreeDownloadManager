/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcTask.h"

vmsFdmBhIpcTask::vmsFdmBhIpcTask ()
{
}

vmsFdmBhIpcTask::~vmsFdmBhIpcTask ()
{
}

fdm_ipc_task_type vmsFdmBhIpcTask::get_type () const
{
	std::wstringstream wss;
	wss << m_type;
	fdm_ipc_task_type t;
	if (wss >> t)
		return t;
	return fdm_ipc_task_type::unknown;
}

bool vmsFdmBhIpcTask::Serialize (vmsSerializationIoStream *pStm, unsigned flags )
{
	if (!pStm->SerializeValue (L"id", m_id) ||
		!pStm->SerializeValue (L"type", m_type))
	{
		return false;
	}

	std::pair <fdm_ipc_task_type, vmsSerializable*> ser_objs [] = 
	{
		std::make_pair (fdm_ipc_task_type::handshake, &m_handshake),
		std::make_pair (fdm_ipc_task_type::create_downloads, &m_createDownloads),
		std::make_pair (fdm_ipc_task_type::browser_proxy, &m_browserProxy),
		std::make_pair (fdm_ipc_task_type::window, &m_window),
		std::make_pair (fdm_ipc_task_type::video_sniffer, &m_videoSniffer),
		std::make_pair (fdm_ipc_task_type::network_request_notification, &m_netReqNotif),
		std::make_pair (fdm_ipc_task_type::browser_download_state_report, &m_downloadState)
	};

	auto tt = get_type ();

	for (const auto &obj : ser_objs)
	{
		if (obj.first == tt)
		{
			std::wstringstream wss; wss << obj.first;
			if (!SerializeObject (pStm, obj.second, wss.str ()))
				return false;
			break;
		}
	}

	return true;
}
