/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>

#ifndef COINIT_DISABLE_OLE1DDE
#define COINIT_DISABLE_OLE1DDE	(0x4)
#endif

#ifndef _X86_

#define LLtoU64(x) (*(unsigned __int64*)(void*)(&(x)))

__inline
ULONG
WINAPI
EnlargedUnsignedDivide (
    IN ULARGE_INTEGER Dividend,
    IN ULONG Divisor,
    IN PULONG Remainder
    )
{
        
        if (Remainder != NULL)
                *Remainder = (ULONG)(LLtoU64(Dividend) % Divisor);
        return (ULONG)(LLtoU64(Dividend) / Divisor);
}

#else
__inline
ULONG
WINAPI
EnlargedUnsignedDivide (
    IN ULARGE_INTEGER Dividend,
    IN ULONG Divisor,
    IN PULONG Remainder
    )
{
    ULONG ulResult;
    _asm {
        mov eax,Dividend.LowPart
        mov edx,Dividend.HighPart
        mov ecx,Remainder
        div Divisor
        or  ecx,ecx
        jz  short label
        mov [ecx],edx
label:
        mov ulResult,eax
    }
    return ulResult;
}
#endif

CAMEvent::CAMEvent(BOOL fManualReset)
{
    m_hEvent = CreateEvent(NULL, fManualReset, FALSE, NULL);
    ASSERT(m_hEvent);
}

CAMEvent::~CAMEvent()
{
    if (m_hEvent) {
	EXECUTE_ASSERT(CloseHandle(m_hEvent));
    }
}

BOOL CAMMsgEvent::WaitMsg(DWORD dwTimeout)
{
    
    
    
    DWORD dwWait;
    DWORD dwStartTime;

    
    DWORD dwWaitTime = dwTimeout;

    
    
    
    if (dwWaitTime != INFINITE) {
        dwStartTime = timeGetTime();
    }

    do {
        dwWait = MsgWaitForMultipleObjects(1,&m_hEvent,FALSE, dwWaitTime, QS_SENDMESSAGE);
        if (dwWait == WAIT_OBJECT_0 + 1) {
	    MSG Message;
            PeekMessage(&Message,NULL,0,0,PM_NOREMOVE);

	    
	    
	    
	    if (dwWaitTime != INFINITE) {

		DWORD dwElapsed = timeGetTime()-dwStartTime;

		dwWaitTime =
		    (dwElapsed >= dwTimeout)
			? 0  
			: dwTimeout-dwElapsed;
	    }
        }
    } while (dwWait == WAIT_OBJECT_0 + 1);

    
    
    return (dwWait == WAIT_OBJECT_0);
}

CAMThread::CAMThread()
    : m_EventSend(TRUE)     
{
    m_hThread = NULL;
}

CAMThread::~CAMThread() {
    Close();
}

DWORD WINAPI
CAMThread::InitialThreadProc(LPVOID pv)
{
    HRESULT hrCoInit = CAMThread::CoInitializeHelper();
    if(FAILED(hrCoInit)) {
        DbgLog((LOG_ERROR, 1, TEXT("CoInitializeEx failed.")));
    }

    CAMThread * pThread = (CAMThread *) pv;

    HRESULT hr = pThread->ThreadProc();

    if(SUCCEEDED(hrCoInit)) {
        CoUninitialize();
    }

    return hr;
}

BOOL
CAMThread::Create()
{
    DWORD threadid;

    CAutoLock lock(&m_AccessLock);

    if (ThreadExists()) {
	return FALSE;
    }

    m_hThread = CreateThread(
		    NULL,
		    0,
		    CAMThread::InitialThreadProc,
		    this,
		    0,
		    &threadid);

    if (!m_hThread) {
	return FALSE;
    }

    return TRUE;
}

DWORD
CAMThread::CallWorker(DWORD dwParam)
{
    
    CAutoLock lock(&m_AccessLock);

    if (!ThreadExists()) {
	return (DWORD) E_FAIL;
    }

    
    m_dwParam = dwParam;

    
    m_EventSend.Set();

    
    m_EventComplete.Wait();

    
    return m_dwReturnVal;
}

DWORD
CAMThread::GetRequest()
{
    m_EventSend.Wait();
    return m_dwParam;
}

BOOL
CAMThread::CheckRequest(DWORD * pParam)
{
    if (!m_EventSend.Check()) {
	return FALSE;
    } else {
	if (pParam) {
	    *pParam = m_dwParam;
	}
	return TRUE;
    }
}

void
CAMThread::Reply(DWORD dw)
{
    m_dwReturnVal = dw;

    
    
    
    
    
    

    m_EventSend.Reset();

    

    m_EventComplete.Set();
}

HRESULT CAMThread::CoInitializeHelper()
{
    
    
    
    
    
    
    
    
    

    

    HRESULT hr = E_FAIL;
    HINSTANCE hOle = GetModuleHandle(TEXT("ole32.dll"));
    if(hOle)
    {
        typedef HRESULT (STDAPICALLTYPE *PCoInitializeEx)(
            LPVOID pvReserved, DWORD dwCoInit);
        PCoInitializeEx pCoInitializeEx =
            (PCoInitializeEx)(GetProcAddress(hOle, "CoInitializeEx"));
        if(pCoInitializeEx)
        {
            hr = (*pCoInitializeEx)(0, COINIT_DISABLE_OLE1DDE );
        }
    }
    else
    {
        
        DbgBreak("couldn't locate ole32.dll");
    }

    return hr;
}

CMsgThread::~CMsgThread()
{
    if (m_hThread != NULL) {
        WaitForSingleObject(m_hThread, INFINITE);
        EXECUTE_ASSERT(CloseHandle(m_hThread));
    }

    POSITION pos = m_ThreadQueue.GetHeadPosition();
    while (pos) {
        CMsg * pMsg = m_ThreadQueue.GetNext(pos);
        delete pMsg;
    }
    m_ThreadQueue.RemoveAll();

    if (m_hSem != NULL) {
        EXECUTE_ASSERT(CloseHandle(m_hSem));
    }
}

BOOL
CMsgThread::CreateThread(
    )
{
    m_hSem = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
    if (m_hSem == NULL) {
        return FALSE;
    }

    m_hThread = ::CreateThread(NULL, 0, DefaultThreadProc,
			       (LPVOID)this, 0, &m_ThreadId);
    return m_hThread != NULL;
}

DWORD WINAPI
CMsgThread::DefaultThreadProc(
    LPVOID lpParam
    )
{
    CMsgThread *lpThis = (CMsgThread *)lpParam;
    CMsg msg;
    LRESULT lResult;

    
    CoInitialize(NULL);

    
    lpThis->OnThreadInit();

    do {
	lpThis->GetThreadMsg(&msg);
	lResult = lpThis->ThreadMessageProc(msg.uMsg,msg.dwFlags,
					    msg.lpParam, msg.pEvent);
    } while (lResult == 0L);

    
    CoUninitialize();

    return (DWORD)lResult;
}

void
CMsgThread::GetThreadMsg(CMsg *msg)
{
    CMsg * pmsg = NULL;

    
    while (TRUE) {
        {
            CAutoLock lck(&m_Lock);
            pmsg = m_ThreadQueue.RemoveHead();
            if (pmsg == NULL) {
                m_lWaiting++;
            } else {
                break;
            }
        }
        
        WaitForSingleObject(m_hSem, INFINITE);
    }
    
    *msg = *pmsg;

    
    delete pmsg;

}

#ifndef UNICODE

LPWSTR
WINAPI
lstrcpyWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2
    )
{
    LPWSTR  lpReturn = lpString1;
    while (*lpString1++ = *lpString2++);

    return lpReturn;
}

LPWSTR
WINAPI
lstrcpynWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2,
    int     iMaxLength
    )
{
    ASSERT(iMaxLength);
    LPWSTR  lpReturn = lpString1;
    if (iMaxLength) {
        while (--iMaxLength && (*lpString1++ = *lpString2++));

        
        
        
        if (!iMaxLength) *lpString1 = L'\0';
    }
    return lpReturn;
}

int
WINAPI
lstrcmpWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    )
{
    do {
	WCHAR c1 = *lpString1;
	WCHAR c2 = *lpString2;
	if (c1 != c2)
	    return (int) c1 - (int) c2;
    } while (*lpString1++ && *lpString2++);
    return 0;
}

int
WINAPI
lstrcmpiWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    )
{
    do {
	WCHAR c1 = *lpString1;
	WCHAR c2 = *lpString2;
	if (c1 >= L'A' && c1 <= L'Z')
	    c1 -= (WCHAR) (L'A' - L'a');
	if (c2 >= L'A' && c2 <= L'Z')
	    c2 -= (WCHAR) (L'A' - L'a');
	
	if (c1 != c2)
	    return (int) c1 - (int) c2;
    } while (*lpString1++ && *lpString2++);

    return 0;
}

int
WINAPI
lstrlenWInternal(
    LPCWSTR lpString
    )
{
    int i = -1;
    while (*(lpString+(++i)))
        ;
    return i;
}

int WINAPIV wsprintfWInternal(LPWSTR wszOut, LPCWSTR pszFmt, ...)
{
    char fmt[256]; 
    char ach[256]; 
    int i;

    va_list va;
    va_start(va, pszFmt);
    WideCharToMultiByte(GetACP(), 0, pszFmt, -1, fmt, 256, NULL, NULL);
    i = wvsprintfA(ach, fmt, va);
    va_end(va);

    MultiByteToWideChar(CP_ACP, 0, ach, -1, wszOut, i+1);

    return i;
}
#else

LPWSTR WINAPI lstrcpyWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2
    )
{
    return lstrcpyW(lpString1, lpString2);
}

LPWSTR WINAPI lstrcpynWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2,
    int     iMaxLength
    )
{
    return lstrcpynW(lpString1, lpString2, iMaxLength);
}

int WINAPI lstrcmpWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    )
{
    return lstrcmpW(lpString1, lpString2);
}

int WINAPI lstrcmpiWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    )
{
    return lstrcmpiW(lpString1, lpString2);
}

int WINAPI lstrlenWInternal(
    LPCWSTR lpString
    )
{
    return lstrlenW(lpString);
}

int WINAPIV wsprintfWInternal(
    LPWSTR wszOut, LPCWSTR pszFmt, ...)
{
    va_list va;
    va_start(va, pszFmt);
    int i = wvsprintfW(wszOut, pszFmt, va);
    va_end(va);
    return i;
}
#endif

void WINAPI IntToWstr(int i, LPWSTR wstr)
{
#ifdef UNICODE
    wsprintf(wstr, L"%d", i);
#else
    TCHAR temp[32];
    wsprintf(temp, "%d", i);
    MultiByteToWideChar(CP_ACP, 0, temp, -1, wstr, 32);
#endif
} 

#if 0
void * memchrInternal(const void *pv, int c, size_t sz)
{
    BYTE *pb = (BYTE *) pv;
    while (sz--) {
	if (*pb == c)
	    return (void *) pb;
	pb++;
    }
    return NULL;
}
#endif

#define MEMORY_ALIGNMENT        4
#define MEMORY_ALIGNMENT_LOG2   2
#define MEMORY_ALIGNMENT_MASK   MEMORY_ALIGNMENT - 1

void * __stdcall memmoveInternal(void * dst, const void * src, size_t count)
{
    void * ret = dst;

#ifdef _X86_
    if (dst <= src || (char *)dst >= ((char *)src + count)) {

        
        _asm {
            mov     esi,src
            mov     edi,dst
            mov     ecx,count
            cld
            mov     edx,ecx
            and     edx,MEMORY_ALIGNMENT_MASK
            shr     ecx,MEMORY_ALIGNMENT_LOG2
            rep     movsd
            or      ecx,edx
            jz      memmove_done
            rep     movsb
memmove_done:
        }
    }
    else {

        
        _asm {
            mov     esi,src
            mov     edi,dst
            mov     ecx,count
            std
            add     esi,ecx
            add     edi,ecx
            dec     esi
            dec     edi
            rep     movsb
            cld
        }
    }
#else
    MoveMemory(dst, src, count);
#endif

    return ret;
}

#ifdef _M_ALPHA

#undef UInt32x32To64
#define UInt32x32To64(a, b) (((ULONGLONG)((ULONG)(a)) & 0xffffffff) * ((ULONGLONG)((ULONG)(b)) & 0xffffffff))
#endif

LONGLONG WINAPI llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG d)
{
    
    ULARGE_INTEGER ua, ub;
    DWORDLONG uc;

    ua.QuadPart = (DWORDLONG)(a >= 0 ? a : -a);
    ub.QuadPart = (DWORDLONG)(b >= 0 ? b : -b);
    uc          = (DWORDLONG)(c >= 0 ? c : -c);
    BOOL bSign = (a < 0) ^ (b < 0);

    
    ULARGE_INTEGER p[2];
    p[0].QuadPart  = UInt32x32To64(ua.LowPart, ub.LowPart);

    

    ULARGE_INTEGER x;
    x.QuadPart     = UInt32x32To64(ua.LowPart, ub.HighPart) +
                     UInt32x32To64(ua.HighPart, ub.LowPart) +
                     p[0].HighPart;
    p[0].HighPart  = x.LowPart;
    p[1].QuadPart  = UInt32x32To64(ua.HighPart, ub.HighPart) + x.HighPart;

    if (d != 0) {
        ULARGE_INTEGER ud[2];
        if (bSign) {
            ud[0].QuadPart = (DWORDLONG)(-d);
            if (d > 0) {
                
                ud[1].QuadPart = (DWORDLONG)(LONGLONG)-1;
            } else {
                ud[1].QuadPart = (DWORDLONG)0;
            }
        } else {
            ud[0].QuadPart = (DWORDLONG)d;
            if (d < 0) {
                ud[1].QuadPart = (DWORDLONG)(LONGLONG)-1;
            } else {
                ud[1].QuadPart = (DWORDLONG)0;
            }
        }
        
        ULARGE_INTEGER uliTotal;

        
        uliTotal.QuadPart  = (DWORDLONG)ud[0].LowPart + p[0].LowPart;
        p[0].LowPart       = uliTotal.LowPart;

        
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        
        uliTotal.QuadPart += (DWORDLONG)ud[0].HighPart + p[0].HighPart;
        p[0].HighPart      = uliTotal.LowPart;

        
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        
        p[1].QuadPart     += ud[1].QuadPart + uliTotal.QuadPart;

        
        if ((LONG)p[1].HighPart < 0) {
            bSign = !bSign;

            
            p[0].QuadPart  = ~p[0].QuadPart;
            p[1].QuadPart  = ~p[1].QuadPart;
            p[0].QuadPart += 1;
            p[1].QuadPart += (p[0].QuadPart == 0);
        }
    }

    
    if (c < 0) {
        bSign = !bSign;
    }

    
    if (uc <= p[1].QuadPart) {
        return bSign ? (LONGLONG)0x8000000000000000 :
                       (LONGLONG)0x7FFFFFFFFFFFFFFF;
    }

    DWORDLONG ullResult;

    
    
    if (p[1].QuadPart == 0) {
        ullResult = p[0].QuadPart / uc;
        return bSign ? -(LONGLONG)ullResult : (LONGLONG)ullResult;
    }

    
    ULARGE_INTEGER ulic;
    ulic.QuadPart = uc;
    if (ulic.HighPart == 0) {
        ULARGE_INTEGER uliDividend;
        ULARGE_INTEGER uliResult;
        DWORD dwDivisor = (DWORD)uc;
        
        uliDividend.HighPart = p[1].LowPart;
        uliDividend.LowPart = p[0].HighPart;
#ifndef USE_LARGEINT
        uliResult.HighPart = (DWORD)(uliDividend.QuadPart / dwDivisor);
        p[0].HighPart = (DWORD)(uliDividend.QuadPart % dwDivisor);
        uliResult.LowPart = 0;
        uliResult.QuadPart = p[0].QuadPart / dwDivisor + uliResult.QuadPart;
#else
        
        if (uliDividend.QuadPart >= (DWORDLONG)dwDivisor) {
            uliResult.HighPart = EnlargedUnsignedDivide(
                                     uliDividend,
                                     dwDivisor,
                                     &p[0].HighPart);
        } else {
            uliResult.HighPart = 0;
        }
        uliResult.LowPart = EnlargedUnsignedDivide(
                                 p[0],
                                 dwDivisor,
                                 NULL);
#endif
        return bSign ? -(LONGLONG)uliResult.QuadPart :
                        (LONGLONG)uliResult.QuadPart;
    }

    ullResult = 0;

    
    for (int i = 0; i < 64; i++) {
        ullResult <<= 1;

        
        p[1].QuadPart <<= 1;
        if ((p[0].HighPart & 0x80000000) != 0) {
            p[1].LowPart++;
        }
        p[0].QuadPart <<= 1;

        
        if (uc <= p[1].QuadPart) {
            p[1].QuadPart -= uc;
            ullResult += 1;
        }
    }

    return bSign ? - (LONGLONG)ullResult : (LONGLONG)ullResult;
}

LONGLONG WINAPI Int64x32Div32(LONGLONG a, LONG b, LONG c, LONG d)
{
    ULARGE_INTEGER ua;
    DWORD ub;
    DWORD uc;

    
    ua.QuadPart = (DWORDLONG)(a >= 0 ? a : -a);
    ub = (DWORD)(b >= 0 ? b : -b);
    uc = (DWORD)(c >= 0 ? c : -c);
    BOOL bSign = (a < 0) ^ (b < 0);

    
    ULARGE_INTEGER p0;
    DWORD p1;
    p0.QuadPart  = UInt32x32To64(ua.LowPart, ub);

    if (ua.HighPart != 0) {
        ULARGE_INTEGER x;
        x.QuadPart     = UInt32x32To64(ua.HighPart, ub) + p0.HighPart;
        p0.HighPart  = x.LowPart;
        p1   = x.HighPart;
    } else {
        p1 = 0;
    }

    if (d != 0) {
        ULARGE_INTEGER ud0;
        DWORD ud1;

        if (bSign) {
            
            
            
            
            ud0.QuadPart = (DWORDLONG)(-(LONGLONG)d);
            if (d > 0) {
                
                ud1 = (DWORD)-1;
            } else {
                ud1 = (DWORD)0;
            }
        } else {
            ud0.QuadPart = (DWORDLONG)d;
            if (d < 0) {
                ud1 = (DWORD)-1;
            } else {
                ud1 = (DWORD)0;
            }
        }
        
        ULARGE_INTEGER uliTotal;

        
        uliTotal.QuadPart  = (DWORDLONG)ud0.LowPart + p0.LowPart;
        p0.LowPart       = uliTotal.LowPart;

        
        uliTotal.LowPart   = uliTotal.HighPart;
        uliTotal.HighPart  = 0;

        
        uliTotal.QuadPart += (DWORDLONG)ud0.HighPart + p0.HighPart;
        p0.HighPart      = uliTotal.LowPart;

        
        p1 += ud1 + uliTotal.HighPart;

        
        if ((LONG)p1 < 0) {
            bSign = !bSign;

            
            p0.QuadPart  = ~p0.QuadPart;
            p1 = ~p1;
            p0.QuadPart += 1;
            p1 += (p0.QuadPart == 0);
        }
    }

    
    if (c < 0) {
        bSign = !bSign;
    }

    
    if (uc <= p1) {
        return bSign ? (LONGLONG)0x8000000000000000 :
                       (LONGLONG)0x7FFFFFFFFFFFFFFF;
    }

    

    
    ULARGE_INTEGER uliDividend;
    ULARGE_INTEGER uliResult;
    DWORD dwDivisor = uc;
    uliDividend.HighPart = p1;
    uliDividend.LowPart = p0.HighPart;
    
    if (uliDividend.QuadPart >= (DWORDLONG)dwDivisor) {
        uliResult.HighPart = EnlargedUnsignedDivide(
                                 uliDividend,
                                 dwDivisor,
                                 &p0.HighPart);
    } else {
        uliResult.HighPart = 0;
    }
    uliResult.LowPart = EnlargedUnsignedDivide(
                             p0,
                             dwDivisor,
                             NULL);
    return bSign ? -(LONGLONG)uliResult.QuadPart :
                    (LONGLONG)uliResult.QuadPart;
}

#ifdef DEBUG

CCritSec::CCritSec()
{
    InitializeCriticalSection(&m_CritSec);
    m_currentOwner = m_lockCount = 0;
    m_fTrace = FALSE;
}

CCritSec::~CCritSec()
{
    DeleteCriticalSection(&m_CritSec);
}

void CCritSec::Lock()
{
    UINT tracelevel=3;
    DWORD us = GetCurrentThreadId();
    DWORD currentOwner = m_currentOwner;
    if (currentOwner && (currentOwner != us)) {
        
        if (m_fTrace) {
            DbgLog((LOG_LOCKING, 2, TEXT("Thread %d about to wait for lock %x owned by %d"),
                GetCurrentThreadId(), &m_CritSec, currentOwner));
            tracelevel=2;
	        
	        
	        
        }
    }
    EnterCriticalSection(&m_CritSec);
    if (0 == m_lockCount++) {
        
        m_currentOwner = us;

        if (m_fTrace) {
            DbgLog((LOG_LOCKING, tracelevel, TEXT("Thread %d now owns lock %x"), m_currentOwner, &m_CritSec));
        }
    }
}

void CCritSec::Unlock() {
    if (0 == --m_lockCount) {
        
        if (m_fTrace) {
            DbgLog((LOG_LOCKING, 3, TEXT("Thread %d releasing lock %x"), m_currentOwner, &m_CritSec));
        }

        m_currentOwner = 0;
    }
    LeaveCriticalSection(&m_CritSec);
}

void WINAPI DbgLockTrace(CCritSec * pcCrit, BOOL fTrace)
{
    pcCrit->m_fTrace = fTrace;
}

BOOL WINAPI CritCheckIn(CCritSec * pcCrit)
{
    return (GetCurrentThreadId() == pcCrit->m_currentOwner);
}

BOOL WINAPI CritCheckIn(const CCritSec * pcCrit)
{
    return (GetCurrentThreadId() == pcCrit->m_currentOwner);
}

BOOL WINAPI CritCheckOut(CCritSec * pcCrit)
{
    return (GetCurrentThreadId() != pcCrit->m_currentOwner);
}

BOOL WINAPI CritCheckOut(const CCritSec * pcCrit)
{
    return (GetCurrentThreadId() != pcCrit->m_currentOwner);
}
#endif

STDAPI WriteBSTR(BSTR *pstrDest, LPCWSTR szSrc)
{
    *pstrDest = SysAllocString( szSrc );
    if( !(*pstrDest) ) return E_OUTOFMEMORY;
    return NOERROR;
}

STDAPI FreeBSTR(BSTR* pstr)
{
    if( *pstr == NULL ) return S_FALSE;
    SysFreeString( *pstr );
    return NOERROR;
}

STDAPI AMGetWideString(LPCWSTR psz, LPWSTR *ppszReturn)
{
    CheckPointer(ppszReturn, E_POINTER);
    ValidateReadWritePtr(ppszReturn, sizeof(LPWSTR));
    DWORD nameLen = sizeof(WCHAR) * (lstrlenW(psz)+1);
    *ppszReturn = (LPWSTR)CoTaskMemAlloc(nameLen);
    if (*ppszReturn == NULL) {
       return E_OUTOFMEMORY;
    }
    CopyMemory(*ppszReturn, psz, nameLen);
    return NOERROR;
}

DWORD WINAPI WaitDispatchingMessages(
    HANDLE hObject,
    DWORD dwWait,
    HWND hwnd,
    UINT uMsg,
    HANDLE hEvent)
{
    BOOL bPeeked = FALSE;
    DWORD dwResult;
    DWORD dwStart;
    DWORD dwThreadPriority;

    static UINT uMsgId = 0;

    HANDLE hObjects[2] = { hObject, hEvent };
    if (dwWait != INFINITE && dwWait != 0) {
        dwStart = GetTickCount();
    }
    for (; ; ) {
        DWORD nCount = NULL != hEvent ? 2 : 1;

        
        
        dwResult = WaitForMultipleObjects(nCount, hObjects, FALSE, 0);
        if (dwResult < WAIT_OBJECT_0 + nCount) {
            return dwResult;
        }

        DWORD dwTimeOut = dwWait;
        if (dwTimeOut > 10) {
            dwTimeOut = 10;
        }
        dwResult = MsgWaitForMultipleObjects(
                             nCount,
                             hObjects,
                             FALSE,
                             dwTimeOut,
                             hwnd == NULL ? QS_SENDMESSAGE :
                                            QS_SENDMESSAGE + QS_POSTMESSAGE);
        if (dwResult == WAIT_OBJECT_0 + nCount ||
            dwResult == WAIT_TIMEOUT && dwTimeOut != dwWait) {
            MSG msg;
            if (hwnd != NULL) {
                while (PeekMessage(&msg, hwnd, uMsg, uMsg, PM_REMOVE)) {
                    DispatchMessage(&msg);
                }
            }
            
            
            PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

            if (dwWait != INFINITE && dwWait != 0) {
                DWORD dwNow = GetTickCount();

                
                DWORD dwDiff = dwNow - dwStart;
                if (dwDiff > dwWait) {
                    dwWait = 0;
                } else {
                    dwWait -= dwDiff;
                }
                dwStart = dwNow;
            }
            if (!bPeeked) {
                
                
                dwThreadPriority = GetThreadPriority(GetCurrentThread());
                if (dwThreadPriority < THREAD_PRIORITY_HIGHEST) {
                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
                }
                bPeeked = TRUE;
            }
        } else {
            break;
        }
    }
    if (bPeeked) {
        SetThreadPriority(GetCurrentThread(), dwThreadPriority);
        if (HIWORD(GetQueueStatus(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
            if (uMsgId == 0) {
                uMsgId = RegisterWindowMessage(TEXT("AMUnblock"));
            }
            if (uMsgId != 0) {
                MSG msg;
                
                while (PeekMessage(&msg, (HWND)-1, uMsgId, uMsgId, PM_REMOVE)) {
                }
            }
            PostThreadMessage(GetCurrentThreadId(), uMsgId, 0, 0);
        }
    }
    return dwResult;
}

HRESULT AmGetLastErrorToHResult()
{
    DWORD dwLastError = GetLastError();
    if(dwLastError != 0)
    {
        return HRESULT_FROM_WIN32(dwLastError);
    }
    else
    {
        return E_FAIL;
    }
}

IUnknown* QzAtlComPtrAssign(IUnknown** pp, IUnknown* lp)
{
	if (lp != NULL)
		lp->AddRef();
	if (*pp)
		(*pp)->Release();
	*pp = lp;
	return lp;
}

