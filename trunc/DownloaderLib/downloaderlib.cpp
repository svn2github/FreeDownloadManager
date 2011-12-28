/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "downloaderlib.h"
#include "vmsDownloader.h"

BOOL vmsdl_InitializeLibrary ()
{
	fsInternetSession::InitializeWinInet ();
	return TRUE;
}

DLHANDLE vmsdl_CreateDownloader (LPCSTR pszURL, LPCSTR pszOutFile)
{
	vmsDownloader* dldr = new vmsDownloader;
	
	if (IR_SUCCESS != dldr->Initialize (pszURL, pszOutFile))
	{
		delete dldr;
		return NULL;
	}

	return dldr;
}

void vmsdl_SetProxy (DLHANDLE h, LPCSTR pszProxy, LPCSTR pszUser, LPCSTR pszPassword)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_Proxy (pszProxy, pszUser, pszPassword);
}

void vmsdl_SetMaxRetryCount (DLHANDLE h, int nValue)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_MaxRetryCount (nValue);
}

void vmsdl_SetRetriesTime (DLHANDLE h, int nValue)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_RetriesTime (nValue);
}

void vmsdl_SetTimeout (DLHANDLE h, int nValue)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_Timeout (nValue);
}

void vmsdl_ReleaseDownloader (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	delete dldr;
}

void vmsdl_StartDownloading (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->StartDownloading ();
}

void vmsdl_StopDownloading (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->StopDownloading ();
}

int	 vmsdl_GetProgress (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	return dldr->get_Progress ();
}

vmsdl_DownloadState vmsdl_GetState (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	return (vmsdl_DownloadState) dldr->get_State ();
}

UINT vmsdl_GetDownloadingSpeed (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	return dldr->get_Speed ();
}

UINT64	vmsdl_GetFileSize (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	return dldr->get_FileSize ();
}

UINT64	vmsdl_GetDownloadedByteCount (DLHANDLE h)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	return dldr->get_DownloadedByteCount ();
}

void vmsdl_DisableCookies (DLHANDLE h, BOOL bDisable)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_DisableCookies (bDisable);
}

void vmsdl_SetUserAgent (DLHANDLE h, LPCSTR pszAgent)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_UserAgent (pszAgent);
}

extern void vmsdl_SetMaxSections(DLHANDLE h, UINT uiMaxSections)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_MaxSections (uiMaxSections);
}

extern void vmsdl_SetMinSectionSize(DLHANDLE h, UINT uiMinSectionSize)
{
	vmsDownloader* dldr = (vmsDownloader*) h;
	dldr->set_MinSectionSize (uiMinSectionSize);
}
