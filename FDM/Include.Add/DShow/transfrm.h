/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                                    

#ifndef __TRANSFRM__
#define __TRANSFRM__          

class CTransformFilter;        

class CTransformInputPin : public CBaseInputPin
{
    friend class CTransformFilter;

protected:
    CTransformFilter *m_pTransformFilter;  

public:

    CTransformInputPin(
        TCHAR *pObjectName,
        CTransformFilter *pTransformFilter,
        HRESULT * phr,
        LPCWSTR pName);
#ifdef UNICODE
    CTransformInputPin(
        char *pObjectName,
        CTransformFilter *pTransformFilter,
        HRESULT * phr,
        LPCWSTR pName);
#endif

    STDMETHODIMP QueryId(LPWSTR * Id)
    {
        return AMGetWideString(L"In", Id);
    }

    

    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);

    
    HRESULT CheckMediaType(const CMediaType* mtIn);

    
    HRESULT SetMediaType(const CMediaType* mt);

    

    
    
    
    STDMETHODIMP Receive(IMediaSample * pSample);

    
    
    STDMETHODIMP EndOfStream(void);

    
    STDMETHODIMP BeginFlush(void);

    
    STDMETHODIMP EndFlush(void);

    STDMETHODIMP NewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

    
    virtual HRESULT CheckStreaming();

    
public:
    CMediaType& CurrentMediaType() { return m_mt; };

};        

class CTransformOutputPin : public CBaseOutputPin
{
    friend class CTransformFilter;

protected:
    CTransformFilter *m_pTransformFilter;

public:

    
    IUnknown * m_pPosition;

    CTransformOutputPin(
        TCHAR *pObjectName,
        CTransformFilter *pTransformFilter,
        HRESULT * phr,
        LPCWSTR pName);
#ifdef UNICODE
    CTransformOutputPin(
        CHAR *pObjectName,
        CTransformFilter *pTransformFilter,
        HRESULT * phr,
        LPCWSTR pName);
#endif
    ~CTransformOutputPin();

    
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

    

    STDMETHODIMP QueryId(LPWSTR * Id)
    {
        return AMGetWideString(L"Out", Id);
    }

    

    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);

    
    HRESULT CheckMediaType(const CMediaType* mtOut);

    
    HRESULT SetMediaType(const CMediaType *pmt);

    
    
    HRESULT DecideBufferSize(
                IMemAllocator * pAlloc,
                ALLOCATOR_PROPERTIES *pProp);

    
    HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);

    
    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    
public:
    CMediaType& CurrentMediaType() { return m_mt; };
};  

class AM_NOVTABLE CTransformFilter : public CBaseFilter
{

public:

    
    

    virtual int GetPinCount();
    virtual CBasePin * GetPin(int n);
    STDMETHODIMP FindPin(LPCWSTR Id, IPin **ppPin);

    
    
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

public:

    CTransformFilter(TCHAR *, LPUNKNOWN, REFCLSID clsid);
#ifdef UNICODE
    CTransformFilter(CHAR *, LPUNKNOWN, REFCLSID clsid);
#endif
    ~CTransformFilter();

    
    
    

    

    virtual HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);

    
    virtual HRESULT CheckInputType(const CMediaType* mtIn) PURE;

    
    virtual HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut) PURE;

    
    

    
    virtual HRESULT DecideBufferSize(
                        IMemAllocator * pAllocator,
                        ALLOCATOR_PROPERTIES *pprop) PURE;

    
    virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType) PURE;    

    
    
    

    
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();

    
    virtual HRESULT AlterQuality(Quality q);

    
    virtual HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);

    
    virtual HRESULT CheckConnect(PIN_DIRECTION dir,IPin *pPin);
    virtual HRESULT BreakConnect(PIN_DIRECTION dir);
    virtual HRESULT CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin);

    
    virtual HRESULT Receive(IMediaSample *pSample);

    
    HRESULT InitializeOutputSample(IMediaSample *pSample, IMediaSample **ppOutSample);

    
    virtual HRESULT EndOfStream(void);
    virtual HRESULT BeginFlush(void);
    virtual HRESULT EndFlush(void);
    virtual HRESULT NewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

#ifdef PERF
    
    
    virtual void RegisterPerfId()
         {m_idTransform = MSR_REGISTER(TEXT("Transform"));}
#endif       

protected:

#ifdef PERF
    int m_idTransform;                 
#endif
    BOOL m_bEOSDelivered;              
    BOOL m_bSampleSkipped;             
    BOOL m_bQualityChanged;            

    

    CCritSec m_csFilter;

    
    
    
    
    
    
    
    

    CCritSec m_csReceive;

    

    friend class CTransformInputPin;
    friend class CTransformOutputPin;
    CTransformInputPin *m_pInput;
    CTransformOutputPin *m_pOutput;
};

#endif   

