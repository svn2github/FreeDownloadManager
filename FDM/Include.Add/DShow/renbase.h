/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#ifndef __RENBASE__
#define __RENBASE__    

class CBaseRenderer;
class CBaseVideoRenderer;
class CRendererInputPin;    

class CRendererInputPin : public CBaseInputPin
{
protected:

    CBaseRenderer *m_pRenderer;

public:

    CRendererInputPin(CBaseRenderer *pRenderer,
                      HRESULT *phr,
                      LPCWSTR Name);

    

    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);
    HRESULT SetMediaType(const CMediaType *pmt);
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT Active();
    HRESULT Inactive();

    

    STDMETHODIMP QueryId(LPWSTR *Id);
    STDMETHODIMP EndOfStream();
    STDMETHODIMP BeginFlush();
    STDMETHODIMP EndFlush();
    STDMETHODIMP Receive(IMediaSample *pMediaSample);

    
    IMemAllocator inline *Allocator() const
    {
        return m_pAllocator;
    }
};    

class CBaseRenderer : public CBaseFilter
{
protected:

    friend class CRendererInputPin;

    friend void CALLBACK EndOfStreamTimer(UINT uID,      
                                          UINT uMsg,     
                                          DWORD_PTR dwUser,  
                                          DWORD_PTR dw1,     
                                          DWORD_PTR dw2);    

    CRendererPosPassThru *m_pPosition;  
    CAMEvent m_RenderEvent;             
    CAMEvent m_ThreadSignal;            
    CAMEvent m_evComplete;              
    BOOL m_bAbort;                      
    BOOL m_bStreaming;                  
    DWORD_PTR m_dwAdvise;                   
    IMediaSample *m_pMediaSample;       
    BOOL m_bEOS;                        
    BOOL m_bEOSDelivered;               
    CRendererInputPin *m_pInputPin;     
    CCritSec m_InterfaceLock;           
    CCritSec m_RendererLock;            
    IQualityControl * m_pQSink;         
    BOOL m_bRepaintStatus;              
    
    volatile BOOL  m_bInReceive;        
                                        
    REFERENCE_TIME m_SignalTime;        
    UINT m_EndOfStreamTimer;            

public:

    CBaseRenderer(REFCLSID RenderClass, 
                  TCHAR *pName,         
                  LPUNKNOWN pUnk,       
                  HRESULT *phr);        

    ~CBaseRenderer();

    

    virtual HRESULT GetMediaPositionInterface(REFIID riid,void **ppv);
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);

    virtual HRESULT SourceThreadCanWait(BOOL bCanWait);

#ifdef DEBUG
    
    void DisplayRendererState();
#endif
    virtual HRESULT WaitForRenderTime();
    virtual HRESULT CompleteStateChange(FILTER_STATE OldState);

    

    BOOL IsEndOfStream() { return m_bEOS; };
    BOOL IsEndOfStreamDelivered() { return m_bEOSDelivered; };
    BOOL IsStreaming() { return m_bStreaming; };
    void SetAbortSignal(BOOL bAbort) { m_bAbort = bAbort; };
    virtual void OnReceiveFirstSample(IMediaSample *pMediaSample) { };
    CAMEvent *GetRenderEvent() { return &m_RenderEvent; };

    

    void Ready() { m_evComplete.Set(); };
    void NotReady() { m_evComplete.Reset(); };
    BOOL CheckReady() { return m_evComplete.Check(); };

    virtual int GetPinCount();
    virtual CBasePin *GetPin(int n);
    FILTER_STATE GetRealState();
    void SendRepaint();
    void SendNotifyWindow(IPin *pPin,HWND hwnd);
    BOOL OnDisplayChange();
    void SetRepaintStatus(BOOL bRepaint);

    

    STDMETHODIMP Stop();
    STDMETHODIMP Pause();
    STDMETHODIMP Run(REFERENCE_TIME StartTime);
    STDMETHODIMP GetState(DWORD dwMSecs,FILTER_STATE *State);
    STDMETHODIMP FindPin(LPCWSTR Id, IPin **ppPin);

    

    virtual void OnRenderStart(IMediaSample *pMediaSample);
    virtual void OnRenderEnd(IMediaSample *pMediaSample);
    virtual HRESULT OnStartStreaming() { return NOERROR; };
    virtual HRESULT OnStopStreaming() { return NOERROR; };
    virtual void OnWaitStart() { };
    virtual void OnWaitEnd() { };
    virtual void PrepareRender() { };

#ifdef PERF
    REFERENCE_TIME m_trRenderStart; 
                                    
    int m_idBaseStamp;              
    int m_idBaseRenderTime;         
    int m_idBaseAccuracy;           
#endif

    

    virtual BOOL ScheduleSample(IMediaSample *pMediaSample);
    virtual HRESULT GetSampleTimes(IMediaSample *pMediaSample,
                                   REFERENCE_TIME *pStartTime,
                                   REFERENCE_TIME *pEndTime);

    virtual HRESULT ShouldDrawSampleNow(IMediaSample *pMediaSample,
                                        REFERENCE_TIME *ptrStart,
                                        REFERENCE_TIME *ptrEnd);

    

    void TimerCallback();
    void ResetEndOfStreamTimer();
    HRESULT NotifyEndOfStream();
    virtual HRESULT SendEndOfStream();
    virtual HRESULT ResetEndOfStream();
    virtual HRESULT EndOfStream();

    

    void SignalTimerFired();
    virtual HRESULT CancelNotification();
    virtual HRESULT ClearPendingSample();

    

    virtual HRESULT Active();
    virtual HRESULT Inactive();
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();
    virtual HRESULT BeginFlush();
    virtual HRESULT EndFlush();

    

    virtual HRESULT BreakConnect();
    virtual HRESULT SetMediaType(const CMediaType *pmt);
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    

    virtual HRESULT PrepareReceive(IMediaSample *pMediaSample);
    virtual HRESULT Receive(IMediaSample *pMediaSample);
    virtual BOOL HaveCurrentSample();
    virtual IMediaSample *GetCurrentSample();
    virtual HRESULT Render(IMediaSample *pMediaSample);

    
    virtual HRESULT DoRenderSample(IMediaSample *pMediaSample) PURE;
    virtual HRESULT CheckMediaType(const CMediaType *) PURE;

    
    void WaitForReceiveToComplete();
};                                        

#define AVGPERIOD 4
#define DO_MOVING_AVG(avg,obs) (avg = (1024*obs + (AVGPERIOD-1)*avg)/AVGPERIOD)  

class CBaseVideoRenderer : public CBaseRenderer,    
                           public IQualProp,        
                           public IQualityControl   
{
protected:

    
    
    

    
    
    

    
    
    

    
    
    
    

    int m_nNormal;                  
                                    
                                    

#ifdef PERF
    BOOL m_bDrawLateFrames;         
                                    
#endif

    BOOL m_bSupplierHandlingQuality;
                                    
                                    
                                    
                                    

    
    
    
    
    
    
    
    

    
    
    
        

    
    
    
    
    int m_trThrottle;

    
    
    
    int m_trRenderAvg;              
    int m_trRenderLast;             
    int m_tRenderStart;             
                                    

    
    
    
    
    int m_trEarliness;

    
    
    
    
    
    
    
    
    int m_trTarget;

    
    
    
    int m_trWaitAvg;                
                                    
                                    

    
    
    
    int m_trFrameAvg;               
    int m_trDuration;               

#ifdef PERF
    
    int m_idTimeStamp;              
    int m_idEarliness;              
    int m_idTarget;                 
    int m_idWaitReal;               
    int m_idWait;                   
    int m_idFrameAccuracy;          
    int m_idRenderAvg;              
    int m_idSchLateTime;            
    int m_idQualityRate;            
    int m_idQualityTime;            
    int m_idDecision;               
    int m_idDuration;               
    int m_idThrottle;               
    
    
#endif 
    REFERENCE_TIME m_trRememberStampForPerf;  
                                              
#ifdef PERF
    REFERENCE_TIME m_trRememberFrameForPerf;  

    
    int m_idFrameAvg;
    int m_idWaitAvg;
#endif

    
    
    

    int m_cFramesDropped;           
    int m_cFramesDrawn;             
                                    

    
    LONGLONG m_iTotAcc;                  
    LONGLONG m_iSumSqAcc;           

    
    REFERENCE_TIME m_trLastDraw;    
    LONGLONG m_iSumSqFrameTime;     
    LONGLONG m_iSumFrameTime;            

    
    
    
    
    
    int m_trLate;                   
    int m_trFrame;                  

    int m_tStreamingStart;          
                                    
                                    
#ifdef PERF
    LONGLONG m_llTimeOffset;        
#endif

public:  

    CBaseVideoRenderer(REFCLSID RenderClass, 
                       TCHAR *pName,         
                       LPUNKNOWN pUnk,       
                       HRESULT *phr);        

    ~CBaseVideoRenderer();

    

    STDMETHODIMP SetSink( IQualityControl * piqc);
    STDMETHODIMP Notify( IBaseFilter * pSelf, Quality q);

    

    void OnRenderStart(IMediaSample *pMediaSample);
    void OnRenderEnd(IMediaSample *pMediaSample);
    void OnWaitStart();
    void OnWaitEnd();
    HRESULT OnStartStreaming();
    HRESULT OnStopStreaming();
    void ThrottleWait();

    

    void PreparePerformanceData(int trLate, int trFrame);
    virtual void RecordFrameLateness(int trLate, int trFrame);
    virtual void OnDirectRender(IMediaSample *pMediaSample);
    virtual HRESULT ResetStreamingTimes();
    BOOL ScheduleSample(IMediaSample *pMediaSample);
    HRESULT ShouldDrawSampleNow(IMediaSample *pMediaSample,
                                REFERENCE_TIME *ptrStart,
                                REFERENCE_TIME *ptrEnd);

    virtual HRESULT SendQuality(REFERENCE_TIME trLate, REFERENCE_TIME trRealStream);
    STDMETHODIMP JoinFilterGraph(IFilterGraph * pGraph, LPCWSTR pName);

    
    
    
    
    
    
    
    
    HRESULT GetStdDev(
        int nSamples,
        int *piResult,
        LONGLONG llSumSq,
        LONGLONG iTot
    );
public:

    

    STDMETHODIMP get_FramesDroppedInRenderer(int *cFramesDropped);
    STDMETHODIMP get_FramesDrawn(int *pcFramesDrawn);
    STDMETHODIMP get_AvgFrameRate(int *piAvgFrameRate);
    STDMETHODIMP get_Jitter(int *piJitter);
    STDMETHODIMP get_AvgSyncOffset(int *piAvg);
    STDMETHODIMP get_DevSyncOffset(int *piDev);

    

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,VOID **ppv);
};

#endif 

