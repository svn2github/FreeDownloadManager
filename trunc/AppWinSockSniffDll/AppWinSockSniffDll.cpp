/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#include "stdafx.h"
#include "vmsWinSockHttpTrafficCollector.h"
#include "vmsSniffDllWnd.h"
#include "vmsBrowserCachedFlvTrafficCollector.h"
#include "vmsHttpFlvTrafficAnalyzer2.h"
#include "vmsHttpFlvTrafficAnalyzer3.h"
#include "vmsHttpFlvTrafficAnalyzer4.h"
#include "vmsDownloadForTrafficCollector.h"
#include "vmsHttpFlvTrafficAnalyzer5.h"
#include "vmsTransferFlvDownloadsToFdm.h"
#include "vmsWinInetHttpTrafficCollector.h"
#include "vmsExternalHttpTrafficCollector.h"
#include "vmsFindFlvDownloadsResultsCache.h"
#include "vmsBrowserSpecialInfo.h"
#include "vmsUrlMonSpyDll.h"
#include "vmsOverlappedWinsockCalls.h"
#include "../Include.Add/vmsAsyncTaskMgr.h"
#include "vmsFindFlvDownloadsResultsCacheAsyncRequest.h"
#include "../common/component_crash_rep.h"

#include "../common/vms_sifdm_cl/win/process/api_hook/vmsHooksPeCalls.h"
#include "../common/vms_sifdm_cl/win/process/api_hook/vmsHooksPeCallsForLoadingModules.h"
std::shared_ptr <vmsPeFnHook> g_hooker;
std::unique_ptr <vmsHooksPeCallsForLoadingModules> g_hooksFurther;

HMODULE _hModule = NULL;
bool g_oldOS = false;

vmsHttpTrafficCollector _HttpTraffic;
vmsWinSockHttpTrafficCollector _WinSockTrafficCollector (&_HttpTraffic);
vmsWinInetHttpTrafficCollector _WinInetTrafficCollector (&_HttpTraffic);
vmsExternalHttpTrafficCollector _ExternalTrafficCollector (&_HttpTraffic);
vmsFindFlvDownloadsResultsCache _FlvResCache (&_HttpTraffic, &vmsWinSockHttpDlgTree::o ());
vmsOverlappedWinsockCalls _WinsockAsyncCalls;
vmsAsyncTaskMgr _AsyncTasks;

#ifdef USE_DETOURS
#include "detours.h"
#pragma comment (lib, "detours")

HMODULE WINAPI Detoured ()
{
	
	return GetModuleHandle (L"detoured.dll");
}

int (WINAPI *Real_WSASend)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, DWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6) = WSASend;
int (WINAPI *Real_send)(SOCKET a0, CONST char* a1, int a2, int a3) = send;

int (WINAPI *Real_WSARecv)(SOCKET a0, LPWSABUF a1, DWORD a2, LPDWORD a3, LPDWORD a4, LPWSAOVERLAPPED a5, LPWSAOVERLAPPED_COMPLETION_ROUTINE a6) = WSARecv;
int (WINAPI *Real_recv)(SOCKET a0, char* a1, int a2, int a3) = recv;

int (WINAPI * Real_closesocket)(SOCKET a0) = closesocket;

BOOL (WINAPI *Real_HttpSendRequestA)(HINTERNET, LPCSTR, DWORD, LPVOID, DWORD) = HttpSendRequestA;
BOOL (WINAPI *Real_HttpSendRequestW)(HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD) = HttpSendRequestW;
BOOL (WINAPI *Real_InternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD) = InternetReadFile;
BOOL (WINAPI *Real_InternetCloseHandle)(HINTERNET) = InternetCloseHandle;

BOOL (WSAAPI *Real_WSAGetOverlappedResult)(SOCKET, LPWSAOVERLAPPED, LPDWORD, BOOL, LPDWORD) = WSAGetOverlappedResult;

PTP_IO (WINAPI *Real_CreateThreadpoolIo)(HANDLE, PTP_WIN32_IO_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON) = CreateThreadpoolIo;
#endif

int WINAPI my_send (SOCKET s, const char FAR* buf, int len, int flags)
{
	typedef int (WINAPI *FNsend) (SOCKET s, const char FAR* buf, int len, int flags);
	auto fn = (FNsend) g_hooker->getOriginalFunction ((FARPROC)my_send);
	assert (fn);

	int rv = SOCKET_ERROR;
	try {
		rv = fn (s, buf, len, flags);
	}
	catch (...) {}

	if (rv != SOCKET_ERROR)
		_WinSockTrafficCollector.OnDataSent (s, buf, rv);

	return rv;
}

void CALLBACK my_WinsockCompletionRoutine (DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	_WinsockAsyncCalls.LockList (true);
	int iCall = _WinsockAsyncCalls.FindCallIndex (INVALID_SOCKET, lpOverlapped);
	assert (iCall != -1);
	const vmsOverlappedWinsockCalls::Call *pCall = NULL;
	if (iCall != -1)
		pCall = _WinsockAsyncCalls.getCall (iCall);
	assert (pCall != NULL);
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine = NULL;
	if (pCall)
	{
		assert (pCall->lpCompletionRoutine);
		lpCompletionRoutine = pCall->lpCompletionRoutine;

		if (dwError == 0)
		{
			DWORD dwBytesTransferred = cbTransferred;
			for (size_t i = 0; i < pCall->vBuffers.size () && dwBytesTransferred != 0; i++)
			{
				DWORD dwL = min (dwBytesTransferred, pCall->vBuffers [i].len);
				if (pCall->bSend)
					_WinSockTrafficCollector.OnDataSent (pCall->s, pCall->vBuffers [i].buf, dwL);
				else
					_WinSockTrafficCollector.OnDataRcvd (pCall->s, pCall->vBuffers [i].buf, dwL);
				dwBytesTransferred -= dwL;
			}
		}

		_WinsockAsyncCalls.RemoveCall (iCall);
	}

	_WinsockAsyncCalls.LockList (false);

	lpCompletionRoutine (dwError, cbTransferred, lpOverlapped, dwFlags);
}

void ProcessWsaOverlappedResult (SOCKET s, LPWSAOVERLAPPED lpOverlapped, BOOL bResult,
	DWORD dwBytesTransferred)
{
	if (bResult)
	{
		_WinsockAsyncCalls.LockList (true);
		int iCall = _WinsockAsyncCalls.FindCallIndex (s, lpOverlapped);
		const vmsOverlappedWinsockCalls::Call *pCall = NULL;
		if (iCall != -1)
			pCall = _WinsockAsyncCalls.getCall (iCall);
		if (pCall)
		{
			for (size_t i = 0; i < pCall->vBuffers.size () && dwBytesTransferred != 0; i++)
			{
				DWORD dwL = min (dwBytesTransferred, pCall->vBuffers [i].len);
				if (pCall->bSend)
					_WinSockTrafficCollector.OnDataSent (s, pCall->vBuffers [i].buf, dwL);
				else
					_WinSockTrafficCollector.OnDataRcvd (s, pCall->vBuffers [i].buf, dwL);
				dwBytesTransferred -= dwL;
			}
			_WinsockAsyncCalls.RemoveCall (iCall);
		}
		_WinsockAsyncCalls.LockList (false);
	}
	else
	{
		_WinsockAsyncCalls.LockList (true);
		int iCall = _WinsockAsyncCalls.FindCallIndex (s, lpOverlapped);
		if (iCall != -1)
			_WinsockAsyncCalls.RemoveCall (iCall);
		_WinsockAsyncCalls.LockList (false);
	}
}

BOOL WSAAPI my_WSAGetOverlappedResult (SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
{
	LOGFN ("my_WSAGetOverlappedResult");
	typedef BOOL (WSAAPI *FNWSAGetOverlappedResult) (SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
	auto fn = (FNWSAGetOverlappedResult) g_hooker->getOriginalFunction ((FARPROC)my_WSAGetOverlappedResult);
	assert (fn);

	BOOL bResult = FALSE;
	int lastError;

	try
	{
		bResult =  fn (s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
		lastError = WSAGetLastError ();
	}
	catch (...) {assert (false); return FALSE;}

	if (bResult || lastError != WSA_IO_INCOMPLETE)
	{
		assert (lpcbTransfer != NULL);
		ProcessWsaOverlappedResult (s, lpOverlapped, bResult, lpcbTransfer ? *lpcbTransfer : 0);
	}

	WSASetLastError (lastError);
	return bResult;
}

BOOL WINAPI my_GetQueuedCompletionStatus (HANDLE CompletionPort,
	LPDWORD lpNumberOfBytes,
	PULONG_PTR lpCompletionKey,
	LPOVERLAPPED *lpOverlapped,
	DWORD dwMilliseconds)
{
	typedef BOOL (WSAAPI *FNGetQueuedCompletionStatus) (HANDLE CompletionPort,
		LPDWORD lpNumberOfBytes,
		PULONG_PTR lpCompletionKey,
		LPOVERLAPPED *lpOverlapped,
		DWORD dwMilliseconds);
	auto fn = (FNGetQueuedCompletionStatus)g_hooker->getOriginalFunction ((FARPROC)my_GetQueuedCompletionStatus);
	assert (fn);
	return fn (CompletionPort, lpNumberOfBytes, lpCompletionKey,
		lpOverlapped, dwMilliseconds);
}

BOOL WINAPI my_GetQueuedCompletionStatusEx (
	_In_   HANDLE CompletionPort,
	_Out_  LPOVERLAPPED_ENTRY lpCompletionPortEntries,
	_In_   ULONG ulCount,
	_Out_  PULONG ulNumEntriesRemoved,
	_In_   DWORD dwMilliseconds,
	_In_   BOOL fAlertable
	)
{
	typedef BOOL (WSAAPI *FNGetQueuedCompletionStatusEx) (HANDLE CompletionPort,
		_Out_  LPOVERLAPPED_ENTRY lpCompletionPortEntries,
		_In_   ULONG ulCount,
		_Out_  PULONG ulNumEntriesRemoved,
		_In_   DWORD dwMilliseconds,
		_In_   BOOL fAlertable);
	auto fn = (FNGetQueuedCompletionStatusEx)g_hooker->getOriginalFunction ((FARPROC)my_GetQueuedCompletionStatusEx);
	assert (fn);
	return fn (CompletionPort, lpCompletionPortEntries, ulCount,
		ulNumEntriesRemoved, dwMilliseconds, fAlertable);
}

int WINAPI my_WSASend (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, 
					   LPDWORD lpNumberOfBytesSent, DWORD dwFlags, 
					   LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	typedef int (WINAPI *FNWSASend) (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount,
		LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
		LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	auto fn = (FNWSASend) g_hooker->getOriginalFunction ((FARPROC)my_WSASend);
	assert (fn);

	int rv = SOCKET_ERROR;
	int lastError;
	try {
		rv = fn (s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, 
			lpCompletionRoutine ? my_WinsockCompletionRoutine : nullptr);
		lastError = WSAGetLastError ();
	}
	catch (...) {return SOCKET_ERROR;}

	if (rv == SOCKET_ERROR)
	{
		if (lastError == WSA_IO_PENDING)
			_WinsockAsyncCalls.AddCall (s, lpBuffers, dwBufferCount, lpOverlapped, lpCompletionRoutine, true);
	}
	else
	{
		if (lpNumberOfBytesSent)
		{
			DWORD dwBytesSent = *lpNumberOfBytesSent;
			for (int i = 0; i < dwBufferCount && dwBytesSent != 0; i++)
			{
				DWORD dwL = min (dwBytesSent, lpBuffers [i].len);
				_WinSockTrafficCollector.OnDataSent (s, lpBuffers [i].buf, dwL);
				dwBytesSent -= dwL;
			}
		}
	}

	WSASetLastError (lastError);
	return rv;
}

int WINAPI my_recv (SOCKET s, char* buf, int len, int flags)
{
	LOGFN ("my_recv");
	typedef int (WINAPI *FNrecv) (SOCKET s, char* buf, int len, int flags);
	auto fn = (FNrecv) g_hooker->getOriginalFunction ((FARPROC)my_recv);
	assert (fn);

	int rv = SOCKET_ERROR;
	try {
		rv = fn (s, buf, len, flags);
	}
	catch (...) {}

	if (rv != SOCKET_ERROR)
		_WinSockTrafficCollector.OnDataRcvd (s, buf, rv);

	return rv;
}

int WINAPI my_WSARecv (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
                        LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	LOGFN ("my_WSARecv");
	assert ("lpCompletionRoutine is not tested" && !lpCompletionRoutine);
	typedef int (WINAPI *FNWSARecv) (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
		LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	auto fn = (FNWSARecv) g_hooker->getOriginalFunction ((FARPROC)my_WSARecv);
	assert (fn);

	int rv = SOCKET_ERROR;
	int lastError;
	try {
		rv = fn (s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, 
			lpCompletionRoutine ? my_WinsockCompletionRoutine : nullptr);
		lastError = WSAGetLastError ();
	}
	catch (...) {LOGsnl (" unexpected exception"); return SOCKET_ERROR;}

	assert (rv == 0 || rv == SOCKET_ERROR);

	if (rv == SOCKET_ERROR)
	{
		if (lastError == WSA_IO_PENDING)
		{
			_WinsockAsyncCalls.AddCall (s, lpBuffers, dwBufferCount, lpOverlapped, lpCompletionRoutine, false);
			LOGsnl (" io pending");
		}
	}
	else
	{
		assert (lpNumberOfBytesRecvd);
		if (lpNumberOfBytesRecvd)
		{
			DWORD dwBytesRecvd = *lpNumberOfBytesRecvd;

			for (int i = 0; i < dwBufferCount && dwBytesRecvd != 0; i++)
			{
				DWORD dwL = min (dwBytesRecvd, lpBuffers [i].len);
				if (dwL)
					_WinSockTrafficCollector.OnDataRcvd (s, lpBuffers [i].buf, dwL);
				dwBytesRecvd -= dwL;
			}
		}
	}

	WSASetLastError (lastError);	
	return rv;
}

int WINAPI my_closesocket (SOCKET s)
{
	typedef int (WINAPI *FNclosesocket) (SOCKET s);
	auto fn = (FNclosesocket) g_hooker->getOriginalFunction ((FARPROC)my_closesocket);
	assert (fn);

	_WinSockTrafficCollector.OnBeforeCloseSocket (s);
	return fn (s);
}

BOOL WINAPI my_InternetReadFile (HINTERNET hFile, LPVOID lpBuffer, DWORD dwToRead, LPDWORD pdwRead)
{
	typedef BOOL (WINAPI *FNInternetReadFile) (HINTERNET hFile, LPVOID lpBuffer, DWORD dwToRead, LPDWORD pdwRead);
	auto fn = (FNInternetReadFile) g_hooker->getOriginalFunction ((FARPROC)my_InternetReadFile);
	assert (fn);

	assert (pdwRead);
	BOOL bResult = fn (hFile, lpBuffer, dwToRead, pdwRead);
	if (bResult && dwToRead && pdwRead)
		_WinInetTrafficCollector.OnInternetReadFile (hFile, lpBuffer, *pdwRead);
	return bResult;
}

BOOL WINAPI my_InternetCloseHandle (HINTERNET hInternet)
{
	typedef BOOL (WINAPI *FNInternetCloseHandle) (HINTERNET hInternet);
	auto fn = (FNInternetCloseHandle) g_hooker->getOriginalFunction ((FARPROC)my_InternetCloseHandle);
	assert (fn);
	_WinInetTrafficCollector.OnInternetCloseHandle (hInternet);
	return fn (hInternet);
}

BOOL WINAPI my_HttpSendRequestA (HINTERNET hRequest, LPCSTR pszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	typedef BOOL (WINAPI *FNHttpSendRequestA) (HINTERNET hRequest, LPCSTR pszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
	auto fn = (FNHttpSendRequestA) g_hooker->getOriginalFunction ((FARPROC)my_HttpSendRequestA);
	assert (fn);
	if (dwOptionalLength)
		_WinInetTrafficCollector.OnSendPostData (hRequest, lpOptional, dwOptionalLength);
	return fn (hRequest, pszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}

BOOL WINAPI my_HttpSendRequestW (HINTERNET hRequest, LPCWSTR pwszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength)
{
	typedef BOOL (WINAPI *FNHttpSendRequestW) (HINTERNET hRequest, LPCWSTR pszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
	auto fn = (FNHttpSendRequestW)g_hooker->getOriginalFunction ((FARPROC)my_HttpSendRequestW);
	assert (fn);
	if (dwOptionalLength)
		_WinInetTrafficCollector.OnSendPostData (hRequest, lpOptional, dwOptionalLength);
	return fn (hRequest, pwszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}

unsigned g_ThisIsIeOfVersion = 0;

void CheckIfProcessIsInternetExplorer ()
{
	TCHAR tsz [MAX_PATH] = _T ("");
	GetModuleFileName (NULL, tsz, _countof (tsz));

	LPTSTR ptsz = _tcsrchr (tsz, '\\');
	assert (ptsz != NULL);
	if (!ptsz)
		return;

	if (_tcsicmp (ptsz + 1, _T ("iexplore.exe")))
		return;

	vmsBinaryFileVersionInfo bfvi;
	if (!bfvi.Retrieve (tsz))
		return;

	if (bfvi.m_appVersion.empty ())
		return;

	g_ThisIsIeOfVersion = bfvi.m_appVersion [0].dwVal;
}

struct myIoCompletionCallbackData
{
	HANDLE fl;
	PTP_WIN32_IO_CALLBACK pfnio;
	PVOID pv;
};

VOID CALLBACK myIoCompletionCallback (
	_Inout_      PTP_CALLBACK_INSTANCE Instance,
	_Inout_opt_  PVOID Context,
	_Inout_opt_  PVOID Overlapped,
	_In_         ULONG IoResult,
	_In_         ULONG_PTR NumberOfBytesTransferred,
	_Inout_      PTP_IO Io
	)
{
	auto data = reinterpret_cast <myIoCompletionCallbackData*> (Context);
	ProcessWsaOverlappedResult ((SOCKET)data->fl, (LPWSAOVERLAPPED)Overlapped, 
		!IoResult, (DWORD)NumberOfBytesTransferred);
	return data->pfnio (Instance, data->pv, Overlapped, IoResult, NumberOfBytesTransferred, Io);
}

PTP_IO WINAPI myCreateThreadpoolIo (
	_In_         HANDLE fl,
	_In_         PTP_WIN32_IO_CALLBACK pfnio,
	_Inout_opt_  PVOID pv,
	_In_opt_     PTP_CALLBACK_ENVIRON pcbe
	)
{
	typedef PTP_IO (WINAPI *FNCreateThreadpoolIo) (
		_In_         HANDLE fl,
		_In_         PTP_WIN32_IO_CALLBACK pfnio,
		_Inout_opt_  PVOID pv,
		_In_opt_     PTP_CALLBACK_ENVIRON pcbe
		);
	auto fn = (FNCreateThreadpoolIo)g_hooker->getOriginalFunction ((FARPROC)myCreateThreadpoolIo);

	auto data = new myIoCompletionCallbackData {fl, pfnio, pv};

	return fn (fl, myIoCompletionCallback, data, pcbe);
};

VOID WINAPI mySetThreadpoolWait (
	_Inout_  PTP_WAIT  pwa,
	_In_opt_ HANDLE    h,
	_In_opt_ PFILETIME pftTimeout
	)
{
	typedef VOID (WINAPI *FNSetThreadpoolWait) (
		_Inout_  PTP_WAIT  pwa,
		_In_opt_ HANDLE    h,
		_In_opt_ PFILETIME pftTimeout
		);
	auto fn = (FNSetThreadpoolWait)g_hooker->getOriginalFunction ((FARPROC)mySetThreadpoolWait);

	fn (pwa, h, pftTimeout);
}

HANDLE WINAPI myCreateIoCompletionPort (
	_In_     HANDLE    FileHandle,
	_In_opt_ HANDLE    ExistingCompletionPort,
	_In_     ULONG_PTR CompletionKey,
	_In_     DWORD     NumberOfConcurrentThreads
	)
{
	typedef HANDLE (WINAPI *FNCreateIoCompletionPort) (
		_In_     HANDLE    FileHandle,
		_In_opt_ HANDLE    ExistingCompletionPort,
		_In_     ULONG_PTR CompletionKey,
		_In_     DWORD     NumberOfConcurrentThreads
		);
	auto fn = (FNCreateIoCompletionPort)g_hooker->getOriginalFunction ((FARPROC)myCreateIoCompletionPort);

	return fn (FileHandle, ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);
}

void HookAPIs (bool bHook)
{
#ifndef USE_DETOURS
	if (bHook)
	{
		bool hook_winsock = true;

		if (g_ThisIsIeOfVersion >= 11)
		{
			vmsWinOsVersion osver;
			hook_winsock = osver.major_version () < 10;
		}

		g_hooker = std::make_shared <vmsPeFnHook> ();
		
		g_hooksFurther = std::make_unique <vmsHooksPeCallsForLoadingModules> (
			g_hooker, 
			g_oldOS ? vmsHooksPeCallsForLoadingModules::DontHookGetProcAddress : 0);
		g_hooksFurther->hook ();

		if (hook_winsock)
		{
			g_hooker->HookFunction ("Ws2_32.dll", "send", (FARPROC)my_send);
			g_hooker->HookFunction ("Ws2_32.dll", "WSASend", (FARPROC)my_WSASend);
			g_hooker->HookFunction ("Ws2_32.dll", "recv", (FARPROC)my_recv);
			g_hooker->HookFunction ("Ws2_32.dll", "WSARecv", (FARPROC)my_WSARecv);
			g_hooker->HookFunction ("Ws2_32.dll", "WSAGetOverlappedResult", (FARPROC)my_WSAGetOverlappedResult);
			g_hooker->HookFunction ("Ws2_32.dll", "closesocket", (FARPROC)my_closesocket);
			g_hooker->HookFunction ("kernel32.dll", "CreateThreadpoolIo", (FARPROC)myCreateThreadpoolIo);
		}
		
		g_hooker->HookFunction ("Wininet.dll", "HttpSendRequestA", (FARPROC)my_HttpSendRequestA);
		g_hooker->HookFunction ("Wininet.dll", "HttpSendRequestW", (FARPROC)my_HttpSendRequestW);
		g_hooker->HookFunction ("Wininet.dll", "InternetReadFile", (FARPROC)my_InternetReadFile);
		g_hooker->HookFunction ("Wininet.dll", "InternetCloseHandle", (FARPROC)my_InternetCloseHandle);
		
		
	}
	else
	{
		g_hooksFurther = nullptr;
		g_hooker->RemoveAllHooks ();
		g_hooker = nullptr;
	}
#else
	LONG (WINAPI *DetourFN)(PVOID*, PVOID) = bHook ? DetourAttach : DetourDetach;
	
	DetourTransactionBegin ();
	DetourUpdateThread (GetCurrentThread ());

	

	DetourFN ((PVOID*)&Real_CreateThreadpoolIo, myCreateThreadpoolIo);

	
	

	DetourTransactionCommit ();
#endif
}

DWORD WINAPI _threadFlvSniffDllWnd (LPVOID)
{
	Sleep (3000);
	vmsSniffDllWnd wnd;
	wnd.Create ();
	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
		DispatchMessage (&msg);
	return 0;
}

void CreateFlvSniffDllWnd ()
{
	DWORD dw;
	CloseHandle (
		CreateThread (NULL, 0, _threadFlvSniffDllWnd, NULL, 0, &dw));
}

HANDLE _hevShuttingDown = CreateEvent (NULL, TRUE, FALSE, NULL);
HANDLE _htTrafficCollectorCleanuper = NULL;

DWORD WINAPI _threadTrafficCollectorCleanuper (LPVOID)
{
	
	const int RECHECK_INTERVAL = 10;
	const int HTTPTRAFF_KEEPDLGSTIME = 10;
	const int DLGTREE_KEEPDLGSTIME = 15;
	const int DLGTREE_KEEPWEBPAGESANDFLVSTIME = 2*60;

	while (WaitForSingleObject (_hevShuttingDown, RECHECK_INTERVAL * 60 * 1000) == WAIT_TIMEOUT)
	{
		_HttpTraffic.RemoveAllDialogsOlderWhen (HTTPTRAFF_KEEPDLGSTIME * 60);
		vmsWinSockHttpDlgTree::o ().RemoveAllDialogsOlderWhen (DLGTREE_KEEPDLGSTIME * 60, DLGTREE_KEEPWEBPAGESANDFLVSTIME * 60);
	}
	
	return 0;
}

#ifdef LOG_WEBFILES_TREE
void log_webfiles_init ()
{
	TCHAR tszPath [MAX_PATH] = _T ("");
	GetTempPath (MAX_PATH, tszPath);
	_tcscat (tszPath, _T ("\\fdmflvsniff"));

	SHFILEOPSTRUCT sfos = {0};
	sfos.wFunc = FO_DELETE;
	sfos.pFrom = tszPath;
	sfos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	SHFileOperation (&sfos);
}
void log_webfiles_init2 ()
{
	TCHAR tszPath [MAX_PATH] = _T ("");
	GetTempPath (MAX_PATH, tszPath);
	_tcscat (tszPath, _T ("\\fdmflvsniff"));
		
	CreateDirectory (tszPath, NULL);
	
	_tcscat (tszPath, _T ("\\webfiles"));
	CreateDirectory (tszPath, NULL);
}
#endif

DWORD WINAPI _threadInitialize (LPVOID)
{
	initializeCrashReporter ();
	

	#ifdef LOG_WEBFILES_TREE
		log_webfiles_init ();
	#endif

	vmsUrlMonSpyDll::MakeSureDllLoaded ();

	return 0;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		

		_hModule = (HMODULE)hModule;
		g_oldOS = !vmsWinOsVersion ().isVistaOrHigher ();

		CheckIfProcessIsInternetExplorer ();

		DWORD dw;
		CloseHandle  (
			CreateThread (NULL, 0, _threadInitialize, NULL, 0, &dw));
	
		HookAPIs (true);

		CreateFlvSniffDllWnd ();

		_htTrafficCollectorCleanuper = CreateThread (NULL, 0, _threadTrafficCollectorCleanuper, NULL, 0, &dw);
	}
	
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		if (lpReserved)
			new std::shared_ptr <vmsPeFnHook> (g_hooker);
		SetEvent (_hevShuttingDown);
		HookAPIs (false);
		WaitForSingleObject (_htTrafficCollectorCleanuper, INFINITE);
	}
	
    return TRUE;
}

#if defined (LOG_WEBFILES_TREE) || defined (SCL_ENABLE)
LONG _cInOnGetItBtnClicked = 0;
#endif

HRESULT WINAPI onGetItBtnClicked3 (LPCSTR pszUrl, LPCSTR pszFrameUrl, LPCSTR pszSwfUrl, LPCSTR pszFlashVars, LPCSTR pszOtherSwfUrls, LPCSTR pszOtherFlashVars)
{
	LOGFN ("onGetItBtnClicked3");

#ifdef LOG_WEBFILES_TREE
	InterlockedIncrement (&_cInOnGetItBtnClicked);
#endif

	vmsFindFlvDownloadsResultsCache::ResultPtr result;
	HRESULT hr = _FlvResCache.FindFlvDownloads (pszUrl, pszFrameUrl, pszSwfUrl, pszFlashVars, pszOtherSwfUrls, 
		pszOtherFlashVars, result);

#ifdef LOG_WEBFILES_TREE
	InterlockedDecrement (&_cInOnGetItBtnClicked);
#endif

	if (hr != S_OK)
		return hr;

	assert (result->pTa->get_FlvDownloadCount () != 0);
	if (!result->pTa->get_FlvDownloadCount ())
		return S_FALSE;
	
	return vmsTransferFlvDownloadsToFdm::Do (pszUrl, result->pTa->getFlvDownloads ());
}

HRESULT WINAPI onGetItBtnClicked2 (LPCSTR pszUrl, LPCSTR pszSwfUrl, LPCSTR pszFlashVars)
{
	return onGetItBtnClicked3 (pszUrl, NULL, pszSwfUrl, pszFlashVars, NULL, NULL);
}

HRESULT WINAPI onGetItBtnClicked (LPCSTR pszUrl)
{
	return onGetItBtnClicked2 (pszUrl, NULL, NULL);
}

BOOL WINAPI isVideoFlash (LPCSTR pszWebPageUrl, LPCSTR pszFrameUrl, 
						  LPCSTR pszSwfUrl, LPCSTR pszFlashVars,
						  LPCSTR pszOtherSwfUrls, LPCSTR pszOtherFlashVars)
{
	LOGFN ("isVideoFlash");
	LOG (" Page URL: %s", pszWebPageUrl);
	LOG (" Frame URL: %s", pszFrameUrl);
	LOG (" Swf URL: %s", pszSwfUrl);
	LOG (" Flash Vars: %s", pszFlashVars);
	LOG (" Other Swf URLs: %s", pszOtherSwfUrls);
	LOG (" OtherFlashVars: %s", pszOtherFlashVars);
	
	vmsFindFlvDownloadsResultsCache::ResultPtr result;

	_FlvResCache.CheckNeedCacheReset ();
	result = _FlvResCache.FindResult (pszWebPageUrl, pszFrameUrl, pszSwfUrl, pszFlashVars);

	if (result)
		return result->pTa->get_FlvDownloadCount () != 0;

	vmsFindFlvDownloadsResultsCacheAsyncRequest *pRequest = new vmsFindFlvDownloadsResultsCacheAsyncRequest;
	pRequest->m_strWebPageUrl = pszWebPageUrl;
	pRequest->m_strFrameUrl = pszFrameUrl;
	pRequest->m_strSwfUrl = pszSwfUrl;
	pRequest->m_strFlashVars = pszFlashVars;
	pRequest->m_strOtherSwfUrls = pszOtherSwfUrls;
	pRequest->m_strOtherFlashVars = pszOtherFlashVars;
	if (_AsyncTasks.HasEqualTask (pRequest, NULL))
		return FALSE;

	HANDLE hevDone = CreateEvent (NULL, TRUE, FALSE, NULL);
	pRequest->setOnDoneEvent (hevDone);
	pRequest->setFlags (vmsAsyncTask::RemoveWhenDone);

	tsp_vmsAsyncTask spTask (pRequest);
	_AsyncTasks.AddTask (spTask);

	if (WAIT_TIMEOUT == WaitForSingleObject (hevDone, 300))
		return FALSE;

	result = pRequest->m_result;
	
	return result != NULL && result->pTa->get_FlvDownloadCount () != 0;
}

UINT WINAPI onNewHttpDialog (LPCSTR pszUrl, LPCSTR pszRequestHdrs, LPCSTR pszRespHdrs)
{
	assert (pszUrl != NULL && pszRequestHdrs != NULL && pszRespHdrs != NULL);
	if (!pszUrl || !pszRequestHdrs || !pszRespHdrs)
		return (UINT)-1;
	return _ExternalTrafficCollector.OnNewHttpDialog (pszUrl, pszRequestHdrs, pszRespHdrs);
}

void WINAPI onHttpDialogDataReceived (UINT nUID, LPBYTE pbData, UINT nSize)
{
	assert (nUID != (UINT)-1);
	if (nUID == (UINT)-1)
		return;
	_ExternalTrafficCollector.OnHttpDialogDataReceived (nUID, pbData, nSize);
}

void WINAPI onHttpDialogClosed (UINT nUID)
{
	assert (nUID != (UINT)-1);
	if (nUID == (UINT)-1)
		return;
	_ExternalTrafficCollector.OnHttpDialogClosed (nUID);
}

void WINAPI onNewHttpRequest (LPCSTR pszUrl, LPCSTR pszSrcTabUrl)
{
	assert (pszUrl != NULL && pszSrcTabUrl != NULL);
	if (!pszUrl || !pszSrcTabUrl)
		return;
	LOGsnl ("onNewHttpRequest:");
	LOG (" url: %s", pszUrl);
	LOG (" srcTab: %s", pszSrcTabUrl);
	vmsBrowserSpecialInfo::UrlInfo url;
	url.strUrl = pszUrl;
	url.strSrcTabUrl = pszSrcTabUrl;
	vmsBrowserSpecialInfo::o ().AddUrl (url);
}