/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                        

#include <streams.h>                                                        

COutputQueue::COutputQueue(
             IPin         *pInputPin,          
             HRESULT      *phr,                
             BOOL          bAuto,              
             BOOL          bQueue,             
             LONG          lBatchSize,         
             BOOL          bBatchExact,        
             LONG          lListSize,
             DWORD         dwPriority,
             bool          bFlushingOpt        
            ) : m_lBatchSize(lBatchSize),
                m_bBatchExact(bBatchExact && (lBatchSize > 1)),
                m_hThread(NULL),
                m_hSem(NULL),
                m_List(NULL),
                m_pPin(pInputPin),
                m_ppSamples(NULL),
                m_lWaiting(0),
                m_pInputPin(NULL),
                m_bSendAnyway(FALSE),
                m_nBatched(0),
                m_bFlushing(FALSE),
                m_bFlushed(TRUE),
                m_bFlushingOpt(bFlushingOpt),
                m_bTerminate(FALSE),
                m_hEventPop(NULL),
                m_hr(S_OK)
{
    ASSERT(m_lBatchSize > 0);  

    if (FAILED(*phr)) {
        return;
    }

    

    *phr = pInputPin->QueryInterface(IID_IMemInputPin, (void **)&m_pInputPin);
    if (FAILED(*phr)) {
        return;
    }

    

    if (bAuto) {
        HRESULT hr = m_pInputPin->ReceiveCanBlock();
        if (SUCCEEDED(hr)) {
            bQueue = hr == S_OK;
        }
    }

    

    m_ppSamples = new PMEDIASAMPLE[m_lBatchSize];
    if (m_ppSamples == NULL) {
        *phr = E_OUTOFMEMORY;
        return;
    }

    

    if (bQueue) {
        DbgLog((LOG_TRACE, 2, TEXT("Creating thread for output pin")));
        m_hSem = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
        if (m_hSem == NULL) {
            DWORD dwError = GetLastError();
            *phr = AmHresultFromWin32(dwError);
            return;
        }
        m_List = new CSampleList(NAME("Sample Queue List"),
                                 lListSize,
                                 FALSE         
                                );
        if (m_List == NULL) {
            *phr = E_OUTOFMEMORY;
            return;
        }  

        DWORD dwThreadId;
        m_hThread = CreateThread(NULL,
                                 0,
                                 InitialThreadProc,
                                 (LPVOID)this,
                                 0,
                                 &dwThreadId);
        if (m_hThread == NULL) {
            DWORD dwError = GetLastError();
            *phr = AmHresultFromWin32(dwError);
            return;
        }
        SetThreadPriority(m_hThread, dwPriority);
    } else {
        DbgLog((LOG_TRACE, 2, TEXT("Calling input pin directly - no thread")));
    }
}                

COutputQueue::~COutputQueue()
{
    DbgLog((LOG_TRACE, 3, TEXT("COutputQueue::~COutputQueue")));
    
    if (m_pInputPin != NULL) {
        m_pInputPin->Release();
    }
    if (m_hThread != NULL) {
        {
            CAutoLock lck(this);
            m_bTerminate = TRUE;
            m_hr = S_FALSE;
            NotifyThread();
        }
        DbgWaitForSingleObject(m_hThread);
        EXECUTE_ASSERT(CloseHandle(m_hThread));

        

        ASSERT(m_List->GetCount() == 0);
        delete m_List;
    } else {
        FreeSamples();
    }
    if (m_hSem != NULL) {
        EXECUTE_ASSERT(CloseHandle(m_hSem));
    }
    delete [] m_ppSamples;
}      

DWORD WINAPI COutputQueue::InitialThreadProc(LPVOID pv)
{
    HRESULT hrCoInit = CAMThread::CoInitializeHelper();
    
    COutputQueue *pSampleQueue = (COutputQueue *)pv;
    DWORD dwReturn = pSampleQueue->ThreadProc();

    if(hrCoInit == S_OK) {
        CoUninitialize();
    }
    
    return dwReturn;
}              

DWORD COutputQueue::ThreadProc()
{
    while (TRUE) {
        BOOL          bWait = FALSE;
        IMediaSample *pSample;
        LONG          lNumberToSend; 
        NewSegmentPacket* ppacket;

        
        
        
        
        
        {
            CAutoLock lck(this);
            while (TRUE) {

                if (m_bTerminate) {
                    FreeSamples();
                    return 0;
                }
                if (m_bFlushing) {
                    FreeSamples();
                    SetEvent(m_evFlushComplete);
                }

                

                pSample = m_List->RemoveHead();
		
		if (m_hEventPop) {
                    
		    SetEvent(m_hEventPop);
		}

                if (pSample != NULL &&
                    !IsSpecialSample(pSample)) {

                    
                    

                    m_ppSamples[m_nBatched++] = pSample;
                    if (m_nBatched == m_lBatchSize) {
                        break;
                    }
                } else {

                    
                    
                    

                    if (pSample == NULL &&
                        (m_bBatchExact || m_nBatched == 0)) {

                        
                        

                        ASSERT(m_lWaiting == 0);
                        m_lWaiting++;
                        bWait      = TRUE;
                    } else {

                        
                        

                        if (pSample == SEND_PACKET && m_nBatched == 0) {
                            continue;
                        }

                        if (pSample == NEW_SEGMENT) {
                            
                            
                            ppacket = (NewSegmentPacket *) m_List->RemoveHead();
			    
			    if (m_hEventPop) {
                    	        
		    	        SetEvent(m_hEventPop);
			    }

                            ASSERT(ppacket);
                        }
                        
                        
                    }
                    break;
                }
            }
            if (!bWait) {
                
                
                lNumberToSend = m_nBatched;  
                m_nBatched = 0;
            }
        }

        

        if (bWait) {
            DbgWaitForSingleObject(m_hSem);
            continue;
        }    

        
        
        
        
        

        if (lNumberToSend != 0) {
            long nProcessed;
            if (m_hr == S_OK) {
                ASSERT(!m_bFlushed);
                HRESULT hr = m_pInputPin->ReceiveMultiple(m_ppSamples,
                                                          lNumberToSend,
                                                          &nProcessed);
                
                CAutoLock lck(this);
                if (m_hr == S_OK) {
                    m_hr = hr;
                }
                ASSERT(!m_bFlushed);
            }
            while (lNumberToSend != 0) {
                m_ppSamples[--lNumberToSend]->Release();
            }
            if (m_hr != S_OK) {

                
                

                DbgLog((LOG_ERROR, 2, TEXT("ReceiveMultiple returned %8.8X"),
                       m_hr));
            }
        }

        

        if (pSample == EOS_PACKET) {

            
            
            
            
            

            if (m_hr == S_OK) {
                DbgLog((LOG_TRACE, 2, TEXT("COutputQueue sending EndOfStream()")));
                HRESULT hr = m_pPin->EndOfStream();
                if (FAILED(hr)) {
                    DbgLog((LOG_ERROR, 2, TEXT("COutputQueue got code 0x%8.8X from EndOfStream()")));
                }
            }
        }

        

        if (pSample == RESET_PACKET) {
            m_hr = S_OK;
            SetEvent(m_evFlushComplete);
        }

        if (pSample == NEW_SEGMENT) {
            m_pPin->NewSegment(ppacket->tStart, ppacket->tStop, ppacket->dRate);
            delete ppacket;
        }
    }
}  

void COutputQueue::SendAnyway()
{
    if (!IsQueued()) {

        

        m_bSendAnyway = TRUE;
        LONG nProcessed;
        ReceiveMultiple(NULL, 0, &nProcessed);
        m_bSendAnyway = FALSE;

    } else {
        CAutoLock lck(this);
        QueueSample(SEND_PACKET);
        NotifyThread();
    }
}

void
COutputQueue::NewSegment(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop,
    double dRate)
{
    if (!IsQueued()) {
        if (S_OK == m_hr) {
            if (m_bBatchExact) {
                SendAnyway();
            }
            m_pPin->NewSegment(tStart, tStop, dRate);
        }
    } else {
        if (m_hr == S_OK) {
            
            
            
            
            
            
            
            
            
            NewSegmentPacket * ppack = new NewSegmentPacket;
            if (ppack == NULL) {
                return;
            }
            ppack->tStart = tStart;
            ppack->tStop = tStop;
            ppack->dRate = dRate;

            CAutoLock lck(this);
            QueueSample(NEW_SEGMENT);
            QueueSample( (IMediaSample*) ppack);
            NotifyThread();
        }
    }
}        

void COutputQueue::EOS()
{
    CAutoLock lck(this);
    if (!IsQueued()) {
        if (m_bBatchExact) {
            SendAnyway();
        }
        if (m_hr == S_OK) {
            DbgLog((LOG_TRACE, 2, TEXT("COutputQueue sending EndOfStream()")));
            m_bFlushed = FALSE;
            HRESULT hr = m_pPin->EndOfStream();
            if (FAILED(hr)) {
                DbgLog((LOG_ERROR, 2, TEXT("COutputQueue got code 0x%8.8X from EndOfStream()")));
            }
        }
    } else {
        if (m_hr == S_OK) {
            m_bFlushed = FALSE;
            QueueSample(EOS_PACKET);
            NotifyThread();
        }
    }
}      

void COutputQueue::BeginFlush()
{
    if (IsQueued()) {
        {
            CAutoLock lck(this);

            
            

            

            m_bFlushing = TRUE;

            

            if (m_hr == S_OK) {
                m_hr = S_FALSE;
            }

            

            if (m_bFlushed && m_bFlushingOpt) {
                return;
            }

            
            m_evFlushComplete.Reset();

            NotifyThread();
        }

        

        m_pPin->BeginFlush();
    } else {
        
        m_pPin->BeginFlush();
        CAutoLock lck(this);
        

        m_bFlushing = TRUE;

        

        if (m_hr == S_OK) {
            m_hr = S_FALSE;
        }
    }

}    

void COutputQueue::EndFlush()
{
    {
        CAutoLock lck(this);
        ASSERT(m_bFlushing);
        if (m_bFlushingOpt && m_bFlushed && IsQueued()) {
            m_bFlushing = FALSE;
            m_hr = S_OK;
            return;
        }
    }

    
    
    
    
    

    if (IsQueued()) {
        m_evFlushComplete.Wait();
    } else {
        FreeSamples();
    }

    
    

    m_bFlushing = FALSE;
    m_bFlushed  = TRUE;

    

    m_pPin->EndFlush();

    m_hr = S_OK;
}          

void COutputQueue::QueueSample(IMediaSample *pSample)
{
    if (NULL == m_List->AddTail(pSample)) {
        if (!IsSpecialSample(pSample)) {
            pSample->Release();
        }
    }
}                  

HRESULT COutputQueue::Receive(IMediaSample *pSample)
{
    LONG nProcessed;
    return ReceiveMultiple(&pSample, 1, &nProcessed);
}                        

HRESULT COutputQueue::ReceiveMultiple (
    IMediaSample **ppSamples,
    long nSamples,
    long *nSamplesProcessed)
{
    CAutoLock lck(this);
    

    if (!IsQueued()) {

        

        if (S_OK != m_hr) {

            
            
            
            
            ASSERT(!m_bFlushed || m_bFlushing);

            
            *nSamplesProcessed = 0;
            for (int i = 0; i < nSamples; i++) {
                DbgLog((LOG_TRACE, 3, TEXT("COutputQueue (direct) : Discarding %d samples code 0x%8.8X"),
                        nSamples, m_hr));
                ppSamples[i]->Release();
            }

            return m_hr;
        }
        
        
        
        ASSERT(!m_bFlushing);
        m_bFlushed = FALSE;

        ASSERT(m_nBatched < m_lBatchSize);
        ASSERT(m_nBatched == 0 || m_bBatchExact);

        

        LONG iLost = 0;
        for (long iDone = 0;
             iDone < nSamples || (m_nBatched != 0 && m_bSendAnyway);
            ) {  

            ASSERT(m_nBatched < m_lBatchSize);
            if (iDone < nSamples) {
                m_ppSamples[m_nBatched++] = ppSamples[iDone++];
            }
            if (m_nBatched == m_lBatchSize ||
                nSamples == 0 && (m_bSendAnyway || !m_bBatchExact)) {
                LONG nDone;
                DbgLog((LOG_TRACE, 4, TEXT("Batching %d samples"),
                       m_nBatched));

                if (m_hr == S_OK) {
                    m_hr = m_pInputPin->ReceiveMultiple(m_ppSamples,
                                                        m_nBatched,
                                                        &nDone);
                } else {
                    nDone = 0;
                }
                iLost += m_nBatched - nDone;
                for (LONG i = 0; i < m_nBatched; i++) {
                    m_ppSamples[i]->Release();
                }
                m_nBatched = 0;
            }
        }
        *nSamplesProcessed = iDone - iLost;
        if (*nSamplesProcessed < 0) {
            *nSamplesProcessed = 0;
        }
        return m_hr;
    } else {
        

        if (m_hr != S_OK) {
            *nSamplesProcessed = 0;
            DbgLog((LOG_TRACE, 3, TEXT("COutputQueue (queued) : Discarding %d samples code 0x%8.8X"),
                    nSamples, m_hr));
            for (int i = 0; i < nSamples; i++) {
                ppSamples[i]->Release();
            }
            return m_hr;
        }
        m_bFlushed = FALSE;
        for (long i = 0; i < nSamples; i++) {
            QueueSample(ppSamples[i]);
        }
        *nSamplesProcessed = nSamples;
        if (!m_bBatchExact ||
            m_nBatched + m_List->GetCount() >= m_lBatchSize) {
            NotifyThread();
        }
        return S_OK;
    }
}  

void COutputQueue::Reset()
{
    if (!IsQueued()) {
        m_hr = S_OK;
    } else {
        CAutoLock lck(this);
        QueueSample(RESET_PACKET);
        NotifyThread();
        m_evFlushComplete.Wait();
    }
}  

void COutputQueue::FreeSamples()
{
    CAutoLock lck(this);
    if (IsQueued()) {
        while (TRUE) {
            IMediaSample *pSample = m_List->RemoveHead();
	    
	    if (m_hEventPop) {
                
	        SetEvent(m_hEventPop);
	    }

            if (pSample == NULL) {
                break;
            }
            if (!IsSpecialSample(pSample)) {
                pSample->Release();
            } else {
                if (pSample == NEW_SEGMENT) {
                    
                    NewSegmentPacket *ppacket =
                        (NewSegmentPacket *) m_List->RemoveHead();
		    
		    if (m_hEventPop) {
                        
		        SetEvent(m_hEventPop);
		    }

                    ASSERT(ppacket != NULL);
                    delete ppacket;
                }
            }
        }
    }
    for (int i = 0; i < m_nBatched; i++) {
        m_ppSamples[i]->Release();
    }
    m_nBatched = 0;
}      

void COutputQueue::NotifyThread()
{
    
    ASSERT(IsQueued());
    if (m_lWaiting) {
        ReleaseSemaphore(m_hSem, m_lWaiting, NULL);
        m_lWaiting = 0;
    }
}            

BOOL COutputQueue::IsIdle()
{
    CAutoLock lck(this);

    
    
    
    
    

    if (IsQueued() && m_lWaiting == 0 || m_nBatched != 0) {
        return FALSE;
    } else {

        
        

        ASSERT(!IsQueued() || m_List->GetCount() == 0);
        return TRUE;
    }
}  

void COutputQueue::SetPopEvent(HANDLE hEvent)
{
    m_hEventPop = hEvent;
}
