/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "FdmApp.h"
#include "FdmTorrentFilesRcvr.h"
#include "DownloadsWnd.h"
#include "UIThread.h"        

STDMETHODIMP CFdmTorrentFilesRcvr::CreateBtDownloadFromFile(BSTR bstrFile)
{
	USES_CONVERSION;

	if (Is9xME)
		return E_NOTIMPL;

	_inc_CBRFF_ctx *p = new _inc_CBRFF_ctx;

	p->strFile = W2A (bstrFile);
	p->strFileUrl = "file://"; p->strFileUrl += p->strFile;
	p->bForceSilent = m_bForceSilent;

	UIThread *thr = (UIThread*) RUNTIME_CLASS (UIThread)->CreateObject ();
	thr->set_Thread (_threadCreateBtDownloadFromFile, p);
	thr->CreateThread ();

	return S_OK;
}

STDMETHODIMP CFdmTorrentFilesRcvr::get_ForceSilent(BOOL *pVal)
{
	*pVal = m_bForceSilent;
	return S_OK;
}

STDMETHODIMP CFdmTorrentFilesRcvr::put_ForceSilent(BOOL newVal)
{
	m_bForceSilent = newVal;
	return S_OK;
}

DWORD WINAPI CFdmTorrentFilesRcvr::_threadCreateBtDownloadFromFile(LPVOID lp)
{
	_inc_CBRFF_ctx *p = (_inc_CBRFF_ctx*) lp;

	_pwndDownloads->CreateBtDownloadFromFile (p->strFile, p->strFileUrl,
		p->bForceSilent ? TRUE : _App.Monitor_Silent ());

	delete p;

	return 0;
}
