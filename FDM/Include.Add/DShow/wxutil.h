/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#ifndef __WXUTIL__
#define __WXUTIL__  

#pragma warning(disable: 4705)  

class CCritSec {

    

    CCritSec(const CCritSec &refCritSec);
    CCritSec &operator=(const CCritSec &refCritSec);

    CRITICAL_SECTION m_CritSec;

#ifdef DEBUG
public:
    DWORD   m_currentOwner;
    DWORD   m_lockCount;
    BOOL    m_fTrace;        
public:
    CCritSec();
    ~CCritSec();
    void Lock();
    void Unlock();
#else

public:
    CCritSec() {
	InitializeCriticalSection(&m_CritSec);
    };

    ~CCritSec() {
	DeleteCriticalSection(&m_CritSec);
    };

    void Lock() {
	EnterCriticalSection(&m_CritSec);
    };

    void Unlock() {
	LeaveCriticalSection(&m_CritSec);
    };
#endif
};                    

#ifdef DEBUG
    BOOL WINAPI CritCheckIn(CCritSec * pcCrit);
    BOOL WINAPI CritCheckIn(const CCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(CCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(const CCritSec * pcCrit);
    void WINAPI DbgLockTrace(CCritSec * pcCrit, BOOL fTrace);
#else
    #define CritCheckIn(x) TRUE
    #define CritCheckOut(x) TRUE
    #define DbgLockTrace(pc, fT)
#endif      

class CAutoLock {

    

    CAutoLock(const CAutoLock &refAutoLock);
    CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
    CCritSec * m_pLock;

public:
    CAutoLock(CCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~CAutoLock() {
        m_pLock->Unlock();
    };
};      

class CAMEvent
{

    

    CAMEvent(const CAMEvent &refEvent);
    CAMEvent &operator=(const CAMEvent &refEvent);

protected:
    HANDLE m_hEvent;
public:
    CAMEvent(BOOL fManualReset = FALSE);
    ~CAMEvent();

    
    operator HANDLE () const { return m_hEvent; };

    void Set() {EXECUTE_ASSERT(SetEvent(m_hEvent));};
    BOOL Wait(DWORD dwTimeout = INFINITE) {
	return (WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0);
    };
    void Reset() { ResetEvent(m_hEvent); };
    BOOL Check() { return Wait(0); };
};          

class CAMMsgEvent : public CAMEvent
{

public:

    
    BOOL WaitMsg(DWORD dwTimeout = INFINITE);
};  

#define CTimeoutEvent CAMEvent        

class AM_NOVTABLE CAMThread {

    

    CAMThread(const CAMThread &refThread);
    CAMThread &operator=(const CAMThread &refThread);

    CAMEvent m_EventSend;
    CAMEvent m_EventComplete;

    DWORD m_dwParam;
    DWORD m_dwReturnVal;

protected:
    HANDLE m_hThread;

    
    
    virtual DWORD ThreadProc() = 0;

public:
    CAMThread();
    virtual ~CAMThread();

    CCritSec m_AccessLock;	
    CCritSec m_WorkerLock;	

    
    
    static DWORD WINAPI InitialThreadProc(LPVOID pv);

    
    BOOL Create();

    
    
    DWORD CallWorker(DWORD);

    
    
    void Close() {
        HANDLE hThread = (HANDLE)InterlockedExchangePointer(&m_hThread, 0);
        if (hThread) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }
    };

    
    
    BOOL ThreadExists(void) const
    {
        if (m_hThread == 0) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    
    DWORD GetRequest();

    
    BOOL CheckRequest(DWORD * pParam);

    
    void Reply(DWORD);

    
    
    HANDLE GetRequestHandle() const { return m_EventSend; };

    
    DWORD GetRequestParam() const { return m_dwParam; };

    
    
    static HRESULT CoInitializeHelper();
};                            

#define DEFAULT_QUEUESIZE   2

template <class T> class CQueue {
private:
    HANDLE          hSemPut;        
    HANDLE          hSemGet;        
    CRITICAL_SECTION CritSect;      
    int             nMax;           
    int             iNextPut;       
    int             iNextGet;       
    T              *QueueObjects;   

    void Initialize(int n) {
        iNextPut = iNextGet = 0;
        nMax = n;
        InitializeCriticalSection(&CritSect);
        hSemPut = CreateSemaphore(NULL, n, n, NULL);
        hSemGet = CreateSemaphore(NULL, 0, n, NULL);
        QueueObjects = new T[n];
    }  

public:
    CQueue(int n) {
        Initialize(n);
    }

    CQueue() {
        Initialize(DEFAULT_QUEUESIZE);
    }

    ~CQueue() {
        delete [] QueueObjects;
        DeleteCriticalSection(&CritSect);
        CloseHandle(hSemPut);
        CloseHandle(hSemGet);
    }

    T GetQueueObject() {
        int iSlot;
        T Object;
        LONG lPrevious;

        
        
        
        WaitForSingleObject(hSemGet, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextGet++ % nMax;
        Object = QueueObjects[iSlot];
        LeaveCriticalSection(&CritSect);

        
        
        
        ReleaseSemaphore(hSemPut, 1L, &lPrevious);
        return Object;
    }

    void PutQueueObject(T Object) {
        int iSlot;
        LONG lPrevious;

        
        
        
        WaitForSingleObject(hSemPut, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextPut++ % nMax;
        QueueObjects[iSlot] = Object;
        LeaveCriticalSection(&CritSect);

        
        
        
        ReleaseSemaphore(hSemGet, 1L, &lPrevious);
    }
};          

int WINAPIV wsprintfWInternal(LPWSTR, LPCWSTR, ...);

LPWSTR
WINAPI
lstrcpyWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2
    );
LPWSTR
WINAPI
lstrcpynWInternal(
    LPWSTR lpString1,
    LPCWSTR lpString2,
    int     iMaxLength
    );
int
WINAPI
lstrcmpWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    );
int
WINAPI
lstrcmpiWInternal(
    LPCWSTR lpString1,
    LPCWSTR lpString2
    );
int
WINAPI
lstrlenWInternal(
    LPCWSTR lpString
    );

#ifndef UNICODE
#define wsprintfW wsprintfWInternal
#define lstrcpyW lstrcpyWInternal
#define lstrcpynW lstrcpynWInternal
#define lstrcmpW lstrcmpWInternal
#define lstrcmpiW lstrcmpiWInternal
#define lstrlenW lstrlenWInternal
#endif

extern "C"
void * __stdcall memmoveInternal(void *, const void *, size_t);

inline void * __cdecl memchrInternal(const void *buf, int chr, size_t cnt)
{
#ifdef _X86_
    void *pRet = NULL;

    _asm {
        cld                 
        mov     ecx, cnt    
        mov     edi, buf    
        mov     eax, chr    
        repne   scasb       
        jnz     exit_memchr 
        dec     edi         
                            
        mov     pRet, edi
exit_memchr:
    }
    return pRet;

#else
    while ( cnt && (*(unsigned char *)buf != (unsigned char)chr) ) {
        buf = (unsigned char *)buf + 1;
        cnt--;
    }

    return(cnt ? (void *)buf : NULL);
#endif
}

void WINAPI IntToWstr(int i, LPWSTR wstr);

#define WstrToInt(sz) _wtoi(sz)
#define atoiW(sz) _wtoi(sz)
#define atoiA(sz) atoi(sz)    

extern const DWORD bits555[3];
extern const DWORD bits565[3];
extern const DWORD bits888[3];    

STDAPI_(const GUID) GetTrueColorType(const BITMAPINFOHEADER *pbmiHeader);
STDAPI_(const GUID) GetBitmapSubtype(const BITMAPINFOHEADER *pbmiHeader);
STDAPI_(WORD) GetBitCount(const GUID *pSubtype);          

STDAPI_(CHAR *) GetSubtypeNameA(const GUID *pSubtype);
STDAPI_(WCHAR *) GetSubtypeNameW(const GUID *pSubtype);

#ifdef UNICODE
#define GetSubtypeName GetSubtypeNameW
#else
#define GetSubtypeName GetSubtypeNameA
#endif

STDAPI_(LONG) GetBitmapFormatSize(const BITMAPINFOHEADER *pHeader);
STDAPI_(DWORD) GetBitmapSize(const BITMAPINFOHEADER *pHeader);
STDAPI_(BOOL) ContainsPalette(const VIDEOINFOHEADER *pVideoInfo);
STDAPI_(const RGBQUAD *) GetBitmapPalette(const VIDEOINFOHEADER *pVideoInfo);    

BOOL WINAPI IsEqualObject(IUnknown *pFirst, IUnknown *pSecond);  

#define EqualPins(pPin1, pPin2) IsEqualObject(pPin1, pPin2)        

LONGLONG WINAPI llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG rnd);
LONGLONG WINAPI Int64x32Div32(LONGLONG a, LONG b, LONG c, LONG rnd);    

STDAPI WriteBSTR(BSTR * pstrDest, LPCWSTR szSrc);
STDAPI FreeBSTR(BSTR* pstr);          

STDAPI AMGetWideString(LPCWSTR pszString, LPWSTR *ppszReturn);  

DWORD WINAPI WaitDispatchingMessages(
    HANDLE hObject,
    DWORD dwWait,
    HWND hwnd = NULL,
    UINT uMsg = 0,
    HANDLE hEvent = NULL);        

#define AmHresultFromWin32(x) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, x))    

HRESULT AmGetLastErrorToHResult(void);    

IUnknown* QzAtlComPtrAssign(IUnknown** pp, IUnknown* lp);

template <class T>
class QzCComPtr
{
public:
	typedef T _PtrClass;
	QzCComPtr() {p=NULL;}
	QzCComPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	QzCComPtr(const QzCComPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~QzCComPtr() {if (p) p->Release();}
	void Release() {if (p) p->Release(); p=NULL;}
	operator T*() {return (T*)p;}
	T& operator*() {ASSERT(p!=NULL); return *p; }
	
	
	T** operator&() { ASSERT(p==NULL); return &p; }
	T* operator->() { ASSERT(p!=NULL); return p; }
	T* operator=(T* lp){return (T*)QzAtlComPtrAssign((IUnknown**)&p, lp);}
	T* operator=(const QzCComPtr<T>& lp)
	{
		return (T*)QzAtlComPtrAssign((IUnknown**)&p, lp.p);
	}
#if _MSC_VER>1020
	bool operator!(){return (p == NULL);}
#else
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}
#endif
	T* p;
};    

#endif 
