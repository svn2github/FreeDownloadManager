/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                                                          

#include <streams.h>          

STDAPI CreateMemoryAllocator(IMemAllocator **ppAllocator)
{
    return CoCreateInstance(CLSID_MemoryAllocator,
                            0,
                            CLSCTX_INPROC_SERVER,
                            IID_IMemAllocator,
                            (void **)ppAllocator);
}    

STDAPI CreatePosPassThru(
    LPUNKNOWN pAgg,
    BOOL bRenderer,
    IPin *pPin,
    IUnknown **ppPassThru
)
{
    *ppPassThru = NULL;
    IUnknown *pUnkSeek;
    HRESULT hr = CoCreateInstance(CLSID_SeekingPassThru,
                                  pAgg,
                                  CLSCTX_INPROC_SERVER,
                                  IID_IUnknown,
                                  (void **)&pUnkSeek
                                 );
    if (FAILED(hr)) {
        return hr;
    }

    ISeekingPassThru *pPassThru;
    hr = pUnkSeek->QueryInterface(IID_ISeekingPassThru, (void**)&pPassThru);
    if (FAILED(hr)) {
        pUnkSeek->Release();
        return hr;
    }
    hr = pPassThru->Init(bRenderer, pPin);
    pPassThru->Release();
    if (FAILED(hr)) {
        pUnkSeek->Release();
        return hr;
    }
    *ppPassThru = pUnkSeek;
    return S_OK;
}    

#define CONNECT_TRACE_LEVEL 3                  

CBaseMediaFilter::CBaseMediaFilter(const TCHAR  *pName,
                   LPUNKNOWN    pUnk,
                   CCritSec *pLock,
                   REFCLSID clsid) :
    CUnknown(pName, pUnk),
    m_pLock(pLock),
    m_clsid(clsid),
    m_State(State_Stopped),
    m_pClock(NULL)
{
}      

CBaseMediaFilter::~CBaseMediaFilter()
{
    
    

    

    if (m_pClock) {
        m_pClock->Release();
        m_pClock = NULL;
    }
}      

STDMETHODIMP
CBaseMediaFilter::NonDelegatingQueryInterface(
    REFIID riid,
    void ** ppv)
{
    if (riid == IID_IMediaFilter) {
        return GetInterface((IMediaFilter *) this, ppv);
    } else if (riid == IID_IPersist) {
        return GetInterface((IPersist *) this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}  

STDMETHODIMP
CBaseMediaFilter::GetClassID(CLSID *pClsID)
{
    CheckPointer(pClsID,E_POINTER);
    ValidateReadWritePtr(pClsID,sizeof(CLSID));
    *pClsID = m_clsid;
    return NOERROR;
}    

STDMETHODIMP
CBaseMediaFilter::GetState(DWORD dwMSecs, FILTER_STATE *State)
{
    UNREFERENCED_PARAMETER(dwMSecs);
    CheckPointer(State,E_POINTER);
    ValidateReadWritePtr(State,sizeof(FILTER_STATE));

    *State = m_State;
    return S_OK;
}      

STDMETHODIMP
CBaseMediaFilter::SetSyncSource(IReferenceClock *pClock)
{
    CAutoLock cObjectLock(m_pLock);

    
    if (pClock) {
        pClock->AddRef();
    }

    
    if (m_pClock) {
        m_pClock->Release();
    }

    
    
    m_pClock = pClock;

    return NOERROR;
}  

STDMETHODIMP
CBaseMediaFilter::GetSyncSource(IReferenceClock **pClock)
{
    CheckPointer(pClock,E_POINTER);
    ValidateReadWritePtr(pClock,sizeof(IReferenceClock *));
    CAutoLock cObjectLock(m_pLock);

    if (m_pClock) {
        
        m_pClock->AddRef();
    }
    *pClock = (IReferenceClock*)m_pClock;
    return NOERROR;
}      

STDMETHODIMP
CBaseMediaFilter::Stop()
{
    CAutoLock cObjectLock(m_pLock);

    m_State = State_Stopped;
    return S_OK;
}      

STDMETHODIMP
CBaseMediaFilter::Pause()
{
    CAutoLock cObjectLock(m_pLock);

    m_State = State_Paused;
    return S_OK;
}                    

STDMETHODIMP
CBaseMediaFilter::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);

    
    m_tStart = tStart;

    if (m_State == State_Stopped){
        HRESULT hr = Pause();

        if (FAILED(hr)) {
            return hr;
        }
    }
    m_State = State_Running;
    return S_OK;
}        

HRESULT
CBaseMediaFilter::StreamTime(CRefTime& rtStream)
{
    
    
    

    if (m_pClock == NULL) {
        return VFW_E_NO_CLOCK;
    }

    
    HRESULT hr = m_pClock->GetTime((REFERENCE_TIME*)&rtStream);
    if (FAILED(hr)) {
        return hr;
    }

    
    rtStream -= m_tStart;

    return S_OK;
}                    

STDMETHODIMP CBaseFilter::NonDelegatingQueryInterface(REFIID riid,
                                                      void **ppv)
{
    

    if (riid == IID_IBaseFilter) {
        return GetInterface((IBaseFilter *) this, ppv);
    } else if (riid == IID_IMediaFilter) {
        return GetInterface((IMediaFilter *) this, ppv);
    } else if (riid == IID_IPersist) {
        return GetInterface((IPersist *) this, ppv);
    } else if (riid == IID_IAMovieSetup) {
        return GetInterface((IAMovieSetup *) this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}

#ifdef DEBUG
STDMETHODIMP_(ULONG) CBaseFilter::NonDelegatingRelease()
{
    if (m_cRef == 1) {
        KASSERT(m_pGraph == NULL);
    }
    return CUnknown::NonDelegatingRelease();
}
#endif      

CBaseFilter::CBaseFilter(const TCHAR    *pName,
             LPUNKNOWN  pUnk,
             CCritSec   *pLock,
             REFCLSID   clsid) :
    CUnknown( pName, pUnk ),
    m_pLock(pLock),
    m_clsid(clsid),
    m_State(State_Stopped),
    m_pClock(NULL),
    m_pGraph(NULL),
    m_pSink(NULL),
    m_pName(NULL),
    m_PinVersion(1)
{

    ASSERT(pLock != NULL);
}    

CBaseFilter::CBaseFilter(TCHAR     *pName,
                         LPUNKNOWN  pUnk,
                         CCritSec  *pLock,
                         REFCLSID   clsid,
                         HRESULT   *phr) :
    CUnknown( pName, pUnk ),
    m_pLock(pLock),
    m_clsid(clsid),
    m_State(State_Stopped),
    m_pClock(NULL),
    m_pGraph(NULL),
    m_pSink(NULL),
    m_pName(NULL),
    m_PinVersion(1)
{

    ASSERT(pLock != NULL);
    UNREFERENCED_PARAMETER(phr);
}

#ifdef UNICODE
CBaseFilter::CBaseFilter(const CHAR *pName,
             LPUNKNOWN  pUnk,
             CCritSec   *pLock,
             REFCLSID   clsid) :
    CUnknown( pName, pUnk ),
    m_pLock(pLock),
    m_clsid(clsid),
    m_State(State_Stopped),
    m_pClock(NULL),
    m_pGraph(NULL),
    m_pSink(NULL),
    m_pName(NULL),
    m_PinVersion(1)
{

    ASSERT(pLock != NULL);
}
CBaseFilter::CBaseFilter(CHAR     *pName,
                         LPUNKNOWN  pUnk,
                         CCritSec  *pLock,
                         REFCLSID   clsid,
                         HRESULT   *phr) :
    CUnknown( pName, pUnk ),
    m_pLock(pLock),
    m_clsid(clsid),
    m_State(State_Stopped),
    m_pClock(NULL),
    m_pGraph(NULL),
    m_pSink(NULL),
    m_pName(NULL),
    m_PinVersion(1)
{

    ASSERT(pLock != NULL);
    UNREFERENCED_PARAMETER(phr);
}
#endif    

CBaseFilter::~CBaseFilter()
{

    
    

    delete[] m_pName;

    
    

    
    if (m_pClock) {
        m_pClock->Release();
        m_pClock = NULL;
    }
}  

STDMETHODIMP
CBaseFilter::GetClassID(CLSID *pClsID)
{
    CheckPointer(pClsID,E_POINTER);
    ValidateReadWritePtr(pClsID,sizeof(CLSID));
    *pClsID = m_clsid;
    return NOERROR;
}  

STDMETHODIMP
CBaseFilter::GetState(DWORD dwMSecs, FILTER_STATE *State)
{
    UNREFERENCED_PARAMETER(dwMSecs);
    CheckPointer(State,E_POINTER);
    ValidateReadWritePtr(State,sizeof(FILTER_STATE));

    *State = m_State;
    return S_OK;
}      

STDMETHODIMP
CBaseFilter::SetSyncSource(IReferenceClock *pClock)
{
    CAutoLock cObjectLock(m_pLock);

    
    if (pClock) {
        pClock->AddRef();
    }

    
    if (m_pClock) {
        m_pClock->Release();
    }

    
    
    m_pClock = pClock;

    return NOERROR;
}  

STDMETHODIMP
CBaseFilter::GetSyncSource(IReferenceClock **pClock)
{
    CheckPointer(pClock,E_POINTER);
    ValidateReadWritePtr(pClock,sizeof(IReferenceClock *));
    CAutoLock cObjectLock(m_pLock);

    if (m_pClock) {
        
        m_pClock->AddRef();
    }
    *pClock = (IReferenceClock*)m_pClock;
    return NOERROR;
}        

STDMETHODIMP
CBaseFilter::Stop()
{
    CAutoLock cObjectLock(m_pLock);
    HRESULT hr = NOERROR;

    
    if (m_State != State_Stopped) {
        int cPins = GetPinCount();
        for (int c = 0; c < cPins; c++) {

            CBasePin *pPin = GetPin(c);

            
            
            
            
            

            if (pPin->IsConnected()) {
                HRESULT hrTmp = pPin->Inactive();
                if (FAILED(hrTmp) && SUCCEEDED(hr)) {
                    hr = hrTmp;
                }
            }
        }
    }  

    m_State = State_Stopped;
    return hr;
}        

STDMETHODIMP
CBaseFilter::Pause()
{
    CAutoLock cObjectLock(m_pLock);

    
    if (m_State == State_Stopped) {
        int cPins = GetPinCount();
        for (int c = 0; c < cPins; c++) {

            CBasePin *pPin = GetPin(c);

            
            

            if (pPin->IsConnected()) {
                HRESULT hr = pPin->Active();
                if (FAILED(hr)) {
                    return hr;
                }
            }
        }
    }    

    m_State = State_Paused;
    return S_OK;
}                  

STDMETHODIMP
CBaseFilter::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);

    
    m_tStart = tStart;

    if (m_State == State_Stopped){
    HRESULT hr = Pause();

    if (FAILED(hr)) {
        return hr;
    }
    }
    
    if (m_State != State_Running) {
        int cPins = GetPinCount();
        for (int c = 0; c < cPins; c++) {

            CBasePin *pPin = GetPin(c);

            
            

            if (pPin->IsConnected()) {
                HRESULT hr = pPin->Run(tStart);
                if (FAILED(hr)) {
                    return hr;
                }
            }
        }
    }  

    m_State = State_Running;
    return S_OK;
}      

HRESULT
CBaseFilter::StreamTime(CRefTime& rtStream)
{
    
    
    

    if (m_pClock == NULL) {
        return VFW_E_NO_CLOCK;
    }

    
    HRESULT hr = m_pClock->GetTime((REFERENCE_TIME*)&rtStream);
    if (FAILED(hr)) {
        return hr;
    }

    
    rtStream -= m_tStart;

    return S_OK;
}      

STDMETHODIMP
CBaseFilter::EnumPins(IEnumPins **ppEnum)
{
    CheckPointer(ppEnum,E_POINTER);
    ValidateReadWritePtr(ppEnum,sizeof(IEnumPins *));

    

    *ppEnum = new CEnumPins(this,
                        NULL);

    return *ppEnum == NULL ? E_OUTOFMEMORY : NOERROR;
}      

STDMETHODIMP
CBaseFilter::FindPin(
    LPCWSTR Id,
    IPin ** ppPin
)
{
    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(IPin *));

    
    CAutoLock lck(m_pLock);
    int iCount = GetPinCount();
    for (int i = 0; i < iCount; i++) {
        CBasePin *pPin = GetPin(i);
        ASSERT(pPin != NULL);

        if (0 == lstrcmpW(pPin->Name(), Id)) {
            
            
            
            *ppPin = pPin;
            pPin->AddRef();
            return S_OK;
        }
    }
    *ppPin = NULL;
    return VFW_E_NOT_FOUND;
}    

STDMETHODIMP
CBaseFilter::QueryFilterInfo(FILTER_INFO * pInfo)
{
    CheckPointer(pInfo,E_POINTER);
    ValidateReadWritePtr(pInfo,sizeof(FILTER_INFO));

    if (m_pName) {
        lstrcpynW(pInfo->achName, m_pName, sizeof(pInfo->achName)/sizeof(WCHAR));
    } else {
        pInfo->achName[0] = L'\0';
    }
    pInfo->pGraph = m_pGraph;
    if (m_pGraph)
        m_pGraph->AddRef();
    return NOERROR;
}      

STDMETHODIMP
CBaseFilter::JoinFilterGraph(
    IFilterGraph * pGraph,
    LPCWSTR pName)
{
    CAutoLock cObjectLock(m_pLock);

    

    m_pGraph = pGraph;
    if (m_pGraph) {
        HRESULT hr = m_pGraph->QueryInterface(IID_IMediaEventSink,
                        (void**) &m_pSink);
        if (FAILED(hr)) {
            ASSERT(m_pSink == NULL);
        }
        else m_pSink->Release();        
    } else {
        
        
        
        m_pSink = NULL;
    }  

    if (m_pName) {
        delete[] m_pName;
        m_pName = NULL;
    }

    if (pName) {
        DWORD nameLen = lstrlenW(pName)+1;
        m_pName = new WCHAR[nameLen];
        if (m_pName) {
            CopyMemory(m_pName, pName, nameLen*sizeof(WCHAR));
        } else {
            
        }
    }  

    return NOERROR;
}        

STDMETHODIMP
CBaseFilter::QueryVendorInfo(
    LPWSTR* pVendorInfo)
{
    UNREFERENCED_PARAMETER(pVendorInfo);
    return E_NOTIMPL;
}        

HRESULT
CBaseFilter::NotifyEvent(
    long EventCode,
    LONG_PTR EventParam1,
    LONG_PTR EventParam2)
{
    
    IMediaEventSink *pSink = m_pSink;
    if (pSink) {
        if (EC_COMPLETE == EventCode) {
            EventParam2 = (LONG_PTR)(IBaseFilter*)this;
        }

        return pSink->Notify(EventCode, EventParam1, EventParam2);
    } else {
        return E_NOTIMPL;
    }
}        

HRESULT
CBaseFilter::ReconnectPin(
    IPin *pPin,
    AM_MEDIA_TYPE const *pmt
)
{
    IFilterGraph2 *pGraph2;
    if (m_pGraph != NULL) {
        HRESULT hr = m_pGraph->QueryInterface(IID_IFilterGraph2, (void **)&pGraph2);
        if (SUCCEEDED(hr)) {
            hr = pGraph2->ReconnectEx(pPin, pmt);
            pGraph2->Release();
            return hr;
        } else {
            return m_pGraph->Reconnect(pPin);
        }
    } else {
        return E_NOINTERFACE;
    }
}        

LONG CBaseFilter::GetPinVersion()
{
    return m_PinVersion;
}      

void CBaseFilter::IncrementPinVersion()
{
    InterlockedIncrement(&m_PinVersion);
}    

STDMETHODIMP CBaseFilter::Register()
{
    
    
    LPAMOVIESETUP_FILTER psetupdata = GetSetupData();

    
    
    if( NULL == psetupdata ) return S_FALSE;

    
    
    
    HRESULT hr = CoInitialize( (LPVOID)NULL );
    ASSERT( SUCCEEDED(hr) );

    
    
    IFilterMapper *pIFM;
    hr = CoCreateInstance( CLSID_FilterMapper
                             , NULL
                             , CLSCTX_INPROC_SERVER
                             , IID_IFilterMapper
                             , (void **)&pIFM       );
    if( SUCCEEDED(hr) )
    {
        hr = AMovieSetupRegisterFilter( psetupdata, pIFM, TRUE );
        pIFM->Release();
    }

    
    
    CoFreeUnusedLibraries();
    CoUninitialize();

    return NOERROR;
}      

STDMETHODIMP CBaseFilter::Unregister()
{
    
    
    LPAMOVIESETUP_FILTER psetupdata = GetSetupData();

    
    
    if( NULL == psetupdata ) return S_FALSE;

    
    
    
    HRESULT hr = CoInitialize( (LPVOID)NULL );
    ASSERT( SUCCEEDED(hr) );

    
    
    IFilterMapper *pIFM;
    hr = CoCreateInstance( CLSID_FilterMapper
                             , NULL
                             , CLSCTX_INPROC_SERVER
                             , IID_IFilterMapper
                             , (void **)&pIFM       );
    if( SUCCEEDED(hr) )
    {
        hr = AMovieSetupRegisterFilter( psetupdata, pIFM, FALSE );

        
        
        pIFM->Release();
    }

    
    
    CoFreeUnusedLibraries();
    CoUninitialize();

    
    
    
    
    
    if( 0x80070002 == hr)
      return NOERROR;
    else
      return hr;
}                

CEnumPins::CEnumPins(CBaseFilter *pFilter,
             CEnumPins *pEnumPins) :
    m_Position(0),
    m_PinCount(0),
    m_pFilter(pFilter),
    m_cRef(1),               
    m_PinCache(NAME("Pin Cache"))
{

#ifdef DEBUG
    m_dwCookie = DbgRegisterObjectCreation("CEnumPins", 0);
#endif

    

    ASSERT(pFilter != NULL);

    
    m_pFilter->AddRef();

    

    if (pEnumPins == NULL) {
        m_Version = m_pFilter->GetPinVersion();
        m_PinCount = m_pFilter->GetPinCount();
    } else {
        ASSERT(m_Position <= m_PinCount);
        m_Position = pEnumPins->m_Position;
        m_PinCount = pEnumPins->m_PinCount;
        m_Version = pEnumPins->m_Version;
        m_PinCache.AddTail(&(pEnumPins->m_PinCache));
    }
}      

CEnumPins::~CEnumPins()
{
    m_pFilter->Release();

#ifdef DEBUG
    DbgRegisterObjectDestruction(m_dwCookie);
#endif
}      

STDMETHODIMP
CEnumPins::QueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv, E_POINTER);

    

    if (riid == IID_IEnumPins || riid == IID_IUnknown) {
        return GetInterface((IEnumPins *) this, ppv);
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
CEnumPins::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
CEnumPins::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}    

STDMETHODIMP
CEnumPins::Clone(IEnumPins **ppEnum)
{
    CheckPointer(ppEnum,E_POINTER);
    ValidateReadWritePtr(ppEnum,sizeof(IEnumPins *));
    HRESULT hr = NOERROR;

    
    if (AreWeOutOfSync() == TRUE) {
        *ppEnum = NULL;
        hr =  VFW_E_ENUM_OUT_OF_SYNC;
    } else {

        *ppEnum = new CEnumPins(m_pFilter,
                                this);
        if (*ppEnum == NULL) {
            hr = E_OUTOFMEMORY;
        }
    }
    return hr;
}      

STDMETHODIMP
CEnumPins::Next(ULONG cPins,        
        IPin **ppPins,      
        ULONG *pcFetched)   
{
    CheckPointer(ppPins,E_POINTER);
    ValidateReadWritePtr(ppPins,cPins * sizeof(IPin *));

    ASSERT(ppPins);

    if (pcFetched!=NULL) {
        ValidateWritePtr(pcFetched, sizeof(ULONG));
        *pcFetched = 0;           
    }
    
    else if (cPins>1) {   
        return E_INVALIDARG;
    }
    ULONG cFetched = 0;           

    
    if (AreWeOutOfSync() == TRUE) {
    
    
    
    Refresh();
    }

    

    int cRealPins = min(m_PinCount - m_Position, (int) cPins);
    if (cRealPins == 0) {
        return S_FALSE;
    }

    

    while (cRealPins && (m_PinCount - m_Position)) {

        

        CBasePin *pPin = m_pFilter->GetPin(m_Position++);
        if (pPin == NULL) {
            
            
            
            ASSERT( cFetched==0 );
            return VFW_E_ENUM_OUT_OF_SYNC;
        }

        
        if (0 == m_PinCache.Find(pPin))
        {
            

            *ppPins = pPin;
            pPin->AddRef();

            cFetched++;
            ppPins++;

            m_PinCache.AddTail(pPin);

            cRealPins--;

        }
    }

    if (pcFetched!=NULL) {
        *pcFetched = cFetched;
    }

    return (cPins==cFetched ? NOERROR : S_FALSE);
}      

STDMETHODIMP
CEnumPins::Skip(ULONG cPins)
{
    
    if (AreWeOutOfSync() == TRUE) {
        return VFW_E_ENUM_OUT_OF_SYNC;
    }

    
    
    

    ULONG PinsLeft = m_PinCount - m_Position;
    if (cPins > PinsLeft) {
        return S_FALSE;
    }
    m_Position += cPins;
    return NOERROR;
}        

STDMETHODIMP
CEnumPins::Reset()
{
    m_Version = m_pFilter->GetPinVersion();
    m_PinCount = m_pFilter->GetPinCount();

    m_Position = 0;

    
    m_PinCache.RemoveAll();

    return S_OK;
}        

STDMETHODIMP
CEnumPins::Refresh()
{
    m_Version = m_pFilter->GetPinVersion();
    m_PinCount = m_pFilter->GetPinCount();

    m_Position = 0;
    return S_OK;
}                

CEnumMediaTypes::CEnumMediaTypes(CBasePin *pPin,
                 CEnumMediaTypes *pEnumMediaTypes) :
    m_Position(0),
    m_pPin(pPin),
    m_cRef(1)
{

#ifdef DEBUG
    m_dwCookie = DbgRegisterObjectCreation("CEnumMediaTypes", 0);
#endif

    

    ASSERT(pPin != NULL);

    
    m_pPin->AddRef();

    

    if (pEnumMediaTypes == NULL) {
        m_Version = m_pPin->GetMediaTypeVersion();
        return;
    }

    m_Position = pEnumMediaTypes->m_Position;
    m_Version = pEnumMediaTypes->m_Version;
}      

CEnumMediaTypes::~CEnumMediaTypes()
{
#ifdef DEBUG
    DbgRegisterObjectDestruction(m_dwCookie);
#endif
    m_pPin->Release();
}      

STDMETHODIMP
CEnumMediaTypes::QueryInterface(REFIID riid,void **ppv)
{
    CheckPointer(ppv, E_POINTER);

    

    if (riid == IID_IEnumMediaTypes || riid == IID_IUnknown) {
        return GetInterface((IEnumMediaTypes *) this, ppv);
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
CEnumMediaTypes::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG)
CEnumMediaTypes::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0) {
        delete this;
    }
    return cRef;
}    

STDMETHODIMP
CEnumMediaTypes::Clone(IEnumMediaTypes **ppEnum)
{
    CheckPointer(ppEnum,E_POINTER);
    ValidateReadWritePtr(ppEnum,sizeof(IEnumMediaTypes *));
    HRESULT hr = NOERROR;

    
    if (AreWeOutOfSync() == TRUE) {
        *ppEnum = NULL;
        hr = VFW_E_ENUM_OUT_OF_SYNC;
    } else {

        *ppEnum = new CEnumMediaTypes(m_pPin,
                                      this);

        if (*ppEnum == NULL) {
            hr =  E_OUTOFMEMORY;
        }
    }
    return hr;
}      

STDMETHODIMP
CEnumMediaTypes::Next(ULONG cMediaTypes,          
              AM_MEDIA_TYPE **ppMediaTypes,   
              ULONG *pcFetched)           
{
    CheckPointer(ppMediaTypes,E_POINTER);
    ValidateReadWritePtr(ppMediaTypes,cMediaTypes * sizeof(AM_MEDIA_TYPE *));
    
    if (AreWeOutOfSync() == TRUE) {
        return VFW_E_ENUM_OUT_OF_SYNC;
    }

    if (pcFetched!=NULL) {
        ValidateWritePtr(pcFetched, sizeof(ULONG));
        *pcFetched = 0;           
    }
    
    else if (cMediaTypes>1) {     
        return E_INVALIDARG;
    }
    ULONG cFetched = 0;           

    

    while (cMediaTypes) {

        CMediaType cmt;

        HRESULT hr = m_pPin->GetMediaType(m_Position++, &cmt);
        if (S_OK != hr) {
            break;
        }

        

        

        *ppMediaTypes = (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
        if (*ppMediaTypes == NULL) {
            break;
        }

        
        **ppMediaTypes = (AM_MEDIA_TYPE)cmt;

        
        cmt.pbFormat = NULL;
        cmt.cbFormat = NULL;
        cmt.pUnk     = NULL;  

        ppMediaTypes++;
        cFetched++;
        cMediaTypes--;
    }

    if (pcFetched!=NULL) {
        *pcFetched = cFetched;
    }

    return ( cMediaTypes==0 ? NOERROR : S_FALSE );
}      

STDMETHODIMP
CEnumMediaTypes::Skip(ULONG cMediaTypes)
{
    
    
    if (cMediaTypes == 0) {
        return S_OK;
    }

    
    if (AreWeOutOfSync() == TRUE) {
        return VFW_E_ENUM_OUT_OF_SYNC;
    }

    m_Position += cMediaTypes;

    
    CMediaType cmt;
    return S_OK == m_pPin->GetMediaType(m_Position - 1, &cmt) ? S_OK : S_FALSE;
}        

STDMETHODIMP
CEnumMediaTypes::Reset()

{
    m_Position = 0;

    
    
    
    m_Version = m_pPin->GetMediaTypeVersion();
    return NOERROR;
}                        

CBasePin::CBasePin(TCHAR *pObjectName,
           CBaseFilter *pFilter,
           CCritSec *pLock,
           HRESULT *phr,
           LPCWSTR pName,
           PIN_DIRECTION dir) :
    CUnknown( pObjectName, NULL ),
    m_pFilter(pFilter),
    m_pLock(pLock),
    m_pName(NULL),
    m_Connected(NULL),
    m_dir(dir),
    m_bRunTimeError(FALSE),
    m_pQSink(NULL),
    m_TypeVersion(1),
    m_tStart(),
    m_tStop(MAX_TIME),
    m_bCanReconnectWhenActive(false),
    m_bTryMyTypesFirst(false),
    m_dRate(1.0)
{
    

    ASSERT(pFilter != NULL);
    ASSERT(pLock != NULL);

    if (pName) {
        DWORD nameLen = lstrlenW(pName)+1;
        m_pName = new WCHAR[nameLen];
        if (m_pName) {
            CopyMemory(m_pName, pName, nameLen*sizeof(WCHAR));
        }
    }

#ifdef DEBUG
    m_cRef = 0;
#endif
}

#ifdef UNICODE
CBasePin::CBasePin(CHAR *pObjectName,
           CBaseFilter *pFilter,
           CCritSec *pLock,
           HRESULT *phr,
           LPCWSTR pName,
           PIN_DIRECTION dir) :
    CUnknown( pObjectName, NULL ),
    m_pFilter(pFilter),
    m_pLock(pLock),
    m_pName(NULL),
    m_Connected(NULL),
    m_dir(dir),
    m_bRunTimeError(FALSE),
    m_pQSink(NULL),
    m_TypeVersion(1),
    m_tStart(),
    m_tStop(MAX_TIME),
    m_bCanReconnectWhenActive(false),
    m_bTryMyTypesFirst(false),
    m_dRate(1.0)
{
    

    ASSERT(pFilter != NULL);
    ASSERT(pLock != NULL);

    if (pName) {
        DWORD nameLen = lstrlenW(pName)+1;
        m_pName = new WCHAR[nameLen];
        if (m_pName) {
            CopyMemory(m_pName, pName, nameLen*sizeof(WCHAR));
        }
    }

#ifdef DEBUG
    m_cRef = 0;
#endif
}
#endif    

CBasePin::~CBasePin()
{

    
    
    
    ASSERT(m_Connected == FALSE);

    delete[] m_pName;

    
    ASSERT(m_cRef == 0);
}      

STDMETHODIMP
CBasePin::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    

    if (riid == IID_IPin) {
        return GetInterface((IPin *) this, ppv);
    } else if (riid == IID_IQualityControl) {
        return GetInterface((IQualityControl *) this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP_(ULONG)
CBasePin::NonDelegatingAddRef()
{
    ASSERT(InterlockedIncrement(&m_cRef) > 0);
    return m_pFilter->AddRef();
}      

STDMETHODIMP_(ULONG)
CBasePin::NonDelegatingRelease()
{
    ASSERT(InterlockedDecrement(&m_cRef) >= 0);
    return m_pFilter->Release();
}      

#ifdef DEBUG
void
CBasePin::DisplayPinInfo(IPin *pReceivePin)
{

    if (DbgCheckModuleLevel(LOG_TRACE, CONNECT_TRACE_LEVEL)) {
        PIN_INFO ConnectPinInfo;
        PIN_INFO ReceivePinInfo;

        if (FAILED(QueryPinInfo(&ConnectPinInfo))) {
            lstrcpyW(ConnectPinInfo.achName, L"Bad Pin");
        } else {
            QueryPinInfoReleaseFilter(ConnectPinInfo);
        }

        if (FAILED(pReceivePin->QueryPinInfo(&ReceivePinInfo))) {
            lstrcpyW(ReceivePinInfo.achName, L"Bad Pin");
        } else {
            QueryPinInfoReleaseFilter(ReceivePinInfo);
        }

        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Trying to connect Pins :")));
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("    <%ls>"), ConnectPinInfo.achName));
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("    <%ls>"), ReceivePinInfo.achName));
    }
}
#endif      

#ifdef DEBUG
void CBasePin::DisplayTypeInfo(IPin *pPin, const CMediaType *pmt)
{
    UNREFERENCED_PARAMETER(pPin);
    if (DbgCheckModuleLevel(LOG_TRACE, CONNECT_TRACE_LEVEL)) {
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Trying media type:")));
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("    major type:  %hs"),
               GuidNames[*pmt->Type()]));
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("    sub type  :  %hs"),
               GuidNames[*pmt->Subtype()]));
    }
}
#endif    

STDMETHODIMP
CBasePin::Connect(
    IPin * pReceivePin,
    const AM_MEDIA_TYPE *pmt   
)
{
    CheckPointer(pReceivePin,E_POINTER);
    ValidateReadPtr(pReceivePin,sizeof(IPin));
    CAutoLock cObjectLock(m_pLock);
    DisplayPinInfo(pReceivePin);

    

    if (m_Connected) {
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Already connected")));
        return VFW_E_ALREADY_CONNECTED;
    }

    
    if (!IsStopped() && !m_bCanReconnectWhenActive) {
        return VFW_E_NOT_STOPPED;
    }  

    
    
    
    
    

    const CMediaType * ptype = (CMediaType*)pmt;
    HRESULT hr = AgreeMediaType(pReceivePin, ptype);
    if (FAILED(hr)) {
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Failed to agree type")));

        
        
        EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  

        return hr;
    }

    DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Connection succeeded")));  

    return NOERROR;
}    

HRESULT
CBasePin::AttemptConnection(
    IPin* pReceivePin,      
    const CMediaType* pmt   
)
{
    
    
    HRESULT hr = CheckConnect(pReceivePin);
    if (FAILED(hr)) {
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("CheckConnect failed")));

        
        
        EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

        return hr;
    }

    DisplayTypeInfo(pReceivePin, pmt);

    

    hr = CheckMediaType(pmt);
    if (hr == NOERROR) {

        
        m_Connected = pReceivePin;
        m_Connected->AddRef();
        SetMediaType(pmt);

        

        hr = pReceivePin->ReceiveConnection((IPin *)this, pmt);
        if (SUCCEEDED(hr)) {
            

            hr = CompleteConnect(pReceivePin);
            if (SUCCEEDED(hr)) {
                return hr;
            } else {
                DbgLog((LOG_TRACE,
                        CONNECT_TRACE_LEVEL,
                        TEXT("Failed to complete connection")));
                pReceivePin->Disconnect();
            }
        }
    } else {
        

        
        
        
        if (SUCCEEDED(hr) ||
            (hr == E_FAIL) ||
            (hr == E_INVALIDARG)) {
            hr = VFW_E_TYPE_NOT_ACCEPTED;
        }
    }

    
    
    

    
    
    EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

    
    if (m_Connected) {
        m_Connected->Release();
        m_Connected = NULL;
    }

    return hr;
}    

HRESULT CBasePin::TryMediaTypes(
    IPin *pReceivePin,
    const CMediaType *pmt,
    IEnumMediaTypes *pEnum)
{
    

    HRESULT hr = pEnum->Reset();
    if (FAILED(hr)) {
        return hr;
    }

    CMediaType *pMediaType = NULL;
    ULONG ulMediaCount = 0;

    
    HRESULT hrFailure = S_OK;

    for (;;) {

        

        hr = pEnum->Next(1, (AM_MEDIA_TYPE**)&pMediaType,&ulMediaCount);
        if (hr != S_OK) {
            if (S_OK == hrFailure) {
                hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;
            }
            return hrFailure;
        }  

        ASSERT(ulMediaCount == 1);
        ASSERT(pMediaType);

        

        if ((pmt == NULL) ||
            pMediaType->MatchesPartial(pmt)) {

            hr = AttemptConnection(pReceivePin, pMediaType);

            
            if (FAILED(hr) &&
            SUCCEEDED(hrFailure) &&
            (hr != E_FAIL) &&
            (hr != E_INVALIDARG) &&
            (hr != VFW_E_TYPE_NOT_ACCEPTED)) {
                hrFailure = hr;
            }
        } else {
            hr = VFW_E_NO_ACCEPTABLE_TYPES;
        }

        DeleteMediaType(pMediaType);

        if (S_OK == hr) {
            return hr;
        }
    }
}      

HRESULT CBasePin::AgreeMediaType(
    IPin *pReceivePin,
    const CMediaType *pmt)
{
    ASSERT(pReceivePin);
    IEnumMediaTypes *pEnumMediaTypes = NULL;

    
    if ( (pmt != NULL) && (!pmt->IsPartiallySpecified())) {

        
        
        

        return AttemptConnection(pReceivePin, pmt);
    }  

    

    HRESULT hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;

    for (int i = 0; i < 2; i++) {
        HRESULT hr;
        if (i == (int)m_bTryMyTypesFirst) {
            hr = pReceivePin->EnumMediaTypes(&pEnumMediaTypes);
        } else {
            hr = EnumMediaTypes(&pEnumMediaTypes);
        }
        if (SUCCEEDED(hr)) {
            ASSERT(pEnumMediaTypes);
            hr = TryMediaTypes(pReceivePin,pmt,pEnumMediaTypes);
            pEnumMediaTypes->Release();
            if (SUCCEEDED(hr)) {
                return NOERROR;
            } else {
                
                if ((hr != E_FAIL) &&
                    (hr != E_INVALIDARG) &&
                    (hr != VFW_E_TYPE_NOT_ACCEPTED)) {
                    hrFailure = hr;
                }
            }
        }
    }

    return hrFailure;
}      

HRESULT
CBasePin::CompleteConnect(IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(pReceivePin);
    return NOERROR;
}      

HRESULT
CBasePin::SetMediaType(const CMediaType *pmt)
{
    m_mt = *pmt;
    return NOERROR;
}      

HRESULT
CBasePin::CheckConnect(IPin * pPin)
{
    

    PIN_DIRECTION pd;
    pPin->QueryDirection(&pd);

    ASSERT((pd == PINDIR_OUTPUT) || (pd == PINDIR_INPUT));
    ASSERT((m_dir == PINDIR_OUTPUT) || (m_dir == PINDIR_INPUT));

    
    if (pd == m_dir) {
        return VFW_E_INVALID_DIRECTION;
    }
    return NOERROR;
}      

HRESULT
CBasePin::BreakConnect()
{
    return NOERROR;
}      

STDMETHODIMP
CBasePin::ReceiveConnection(
    IPin * pConnector,      
    const AM_MEDIA_TYPE *pmt    
)
{
    CheckPointer(pConnector,E_POINTER);
    CheckPointer(pmt,E_POINTER);
    ValidateReadPtr(pConnector,sizeof(IPin));
    ValidateReadPtr(pmt,sizeof(AM_MEDIA_TYPE));
    CAutoLock cObjectLock(m_pLock);

    
    if (m_Connected) {
        return VFW_E_ALREADY_CONNECTED;
    }

    
    if (!IsStopped() && !m_bCanReconnectWhenActive) {
        return VFW_E_NOT_STOPPED;
    }

    HRESULT hr = CheckConnect(pConnector);
    if (FAILED(hr)) {
        
        
        EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  

        return hr;
    }

    

    CMediaType * pcmt = (CMediaType*) pmt;
    hr = CheckMediaType(pcmt);
    if (hr != NOERROR) {
        

        
        
        EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );

        
        
        
        if (SUCCEEDED(hr) ||
            (hr == E_FAIL) ||
            (hr == E_INVALIDARG)) {
            hr = VFW_E_TYPE_NOT_ACCEPTED;
        }  

        return hr;
    }

    

    m_Connected = pConnector;
    m_Connected->AddRef();
    SetMediaType(pcmt);

    hr = CompleteConnect(pConnector);
    if (FAILED(hr)) {
        DbgLog((LOG_TRACE, CONNECT_TRACE_LEVEL, TEXT("Failed to complete connection")));
        m_Connected->Release();
        m_Connected = NULL;

        
        
        EXECUTE_ASSERT( SUCCEEDED( BreakConnect() ) );  

        return hr;
    }  

    return NOERROR;
}      

STDMETHODIMP
CBasePin::Disconnect()
{
    CAutoLock cObjectLock(m_pLock);

    
    if (!IsStopped()) {
        return VFW_E_NOT_STOPPED;
    }

    return DisconnectInternal();
}

STDMETHODIMP
CBasePin::DisconnectInternal()
{
    ASSERT(CritCheckIn(m_pLock));

    if (m_Connected) {
        HRESULT hr = BreakConnect();
        if( FAILED( hr ) ) {  

            
            DbgBreak( "WARNING: BreakConnect() failed in CBasePin::Disconnect()." );
            return hr;
        }

        m_Connected->Release();
        m_Connected = NULL;  

        return S_OK;
    } else {
          

        return S_FALSE;
    }
}    

STDMETHODIMP
CBasePin::ConnectedTo(
    IPin **ppPin
)
{
    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(IPin *));
    
    
    
    

    IPin *pPin = m_Connected;
    *ppPin = pPin;
    if (pPin != NULL) {
        pPin->AddRef();
        return S_OK;
    } else {
        ASSERT(*ppPin == NULL);
        return VFW_E_NOT_CONNECTED;
    }
}  

STDMETHODIMP
CBasePin::ConnectionMediaType(
    AM_MEDIA_TYPE *pmt
)
{
    CheckPointer(pmt,E_POINTER);
    ValidateReadWritePtr(pmt,sizeof(AM_MEDIA_TYPE));
    CAutoLock cObjectLock(m_pLock);

    
    if (IsConnected()) {
        CopyMediaType( pmt, &m_mt );
        return S_OK;
    } else {
        ((CMediaType *)pmt)->InitMediaType();
        return VFW_E_NOT_CONNECTED;
    }
}    

STDMETHODIMP
CBasePin::QueryPinInfo(
    PIN_INFO * pInfo
)
{
    CheckPointer(pInfo,E_POINTER);
    ValidateReadWritePtr(pInfo,sizeof(PIN_INFO));

    pInfo->pFilter = m_pFilter;
    if (m_pFilter) {
        m_pFilter->AddRef();
    }

    if (m_pName) {
        lstrcpynW(pInfo->achName, m_pName, sizeof(pInfo->achName)/sizeof(WCHAR));
    } else {
        pInfo->achName[0] = L'\0';
    }

    pInfo->dir = m_dir;

    return NOERROR;
}

STDMETHODIMP
CBasePin::QueryDirection(
    PIN_DIRECTION * pPinDir
)
{
    CheckPointer(pPinDir,E_POINTER);
    ValidateReadWritePtr(pPinDir,sizeof(PIN_DIRECTION));

    *pPinDir = m_dir;
    return NOERROR;
}  

STDMETHODIMP
CBasePin::QueryId(
    LPWSTR * Id
)
{
    
    

    return AMGetWideString(Name(), Id);
}    

STDMETHODIMP
CBasePin::QueryAccept(
    const AM_MEDIA_TYPE *pmt
)
{
    CheckPointer(pmt,E_POINTER);
    ValidateReadPtr(pmt,sizeof(AM_MEDIA_TYPE));

    

    HRESULT hr = CheckMediaType((CMediaType*)pmt);
    if (FAILED(hr)) {
        return S_FALSE;
    }
    
    return hr;
}      

STDMETHODIMP
CBasePin::EnumMediaTypes(
    IEnumMediaTypes **ppEnum
)
{
    CheckPointer(ppEnum,E_POINTER);
    ValidateReadWritePtr(ppEnum,sizeof(IEnumMediaTypes *));

    

    *ppEnum = new CEnumMediaTypes(this,
                              NULL);

    if (*ppEnum == NULL) {
        return E_OUTOFMEMORY;
    }

    return NOERROR;
}        

HRESULT CBasePin::GetMediaType(int iPosition, CMediaType *pMediaType)
{
    UNREFERENCED_PARAMETER(iPosition);
    UNREFERENCED_PARAMETER(pMediaType);
    return E_UNEXPECTED;
}      

LONG CBasePin::GetMediaTypeVersion()
{
    return m_TypeVersion;
}      

void CBasePin::IncrementTypeVersion()
{
    InterlockedIncrement(&m_TypeVersion);
}      

HRESULT
CBasePin::Active(void)
{
    return NOERROR;
}    

HRESULT
CBasePin::Run(REFERENCE_TIME tStart)
{
    UNREFERENCED_PARAMETER(tStart);
    return NOERROR;
}      

HRESULT
CBasePin::Inactive(void)
{
    m_bRunTimeError = FALSE;
    return NOERROR;
}    

STDMETHODIMP
CBasePin::EndOfStream(void)
{
    return S_OK;
}  

STDMETHODIMP
CBasePin::SetSink(IQualityControl * piqc)
{
    CAutoLock cObjectLock(m_pLock);
    if (piqc) ValidateReadPtr(piqc,sizeof(IQualityControl));
    m_pQSink = piqc;
    return NOERROR;
}   

STDMETHODIMP
CBasePin::Notify(IBaseFilter * pSender, Quality q)
{
    UNREFERENCED_PARAMETER(q);
    UNREFERENCED_PARAMETER(pSender);
    DbgBreak("IQualityControl::Notify not over-ridden from CBasePin.  (IGNORE is OK)");
    return E_NOTIMPL;
}           

STDMETHODIMP
CBasePin::NewSegment(
                REFERENCE_TIME tStart,
                REFERENCE_TIME tStop,
                double dRate)
{
    m_tStart = tStart;
    m_tStop = tStop;
    m_dRate = dRate;

    return S_OK;
}                

CBaseOutputPin::CBaseOutputPin(TCHAR *pObjectName,
                   CBaseFilter *pFilter,
                   CCritSec *pLock,
                   HRESULT *phr,
                   LPCWSTR pName) :
    CBasePin(pObjectName, pFilter, pLock, phr, pName, PINDIR_OUTPUT),
    m_pAllocator(NULL),
    m_pInputPin(NULL)
{
    ASSERT(pFilter);
}

#ifdef UNICODE
CBaseOutputPin::CBaseOutputPin(CHAR *pObjectName,
                   CBaseFilter *pFilter,
                   CCritSec *pLock,
                   HRESULT *phr,
                   LPCWSTR pName) :
    CBasePin(pObjectName, pFilter, pLock, phr, pName, PINDIR_OUTPUT),
    m_pAllocator(NULL),
    m_pInputPin(NULL)
{
    ASSERT(pFilter);
}
#endif  

HRESULT
CBaseOutputPin::CompleteConnect(IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(pReceivePin);
    return DecideAllocator(m_pInputPin, &m_pAllocator);
}          

HRESULT
CBaseOutputPin::CheckConnect(IPin * pPin)
{
    HRESULT hr = CBasePin::CheckConnect(pPin);
    if (FAILED(hr)) {
    return hr;
    }

    
    hr = pPin->QueryInterface(IID_IMemInputPin, (void **) &m_pInputPin);
    if (FAILED(hr)) {
        return hr;
    }
    return NOERROR;
}      

HRESULT
CBaseOutputPin::BreakConnect()
{
    

    if (m_pAllocator) {
        
        
        
        HRESULT hr = m_pAllocator->Decommit();
        if( FAILED( hr ) ) {
            return hr;
        }

        m_pAllocator->Release();
        m_pAllocator = NULL;
    }

    

    if (m_pInputPin) {
        m_pInputPin->Release();
        m_pInputPin = NULL;
    }
    return NOERROR;
}      

HRESULT
CBaseOutputPin::InitAllocator(IMemAllocator **ppAlloc)
{
    return CreateMemoryAllocator(ppAlloc);
}                

HRESULT
CBaseOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    HRESULT hr = NOERROR;
    *ppAlloc = NULL;

    
    
    
    
    ALLOCATOR_PROPERTIES prop;
    ZeroMemory(&prop, sizeof(prop));

    
    
    pPin->GetAllocatorRequirements(&prop);

    
    if (prop.cbAlign == 0) {
        prop.cbAlign = 1;
    }

    

    hr = pPin->GetAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {

        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                return NOERROR;
            }
        }
    }

    

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }

    

    hr = InitAllocator(ppAlloc);
    if (SUCCEEDED(hr)) {

        
        
        
        hr = DecideBufferSize(*ppAlloc, &prop);
        if (SUCCEEDED(hr)) {
            hr = pPin->NotifyAllocator(*ppAlloc, FALSE);
            if (SUCCEEDED(hr)) {
                return NOERROR;
            }
        }
    }

    

    if (*ppAlloc) {
        (*ppAlloc)->Release();
        *ppAlloc = NULL;
    }
    return hr;
}      

HRESULT
CBaseOutputPin::GetDeliveryBuffer(IMediaSample ** ppSample,
                                  REFERENCE_TIME * pStartTime,
                                  REFERENCE_TIME * pEndTime,
                                  DWORD dwFlags)
{
    if (m_pAllocator != NULL) {
        return m_pAllocator->GetBuffer(ppSample,pStartTime,pEndTime,dwFlags);
    } else {
        return E_NOINTERFACE;
    }
}                      

HRESULT
CBaseOutputPin::Deliver(IMediaSample * pSample)
{
    if (m_pInputPin == NULL) {
        return VFW_E_NOT_CONNECTED;
    }  

    return m_pInputPin->Receive(pSample);
}      

HRESULT
CBaseOutputPin::DeliverEndOfStream(void)
{
    
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->EndOfStream();
}      

HRESULT
CBaseOutputPin::Active(void)
{
    if (m_pAllocator == NULL) {
        return VFW_E_NO_ALLOCATOR;
    }
    return m_pAllocator->Commit();
}      

HRESULT
CBaseOutputPin::Inactive(void)
{
    m_bRunTimeError = FALSE;
    if (m_pAllocator == NULL) {
        return VFW_E_NO_ALLOCATOR;
    }
    return m_pAllocator->Decommit();
}    

STDMETHODIMP
CBaseOutputPin::EndOfStream(void)
{
    return E_UNEXPECTED;
}    

STDMETHODIMP
CBaseOutputPin::BeginFlush(void)
{
    return E_UNEXPECTED;
}  

STDMETHODIMP
CBaseOutputPin::EndFlush(void)
{
    return E_UNEXPECTED;
}  

HRESULT
CBaseOutputPin::DeliverBeginFlush(void)
{
    
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->BeginFlush();
}  

HRESULT
CBaseOutputPin::DeliverEndFlush(void)
{
    
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->EndFlush();
}

HRESULT
CBaseOutputPin::DeliverNewSegment(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop,
    double dRate)
{
    if (m_Connected == NULL) {
        return VFW_E_NOT_CONNECTED;
    }
    return m_Connected->NewSegment(tStart, tStop, dRate);
}                    

CBaseInputPin::CBaseInputPin(TCHAR *pObjectName,
                 CBaseFilter *pFilter,
                 CCritSec *pLock,
                 HRESULT *phr,
                 LPCWSTR pPinName) :
    CBasePin(pObjectName, pFilter, pLock, phr, pPinName, PINDIR_INPUT),
    m_pAllocator(NULL),
    m_bReadOnly(FALSE),
    m_bFlushing(FALSE)
{
    ZeroMemory(&m_SampleProps, sizeof(m_SampleProps));
}

#ifdef UNICODE
CBaseInputPin::CBaseInputPin(CHAR *pObjectName,
                 CBaseFilter *pFilter,
                 CCritSec *pLock,
                 HRESULT *phr,
                 LPCWSTR pPinName) :
    CBasePin(pObjectName, pFilter, pLock, phr, pPinName, PINDIR_INPUT),
    m_pAllocator(NULL),
    m_bReadOnly(FALSE),
    m_bFlushing(FALSE)
{
    ZeroMemory(&m_SampleProps, sizeof(m_SampleProps));
}
#endif    

CBaseInputPin::~CBaseInputPin()
{
    if (m_pAllocator != NULL) {
    m_pAllocator->Release();
    m_pAllocator = NULL;
    }
}    

STDMETHODIMP
CBaseInputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    

    if (riid == IID_IMemInputPin) {
        return GetInterface((IMemInputPin *) this, ppv);
    } else {
        return CBasePin::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP
CBaseInputPin::GetAllocator(
    IMemAllocator **ppAllocator)
{
    CheckPointer(ppAllocator,E_POINTER);
    ValidateReadWritePtr(ppAllocator,sizeof(IMemAllocator *));
    CAutoLock cObjectLock(m_pLock);

    if (m_pAllocator == NULL) {
        HRESULT hr = CreateMemoryAllocator(&m_pAllocator);
        if (FAILED(hr)) {
            return hr;
        }
    }
    ASSERT(m_pAllocator != NULL);
    *ppAllocator = m_pAllocator;
    m_pAllocator->AddRef();
    return NOERROR;
}      

STDMETHODIMP
CBaseInputPin::NotifyAllocator(
    IMemAllocator * pAllocator,
    BOOL bReadOnly)
{
    CheckPointer(pAllocator,E_POINTER);
    ValidateReadPtr(pAllocator,sizeof(IMemAllocator));
    CAutoLock cObjectLock(m_pLock);

    IMemAllocator *pOldAllocator = m_pAllocator;
    pAllocator->AddRef();
    m_pAllocator = pAllocator;

    if (pOldAllocator != NULL) {
        pOldAllocator->Release();
    }

    
    
    
    m_bReadOnly = (BYTE)bReadOnly;
    return NOERROR;
}  

HRESULT
CBaseInputPin::BreakConnect()
{
    
    if (m_pAllocator) {
        
        
        
        HRESULT hr = m_pAllocator->Decommit();
        if( FAILED( hr ) ) {
            return hr;
        }

        m_pAllocator->Release();
        m_pAllocator = NULL;
    }

    return S_OK;
}      

STDMETHODIMP
CBaseInputPin::Receive(IMediaSample *pSample)
{
    CheckPointer(pSample,E_POINTER);
    ValidateReadPtr(pSample,sizeof(IMediaSample));
    ASSERT(pSample);

    HRESULT hr = CheckStreaming();
    if (S_OK != hr) {
        return hr;
    }    

    
    IMediaSample2 *pSample2;
    if (SUCCEEDED(pSample->QueryInterface(IID_IMediaSample2, (void **)&pSample2))) {
        hr = pSample2->GetProperties(sizeof(m_SampleProps), (PBYTE)&m_SampleProps);
        pSample2->Release();
        if (FAILED(hr)) {
            return hr;
        }
    } else {
        
        m_SampleProps.cbData = sizeof(m_SampleProps);
        m_SampleProps.dwTypeSpecificFlags = 0;
        m_SampleProps.dwStreamId = AM_STREAM_MEDIA;
        m_SampleProps.dwSampleFlags = 0;
        if (S_OK == pSample->IsDiscontinuity()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_DATADISCONTINUITY;
        }
        if (S_OK == pSample->IsPreroll()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_PREROLL;
        }
        if (S_OK == pSample->IsSyncPoint()) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_SPLICEPOINT;
        }
        if (SUCCEEDED(pSample->GetTime(&m_SampleProps.tStart,
                                       &m_SampleProps.tStop))) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_TIMEVALID |
                                           AM_SAMPLE_STOPVALID;
        }
        if (S_OK == pSample->GetMediaType(&m_SampleProps.pMediaType)) {
            m_SampleProps.dwSampleFlags |= AM_SAMPLE_TYPECHANGED;
        }
        pSample->GetPointer(&m_SampleProps.pbBuffer);
        m_SampleProps.lActual = pSample->GetActualDataLength();
        m_SampleProps.cbBuffer = pSample->GetSize();
    }

    

    if (!(m_SampleProps.dwSampleFlags & AM_SAMPLE_TYPECHANGED)) {
        return NOERROR;
    }

    
    

    hr = CheckMediaType((CMediaType *)m_SampleProps.pMediaType);

    if (hr == NOERROR) {
        return NOERROR;
    }

    

    m_bRunTimeError = TRUE;
    EndOfStream();
    m_pFilter->NotifyEvent(EC_ERRORABORT,VFW_E_TYPE_NOT_ACCEPTED,0);
    return VFW_E_INVALIDMEDIATYPE;
}    

STDMETHODIMP
CBaseInputPin::ReceiveMultiple (
    IMediaSample **pSamples,
    long nSamples,
    long *nSamplesProcessed)
{
    CheckPointer(pSamples,E_POINTER);
    ValidateReadPtr(pSamples,nSamples * sizeof(IMediaSample *));

    HRESULT hr = S_OK;
    *nSamplesProcessed = 0;
    while (nSamples-- > 0) {
         hr = Receive(pSamples[*nSamplesProcessed]);

         
         if (hr != S_OK) {
             break;
         }
         (*nSamplesProcessed)++;
    }
    return hr;
}  

STDMETHODIMP
CBaseInputPin::ReceiveCanBlock()
{
    
    int cPins = m_pFilter->GetPinCount();
    int cOutputPins = 0;
    for (int c = 0; c < cPins; c++) {
        CBasePin *pPin = m_pFilter->GetPin(c);
        PIN_DIRECTION pd;
        HRESULT hr = pPin->QueryDirection(&pd);
        if (FAILED(hr)) {
            return hr;
        }

        if (pd == PINDIR_OUTPUT) {

            IPin *pConnected;
            hr = pPin->ConnectedTo(&pConnected);
            if (SUCCEEDED(hr)) {
                ASSERT(pConnected != NULL);
                cOutputPins++;
                IMemInputPin *pInputPin;
                hr = pConnected->QueryInterface(
                                              IID_IMemInputPin,
                                              (void **)&pInputPin);
                pConnected->Release();
                if (SUCCEEDED(hr)) {
                    hr = pInputPin->ReceiveCanBlock();
                    pInputPin->Release();
                    if (hr != S_FALSE) {
                        return S_OK;
                    }
                } else {
                    
                    return S_OK;
                }
            }
        }
    }
    return cOutputPins == 0 ? S_OK : S_FALSE;
}        

STDMETHODIMP
CBaseInputPin::BeginFlush(void)
{
    
    
    CAutoLock lck(m_pLock);

    
    
    ASSERT(!m_bFlushing);

    
    m_bFlushing = TRUE;

    
    
    return S_OK;
}          

STDMETHODIMP
CBaseInputPin::EndFlush(void)
{
    
    
    CAutoLock lck(m_pLock);

    
    ASSERT(m_bFlushing);

    
    
    

    
    m_bFlushing = FALSE;

    
    m_bRunTimeError = FALSE;

    return S_OK;
}  

STDMETHODIMP
CBaseInputPin::Notify(IBaseFilter * pSender, Quality q)
{
    UNREFERENCED_PARAMETER(q);
    CheckPointer(pSender,E_POINTER);
    ValidateReadPtr(pSender,sizeof(IBaseFilter));
    DbgBreak("IQuality::Notify called on an input pin");
    return NOERROR;
}     

HRESULT
CBaseInputPin::Inactive(void)
{
    m_bRunTimeError = FALSE;
    if (m_pAllocator == NULL) {
        return VFW_E_NO_ALLOCATOR;
    }

    m_bFlushing = FALSE;

    return m_pAllocator->Decommit();
}      

STDMETHODIMP
CBaseInputPin::GetAllocatorRequirements(ALLOCATOR_PROPERTIES*pProps)
{
    UNREFERENCED_PARAMETER(pProps);
    return E_NOTIMPL;
}    

HRESULT
CBaseInputPin::CheckStreaming()
{
    
    ASSERT(IsConnected());

    
    if (IsStopped()) {
        return VFW_E_WRONG_STATE;
    }
    if (m_bFlushing) {
        return S_FALSE;
    }
    if (m_bRunTimeError) {
        return VFW_E_RUNTIME_ERROR;
    }
    return S_OK;
}        

HRESULT
CBaseInputPin::PassNotify(Quality& q)
{
    
    

    DbgLog((LOG_TRACE,3,TEXT("Passing Quality notification through transform")));
    if (m_pQSink!=NULL) {
        return m_pQSink->Notify(m_pFilter, q);
    } else {
        
        HRESULT hr;
        IQualityControl * pIQC;

        hr = VFW_E_NOT_FOUND;                   
        if (m_Connected) {
            m_Connected->QueryInterface(IID_IQualityControl, (void**)&pIQC);

            if (pIQC!=NULL) {
                hr = pIQC->Notify(m_pFilter, q);
                pIQC->Release();
            }
        }
        return hr;
    }

}                       

CMediaSample::CMediaSample(TCHAR *pName,
               CBaseAllocator *pAllocator,
               HRESULT *phr,
               LPBYTE pBuffer,
               LONG length) :
    m_pBuffer(pBuffer),             
    m_cbBuffer(length),             
    m_lActual(length),              
    m_pMediaType(NULL),             
    m_dwFlags(0),                   
    m_cRef(0),                      
    m_dwTypeSpecificFlags(0),       
    m_dwStreamId(AM_STREAM_MEDIA),  
    m_pAllocator(pAllocator)        
{

    

    ASSERT(pAllocator);
}

#ifdef UNICODE
CMediaSample::CMediaSample(CHAR *pName,
               CBaseAllocator *pAllocator,
               HRESULT *phr,
               LPBYTE pBuffer,
               LONG length) :
    m_pBuffer(pBuffer),             
    m_cbBuffer(length),             
    m_lActual(length),              
    m_pMediaType(NULL),             
    m_dwFlags(0),                   
    m_cRef(0),                      
    m_dwTypeSpecificFlags(0),       
    m_dwStreamId(AM_STREAM_MEDIA),  
    m_pAllocator(pAllocator)        
{

    

    ASSERT(pAllocator);
}
#endif    

CMediaSample::~CMediaSample()
{

    if (m_pMediaType) {
    DeleteMediaType(m_pMediaType);
    }
}    

STDMETHODIMP
CMediaSample::QueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IMediaSample ||
        riid == IID_IMediaSample2 ||
        riid == IID_IUnknown) {
        return GetInterface((IMediaSample *) this, ppv);
    } else {
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
CMediaSample::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}                                    

STDMETHODIMP_(ULONG)
CMediaSample::Release()
{
    
    LONG lRef;
    if (m_cRef == 1) {
        lRef = 0;
        m_cRef = 0;
    } else {
        lRef = InterlockedDecrement(&m_cRef);
    }
    ASSERT(lRef >= 0);

    DbgLog((LOG_MEMORY,3,TEXT("    Unknown %X ref-- = %d"),
        this, m_cRef));

    
    if (lRef == 0) {
        
        if (m_dwFlags & Sample_TypeChanged) {
            SetMediaType(NULL);
        }
        ASSERT(m_pMediaType == NULL);
        m_dwFlags = 0;
        m_dwTypeSpecificFlags = 0;
        m_dwStreamId = AM_STREAM_MEDIA;

        
        
        m_pAllocator->ReleaseBuffer(this);
    }
    return (ULONG)lRef;
}          

HRESULT
CMediaSample::SetPointer(BYTE * ptr, LONG cBytes)
{
    m_pBuffer = ptr;            
    m_cbBuffer = cBytes;        
    m_lActual = cBytes;         

    return S_OK;
}      

STDMETHODIMP
CMediaSample::GetPointer(BYTE ** ppBuffer)
{
    ValidateReadWritePtr(ppBuffer,sizeof(BYTE *));

    
    
    ASSERT(m_pBuffer);

    *ppBuffer = m_pBuffer;
    return NOERROR;
}    

STDMETHODIMP_(LONG)
CMediaSample::GetSize(void)
{
    return m_cbBuffer;
}    

STDMETHODIMP
CMediaSample::GetTime(
    REFERENCE_TIME * pTimeStart,     
    REFERENCE_TIME * pTimeEnd
)
{
    ValidateReadWritePtr(pTimeStart,sizeof(REFERENCE_TIME));
    ValidateReadWritePtr(pTimeEnd,sizeof(REFERENCE_TIME));

    if (!(m_dwFlags & Sample_StopValid)) {
        if (!(m_dwFlags & Sample_TimeValid)) {
            return VFW_E_SAMPLE_TIME_NOT_SET;
        } else {
            *pTimeStart = m_Start;

            
            *pTimeEnd = m_Start + 1;
            return VFW_S_NO_STOP_TIME;
        }
    }

    *pTimeStart = m_Start;
    *pTimeEnd = m_End;
    return NOERROR;
}      

STDMETHODIMP
CMediaSample::SetTime(
    REFERENCE_TIME * pTimeStart,
    REFERENCE_TIME * pTimeEnd
)
{
    if (pTimeStart == NULL) {
        ASSERT(pTimeEnd == NULL);
        m_dwFlags &= ~(Sample_TimeValid | Sample_StopValid);
    } else {
        if (pTimeEnd == NULL) {
            m_Start = *pTimeStart;
            m_dwFlags |= Sample_TimeValid;
            m_dwFlags &= ~Sample_StopValid;
        } else {
            ValidateReadPtr(pTimeStart,sizeof(REFERENCE_TIME));
            ValidateReadPtr(pTimeEnd,sizeof(REFERENCE_TIME));
            ASSERT(*pTimeEnd >= *pTimeStart);

            m_Start = *pTimeStart;
            m_End = *pTimeEnd;
            m_dwFlags |= Sample_TimeValid | Sample_StopValid;
        }
    }
    return NOERROR;
}    

STDMETHODIMP
CMediaSample::GetMediaTime(
    LONGLONG * pTimeStart,
    LONGLONG * pTimeEnd
)
{
    ValidateReadWritePtr(pTimeStart,sizeof(LONGLONG));
    ValidateReadWritePtr(pTimeEnd,sizeof(LONGLONG));

    if (!(m_dwFlags & Sample_MediaTimeValid)) {
        return VFW_E_MEDIA_TIME_NOT_SET;
    }

    *pTimeStart = m_MediaStart;
    *pTimeEnd = (m_MediaStart + m_MediaEnd);
    return NOERROR;
}    

STDMETHODIMP
CMediaSample::SetMediaTime(
    LONGLONG * pTimeStart,
    LONGLONG * pTimeEnd
)
{
    if (pTimeStart == NULL) {
        ASSERT(pTimeEnd == NULL);
        m_dwFlags &= ~Sample_MediaTimeValid;
    } else {
        ValidateReadPtr(pTimeStart,sizeof(LONGLONG));
        ValidateReadPtr(pTimeEnd,sizeof(LONGLONG));
    ASSERT(*pTimeEnd >= *pTimeStart);

        m_MediaStart = *pTimeStart;
        m_MediaEnd = (LONG)(*pTimeEnd - *pTimeStart);
        m_dwFlags |= Sample_MediaTimeValid;
    }
    return NOERROR;
}  

STDMETHODIMP
CMediaSample::IsSyncPoint(void)
{
    if (m_dwFlags & Sample_SyncPoint) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}  

STDMETHODIMP
CMediaSample::SetSyncPoint(BOOL bIsSyncPoint)
{
    if (bIsSyncPoint) {
        m_dwFlags |= Sample_SyncPoint;
    } else {
        m_dwFlags &= ~Sample_SyncPoint;
    }
    return NOERROR;
}      

STDMETHODIMP
CMediaSample::IsDiscontinuity(void)
{
    if (m_dwFlags & Sample_Discontinuity) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}    

STDMETHODIMP
CMediaSample::SetDiscontinuity(BOOL bDiscont)
{
    
    if (bDiscont) {
        m_dwFlags |= Sample_Discontinuity;
    } else {
        m_dwFlags &= ~Sample_Discontinuity;
    }
    return S_OK;
}

STDMETHODIMP
CMediaSample::IsPreroll(void)
{
    if (m_dwFlags & Sample_Preroll) {
        return S_OK;
    } else {
        return S_FALSE;
    }
}  

STDMETHODIMP
CMediaSample::SetPreroll(BOOL bIsPreroll)
{
    if (bIsPreroll) {
        m_dwFlags |= Sample_Preroll;
    } else {
        m_dwFlags &= ~Sample_Preroll;
    }
    return NOERROR;
}

STDMETHODIMP_(LONG)
CMediaSample::GetActualDataLength(void)
{
    return m_lActual;
}  

STDMETHODIMP
CMediaSample::SetActualDataLength(LONG lActual)
{
    if (lActual > m_cbBuffer) {
        ASSERT(lActual <= GetSize());
        return VFW_E_BUFFER_OVERFLOW;
    }
    m_lActual = lActual;
    return NOERROR;
}      

STDMETHODIMP
CMediaSample::GetMediaType(AM_MEDIA_TYPE **ppMediaType)
{
    ValidateReadWritePtr(ppMediaType,sizeof(AM_MEDIA_TYPE *));
    ASSERT(ppMediaType);

    

    if (!(m_dwFlags & Sample_TypeChanged)) {
        ASSERT(m_pMediaType == NULL);
        *ppMediaType = NULL;
        return S_FALSE;
    }

    ASSERT(m_pMediaType);

    

    *ppMediaType = CreateMediaType(m_pMediaType);
    if (*ppMediaType == NULL) {
        return E_OUTOFMEMORY;
    }
    return NOERROR;
}      

STDMETHODIMP
CMediaSample::SetMediaType(AM_MEDIA_TYPE *pMediaType)
{
    

    if (m_pMediaType) {
        DeleteMediaType(m_pMediaType);
        m_pMediaType = NULL;
    }

    

    if (pMediaType == NULL) {
        m_dwFlags &= ~Sample_TypeChanged;
        return NOERROR;
    }

    ASSERT(pMediaType);
    ValidateReadPtr(pMediaType,sizeof(AM_MEDIA_TYPE));

    

    m_pMediaType = CreateMediaType(pMediaType);
    if (m_pMediaType == NULL) {
        m_dwFlags &= ~Sample_TypeChanged;
        return E_OUTOFMEMORY;
    }

    m_dwFlags |= Sample_TypeChanged;
    return NOERROR;
}  

STDMETHODIMP CMediaSample::GetProperties(
    DWORD cbProperties,
    BYTE * pbProperties
)
{
    if (0 != cbProperties) {
        CheckPointer(pbProperties, E_POINTER);
        
        AM_SAMPLE2_PROPERTIES Props;
        Props.cbData     = min(cbProperties, sizeof(Props));
        Props.dwSampleFlags = m_dwFlags & ~Sample_MediaTimeValid;
        Props.dwTypeSpecificFlags = m_dwTypeSpecificFlags;
        Props.pbBuffer   = m_pBuffer;
        Props.cbBuffer   = m_cbBuffer;
        Props.lActual    = m_lActual;
        Props.tStart     = m_Start;
        Props.tStop      = m_End;
        Props.dwStreamId = m_dwStreamId;
        if (m_dwFlags & AM_SAMPLE_TYPECHANGED) {
            Props.pMediaType = m_pMediaType;
        } else {
            Props.pMediaType = NULL;
        }
        CopyMemory(pbProperties, &Props, Props.cbData);
    }
    return S_OK;
}

#define CONTAINS_FIELD(type, field, offset) \
    ((FIELD_OFFSET(type, field) + sizeof(((type *)0)->field)) <= offset)

HRESULT CMediaSample::SetProperties(
    DWORD cbProperties,
    const BYTE * pbProperties
)
{

    
    AM_MEDIA_TYPE *pMediaType = NULL;

    if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, cbData, cbProperties)) {
        CheckPointer(pbProperties, E_POINTER);
        AM_SAMPLE2_PROPERTIES *pProps =
            (AM_SAMPLE2_PROPERTIES *)pbProperties;

        
        if (pProps->cbData < cbProperties) {
            cbProperties = pProps->cbData;
        }
        
        if (cbProperties > sizeof(*pProps) ||
            pProps->cbData > sizeof(*pProps)) {
            return E_INVALIDARG;
        }
        
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, dwSampleFlags, cbProperties)) {
            
            if (pProps->dwSampleFlags &
                    (~Sample_ValidFlags | Sample_MediaTimeValid)) {
                return E_INVALIDARG;
            }
            
            if ((pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID) &&
                 !(m_dwFlags & AM_SAMPLE_TIMEVALID) &&
                 !CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, tStop, cbProperties)) {
                 return E_INVALIDARG;
            }
        }
        
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, pbBuffer, cbProperties)) {

            
            if (pProps->pbBuffer != 0 && pProps->pbBuffer != m_pBuffer) {
                return E_INVALIDARG;
            }
        }
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, cbBuffer, cbProperties)) {

            
            if (pProps->cbBuffer != 0 && pProps->cbBuffer != m_cbBuffer) {
                return E_INVALIDARG;
            }
        }
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, cbBuffer, cbProperties) &&
            CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, lActual, cbProperties)) {

            
            if (pProps->cbBuffer < pProps->lActual) {
                return E_INVALIDARG;
            }
        }

        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, pMediaType, cbProperties)) {

            
            if (pProps->dwSampleFlags & AM_SAMPLE_TYPECHANGED) {
                CheckPointer(pProps->pMediaType, E_POINTER);
                pMediaType = CreateMediaType(pProps->pMediaType);
                if (pMediaType == NULL) {
                    return E_OUTOFMEMORY;
                }
            }
        }

        
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, dwStreamId, cbProperties)) {
            m_dwStreamId = pProps->dwStreamId;
        }
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, dwSampleFlags, cbProperties)) {
            
            m_dwFlags = pProps->dwSampleFlags |
                                (m_dwFlags & Sample_MediaTimeValid);
            m_dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
        } else {
            if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, dwTypeSpecificFlags, cbProperties)) {
                m_dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
            }
        }

        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, lActual, cbProperties)) {
            
            m_lActual = pProps->lActual;
        }

        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, tStop, cbProperties)) {

            
            m_End   = pProps->tStop;
        }
        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, tStart, cbProperties)) {

            
            m_Start = pProps->tStart;
        }

        if (CONTAINS_FIELD(AM_SAMPLE2_PROPERTIES, pMediaType, cbProperties)) {
            
            if (pProps->dwSampleFlags & AM_SAMPLE_TYPECHANGED) {
                if (m_pMediaType != NULL) {
                    DeleteMediaType(m_pMediaType);
                }
                m_pMediaType = pMediaType;
            }
        }

        
        if (m_pMediaType) {
            m_dwFlags |= Sample_TypeChanged;
        } else {
            m_dwFlags &= ~Sample_TypeChanged;
        }
    }

    return S_OK;
}                                            

CDynamicOutputPin::CDynamicOutputPin(
    TCHAR *pObjectName,
    CBaseFilter *pFilter,
    CCritSec *pLock,
    HRESULT *phr,
    LPCWSTR pName) :
        CBaseOutputPin(pObjectName, pFilter, pLock, phr, pName),
        m_hStopEvent(NULL),
        m_pGraphConfig(NULL),
        m_bPinUsesReadOnlyAllocator(FALSE),
        m_BlockState(NOT_BLOCKED),
        m_hUnblockOutputPinEvent(NULL),
        m_hNotifyCallerPinBlockedEvent(NULL),
        m_dwBlockCallerThreadID(0),
        m_dwNumOutstandingOutputPinUsers(0)
{
    HRESULT hr = Initialize();
    if( FAILED( hr ) ) {
        *phr = hr;
        return;
    }
}

#ifdef UNICODE
CDynamicOutputPin::CDynamicOutputPin(
    CHAR *pObjectName,
    CBaseFilter *pFilter,
    CCritSec *pLock,
    HRESULT *phr,
    LPCWSTR pName) :
        CBaseOutputPin(pObjectName, pFilter, pLock, phr, pName),
        m_hStopEvent(NULL),
        m_pGraphConfig(NULL),
        m_bPinUsesReadOnlyAllocator(FALSE),
        m_BlockState(NOT_BLOCKED),
        m_hUnblockOutputPinEvent(NULL),
        m_hNotifyCallerPinBlockedEvent(NULL),
        m_dwBlockCallerThreadID(0),
        m_dwNumOutstandingOutputPinUsers(0)
{
    HRESULT hr = Initialize();
    if( FAILED( hr ) ) {
        *phr = hr;
        return;
    }
}
#endif

CDynamicOutputPin::~CDynamicOutputPin()
{
    if(NULL != m_hUnblockOutputPinEvent) {
        
        
        EXECUTE_ASSERT(::CloseHandle(m_hUnblockOutputPinEvent));
    }

    if(NULL != m_hNotifyCallerPinBlockedEvent) {
        
        
        EXECUTE_ASSERT(::CloseHandle(m_hNotifyCallerPinBlockedEvent));
    }
}

HRESULT CDynamicOutputPin::Initialize(void)
{
    m_hUnblockOutputPinEvent = ::CreateEvent( NULL,   
                                              TRUE,   
                                              TRUE,   
                                              NULL ); 

    
    if(NULL == m_hUnblockOutputPinEvent) {
        return AmGetLastErrorToHResult();
    }

    
    SetReconnectWhenActive(true);

    return S_OK;
}

STDMETHODIMP CDynamicOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if(riid == IID_IPinFlowControl) {
        return GetInterface(static_cast<IPinFlowControl*>(this), ppv);
    } else {
        return CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
    }
}

STDMETHODIMP CDynamicOutputPin::Disconnect(void)
{
    CAutoLock cObjectLock(m_pLock);
    return DisconnectInternal();
}

STDMETHODIMP CDynamicOutputPin::Block(DWORD dwBlockFlags, HANDLE hEvent)
{
    const DWORD VALID_FLAGS = AM_PIN_FLOW_CONTROL_BLOCK;

    
    if(dwBlockFlags & ~VALID_FLAGS) {
        return E_INVALIDARG;
    }

    
    if((dwBlockFlags & AM_PIN_FLOW_CONTROL_BLOCK) && (NULL != hEvent)) {
        if( !::ResetEvent( hEvent ) ) {
            return AmGetLastErrorToHResult();
        }
    }

    
    if(0 == dwBlockFlags) {

        
        
        if(NULL != hEvent) {
            return E_INVALIDARG;
        }
    }

    #ifdef DEBUG
    AssertValid();
    #endif 

    HRESULT hr;

    if(dwBlockFlags & AM_PIN_FLOW_CONTROL_BLOCK) {
        
        
        if(NULL == hEvent) {
            hr = SynchronousBlockOutputPin();
        } else {
            hr = AsynchronousBlockOutputPin(hEvent);
        }
    } else {
        hr = UnblockOutputPin();
    }

    #ifdef DEBUG
    AssertValid();
    #endif 

    if(FAILED(hr)) {
        return hr;
    }

    return S_OK;
}

HRESULT CDynamicOutputPin::SynchronousBlockOutputPin(void)
{
    HANDLE hNotifyCallerPinBlockedEvent = :: CreateEvent( NULL,   
                                                          FALSE,  
                                                          FALSE,  
                                                          NULL ); 

    
    if(NULL == hNotifyCallerPinBlockedEvent) {
        return AmGetLastErrorToHResult();
    }

    HRESULT hr = AsynchronousBlockOutputPin(hNotifyCallerPinBlockedEvent);
    if(FAILED(hr)) {
        
        
        EXECUTE_ASSERT(::CloseHandle(hNotifyCallerPinBlockedEvent));

        return hr;
    }

    hr = WaitEvent(hNotifyCallerPinBlockedEvent);

    
    
    EXECUTE_ASSERT(::CloseHandle(hNotifyCallerPinBlockedEvent));

    if(FAILED(hr)) {
        return hr;
    }

    return S_OK;
}

HRESULT CDynamicOutputPin::AsynchronousBlockOutputPin(HANDLE hNotifyCallerPinBlockedEvent)
{
    
    
    
    CAutoLock alBlockStateLock(&m_BlockStateLock);

    if(NOT_BLOCKED != m_BlockState) {
        if(m_dwBlockCallerThreadID == ::GetCurrentThreadId()) {
            return VFW_E_PIN_ALREADY_BLOCKED_ON_THIS_THREAD;
        } else {
            return VFW_E_PIN_ALREADY_BLOCKED;
        }
    }

    BOOL fSuccess = ::DuplicateHandle( ::GetCurrentProcess(),
                                       hNotifyCallerPinBlockedEvent,
                                       ::GetCurrentProcess(),
                                       &m_hNotifyCallerPinBlockedEvent,
                                       EVENT_MODIFY_STATE,
                                       FALSE,
                                       0 );
    if( !fSuccess ) {
        return AmGetLastErrorToHResult();
    }

    m_BlockState = PENDING;
    m_dwBlockCallerThreadID = ::GetCurrentThreadId();

    
    
    
    
    
    if(!StreamingThreadUsingOutputPin()) {

        
        BlockOutputPin();
    }

    return S_OK;
}

void CDynamicOutputPin::BlockOutputPin(void)
{
    
    
    ASSERT(CritCheckIn(&m_BlockStateLock));

    
    
    ASSERT(!StreamingThreadUsingOutputPin());

    
    
    EXECUTE_ASSERT(::ResetEvent(m_hUnblockOutputPinEvent));

    
    
    EXECUTE_ASSERT(::SetEvent(m_hNotifyCallerPinBlockedEvent));
    EXECUTE_ASSERT(::CloseHandle(m_hNotifyCallerPinBlockedEvent));

    m_BlockState = BLOCKED;
    m_hNotifyCallerPinBlockedEvent = NULL;
}

HRESULT CDynamicOutputPin::UnblockOutputPin(void)
{
    
    
    
    CAutoLock alBlockStateLock(&m_BlockStateLock);

    if(NOT_BLOCKED == m_BlockState) {
        return S_FALSE;
    }

    
    
    EXECUTE_ASSERT(::SetEvent(m_hUnblockOutputPinEvent));

    
    if(NULL != m_hNotifyCallerPinBlockedEvent) {
        
        
        EXECUTE_ASSERT(::SetEvent(m_hNotifyCallerPinBlockedEvent));
        EXECUTE_ASSERT(::CloseHandle(m_hNotifyCallerPinBlockedEvent));
    }

    m_BlockState = NOT_BLOCKED;
    m_dwBlockCallerThreadID = 0;
    m_hNotifyCallerPinBlockedEvent = NULL;

    return S_OK;
}

HRESULT CDynamicOutputPin::StartUsingOutputPin(void)
{
    
    
    ASSERT(CritCheckOut(&m_BlockStateLock));

    CAutoLock alBlockStateLock(&m_BlockStateLock);

    #ifdef DEBUG
    AssertValid();
    #endif 

    
    while(BLOCKED == m_BlockState) {
        m_BlockStateLock.Unlock();

        
        
        ASSERT(CritCheckOut( &m_BlockStateLock ));

        
        
        
        
        const DWORD UNBLOCK = WAIT_OBJECT_0;
        const DWORD STOP = WAIT_OBJECT_0 + 1;

        HANDLE ahWaitEvents[] = { m_hUnblockOutputPinEvent, m_hStopEvent };
        DWORD dwNumWaitEvents = sizeof(ahWaitEvents)/sizeof(HANDLE);

        DWORD dwReturnValue = ::WaitForMultipleObjects( dwNumWaitEvents, ahWaitEvents, FALSE, INFINITE );

        m_BlockStateLock.Lock();

        #ifdef DEBUG
        AssertValid();
        #endif 

        switch( dwReturnValue ) {
        case UNBLOCK:
            break;

        case STOP:
            return VFW_E_STATE_CHANGED;

        case WAIT_FAILED:
            return AmGetLastErrorToHResult();

        default:
            DbgBreak( "An Unexpected case occured in CDynamicOutputPin::StartUsingOutputPin()." );
            return E_UNEXPECTED;
        }
    }

    m_dwNumOutstandingOutputPinUsers++;

    #ifdef DEBUG
    AssertValid();
    #endif 

    return S_OK;
}

void CDynamicOutputPin::StopUsingOutputPin(void)
{
    CAutoLock alBlockStateLock(&m_BlockStateLock);

    #ifdef DEBUG
    AssertValid();
    #endif 

    m_dwNumOutstandingOutputPinUsers--;

    if((m_dwNumOutstandingOutputPinUsers == 0) && (NOT_BLOCKED != m_BlockState)) {
        BlockOutputPin();
    }

    #ifdef DEBUG
    AssertValid();
    #endif 
}

bool CDynamicOutputPin::StreamingThreadUsingOutputPin(void)
{
    CAutoLock alBlockStateLock(&m_BlockStateLock);

    return (m_dwNumOutstandingOutputPinUsers > 0);
}

void CDynamicOutputPin::SetConfigInfo(IGraphConfig *pGraphConfig, HANDLE hStopEvent)
{
    
    
    
    m_pGraphConfig = pGraphConfig;

    m_hStopEvent = hStopEvent;
}

HRESULT CDynamicOutputPin::Active(void)
{
    
    if((NULL == m_hStopEvent) || (NULL == m_pGraphConfig)) {
        DbgBreak( ERROR: CDynamicOutputPin::Active() failed because m_pGraphConfig and m_hStopEvent were not initialized.  Call SetConfigInfo() to initialize them. );
        return E_FAIL;
    }

    
    
    
    
    EXECUTE_ASSERT(ResetEvent(m_hStopEvent));

    return CBaseOutputPin::Active();
}

HRESULT CDynamicOutputPin::Inactive(void)
{
    
    
    
    
    EXECUTE_ASSERT(SetEvent(m_hStopEvent));

    return CBaseOutputPin::Inactive();
}

HRESULT CDynamicOutputPin::DeliverBeginFlush(void)
{
    
    
    
    
    EXECUTE_ASSERT(SetEvent(m_hStopEvent));

    return CBaseOutputPin::DeliverBeginFlush();
}

HRESULT CDynamicOutputPin::DeliverEndFlush(void)
{
    
    
    
    
    EXECUTE_ASSERT(ResetEvent(m_hStopEvent));

    return CBaseOutputPin::DeliverEndFlush();
}      

HRESULT CDynamicOutputPin::ChangeOutputFormat
    (
    const AM_MEDIA_TYPE *pmt,
    REFERENCE_TIME tSegmentStart,
    REFERENCE_TIME tSegmentStop,
    double dSegmentRate
    )
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    
    ASSERT(NULL != pmt);

    CMediaType cmt(*pmt);
    HRESULT hr = ChangeMediaType(&cmt);
    if (FAILED(hr)) {
        return hr;
    }

    hr = DeliverNewSegment(tSegmentStart, tSegmentStop, dSegmentRate);
    if( FAILED( hr ) ) {
        return hr;
    }

    return S_OK;
}

HRESULT CDynamicOutputPin::ChangeMediaType(const CMediaType *pmt)
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    
    ASSERT(!IsStopped());

    if(!IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }

    
    QzCComPtr<IPinConnection> pConnection;

    m_Connected->QueryInterface(IID_IPinConnection, (void **)&pConnection);
    if(pConnection != NULL) {

        if(S_OK == pConnection->DynamicQueryAccept(pmt)) {

            HRESULT hr = ChangeMediaTypeHelper(pmt);
            if(FAILED(hr)) {
                return hr;
            }

            return S_OK;
        }
    }

    
    return DynamicReconnect(pmt);
}

HRESULT CDynamicOutputPin::ChangeMediaTypeHelper(const CMediaType *pmt)
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    HRESULT hr = m_Connected->ReceiveConnection(this, pmt);
    if(FAILED(hr)) {
        return hr;
    }

    hr = SetMediaType(pmt);
    if(FAILED(hr)) {
        return hr;
    }

    
    if(NULL != m_pInputPin) {
        
        
        ASSERT(::IsEqualObject(m_Connected, m_pInputPin));

        ALLOCATOR_PROPERTIES apInputPinRequirements;
        apInputPinRequirements.cbAlign = 0;
        apInputPinRequirements.cbBuffer = 0;
        apInputPinRequirements.cbPrefix = 0;
        apInputPinRequirements.cBuffers = 0;

        m_pInputPin->GetAllocatorRequirements(&apInputPinRequirements);

        
        if(0 == apInputPinRequirements.cbAlign) {
            apInputPinRequirements.cbAlign = 1;
        }

        hr = m_pAllocator->Decommit();
        if(FAILED(hr)) {
            return hr;
        }

        hr = DecideBufferSize(m_pAllocator,  &apInputPinRequirements);
        if(FAILED(hr)) {
            return hr;
        }

        hr = m_pAllocator->Commit();
        if(FAILED(hr)) {
            return hr;
        }

        hr = m_pInputPin->NotifyAllocator(m_pAllocator, m_bPinUsesReadOnlyAllocator);
        if(FAILED(hr)) {
            return hr;
        }
    }

    return S_OK;
}          

HRESULT CDynamicOutputPin::DynamicReconnect( const CMediaType* pmt )
{
    
    
    ASSERT(StreamingThreadUsingOutputPin());

    if((m_pGraphConfig == NULL) || (NULL == m_hStopEvent)) {
        return E_FAIL;
    }

    HRESULT hr = m_pGraphConfig->Reconnect(
        this,
        NULL,
        pmt,
        NULL,
        m_hStopEvent,
        AM_GRAPH_CONFIG_RECONNECT_CACHE_REMOVED_FILTERS );

    return hr;
}

HRESULT CDynamicOutputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = CBaseOutputPin::CompleteConnect(pReceivePin);
    if(SUCCEEDED(hr)) {
        if(!IsStopped() && m_pAllocator) {
            hr = m_pAllocator->Commit();
            ASSERT(hr != VFW_E_ALREADY_COMMITTED);
        }
    }

    return hr;
}

#ifdef DEBUG
void CDynamicOutputPin::AssertValid(void)
{
    

    
    
    ASSERT(NULL != m_hUnblockOutputPinEvent);

    
    
    ASSERT(NULL != m_hStopEvent);
    ASSERT(NULL != m_pGraphConfig);

    

    CAutoLock alBlockStateLock(&m_BlockStateLock);

    
    ASSERT((NOT_BLOCKED == m_BlockState) || (PENDING == m_BlockState) || (BLOCKED == m_BlockState));

    
    
    ASSERT(((NULL == m_hNotifyCallerPinBlockedEvent) && (PENDING != m_BlockState)) ||
           ((NULL != m_hNotifyCallerPinBlockedEvent) && (PENDING == m_BlockState)) );

    
    
    ASSERT((0 == m_dwBlockCallerThreadID) || (NOT_BLOCKED != m_BlockState));

    
    
    ASSERT(((0 != m_dwNumOutstandingOutputPinUsers) && (BLOCKED != m_BlockState)) ||
           ((0 == m_dwNumOutstandingOutputPinUsers) && (NOT_BLOCKED != m_BlockState)) ||
           ((0 == m_dwNumOutstandingOutputPinUsers) && (NOT_BLOCKED == m_BlockState)) );
}
#endif 

HRESULT CDynamicOutputPin::WaitEvent(HANDLE hEvent)
{
    const DWORD EVENT_SIGNALED = WAIT_OBJECT_0;

    DWORD dwReturnValue = ::WaitForSingleObject(hEvent, INFINITE);

    switch( dwReturnValue ) {
    case EVENT_SIGNALED:
        return S_OK;

    case WAIT_FAILED:
        return AmGetLastErrorToHResult();

    default:
        DbgBreak( "An Unexpected case occured in CDynamicOutputPin::WaitEvent()." );
        return E_UNEXPECTED;
    }
}                  

CBaseAllocator::CBaseAllocator(TCHAR *pName,
                               LPUNKNOWN pUnk,
                               HRESULT *phr,
                               BOOL bEvent,
                               BOOL fEnableReleaseCallback
                               ) :
    CUnknown(pName, pUnk),
    m_lAllocated(0),
    m_bChanged(FALSE),
    m_bCommitted(FALSE),
    m_bDecommitInProgress(FALSE),
    m_lSize(0),
    m_lCount(0),
    m_lAlignment(0),
    m_lPrefix(0),
    m_hSem(NULL),
    m_lWaiting(0),
    m_fEnableReleaseCallback(fEnableReleaseCallback),
    m_pNotify(NULL)
{

    if (bEvent) {
        m_hSem = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
        if (m_hSem == NULL) {
            *phr = E_OUTOFMEMORY;
            return;
        }
    }
}

#ifdef UNICODE
CBaseAllocator::CBaseAllocator(CHAR *pName,
                               LPUNKNOWN pUnk,
                               HRESULT *phr,
                               BOOL bEvent,
                               BOOL fEnableReleaseCallback) :
    CUnknown(pName, pUnk),
    m_lAllocated(0),
    m_bChanged(FALSE),
    m_bCommitted(FALSE),
    m_bDecommitInProgress(FALSE),
    m_lSize(0),
    m_lCount(0),
    m_lAlignment(0),
    m_lPrefix(0),
    m_hSem(NULL),
    m_lWaiting(0),
    m_fEnableReleaseCallback(fEnableReleaseCallback),
    m_pNotify(NULL)
{

    if (bEvent) {
        m_hSem = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
        if (m_hSem == NULL) {
            *phr = E_OUTOFMEMORY;
            return;
        }
    }
}
#endif    

CBaseAllocator::~CBaseAllocator()
{
    
    
    
    

    ASSERT(!m_bCommitted);
    if (m_hSem != NULL) {
        EXECUTE_ASSERT(CloseHandle(m_hSem));
    }
    if (m_pNotify) {
        m_pNotify->Release();
    }
}      

STDMETHODIMP
CBaseAllocator::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    

    if (riid == IID_IMemAllocator ||
        riid == IID_IMemAllocatorCallbackTemp && m_fEnableReleaseCallback) {
        return GetInterface((IMemAllocatorCallbackTemp *) this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}      

STDMETHODIMP
CBaseAllocator::SetProperties(
                ALLOCATOR_PROPERTIES* pRequest,
                ALLOCATOR_PROPERTIES* pActual)
{
    CheckPointer(pRequest, E_POINTER);
    CheckPointer(pActual, E_POINTER);
    ValidateReadWritePtr(pActual, sizeof(ALLOCATOR_PROPERTIES));
    CAutoLock cObjectLock(this);

    ZeroMemory(pActual, sizeof(ALLOCATOR_PROPERTIES));

    ASSERT(pRequest->cbBuffer > 0);

    
    if (pRequest->cbAlign != 1) {
        DbgLog((LOG_ERROR, 2, TEXT("Alignment requested was 0x%x, not 1"),
               pRequest->cbAlign));
        return VFW_E_BADALIGN;
    }

    

    if (m_bCommitted) {
        return VFW_E_ALREADY_COMMITTED;
    }

    

    if (m_lAllocated != m_lFree.GetCount()) {
        return VFW_E_BUFFERS_OUTSTANDING;
    }

    

    pActual->cbBuffer = m_lSize = pRequest->cbBuffer;
    pActual->cBuffers = m_lCount = pRequest->cBuffers;
    pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
    pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;

    m_bChanged = TRUE;
    return NOERROR;
}

STDMETHODIMP
CBaseAllocator::GetProperties(
    ALLOCATOR_PROPERTIES * pActual)
{
    CheckPointer(pActual,E_POINTER);
    ValidateReadWritePtr(pActual,sizeof(ALLOCATOR_PROPERTIES));

    CAutoLock cObjectLock(this);
    pActual->cbBuffer = m_lSize;
    pActual->cBuffers = m_lCount;
    pActual->cbAlign = m_lAlignment;
    pActual->cbPrefix = m_lPrefix;
    return NOERROR;
}          

HRESULT CBaseAllocator::GetBuffer(IMediaSample **ppBuffer,
                                  REFERENCE_TIME *pStartTime,
                                  REFERENCE_TIME *pEndTime,
                                  DWORD dwFlags
                                  )
{
    UNREFERENCED_PARAMETER(pStartTime);
    UNREFERENCED_PARAMETER(pEndTime);
    UNREFERENCED_PARAMETER(dwFlags);
    CMediaSample *pSample;

    *ppBuffer = NULL;
    for (;;)
    {
        {  
            CAutoLock cObjectLock(this);

            
            if (!m_bCommitted) {
                return VFW_E_NOT_COMMITTED;
            }
            pSample = (CMediaSample *) m_lFree.RemoveHead();
            if (pSample == NULL) {
                SetWaiting();
            }
        }

        

        if (pSample) {
            break;
        }
        if (dwFlags & AM_GBF_NOWAIT) {
            return VFW_E_TIMEOUT;
        }
        ASSERT(m_hSem != NULL);
        WaitForSingleObject(m_hSem, INFINITE);
    }

      

    ASSERT(pSample->m_cRef == 0);
    pSample->m_cRef = 1;
    *ppBuffer = pSample;  

    return NOERROR;
}      

STDMETHODIMP
CBaseAllocator::ReleaseBuffer(IMediaSample * pSample)
{
    CheckPointer(pSample,E_POINTER);
    ValidateReadPtr(pSample,sizeof(IMediaSample));    

	BOOL bRelease = FALSE;
    {
        CAutoLock cal(this);

        

        m_lFree.Add((CMediaSample *)pSample);
        if (m_lWaiting != 0) {
            NotifySample();
        }

        
        

        LONG l1 = m_lFree.GetCount();
        if (m_bDecommitInProgress && (l1 == m_lAllocated)) {
            Free();
            m_bDecommitInProgress = FALSE;
            bRelease = TRUE;
        }
    }

    if (m_pNotify) {

        ASSERT(m_fEnableReleaseCallback);

        
        
        
        
        m_pNotify->NotifyRelease();
    }

    

    if (bRelease) {
        Release();
    }
    return NOERROR;
}

STDMETHODIMP
CBaseAllocator::SetNotify(
    IMemAllocatorNotifyCallbackTemp* pNotify
    )
{
    ASSERT(m_fEnableReleaseCallback);
    CAutoLock lck(this);
    if (pNotify) {
        pNotify->AddRef();
    }
    if (m_pNotify) {
        m_pNotify->Release();
    }
    m_pNotify = pNotify;
    return S_OK;
}

STDMETHODIMP
CBaseAllocator::GetFreeCount(
    LONG* plBuffersFree
    )
{
    ASSERT(m_fEnableReleaseCallback);
    CAutoLock cObjectLock(this);
    *plBuffersFree = m_lCount - m_lAllocated + m_lFree.GetCount();
    return NOERROR;
}

void
CBaseAllocator::NotifySample()
{
    if (m_lWaiting != 0) {
        ASSERT(m_hSem != NULL);
        ReleaseSemaphore(m_hSem, m_lWaiting, 0);
        m_lWaiting = 0;
    }
}

STDMETHODIMP
CBaseAllocator::Commit()
{
    
    CAutoLock cObjectLock(this);

    
    if (m_bCommitted) {
        return NOERROR;
    }

    

    m_bCommitted = TRUE;

    
    if (m_bDecommitInProgress) {
        m_bDecommitInProgress = FALSE;

        
        
        
        
        return NOERROR;
    }

    DbgLog((LOG_MEMORY, 1, TEXT("Allocating: %ldx%ld"), m_lCount, m_lSize));

    
    HRESULT hr = Alloc();
    if (FAILED(hr)) {
        m_bCommitted = FALSE;
        return hr;
    }
    AddRef();
    return NOERROR;
}  

STDMETHODIMP
CBaseAllocator::Decommit()
{
    BOOL bRelease = FALSE;
    {
        
        CAutoLock cObjectLock(this);
        if (m_bCommitted == FALSE) {
            if (m_bDecommitInProgress == FALSE) {
                return NOERROR;
            }
        }

        
        m_bCommitted = FALSE;

        
        if (m_lFree.GetCount() < m_lAllocated) {
            
            m_bDecommitInProgress = TRUE;
        } else {
            m_bDecommitInProgress = FALSE;

            
            

            Free();
            bRelease = TRUE;
        }

        
        
        NotifySample();
    }

    if (bRelease) {
        Release();
    }
    return NOERROR;
}      

HRESULT
CBaseAllocator::Alloc(void)
{
    
    if (m_lCount <= 0 || m_lSize <= 0 || m_lAlignment <= 0) {
        return VFW_E_SIZENOTSET;
    }

    
    ASSERT(m_lFree.GetCount() == m_lAllocated);

    
    if (m_bChanged == FALSE) {
        return S_FALSE;
    }

    return NOERROR;
}  

void
CBaseAllocator::CSampleList::Remove(CMediaSample * pSample)
{
    CMediaSample **pSearch;
    for (pSearch = &m_List;
         *pSearch != NULL;
         pSearch = &(CBaseAllocator::NextSample(*pSearch))) {
       if (*pSearch == pSample) {
           *pSearch = CBaseAllocator::NextSample(pSample);
           CBaseAllocator::NextSample(pSample) = NULL;
           m_nOnList--;
           return;
       }
    }
    DbgBreak("Couldn't find sample in list");
}                

CUnknown *CMemAllocator::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    CUnknown *pUnkRet = new CMemAllocator(NAME("CMemAllocator"), pUnk, phr);
    return pUnkRet;
}

CMemAllocator::CMemAllocator(
    TCHAR *pName,
    LPUNKNOWN pUnk,
    HRESULT *phr)
    : CBaseAllocator(pName, pUnk, phr, TRUE, TRUE),
    m_pBuffer(NULL)
{
}

#ifdef UNICODE
CMemAllocator::CMemAllocator(
    CHAR *pName,
    LPUNKNOWN pUnk,
    HRESULT *phr)
    : CBaseAllocator(pName, pUnk, phr, TRUE, TRUE),
    m_pBuffer(NULL)
{
}
#endif  

STDMETHODIMP
CMemAllocator::SetProperties(
                ALLOCATOR_PROPERTIES* pRequest,
                ALLOCATOR_PROPERTIES* pActual)
{
    CheckPointer(pActual,E_POINTER);
    ValidateReadWritePtr(pActual,sizeof(ALLOCATOR_PROPERTIES));
    CAutoLock cObjectLock(this);

    ZeroMemory(pActual, sizeof(ALLOCATOR_PROPERTIES));

    ASSERT(pRequest->cbBuffer > 0);

    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);

    
    if ((-pRequest->cbAlign & pRequest->cbAlign) != pRequest->cbAlign) {
        DbgLog((LOG_ERROR, 1, TEXT("Alignment requested 0x%x not a power of 2!"),
               pRequest->cbAlign));
    }
    
    if (pRequest->cbAlign == 0 ||
    (SysInfo.dwAllocationGranularity & (pRequest->cbAlign - 1)) != 0) {
        DbgLog((LOG_ERROR, 1, TEXT("Invalid alignment 0x%x requested - granularity = 0x%x"),
               pRequest->cbAlign, SysInfo.dwAllocationGranularity));
        return VFW_E_BADALIGN;
    }

    

    if (m_bCommitted == TRUE) {
        return VFW_E_ALREADY_COMMITTED;
    }

    

    if (m_lFree.GetCount() < m_lAllocated) {
        return VFW_E_BUFFERS_OUTSTANDING;
    }

    

    
    
    LONG lSize = pRequest->cbBuffer + pRequest->cbPrefix;
    LONG lRemainder = lSize % pRequest->cbAlign;
    if (lRemainder != 0) {
        lSize = lSize - lRemainder + pRequest->cbAlign;
    }
    pActual->cbBuffer = m_lSize = (lSize - pRequest->cbPrefix);

    pActual->cBuffers = m_lCount = pRequest->cBuffers;
    pActual->cbAlign = m_lAlignment = pRequest->cbAlign;
    pActual->cbPrefix = m_lPrefix = pRequest->cbPrefix;

    m_bChanged = TRUE;
    return NOERROR;
}              

HRESULT
CMemAllocator::Alloc(void)
{
    CAutoLock lck(this);

    
    HRESULT hr = CBaseAllocator::Alloc();
    if (FAILED(hr)) {
        return hr;
    }

    
    if (hr == S_FALSE) {
        ASSERT(m_pBuffer);
        return NOERROR;
    }
    ASSERT(hr == S_OK); 

    
    if (m_pBuffer) {
        ReallyFree();
    }

    
    LONG lAlignedSize = m_lSize + m_lPrefix;
    if (m_lAlignment > 1) {
        LONG lRemainder = lAlignedSize % m_lAlignment;
        if (lRemainder != 0) {
            lAlignedSize += (m_lAlignment - lRemainder);
        }
    }

    
    ASSERT(lAlignedSize % m_lAlignment == 0);

    m_pBuffer = (PBYTE)VirtualAlloc(NULL,
                    m_lCount * lAlignedSize,
                    MEM_COMMIT,
                    PAGE_READWRITE);

    if (m_pBuffer == NULL) {
        return E_OUTOFMEMORY;
    }

    LPBYTE pNext = m_pBuffer;
    CMediaSample *pSample;

    ASSERT(m_lAllocated == 0);

    
    
    
    
    for (; m_lAllocated < m_lCount; m_lAllocated++, pNext += lAlignedSize) {  

        pSample = new CMediaSample(
                            NAME("Default memory media sample"),
                this,
                            &hr,
                            pNext + m_lPrefix,      
                            m_lSize);               

            ASSERT(SUCCEEDED(hr));
        if (pSample == NULL) {
            return E_OUTOFMEMORY;
        }

        
        m_lFree.Add(pSample);
    }

    m_bChanged = FALSE;
    return NOERROR;
}                    

void
CMemAllocator::Free(void)
{
    return;
}      

void
CMemAllocator::ReallyFree(void)
{
    

    ASSERT(m_lAllocated == m_lFree.GetCount());

    

    CMediaSample *pSample;
    for (;;) {
        pSample = m_lFree.RemoveHead();
        if (pSample != NULL) {
            delete pSample;
        } else {
            break;
        }
    }

    m_lAllocated = 0;

    
    if (m_pBuffer) {
        EXECUTE_ASSERT(VirtualFree(m_pBuffer, 0, MEM_RELEASE));
        m_pBuffer = NULL;
    }
}      

CMemAllocator::~CMemAllocator()
{
    Decommit();
    ReallyFree();
}        

STDAPI
AMovieSetupRegisterFilter( const AMOVIESETUP_FILTER * const psetupdata
                         , IFilterMapper *                  pIFM
                         , BOOL                             bRegister  )
{
  DbgLog((LOG_TRACE, 3, TEXT("= AMovieSetupRegisterFilter")));

  
  
  if( NULL == psetupdata ) return S_FALSE;  

  
  
  
  
  DbgLog((LOG_TRACE, 3, TEXT("= = unregister filter")));
  HRESULT hr = pIFM->UnregisterFilter( *(psetupdata->clsID) );  

  if( bRegister )
  {
    
    
    DbgLog((LOG_TRACE, 3, TEXT("= = register filter")));
    hr = pIFM->RegisterFilter( *(psetupdata->clsID)
                             , psetupdata->strName
                             , psetupdata->dwMerit    );
    if( SUCCEEDED(hr) )
    {
      
      
      DbgLog((LOG_TRACE, 3, TEXT("= = register filter pins")));
      for( UINT m1=0; m1 < psetupdata->nPins; m1++ )
      {
        hr = pIFM->RegisterPin( *(psetupdata->clsID)
                              , psetupdata->lpPin[m1].strName
                              , psetupdata->lpPin[m1].bRendered
                              , psetupdata->lpPin[m1].bOutput
                              , psetupdata->lpPin[m1].bZero
                              , psetupdata->lpPin[m1].bMany
                              , *(psetupdata->lpPin[m1].clsConnectsToFilter)
                              , psetupdata->lpPin[m1].strConnectsToPin );

        if( SUCCEEDED(hr) )
        {
          
          
          DbgLog((LOG_TRACE, 3, TEXT("= = register filter pin types")));
          for( UINT m2=0; m2 < psetupdata->lpPin[m1].nMediaTypes; m2++ )
          {
            hr = pIFM->RegisterPinType( *(psetupdata->clsID)
                                      , psetupdata->lpPin[m1].strName
                                      , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMajorType)
                                      , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMinorType) );
            if( FAILED(hr) ) break;
          }
          if( FAILED(hr) ) break;
        }
        if( FAILED(hr) ) break;
      }
    }
  }

  
  
  
  
  
  if( 0x80070002 == hr)
    return NOERROR;
  else
    return hr;
}  

#pragma warning(disable:4514)

