/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsFdmBhIpcUiStringsTaskProcessor.h"
#include "../common/vmsFdmTranslations.h"
#include "vmsFdmBhIpcUiStringsTaskResponse.h"

vmsFdmBhIpcUiStringsTaskProcessor::vmsFdmBhIpcUiStringsTaskProcessor ()
{
}

vmsFdmBhIpcUiStringsTaskProcessor::~vmsFdmBhIpcUiStringsTaskProcessor ()
{
}

void vmsFdmBhIpcUiStringsTaskProcessor::process (std::shared_ptr <vmsFdmBhIpcTask> task,
	process_callback_fn_t callback)
{
	auto resp = construct_response <vmsFdmBhIpcUiStringsTaskResponse> (task);

	resp->m_dllink = vmsFdmTranslations::o ().GetString (L_DLTHISIEMENU);
	if (resp->m_dllink.empty ())
		resp->m_dllink = _T("Download with Free Download Manager");

	resp->m_dlall = vmsFdmTranslations::o ().GetString (L_DLALLIEMENU);
	if (resp->m_dlall.empty ())
		resp->m_dlall = _T("Download all with Free Download Manager");

	resp->m_dlselected = vmsFdmTranslations::o ().GetString (L_DLSELECTEDIEMENU);
	if (resp->m_dlselected.empty ())
		resp->m_dlselected = _T("Download selected with Free Download Manager");

	resp->m_dlvideo = vmsFdmTranslations::o ().GetString (L_DLFLASHVIDEOIEMENU);
	if (resp->m_dlvideo.empty ())
		resp->m_dlvideo = _T("Download video with Free Download Manager");

	resp->m_dlbyfdm = vmsFdmTranslations::o ().GetString (L_DLBYFDM);
	if (resp->m_dlbyfdm.empty ())
		resp->m_dlbyfdm = _T("Download by FDM");

	resp->m_showthisbtn = vmsFdmTranslations::o ().GetString (L_SHOW_THIS_BUTTON);
	if (resp->m_showthisbtn.empty ())
		resp->m_showthisbtn = _T("Show This Button");

	resp->m_sniffModuleNotLoaded = vmsFdmTranslations::o ().GetString (L_FLVSNIFFMODULE_NOTFOUND);
	if (resp->m_sniffModuleNotLoaded.empty ())
		resp->m_sniffModuleNotLoaded = _T ("Flash video monitoring module is not loaded. Make sure FDM is running and you've enabled this option in FDM (see Options | Downloads | Flash Video).");

	resp->m_fdmCallFailed = vmsFdmTranslations::o ().GetString (L_FAILEDTRANSFERDLDSTOFDM);
	if (resp->m_fdmCallFailed.empty ())
		resp->m_fdmCallFailed = _T ("An error occurred while trying to transfer downloads to FDM.\nError: 0x%x.");

	resp->m_noFlashVideoFound = vmsFdmTranslations::o ().GetString (L_NOFLVSFOUND);
	if (resp->m_noFlashVideoFound.empty ())
		resp->m_noFlashVideoFound = _T ("There were no flash videos found on this page. Make sure the videos on this page are playing or try to reload it (refresh page or clear browser's cache).");

	resp->m_flvWebPageNotFound = vmsFdmTranslations::o ().GetString (L_FLVWEBPAGENOTFOUND);
	if (resp->m_flvWebPageNotFound.empty ())
		resp->m_flvWebPageNotFound = _T ("Failed to found information about this web page. Please force your browser to reload it.");

	callback (task, resp);
}