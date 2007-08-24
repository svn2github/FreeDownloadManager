/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#ifndef __FILTER__
#define __FILTER__    

class CBaseMediaFilter;     
class CBaseFilter;          
class CBasePin;             
class CEnumPins;            
class CEnumMediaTypes;      
class CBaseOutputPin;       
class CBaseInputPin;        
class CMediaSample;         
class CBaseAllocator;       
class CMemAllocator;                            

#define QueryFilterInfoReleaseGraph(fi) if ((fi).pGraph) (fi).pGraph->Release();

#define QueryPinInfoReleaseFilter(pi) if ((pi).pFilter) (pi).pFilter->Release();                              

class AM_NOVTABLE CBaseMediaFilter : public CUnknown,
                                     public IMediaFilter
{

protected:

    FILTER_STATE    m_State;            
    IReferenceClock *m_pClock;          
    

    
    CRefTime        m_tStart;

    CLSID	    m_clsid;            
                                        
    CCritSec        *m_pLock;           

public:

    CBaseMediaFilter(
        const TCHAR     *pName,
        LPUNKNOWN pUnk,
        CCritSec  *pLock,
	REFCLSID   clsid);

    virtual ~CBaseMediaFilter();

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    
    
    

    STDMETHODIMP GetClassID(CLSID *pClsID);

    

    STDMETHODIMP GetState(DWORD dwMSecs, FILTER_STATE *State);

    STDMETHODIMP SetSyncSource(IReferenceClock *pClock);

    STDMETHODIMP GetSyncSource(IReferenceClock **pClock);

    
    
    
    
    
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();  

    
    
    
    STDMETHODIMP Run(REFERENCE_TIME tStart);

    

    
    
    virtual HRESULT StreamTime(CRefTime& rtStream);

    
    BOOL IsActive() {
        CAutoLock cObjectLock(m_pLock);
        return ((m_State == State_Paused) || (m_State == State_Running));
    };
};                                      

class AM_NOVTABLE CBaseFilter : public CUnknown,        
                    public IBaseFilter,     
                    public IAMovieSetup     
{

friend class CBasePin;

protected:
    FILTER_STATE    m_State;            
    IReferenceClock *m_pClock;          
    CRefTime        m_tStart;           
    CLSID	    m_clsid;            
                                        
    CCritSec        *m_pLock;           

    WCHAR           *m_pName;           
    IFilterGraph    *m_pGraph;          
    IMediaEventSink *m_pSink;           
    LONG            m_PinVersion;       

public:

    CBaseFilter(
        const TCHAR *pName,     
        LPUNKNOWN pUnk,         
        CCritSec  *pLock,       
	REFCLSID   clsid);      

    CBaseFilter(
        TCHAR     *pName,       
        LPUNKNOWN pUnk,         
        CCritSec  *pLock,       
	REFCLSID   clsid,       
        HRESULT   *phr);        
#ifdef UNICODE
    CBaseFilter(
        const CHAR *pName,     
        LPUNKNOWN pUnk,         
        CCritSec  *pLock,       
	REFCLSID   clsid);      

    CBaseFilter(
        CHAR     *pName,       
        LPUNKNOWN pUnk,         
        CCritSec  *pLock,       
	REFCLSID   clsid,       
        HRESULT   *phr);        
#endif
    ~CBaseFilter();

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
#ifdef DEBUG
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
#endif

    
    
    

    STDMETHODIMP GetClassID(CLSID *pClsID);

    

    STDMETHODIMP GetState(DWORD dwMSecs, FILTER_STATE *State);

    STDMETHODIMP SetSyncSource(IReferenceClock *pClock);

    STDMETHODIMP GetSyncSource(IReferenceClock **pClock);  

    
    
    
    
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

    
    
    
    STDMETHODIMP Run(REFERENCE_TIME tStart);

    

    
    
    virtual HRESULT StreamTime(CRefTime& rtStream);

    
    BOOL IsActive() {
        CAutoLock cObjectLock(m_pLock);
        return ((m_State == State_Paused) || (m_State == State_Running));
    };

    
    BOOL IsStopped() {
        return (m_State == State_Stopped);
    };

    
    
    

    
    STDMETHODIMP EnumPins(
                    IEnumPins ** ppEnum);  

    
    STDMETHODIMP FindPin(
        LPCWSTR Id,
        IPin ** ppPin
    );

    STDMETHODIMP QueryFilterInfo(
                    FILTER_INFO * pInfo);

    STDMETHODIMP JoinFilterGraph(
                    IFilterGraph * pGraph,
                    LPCWSTR pName);

    
    
    
    STDMETHODIMP QueryVendorInfo(
                    LPWSTR* pVendorInfo
            );

    

    
    
    
    HRESULT NotifyEvent(
        long EventCode,
        LONG_PTR EventParam1,
        LONG_PTR EventParam2);

    
    IFilterGraph *GetFilterGraph() {
        return m_pGraph;
    }

    
    
    
    
    HRESULT ReconnectPin(IPin *pPin, AM_MEDIA_TYPE const *pmt);

    
    virtual LONG GetPinVersion();
    void IncrementPinVersion();

    
    
    virtual int GetPinCount() PURE;
    virtual CBasePin *GetPin(int n) PURE;

    

    STDMETHODIMP Register();    
    STDMETHODIMP Unregister();  

    
    

    virtual LPAMOVIESETUP_FILTER GetSetupData(){ return NULL; }

};                  

class  AM_NOVTABLE CBasePin : public CUnknown, public IPin, public IQualityControl
{

protected:

    WCHAR *         m_pName;		        
    IPin            *m_Connected;               
    PIN_DIRECTION   m_dir;                      
    CCritSec        *m_pLock;                   
    bool            m_bRunTimeError;            
    bool            m_bCanReconnectWhenActive;  
    bool            m_bTryMyTypesFirst;         
                                                
    CBaseFilter    *m_pFilter;                  
    IQualityControl *m_pQSink;                  
    LONG            m_TypeVersion;              
    CMediaType      m_mt;                       

    CRefTime        m_tStart;                   
    CRefTime        m_tStop;                    
    double          m_dRate;                    

#ifdef DEBUG
    LONG            m_cRef;                     
#endif

    

#ifdef DEBUG
    void DisplayPinInfo(IPin *pReceivePin);
    void DisplayTypeInfo(IPin *pPin, const CMediaType *pmt);
#else
    void DisplayPinInfo(IPin *pReceivePin) {};
    void DisplayTypeInfo(IPin *pPin, const CMediaType *pmt) {};
#endif

    

    
    
    HRESULT
    AttemptConnection(
        IPin* pReceivePin,      
        const CMediaType* pmt   
    );

    
    
    HRESULT TryMediaTypes(
                        IPin *pReceivePin,      
                        const CMediaType *pmt,        
                        IEnumMediaTypes *pEnum);    

    
    
    
    
    HRESULT AgreeMediaType(
                        IPin *pReceivePin,      
                        const CMediaType *pmt);       

public:

    CBasePin(
        TCHAR *pObjectName,         
        CBaseFilter *pFilter,       
        CCritSec *pLock,            
        HRESULT *phr,               
        LPCWSTR pName,              
        PIN_DIRECTION dir);         
#ifdef UNICODE
    CBasePin(
        CHAR *pObjectName,         
        CBaseFilter *pFilter,       
        CCritSec *pLock,            
        HRESULT *phr,               
        LPCWSTR pName,              
        PIN_DIRECTION dir);         
#endif
    virtual ~CBasePin();

    DECLARE_IUNKNOWN

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();

    

    
    
    
    STDMETHODIMP Connect(
        IPin * pReceivePin,
        const AM_MEDIA_TYPE *pmt   
    );

    
    STDMETHODIMP ReceiveConnection(
        IPin * pConnector,      
        const AM_MEDIA_TYPE *pmt   
    );

    STDMETHODIMP Disconnect();

    STDMETHODIMP ConnectedTo(IPin **pPin);

    STDMETHODIMP ConnectionMediaType(AM_MEDIA_TYPE *pmt);

    STDMETHODIMP QueryPinInfo(
        PIN_INFO * pInfo
    );

    STDMETHODIMP QueryDirection(
    	PIN_DIRECTION * pPinDir
    );

    STDMETHODIMP QueryId(
        LPWSTR * Id
    );

    
    STDMETHODIMP QueryAccept(
        const AM_MEDIA_TYPE *pmt
    );

    
    STDMETHODIMP EnumMediaTypes(
        IEnumMediaTypes **ppEnum
    );

    
    
    
    
    
    
    
    STDMETHODIMP QueryInternalConnections(
        IPin* *apPin,     
        ULONG *nPin       
                          
    ) { return E_NOTIMPL; }

    
    STDMETHODIMP EndOfStream(void);

    

    
    
    
    
    STDMETHODIMP NewSegment(
                    REFERENCE_TIME tStart,
                    REFERENCE_TIME tStop,
                    double dRate);

    
    
    

    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    STDMETHODIMP SetSink(IQualityControl * piqc);

    

    
    BOOL IsConnected(void) {return (m_Connected != NULL); };
    
    IPin * GetConnected() { return m_Connected; };

    
    BOOL IsStopped() {
        return (m_pFilter->m_State == State_Stopped);
    };

    
    virtual LONG GetMediaTypeVersion();
    void IncrementTypeVersion();

    
    
    virtual HRESULT Active(void);

    
    virtual HRESULT Inactive(void);

    
    virtual HRESULT Run(REFERENCE_TIME tStart);

    
    virtual HRESULT CheckMediaType(const CMediaType *) PURE;

    
    virtual HRESULT SetMediaType(const CMediaType *);

    
    
    virtual HRESULT CheckConnect(IPin *);

    
    virtual HRESULT BreakConnect();
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    
    virtual HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);

    
    REFERENCE_TIME CurrentStopTime() {
        return m_tStop;
    }
    REFERENCE_TIME CurrentStartTime() {
        return m_tStart;
    }
    double CurrentRate() {
        return m_dRate;
    }

    
    LPWSTR Name() { return m_pName; };

    
    void SetReconnectWhenActive(bool bCanReconnect)
    {
        m_bCanReconnectWhenActive = bCanReconnect;
    }

    bool CanReconnectWhenActive()
    {
        return m_bCanReconnectWhenActive;
    }

protected:
    STDMETHODIMP DisconnectInternal();
};                            

class CEnumPins : public IEnumPins      
{
    int m_Position;                 
    int m_PinCount;                 
    CBaseFilter *m_pFilter;         
    LONG m_Version;                 
    LONG m_cRef;

    typedef CGenericList<CBasePin> CPinList;

    CPinList m_PinCache;	    
				    
				    

#ifdef DEBUG
    DWORD m_dwCookie;
#endif

    

    BOOL AreWeOutOfSync() {
        return (m_pFilter->GetPinVersion() == m_Version ? FALSE : TRUE);
    };

    

    STDMETHODIMP Refresh();

public:

    CEnumPins(
        CBaseFilter *pFilter,
        CEnumPins *pEnumPins);

    virtual ~CEnumPins();

    
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    
    STDMETHODIMP Next(
        ULONG cPins,         
        IPin ** ppPins,      
        ULONG * pcFetched    
    );

    STDMETHODIMP Skip(ULONG cPins);
    STDMETHODIMP Reset();
    STDMETHODIMP Clone(IEnumPins **ppEnum);  

};                  

class CEnumMediaTypes : public IEnumMediaTypes    
{
    int m_Position;           
    CBasePin *m_pPin;         
    LONG m_Version;           
    LONG m_cRef;
#ifdef DEBUG
    DWORD m_dwCookie;
#endif

    

    BOOL AreWeOutOfSync() {
        return (m_pPin->GetMediaTypeVersion() == m_Version ? FALSE : TRUE);
    };

public:

    CEnumMediaTypes(
        CBasePin *pPin,
        CEnumMediaTypes *pEnumMediaTypes);

    virtual ~CEnumMediaTypes();

    
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    
    STDMETHODIMP Next(
        ULONG cMediaTypes,          
        AM_MEDIA_TYPE ** ppMediaTypes,  
        ULONG * pcFetched           
    );

    STDMETHODIMP Skip(ULONG cMediaTypes);
    STDMETHODIMP Reset();
    STDMETHODIMP Clone(IEnumMediaTypes **ppEnum);
};                              

class  AM_NOVTABLE CBaseOutputPin : public CBasePin
{

protected:

    IMemAllocator *m_pAllocator;
    IMemInputPin *m_pInputPin;        
                                      

public:

    CBaseOutputPin(
        TCHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);
#ifdef UNICODE
    CBaseOutputPin(
        CHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);
#endif
    
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    
    
    virtual HRESULT DecideAllocator(IMemInputPin * pPin, IMemAllocator ** pAlloc);

    
    
    
    
    
    virtual HRESULT DecideBufferSize(
        IMemAllocator * pAlloc,
        ALLOCATOR_PROPERTIES * ppropInputRequest
    ) PURE;

    
    virtual HRESULT GetDeliveryBuffer(IMediaSample ** ppSample,
                                      REFERENCE_TIME * pStartTime,
                                      REFERENCE_TIME * pEndTime,
                                      DWORD dwFlags);

    
    
    
    
    virtual HRESULT Deliver(IMediaSample *);

    
    virtual HRESULT InitAllocator(IMemAllocator **ppAlloc);
    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();

    
    HRESULT Active(void);
    HRESULT Inactive(void);

    
    
    STDMETHODIMP EndOfStream(void);

    
    
    virtual HRESULT DeliverEndOfStream(void);

    
    
    
    STDMETHODIMP BeginFlush(void);
    STDMETHODIMP EndFlush(void);
    virtual HRESULT DeliverBeginFlush(void);
    virtual HRESULT DeliverEndFlush(void);

    
    
    virtual HRESULT DeliverNewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

    
    
    

    
    
    
};                        

class AM_NOVTABLE CBaseInputPin : public CBasePin,
                                  public IMemInputPin
{

protected:

    IMemAllocator *m_pAllocator;    

    
    
    
    BYTE m_bReadOnly;    

    
    
    BYTE m_bFlushing;

    
    AM_SAMPLE2_PROPERTIES m_SampleProps;

public:

    CBaseInputPin(
        TCHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);
#ifdef UNICODE
    CBaseInputPin(
        CHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);
#endif
    virtual ~CBaseInputPin();

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    
    STDMETHODIMP GetAllocator(IMemAllocator ** ppAllocator);

    
    
    STDMETHODIMP NotifyAllocator(
                    IMemAllocator * pAllocator,
                    BOOL bReadOnly);

    
    STDMETHODIMP Receive(IMediaSample *pSample);

    
    STDMETHODIMP ReceiveMultiple (
        IMediaSample **pSamples,
        long nSamples,
        long *nSamplesProcessed);

    
    STDMETHODIMP ReceiveCanBlock();

    
    
    
    
    STDMETHODIMP BeginFlush(void);

    
    
    
    
    
    STDMETHODIMP EndFlush(void);

    
    
    
    
    STDMETHODIMP GetAllocatorRequirements(ALLOCATOR_PROPERTIES*pProps);

    
    HRESULT BreakConnect();

    
    BOOL IsReadOnly() {
        return m_bReadOnly;
    };

    
    BOOL IsFlushing() {
        return m_bFlushing;
    };

    
    
    virtual HRESULT CheckStreaming();

    
    HRESULT PassNotify(Quality& q);  

    
    
    

    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    
      

    
    virtual HRESULT Inactive(void);

    
    AM_SAMPLE2_PROPERTIES * SampleProps() {
        ASSERT(m_SampleProps.cbData != 0);
        return &m_SampleProps;
    }

};        

class CDynamicOutputPin : public CBaseOutputPin,
                          public IPinFlowControl
{
public:
#ifdef UNICODE
    CDynamicOutputPin(
        CHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);
#endif

    CDynamicOutputPin(
        TCHAR *pObjectName,
        CBaseFilter *pFilter,
        CCritSec *pLock,
        HRESULT *phr,
        LPCWSTR pName);

    ~CDynamicOutputPin();

    
    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    
    STDMETHODIMP Disconnect(void);

    
    STDMETHODIMP Block(DWORD dwBlockFlags, HANDLE hEvent);

    
    void SetConfigInfo(IGraphConfig *pGraphConfig, HANDLE hStopEvent);

    #ifdef DEBUG
    virtual HRESULT Deliver(IMediaSample *pSample);
    virtual HRESULT DeliverEndOfStream(void);
    virtual HRESULT DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
    #endif 

    HRESULT DeliverBeginFlush(void);
    HRESULT DeliverEndFlush(void);

    HRESULT Inactive(void);
    HRESULT Active(void);
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    virtual HRESULT StartUsingOutputPin(void);
    virtual void StopUsingOutputPin(void);
    virtual bool StreamingThreadUsingOutputPin(void);

    HRESULT ChangeOutputFormat
        (
        const AM_MEDIA_TYPE *pmt,
        REFERENCE_TIME tSegmentStart,
        REFERENCE_TIME tSegmentStop,
        double dSegmentRate
        );
    HRESULT ChangeMediaType(const CMediaType *pmt);
    HRESULT DynamicReconnect(const CMediaType *pmt);

protected:
    HRESULT SynchronousBlockOutputPin(void);
    HRESULT AsynchronousBlockOutputPin(HANDLE hNotifyCallerPinBlockedEvent);
    HRESULT UnblockOutputPin(void);

    void BlockOutputPin(void);
    void ResetBlockState(void);

    static HRESULT WaitEvent(HANDLE hEvent);

    enum BLOCK_STATE
    {
        NOT_BLOCKED,
        PENDING,
        BLOCKED
    };

    
    
    
    CCritSec m_BlockStateLock;

    
    
    
    
    HANDLE m_hUnblockOutputPinEvent;

    
    
    
    
    HANDLE m_hNotifyCallerPinBlockedEvent;

    
    BLOCK_STATE m_BlockState;

    
    
    
    DWORD m_dwBlockCallerThreadID;

    
    
    
    
    
    
    
    
    DWORD m_dwNumOutstandingOutputPinUsers;

    
    
    
    HANDLE m_hStopEvent;
    IGraphConfig* m_pGraphConfig;

    
    
    
    BOOL m_bPinUsesReadOnlyAllocator;

private:
    HRESULT Initialize(void);
    HRESULT ChangeMediaTypeHelper(const CMediaType *pmt);

    #ifdef DEBUG
    void AssertValid(void);
    #endif 
};

class CAutoUsingOutputPin
{
public:
    CAutoUsingOutputPin( CDynamicOutputPin* pOutputPin, HRESULT* phr );
    ~CAutoUsingOutputPin();

private:
    CDynamicOutputPin* m_pOutputPin;
};

inline CAutoUsingOutputPin::CAutoUsingOutputPin( CDynamicOutputPin* pOutputPin, HRESULT* phr ) :
    m_pOutputPin(NULL)
{
    
    ASSERT( NULL != pOutputPin );
    ASSERT( NULL != phr );

    
    ASSERT( S_OK == *phr );

    HRESULT hr = pOutputPin->StartUsingOutputPin();
    if( FAILED( hr ) )
    {
        *phr = hr;
        return;
    }

    m_pOutputPin = pOutputPin;
}

inline CAutoUsingOutputPin::~CAutoUsingOutputPin()
{
    if( NULL != m_pOutputPin )
    {
        m_pOutputPin->StopUsingOutputPin();
    }
}

#ifdef DEBUG

inline HRESULT CDynamicOutputPin::Deliver(IMediaSample *pSample)
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    return CBaseOutputPin::Deliver(pSample);
}

inline HRESULT CDynamicOutputPin::DeliverEndOfStream(void)
{
    
    
    ASSERT( StreamingThreadUsingOutputPin() );

    return CBaseOutputPin::DeliverEndOfStream();
}

inline HRESULT CDynamicOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    return CBaseOutputPin::DeliverNewSegment(tStart, tStop, dRate);
}

#endif                                                                   

class CMediaSample : public IMediaSample2    
{

protected:

    friend class CBaseAllocator;

    
    enum { Sample_SyncPoint       = 0x01,   
           Sample_Preroll         = 0x02,   
           Sample_Discontinuity   = 0x04,   
           Sample_TypeChanged     = 0x08,   
           Sample_TimeValid       = 0x10,   
           Sample_MediaTimeValid  = 0x20,   
           Sample_TimeDiscontinuity = 0x40, 
           Sample_StopValid       = 0x100,  
           Sample_ValidFlags      = 0x1FF
         };

    

    DWORD            m_dwFlags;         
                                        
    DWORD            m_dwTypeSpecificFlags; 
    LPBYTE           m_pBuffer;         
    LONG             m_lActual;         
    LONG             m_cbBuffer;        
    CBaseAllocator  *m_pAllocator;      
    CMediaSample     *m_pNext;          
    REFERENCE_TIME   m_Start;           
    REFERENCE_TIME   m_End;             
    LONGLONG         m_MediaStart;      
    LONG             m_MediaEnd;        
    AM_MEDIA_TYPE    *m_pMediaType;     
    DWORD            m_dwStreamId;      
public:
    LONG             m_cRef;              

public:

    CMediaSample(
        TCHAR *pName,
        CBaseAllocator *pAllocator,
        HRESULT *phr,
        LPBYTE pBuffer = NULL,
        LONG length = 0);
#ifdef UNICODE
    CMediaSample(
        CHAR *pName,
        CBaseAllocator *pAllocator,
        HRESULT *phr,
        LPBYTE pBuffer = NULL,
        LONG length = 0);
#endif

    virtual ~CMediaSample();

    

    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    
    
    
    
    HRESULT SetPointer(BYTE * ptr, LONG cBytes);

    
    STDMETHODIMP GetPointer(BYTE ** ppBuffer);

    STDMETHODIMP_(LONG) GetSize(void);

    
    STDMETHODIMP GetTime(
        REFERENCE_TIME * pTimeStart,     
        REFERENCE_TIME * pTimeEnd
    );

    
    STDMETHODIMP SetTime(
        REFERENCE_TIME * pTimeStart,     
        REFERENCE_TIME * pTimeEnd
    );
    STDMETHODIMP IsSyncPoint(void);
    STDMETHODIMP SetSyncPoint(BOOL bIsSyncPoint);
    STDMETHODIMP IsPreroll(void);
    STDMETHODIMP SetPreroll(BOOL bIsPreroll);

    STDMETHODIMP_(LONG) GetActualDataLength(void);
    STDMETHODIMP SetActualDataLength(LONG lActual);

    

    STDMETHODIMP GetMediaType(AM_MEDIA_TYPE **ppMediaType);
    STDMETHODIMP SetMediaType(AM_MEDIA_TYPE *pMediaType);

    
    
    
    STDMETHODIMP IsDiscontinuity(void);
    
    
    STDMETHODIMP SetDiscontinuity(BOOL bDiscontinuity);

    
    STDMETHODIMP GetMediaTime(
    	LONGLONG * pTimeStart,
	LONGLONG * pTimeEnd
    );

    
    STDMETHODIMP SetMediaTime(
    	LONGLONG * pTimeStart,
	LONGLONG * pTimeEnd
    );

    
    STDMETHODIMP GetProperties(
        DWORD cbProperties,
        BYTE * pbProperties
    );

    STDMETHODIMP SetProperties(
        DWORD cbProperties,
        const BYTE * pbProperties
    );
};                                

class AM_NOVTABLE CBaseAllocator : public CUnknown,
                       public IMemAllocatorCallbackTemp, 
                       public CCritSec             
{
    class CSampleList;
    friend class CSampleList;

    
    static CMediaSample * &NextSample(CMediaSample *pSample)
    {
        return pSample->m_pNext;
    };

    
    class CSampleList
    {
    public:
        CSampleList() : m_List(NULL), m_nOnList(0) {};
#ifdef DEBUG
        ~CSampleList()
        {
            ASSERT(m_nOnList == 0);
        };
#endif
        CMediaSample *Head() const { return m_List; };
        CMediaSample *Next(CMediaSample *pSample) const { return CBaseAllocator::NextSample(pSample); };
        int GetCount() const { return m_nOnList; };
        void Add(CMediaSample *pSample)
        {
            ASSERT(pSample != NULL);
            CBaseAllocator::NextSample(pSample) = m_List;
            m_List = pSample;
            m_nOnList++;
        };
        CMediaSample *RemoveHead()
        {
            CMediaSample *pSample = m_List;
            if (pSample != NULL) {
                m_List = CBaseAllocator::NextSample(m_List);
                m_nOnList--;
            }
            return pSample;
        };
        void Remove(CMediaSample *pSample);

    public:
        CMediaSample *m_List;
        int           m_nOnList;
    };
protected:

    CSampleList m_lFree;        

    

    HANDLE m_hSem;              
    long m_lWaiting;            
    long m_lCount;              
    long m_lAllocated;          
    long m_lSize;               
    long m_lAlignment;          
    long m_lPrefix;             
    BOOL m_bChanged;            

    
    BOOL m_bCommitted;
    
    
    BOOL m_bDecommitInProgress;

    
    IMemAllocatorNotifyCallbackTemp *m_pNotify;

    BOOL m_fEnableReleaseCallback;

    
    
    virtual void Free(void) PURE;

    
    virtual HRESULT Alloc(void);

public:

    CBaseAllocator(
        TCHAR *, LPUNKNOWN, HRESULT *,
        BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#ifdef UNICODE
    CBaseAllocator(
        CHAR *, LPUNKNOWN, HRESULT *,
        BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#endif
    virtual ~CBaseAllocator();

    DECLARE_IUNKNOWN

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    STDMETHODIMP SetProperties(
		    ALLOCATOR_PROPERTIES* pRequest,
		    ALLOCATOR_PROPERTIES* pActual);

    
    STDMETHODIMP GetProperties(
		    ALLOCATOR_PROPERTIES* pProps);

    
    
    STDMETHODIMP Commit();

    
    
    STDMETHODIMP Decommit();

    
    
    
    
    
    
    
    

    STDMETHODIMP GetBuffer(IMediaSample **ppBuffer,
                           REFERENCE_TIME * pStartTime,
                           REFERENCE_TIME * pEndTime,
                           DWORD dwFlags);

    
    STDMETHODIMP ReleaseBuffer(IMediaSample *pBuffer);
    

    STDMETHODIMP SetNotify(IMemAllocatorNotifyCallbackTemp *pNotify);

    STDMETHODIMP GetFreeCount(LONG *plBuffersFree);

    
    void NotifySample();

    
    void SetWaiting() { m_lWaiting++; };
};                                    

STDAPI CreateMemoryAllocator(IMemAllocator **ppAllocator);

class CMemAllocator : public CBaseAllocator
{

protected:

    LPBYTE m_pBuffer;   

    
    
    void Free(void);

    
    
    void ReallyFree(void);

    
    HRESULT Alloc(void);

public:
    
    static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *);

    STDMETHODIMP SetProperties(
		    ALLOCATOR_PROPERTIES* pRequest,
		    ALLOCATOR_PROPERTIES* pActual);

    CMemAllocator(TCHAR *, LPUNKNOWN, HRESULT *);
#ifdef UNICODE
    CMemAllocator(CHAR *, LPUNKNOWN, HRESULT *);
#endif
    ~CMemAllocator();
};  

STDAPI
AMovieSetupRegisterFilter( const AMOVIESETUP_FILTER * const psetupdata
                         , IFilterMapper *                  pIFM
                         , BOOL                             bRegister  );                

#endif     

