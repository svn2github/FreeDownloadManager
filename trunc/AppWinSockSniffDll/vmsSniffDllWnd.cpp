/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsSniffDllWnd.h"
#include "../include.add/vmsSharedData.h"
#include "vmsHttpFlvTrafficAnalyzer.h"
#include "vmsTransferFlvDownloadsToFdm.h"
#include <objbase.h>
#include "vmsFindFlvDownloadsResultsCache.h"

vmsSniffDllWnd::vmsSniffDllWnd()
{
	m_hWnd = NULL;
}

vmsSniffDllWnd::~vmsSniffDllWnd()
{

}

void vmsSniffDllWnd::Create()
{
	extern HMODULE _hModule;

	static bool _bClassRegistered = false;
	if (!_bClassRegistered)
	{
		WNDCLASSEX wc;
		ZeroMemory (&wc, sizeof (wc));
		wc.cbSize = sizeof (wc);
		wc.lpfnWndProc = _WndProc;
		wc.hInstance = _hModule;
		wc.lpszClassName = _T("FdmFlvSniffDllWnd");
		RegisterClassEx (&wc);
		_bClassRegistered = true;
	}

	m_hWnd = CreateWindow (_T("FdmFlvSniffDllWnd"), NULL, WS_POPUP, 0, 0, 0, 0, NULL, NULL,
		_hModule, NULL);
}

LRESULT WINAPI vmsSniffDllWnd::_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_COMMAND)
	{
		if (wp == 0)
			return OnProcessWebPageUrl (lp);

		return 1;
	}

	return DefWindowProc (hwnd, msg, wp, lp);
}

LRESULT vmsSniffDllWnd::OnProcessWebPageUrl(LPARAM lp)
{
	LOGFN ("vmsSniffDllWnd::OnProcessWebPageUrl");

	static HANDLE _hmxAccMem = CreateMutex (NULL, FALSE, L"FdmFlvSniffDll::mutex::AccMem");

	WaitForSingleObject (_hmxAccMem, INFINITE);

	LOGsnl ("Got mem mutex");

	vmsSharedData sdata (L"Fdm::mem::passUrlToFlvSniffDll", TRUE, 0, FILE_MAP_READ);

	if (sdata.getData () == NULL)
	{
		LOGsnl ("Failed to access shared data");
		ReleaseMutex (_hmxAccMem);
		return E_FAIL;
	}

	LPTSTR pszUrl = new TCHAR [_tcslen ((LPTSTR)sdata.getData ())+1];
	_tcscpy (pszUrl, (LPTSTR)sdata.getData ());

	sdata.Release ();

	LOG ("Got URL: %s", pszUrl);

	ReleaseMutex (_hmxAccMem);

	extern vmsFindFlvDownloadsResultsCache _FlvResCache;

	vmsFindFlvDownloadsResultsCache::ResultPtr result;
	
#ifdef LOG_WEBFILES_TREE
	extern LONG _cInOnGetItBtnClicked;
	InterlockedIncrement (&_cInOnGetItBtnClicked);
#endif
	HRESULT hr = _FlvResCache.FindFlvDownloads (stringFromTstring (pszUrl).c_str (), 
		NULL, NULL, NULL, NULL, NULL, result);
#ifdef LOG_WEBFILES_TREE
	InterlockedDecrement (&_cInOnGetItBtnClicked);
#endif

	LOG ("Analyzed");

	if (hr == S_OK && result->pTa->get_FlvDownloadCount () != 0)
	{
		LOGsnl ("Creating transfer to FDM thread");
		result->AddRef ();
		DWORD dw;
		CloseHandle (
			CreateThread (NULL, 0, _threadTransferDldsToFdm, result, 0, &dw));
	}
	else
	{
		LOGsnl ("No downloads found");
	}

	delete [] pszUrl;

	return hr;
}

DWORD WINAPI vmsSniffDllWnd::_threadTransferDldsToFdm(LPVOID lp)
{
	vmsFindFlvDownloadsResultsCache::Result *result = (vmsFindFlvDownloadsResultsCache::Result*)lp;
	CoInitialize (NULL);
	HRESULT hr = vmsTransferFlvDownloadsToFdm::Do (result->strWebPageUrl.c_str (), result->pTa->getFlvDownloads ());
	CoUninitialize ();
	result->Release ();
	return hr;
}
