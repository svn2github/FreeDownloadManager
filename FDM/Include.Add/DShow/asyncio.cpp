/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>
#include <asyncio.h>

#pragma warning(disable: 4706)                    

HRESULT
CAsyncRequest::Request(
    CAsyncIo *pIo,
    CAsyncStream *pStream,
    LONGLONG llPos,
    LONG lLength,
    BOOL bAligned,
    BYTE* pBuffer,
    LPVOID pContext,	
    DWORD dwUser)	
{
    m_pIo = pIo;
    m_pStream = pStream;
    m_llPos = llPos;
    m_lLength = lLength;
    m_bAligned = bAligned;
    m_pBuffer = pBuffer;
    m_pContext = pContext;
    m_dwUser = dwUser;
    m_hr = VFW_E_TIMEOUT;   

    return S_OK;
}          

HRESULT
CAsyncRequest::Complete()
{

    m_pStream->Lock();

    m_hr = m_pStream->SetPointer(m_llPos);
    if (S_OK == m_hr) {

        DWORD dwActual;
        m_hr = m_pStream->Read(m_pBuffer, m_lLength, m_bAligned, &dwActual);
        if (m_hr == OLE_S_FIRST) {
            if (m_pContext) {
                IMediaSample *pSample = reinterpret_cast<IMediaSample *>(m_pContext);
                pSample->SetDiscontinuity(TRUE);
                m_hr = S_OK;
            }
        }
        if (FAILED(m_hr)) {
        } else if (dwActual != (DWORD)m_lLength) {
            
            m_lLength = (LONG) dwActual;
            m_hr = S_FALSE;
        } else {
            m_hr = S_OK;
        }
    }

    m_pStream->Unlock();
    return m_hr;
}            

CAsyncIo::CAsyncIo(CAsyncStream *pStream)
 : m_hThread(NULL),
   m_evWork(TRUE),
   m_evDone(TRUE),
   m_evStop(TRUE),
   m_listWork(NAME("Work list")),
   m_listDone(NAME("Done list")),
   m_bFlushing(FALSE),
   m_cItemsOut(0),
   m_bWaiting(FALSE),
   m_pStream(pStream)
{

}  

CAsyncIo::~CAsyncIo()
{
    
    BeginFlush();

    
    CloseThread();

    
    POSITION pos = m_listDone.GetHeadPosition();
    while (pos) {
        CAsyncRequest* pRequest = m_listDone.GetNext(pos);
        delete pRequest;
    }
    m_listDone.RemoveAll();
}            

HRESULT
CAsyncIo::AsyncActive(void)
{
    return StartThread();
}        

HRESULT
CAsyncIo::AsyncInactive(void)
{
    return CloseThread();
}    

HRESULT
CAsyncIo::Request(
            LONGLONG llPos,
            LONG lLength,
            BOOL bAligned,
            BYTE* pBuffer,
            LPVOID pContext,
            DWORD dwUser)
{
    if (bAligned) {
        if (!IsAligned(llPos) ||
    	!IsAligned(lLength) ||
    	!IsAligned((LONG) pBuffer)) {
            return VFW_E_BADALIGN;
        }
    }

    CAsyncRequest* pRequest = new CAsyncRequest;

    HRESULT hr = pRequest->Request(
                            this,
                            m_pStream,
                            llPos,
                            lLength,
                            bAligned,
                            pBuffer,
                            pContext,
                            dwUser);
    if (SUCCEEDED(hr)) {
        
        hr = PutWorkItem(pRequest);
    }

    if (FAILED(hr)) {
        delete pRequest;
    }
    return hr;
}    

HRESULT
CAsyncIo::WaitForNext(
    DWORD dwTimeout,
    LPVOID *ppContext,
    DWORD * pdwUser,
    LONG* pcbActual)
{
    
    
    *ppContext = NULL;

    
    
    for (;;) {

        if (!m_evDone.Wait(dwTimeout)) {
            
            return VFW_E_TIMEOUT;
        }

        
        CAsyncRequest* pRequest = GetDoneItem();
        if (pRequest) {
            

            
            HRESULT hr = pRequest->GetHResult();
            if (hr == S_FALSE) {

                
                
                if ((pRequest->GetActualLength() +
                     pRequest->GetStart()) == Size()) {
                        hr = S_OK;
                } else {
                    
                    hr = E_FAIL;
                }
            }

            
            *pcbActual = pRequest->GetActualLength();

            
            *ppContext = pRequest->GetContext();
            *pdwUser = pRequest->GetUser();
            delete pRequest;
            return hr;
        } else {
            
            
            CAutoLock lck(&m_csLists);
            if (m_bFlushing && !m_bWaiting) {

                

                
                

                return VFW_E_WRONG_STATE;
            }
        }

        
        
    }
}      

HRESULT
CAsyncIo::SyncReadAligned(
            LONGLONG llPos,
            LONG lLength,
            BYTE* pBuffer,
            LONG* pcbActual,
            PVOID pvContext
            )
{
    if (!IsAligned(llPos) ||
	!IsAligned(lLength) ||
	!IsAligned((LONG) pBuffer)) {
            return VFW_E_BADALIGN;
    }

    CAsyncRequest request;

    HRESULT hr = request.Request(
                    this,
                    m_pStream,
                    llPos,
                    lLength,
                    TRUE,
                    pBuffer,
                    pvContext,
                    0);

    if (FAILED(hr)) {
        return hr;
    }

    hr = request.Complete();

    
    *pcbActual = request.GetActualLength();
    return hr;
}

HRESULT
CAsyncIo::Length(LONGLONG *pllTotal, LONGLONG* pllAvailable)
{
    return m_pStream->Size(pllTotal, pllAvailable);
}                              

HRESULT
CAsyncIo::BeginFlush()
{
    
    {
        CAutoLock lock(&m_csLists);

        
        
        
        
        m_bFlushing = TRUE;

        CAsyncRequest * preq;
        while(preq = GetWorkItem()) {
            preq->Cancel();
            PutDoneItem(preq);
        }  

        
        if (m_cItemsOut > 0) {

            
            ASSERT(!m_bWaiting);

            
            
            
            m_bWaiting = TRUE;
        } else {
            

            
            
            
            
            m_evDone.Set();
            return S_OK;
        }
    }

    ASSERT(m_bWaiting);

    
    for (;;) {
        m_evAllDone.Wait();
        {
            
            CAutoLock lock(&m_csLists);

            if (m_cItemsOut == 0) {

                
                
                m_bWaiting = FALSE;

                
                
                
                
                m_evDone.Set();

                return S_OK;
            }
        }
    }
}  

HRESULT
CAsyncIo::EndFlush()
{
    CAutoLock lock(&m_csLists);

    m_bFlushing = FALSE;

    ASSERT(!m_bWaiting);

    
    
    if (m_listDone.GetCount() > 0) {
        m_evDone.Set();
    } else {
        m_evDone.Reset();
    }

    return S_OK;
}  

HRESULT
CAsyncIo::StartThread(void)
{
    if (m_hThread) {
        return S_OK;
    }

    
    m_evStop.Reset();

    DWORD dwThreadID;
    m_hThread = CreateThread(
                    NULL,
                    0,
                    InitialThreadProc,
                    this,
                    0,
                    &dwThreadID);
    if (!m_hThread) {
	DWORD dwErr = GetLastError();
        return HRESULT_FROM_WIN32(dwErr);
    }
    return S_OK;
}  

HRESULT
CAsyncIo::CloseThread(void)
{
    
    m_evStop.Set();

    if (m_hThread) {

        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        m_hThread = NULL;
    }
    return S_OK;
}          

CAsyncRequest*
CAsyncIo::GetWorkItem()
{
    CAutoLock lck(&m_csLists);

    CAsyncRequest * preq  = m_listWork.RemoveHead();

    
    if (m_listWork.GetCount() == 0) {
        m_evWork.Reset();
    }
    return preq;
}  

CAsyncRequest*
CAsyncIo::GetDoneItem()
{
    CAutoLock lock(&m_csLists);

    CAsyncRequest * preq  = m_listDone.RemoveHead();

    
    
    
    
    
    
    
    
    

    if (m_listDone.GetCount() == 0 &&
        (!m_bFlushing || m_bWaiting)) {
        m_evDone.Reset();
    }

    return preq;
}  

HRESULT
CAsyncIo::PutWorkItem(CAsyncRequest* pRequest)
{
    CAutoLock lock(&m_csLists);
    HRESULT hr;

    if (m_bFlushing) {
        hr = VFW_E_WRONG_STATE;
    }
    else if (m_listWork.AddTail(pRequest)) {

        
        m_evWork.Set();

        
        hr = StartThread();

    } else {
        hr = E_OUTOFMEMORY;
    }
    return(hr);
}    

HRESULT
CAsyncIo::PutDoneItem(CAsyncRequest* pRequest)
{
    ASSERT(CritCheckIn(&m_csLists));

    if (m_listDone.AddTail(pRequest)) {

        
        m_evDone.Set();
        return S_OK;
    } else {
        return E_OUTOFMEMORY;
    }
}  

void
CAsyncIo::ProcessRequests(void)
{
    
    CAsyncRequest * preq = NULL;
    for (;;) {
        {
            CAutoLock lock(&m_csLists);

            preq = GetWorkItem();
            if (preq == NULL) {
                
                return;
            }

            
            m_cItemsOut++;

            
        }

        preq->Complete();

        
        {
            CAutoLock l(&m_csLists);

            PutDoneItem(preq);

            if (--m_cItemsOut == 0) {
                if (m_bWaiting) {
                    m_evAllDone.Set();
                }
            }
        }
    }
}    

DWORD
CAsyncIo::ThreadProc(void)
{
    HANDLE ahev[] = {m_evStop, m_evWork};

    for (;;) {
	DWORD dw = WaitForMultipleObjects(
		    2,
		    ahev,
		    FALSE,
		    INFINITE);
	if (dw == WAIT_OBJECT_0+1) {

	    
	    ProcessRequests();
	} else {
	    
	    return 0;
	}
    }
}        

HRESULT
CAsyncIo::SyncRead(
            LONGLONG llPos,
            LONG lLength,
            BYTE* pBuffer)
{
    if (IsAligned(llPos) &&
	IsAligned(lLength) &&
	IsAligned((LONG) pBuffer)) {
            LONG cbUnused;
	    return SyncReadAligned(llPos, lLength, pBuffer, &cbUnused, NULL);
    }

    
    

    CAsyncRequest request;

    HRESULT hr = request.Request(
                    this,
                    m_pStream,
                    llPos,
                    lLength,
                    FALSE,
                    pBuffer,
                    NULL,
                    0);

    if (FAILED(hr)) {
        return hr;
    }

    return request.Complete();
}  

HRESULT
CAsyncIo::Alignment(LONG *pl)
{
    *pl = Alignment();
    return S_OK;
}

