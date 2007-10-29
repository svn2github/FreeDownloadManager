/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

#include "stdafx.h"
#include "FdmApp.h"
#include "FDMFlashVideoDownloads.h"
#include "vmsVideoSiteHtmlCodeParser.h"
#include "FlashVideoDownloadsWnd.h"
#include "UIThread.h"

_COM_SMARTPTR_TYPEDEF (IWGUrlReceiver, __uuidof (IWGUrlReceiver));        

STDMETHODIMP CFDMFlashVideoDownloads::ProcessIeDocument(IDispatch *pDispatch)
{
	IHTMLDocument2Ptr spDoc (pDispatch);
	if (spDoc == NULL)
		return E_INVALIDARG;

	IPersistFilePtr spFile (pDispatch);
	if (spFile == NULL)
		return E_INVALIDARG;

	

	USES_CONVERSION;
	BSTR bstrHost = NULL;
	spDoc->get_URL (&bstrHost);
	fsURL url;
	if (url.Crack (W2A (bstrHost)) != IR_SUCCESS)
		return E_FAIL;
	SysFreeString (bstrHost);

	char szPath [MY_MAX_PATH];
	GetTempPath (sizeof (szPath), szPath);
	char szFile [MY_MAX_PATH];
	GetTempFileName (szPath, "fdm", 0, szFile);

	COleVariant vaFile (szFile);
	if (FAILED (spFile->Save (vaFile.bstrVal, FALSE)))
		return E_FAIL;

	HANDLE hFile = CreateFile (szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	DWORD dw = GetFileSize (hFile, NULL);

	LPSTR pszHtml = new char [dw + 1];
	ReadFile (hFile, pszHtml, dw, &dw, NULL);
	pszHtml [dw] = 0;

	CloseHandle (hFile);
	DeleteFile (szFile);

	ProcessHtml (url.GetHostName (), pszHtml);
	delete [] pszHtml;

	return S_OK;
}

STDMETHODIMP CFDMFlashVideoDownloads::ProcessHtml(BSTR bstrHost, BSTR bstrHtml)
{
	USES_CONVERSION;
	ProcessHtml (W2A (bstrHost), W2A (bstrHtml));
	return S_OK;
}

void CFDMFlashVideoDownloads::ProcessHtml(LPCSTR pszHost, LPCSTR pszHtml)
{
	vmsVideoSiteHtmlCodeParser vshcp;
	if (FALSE == vshcp.Parse (pszHost, pszHtml))
		return;

	USES_CONVERSION;

	IWGUrlReceiverPtr spRcvr;
	spRcvr.CreateInstance (__uuidof (WGUrlReceiver));
	spRcvr->put_Url (A2W (vshcp.get_VideoUrl ()));

	if (vshcp.get_IsVideoUrlDirectLink ())
	{
		CString str = vshcp.get_VideoTitle ();
		str += "."; str += vshcp.get_VideoType ();
		spRcvr->put_FileName (A2W (str));

		spRcvr->put_Comment (A2W (vshcp.get_VideoTitle ()));

		spRcvr->put_FlashVideoDownload (TRUE);
	}

	spRcvr->AddDownload ();
}

DWORD WINAPI CFDMFlashVideoDownloads::_threadCreateDownload(LPVOID lp)
{
	USES_CONVERSION;

	BSTR bstrUrl = (BSTR) lp;

	_pwndFVDownloads->CreateDownload (W2A (bstrUrl));

	SysFreeString (bstrUrl);

	return 0;
}

STDMETHODIMP CFDMFlashVideoDownloads::CreateFromUrl(BSTR bstrUrl)
{
	UIThread *thr = (UIThread*) RUNTIME_CLASS (UIThread)->CreateObject ();
	thr->set_Thread (_threadCreateDownload, SysAllocString (bstrUrl));
	thr->CreateThread ();

	return S_OK;
}
