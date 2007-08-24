/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#ifndef __ASYNCIO_H__
#define __ASYNCIO_H__                

class CAsyncIo;
class CAsyncStream;      

class CAsyncStream
{
public:
    virtual ~CAsyncStream() {};
    virtual HRESULT SetPointer(LONGLONG llPos) = 0;
    virtual HRESULT Read(PBYTE pbBuffer,
                         DWORD dwBytesToRead,
                         BOOL bAlign,
                         LPDWORD pdwBytesRead) = 0;
    virtual HRESULT Size(LONGLONG* pllSizeTotal, LONGLONG *pSizeAvailable) = 0;
    virtual DWORD Alignment() = 0;
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
    
};      

class CAsyncRequest
{
    CAsyncIo *m_pIo;
    CAsyncStream *m_pStream;
    LONGLONG      m_llPos;
    BOOL        m_bAligned;
    LONG 	m_lLength;
    BYTE* 	m_pBuffer;
    LPVOID 	m_pContext;
    DWORD	m_dwUser;
    HRESULT     m_hr;

public:
    
    
    HRESULT Request(
    	CAsyncIo *pIo,
        CAsyncStream *pStream,
    	LONGLONG llPos,
	LONG lLength,
        BOOL bAligned,
	BYTE* pBuffer,
	LPVOID pContext,	
	DWORD dwUser);		

    
    
    HRESULT Complete();

    
    HRESULT Cancel()
    {
	return S_OK;
    };

    
    LPVOID GetContext()
    {
    	return m_pContext;
    };

    DWORD GetUser()
    {
	return m_dwUser;
    };

    HRESULT GetHResult() {
        return m_hr;
    };

    
    LONG GetActualLength() {
        return m_lLength;
    };

    LONGLONG GetStart() {
        return m_llPos;
    };
};  

typedef CGenericList<CAsyncRequest> CRequestList;                                            

class CAsyncIo
{

    CCritSec m_csReader;
    CAsyncStream *m_pStream;

    CCritSec m_csLists;      
    BOOL m_bFlushing;        
    CRequestList m_listWork;
    CRequestList m_listDone;
    CAMEvent m_evWork;         
    CAMEvent m_evDone;

    
    LONG    m_cItemsOut;    
    BOOL    m_bWaiting;     
    CAMEvent m_evAllDone;     

    CAMEvent m_evStop;         
    HANDLE m_hThread;

    LONGLONG Size() {
        ASSERT(m_pStream != NULL);
		LONGLONG llTotal;
        m_pStream->Size(&llTotal, NULL);
		return llTotal;
    };

    
    HRESULT StartThread(void);

    
    HRESULT CloseThread(void);

    
    
    
    
    CAsyncRequest* GetWorkItem();

    
    CAsyncRequest* GetDoneItem();

    
    HRESULT PutWorkItem(CAsyncRequest* pRequest);

    
    HRESULT PutDoneItem(CAsyncRequest* pRequest);

    
    void ProcessRequests(void);

    
    
    static DWORD WINAPI InitialThreadProc(LPVOID pv) {
	CAsyncIo * pThis = (CAsyncIo*) pv;
	return pThis->ThreadProc();
    };

    DWORD ThreadProc(void);

public:

    CAsyncIo(CAsyncStream *pStream);
    ~CAsyncIo();

    
    HRESULT Open(LPCTSTR pName);

    
    
    HRESULT AsyncActive(void);

    
    
    HRESULT AsyncInactive(void);

    
    HRESULT Request(
	    	LONGLONG llPos,
		LONG lLength,
                BOOL bAligned,
		BYTE* pBuffer,
		LPVOID pContext,
		DWORD dwUser);

    
    HRESULT WaitForNext(
	    	DWORD dwTimeout,
		LPVOID *ppContext,
		DWORD * pdwUser,
                LONG * pcbActual
                );

    
    HRESULT SyncReadAligned(
	    	LONGLONG llPos,
		LONG lLength,
		BYTE* pBuffer,
                LONG* pcbActual,
                PVOID pvContext
                );

    
    
    HRESULT SyncRead(
                LONGLONG llPos,
                LONG lLength,
                BYTE* pBuffer);

    
    HRESULT Length(LONGLONG *pllTotal, LONGLONG* pllAvailable);

    
    
    HRESULT Alignment(LONG* pl);

    HRESULT BeginFlush();
    HRESULT EndFlush();

    LONG Alignment()
    {
        return m_pStream->Alignment();
    };

    BOOL IsAligned(LONG l) {
	if ((l & (Alignment() -1)) == 0) {
	    return TRUE;
	} else {
	    return FALSE;
	}
    };

    BOOL IsAligned(LONGLONG ll) {
	return IsAligned( (LONG) (ll & 0xffffffff));
    };

    
    HANDLE StopEvent() const { return m_evDone; }
};

#endif 
