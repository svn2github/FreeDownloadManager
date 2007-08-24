/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                      

typedef CGenericList<IMediaSample> CSampleList;

class COutputQueue : public CCritSec
{
public:
    
    COutputQueue(IPin      *pInputPin,          
                 HRESULT   *phr,                
                 BOOL       bAuto = TRUE,       
                 BOOL       bQueue = TRUE,      
                                                
                 LONG       lBatchSize = 1,     
                 BOOL       bBatchExact = FALSE,
                 LONG       lListSize =         
                                DEFAULTCACHE,
                 DWORD      dwPriority =        
                                THREAD_PRIORITY_NORMAL,
                 bool       bFlushingOpt = false 
                );
    ~COutputQueue();

    
    void BeginFlush();      

    
    void EndFlush();        
                            

    void EOS();             

    void SendAnyway();      

    void NewSegment(
            REFERENCE_TIME tStart,
            REFERENCE_TIME tStop,
            double dRate);

    HRESULT Receive(IMediaSample *pSample);

    
    HRESULT ReceiveMultiple (
        IMediaSample **pSamples,
        long nSamples,
        long *nSamplesProcessed);

    void Reset();           

    
    BOOL IsIdle();

    
    void SetPopEvent(HANDLE hEvent);

protected:
    static DWORD WINAPI InitialThreadProc(LPVOID pv);
    DWORD ThreadProc();
    BOOL  IsQueued()
    {
        return m_List != NULL;
    };

    
    void QueueSample(IMediaSample *pSample);

    BOOL IsSpecialSample(IMediaSample *pSample)
    {
        return (DWORD_PTR)pSample > (DWORD_PTR)(LONG_PTR)(-16);
    };

    
    void FreeSamples();

    
    void NotifyThread();  

protected:
    
    #define SEND_PACKET      ((IMediaSample *)(LONG_PTR)(-2))  
    #define EOS_PACKET       ((IMediaSample *)(LONG_PTR)(-3))  
    #define RESET_PACKET     ((IMediaSample *)(LONG_PTR)(-4))  
    #define NEW_SEGMENT      ((IMediaSample *)(LONG_PTR)(-5))  

    
    struct NewSegmentPacket {
        REFERENCE_TIME tStart;
        REFERENCE_TIME tStop;
        double dRate;
    };

    
    IPin          * const m_pPin;
    IMemInputPin  *       m_pInputPin;
    BOOL            const m_bBatchExact;
    LONG            const m_lBatchSize;

    CSampleList   *       m_List;
    HANDLE                m_hSem;
    CAMEvent                m_evFlushComplete;
    HANDLE                m_hThread;
    IMediaSample  **      m_ppSamples;
    LONG                  m_nBatched;

    
    LONG                  m_lWaiting;
    
    BOOL                  m_bFlushing;

    
    
    BOOL                  m_bFlushed;
    bool                  m_bFlushingOpt;

    
    BOOL                  m_bTerminate;

    
    BOOL                  m_bSendAnyway;

    
    BOOL volatile         m_hr;

    
    HANDLE m_hEventPop;
};

