/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#include <streams.h>
#include <measure.h>          

CTransformFilter::CTransformFilter(TCHAR     *pName,
                                   LPUNKNOWN pUnk,
                                   REFCLSID  clsid) :
    CBaseFilter(pName,pUnk,&m_csFilter, clsid),
    m_pInput(NULL),
    m_pOutput(NULL),
    m_bEOSDelivered(FALSE),
    m_bQualityChanged(FALSE),
    m_bSampleSkipped(FALSE)
{
#ifdef PERF
    RegisterPerfId();
#endif 
}

#ifdef UNICODE
CTransformFilter::CTransformFilter(char     *pName,
                                   LPUNKNOWN pUnk,
                                   REFCLSID  clsid) :
    CBaseFilter(pName,pUnk,&m_csFilter, clsid),
    m_pInput(NULL),
    m_pOutput(NULL),
    m_bEOSDelivered(FALSE),
    m_bQualityChanged(FALSE),
    m_bSampleSkipped(FALSE)
{
#ifdef PERF
    RegisterPerfId();
#endif 
}
#endif    

CTransformFilter::~CTransformFilter()
{
    

    delete m_pInput;
    delete m_pOutput;
}    

HRESULT CTransformFilter::Transform(IMediaSample * pIn, IMediaSample *pOut)
{
    UNREFERENCED_PARAMETER(pIn);
    UNREFERENCED_PARAMETER(pOut);
    DbgBreak("CTransformFilter::Transform() should never be called");
    return E_UNEXPECTED;
}      

int CTransformFilter::GetPinCount()
{
    return 2;
}                    

CBasePin *
CTransformFilter::GetPin(int n)
{
    HRESULT hr = S_OK;

    

    if (m_pInput == NULL) {

        m_pInput = new CTransformInputPin(NAME("Transform input pin"),
                                          this,              
                                          &hr,               
                                          L"XForm In");        

        
        ASSERT(SUCCEEDED(hr));
        if (m_pInput == NULL) {
            return NULL;
        }
        m_pOutput = (CTransformOutputPin *)
		   new CTransformOutputPin(NAME("Transform output pin"),
                                            this,            
                                            &hr,             
                                            L"XForm Out");     

        
        ASSERT(SUCCEEDED(hr));
        if (m_pOutput == NULL) {
            delete m_pInput;
            m_pInput = NULL;
        }
    }

    

    if (n == 0) {
        return m_pInput;
    } else
    if (n == 1) {
        return m_pOutput;
    } else {
        return NULL;
    }
}              

STDMETHODIMP CTransformFilter::FindPin(LPCWSTR Id, IPin **ppPin)
{
    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(IPin *));

    if (0==lstrcmpW(Id,L"In")) {
        *ppPin = GetPin(0);
    } else if (0==lstrcmpW(Id,L"Out")) {
        *ppPin = GetPin(1);
    } else {
        *ppPin = NULL;
        return VFW_E_NOT_FOUND;
    }

    HRESULT hr = NOERROR;
    
    if (*ppPin) {
        (*ppPin)->AddRef();
    } else {
        hr = E_OUTOFMEMORY;  
    }
    return hr;
}        

HRESULT
CTransformFilter::StartStreaming()
{
    return NOERROR;
}  

HRESULT
CTransformFilter::StopStreaming()
{
    return NOERROR;
}      

HRESULT
CTransformFilter::CheckConnect(PIN_DIRECTION dir,IPin *pPin)
{
    UNREFERENCED_PARAMETER(dir);
    UNREFERENCED_PARAMETER(pPin);
    return NOERROR;
}      

HRESULT
CTransformFilter::BreakConnect(PIN_DIRECTION dir)
{
    UNREFERENCED_PARAMETER(dir);
    return NOERROR;
}      

HRESULT
CTransformFilter::CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(direction);
    UNREFERENCED_PARAMETER(pReceivePin);
    return NOERROR;
}      

HRESULT
CTransformFilter::SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt)
{
    UNREFERENCED_PARAMETER(direction);
    UNREFERENCED_PARAMETER(pmt);
    return NOERROR;
}    

HRESULT
CTransformFilter::InitializeOutputSample(IMediaSample *pSample, IMediaSample **ppOutSample)
{
    IMediaSample *pOutSample;

    

    AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    DWORD dwFlags = m_bSampleSkipped ? AM_GBF_PREVFRAMESKIPPED : 0;

    
    
    
    
    if (!(pProps->dwSampleFlags & AM_SAMPLE_SPLICEPOINT)) {
	dwFlags |= AM_GBF_NOTASYNCPOINT;
    }

    ASSERT(m_pOutput->m_pAllocator != NULL);
    HRESULT hr = m_pOutput->m_pAllocator->GetBuffer(
             &pOutSample
             , pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID ?
                   &pProps->tStart : NULL
             , pProps->dwSampleFlags & AM_SAMPLE_STOPVALID ?
                   &pProps->tStop : NULL
             , dwFlags
         );
    *ppOutSample = pOutSample;
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT(pOutSample);
    IMediaSample2 *pOutSample2;
    if (SUCCEEDED(pOutSample->QueryInterface(IID_IMediaSample2,
                                             (void **)&pOutSample2))) {
        
        AM_SAMPLE2_PROPERTIES OutProps;
        EXECUTE_ASSERT(SUCCEEDED(pOutSample2->GetProperties(
            FIELD_OFFSET(AM_SAMPLE2_PROPERTIES, tStart), (PBYTE)&OutProps)
        ));
        OutProps.dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;
        OutProps.dwSampleFlags =
            (OutProps.dwSampleFlags & AM_SAMPLE_TYPECHANGED) |
            (pProps->dwSampleFlags & ~AM_SAMPLE_TYPECHANGED);
        OutProps.tStart = pProps->tStart;
        OutProps.tStop  = pProps->tStop;
        OutProps.cbData = FIELD_OFFSET(AM_SAMPLE2_PROPERTIES, dwStreamId);
        hr = pOutSample2->SetProperties(
            FIELD_OFFSET(AM_SAMPLE2_PROPERTIES, dwStreamId),
            (PBYTE)&OutProps
        );
        if (pProps->dwSampleFlags & AM_SAMPLE_DATADISCONTINUITY) {
            m_bSampleSkipped = FALSE;
        }
        pOutSample2->Release();
    } else {
        if (pProps->dwSampleFlags & AM_SAMPLE_TIMEVALID) {
            pOutSample->SetTime(&pProps->tStart,
                                &pProps->tStop);
        }
        if (pProps->dwSampleFlags & AM_SAMPLE_SPLICEPOINT) {
            pOutSample->SetSyncPoint(TRUE);
        }
        if (pProps->dwSampleFlags & AM_SAMPLE_DATADISCONTINUITY) {
            pOutSample->SetDiscontinuity(TRUE);
            m_bSampleSkipped = FALSE;
        }
        

        LONGLONG MediaStart, MediaEnd;
        if (pSample->GetMediaTime(&MediaStart,&MediaEnd) == NOERROR) {
            pOutSample->SetMediaTime(&MediaStart,&MediaEnd);
        }
    }
    return S_OK;
}    

HRESULT
CTransformFilter::Receive(IMediaSample *pSample)
{
    
    AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    if (pProps->dwStreamId != AM_STREAM_MEDIA) {
        return m_pOutput->m_pInputPin->Receive(pSample);
    }
    HRESULT hr;
    ASSERT(pSample);
    IMediaSample * pOutSample;

    

    ASSERT (m_pOutput != NULL) ;

    
    hr = InitializeOutputSample(pSample, &pOutSample);

    if (FAILED(hr)) {
        return hr;
    }

    
    MSR_START(m_idTransform);

    

    hr = Transform(pSample, pOutSample);

    
    MSR_STOP(m_idTransform);

    if (FAILED(hr)) {
	DbgLog((LOG_TRACE,1,TEXT("Error from transform")));
    } else {
        
        
        
        if (hr == NOERROR) {
    	    hr = m_pOutput->m_pInputPin->Receive(pOutSample);
            m_bSampleSkipped = FALSE;	
        } else {
            
            
            
            
            if (S_FALSE == hr) {

                
                
                
                pOutSample->Release();
                m_bSampleSkipped = TRUE;
                if (!m_bQualityChanged) {
                    NotifyEvent(EC_QUALITY_CHANGE,0,0);
                    m_bQualityChanged = TRUE;
                }
                return NOERROR;
            }
        }
    }

    
    
    pOutSample->Release();

    return hr;
}        

HRESULT CTransformFilter::AlterQuality(Quality q)
{
    UNREFERENCED_PARAMETER(q);
    return S_FALSE;
}          

HRESULT
CTransformFilter::EndOfStream(void)
{
    HRESULT hr = NOERROR;
    if (m_pOutput != NULL) {
        hr = m_pOutput->DeliverEndOfStream();
    }

    return hr;
}      

HRESULT
CTransformFilter::BeginFlush(void)
{
    HRESULT hr = NOERROR;
    if (m_pOutput != NULL) {
	

	

	

	
	hr = m_pOutput->DeliverBeginFlush();
    }
    return hr;
}      

HRESULT
CTransformFilter::EndFlush(void)
{
    

    

    
    ASSERT (m_pOutput != NULL);
    return m_pOutput->DeliverEndFlush();

    
}      

STDMETHODIMP
CTransformFilter::Stop()
{
    CAutoLock lck1(&m_csFilter);
    if (m_State == State_Stopped) {
        return NOERROR;
    }

    

    ASSERT(m_pInput == NULL || m_pOutput != NULL);
    if (m_pInput == NULL || m_pInput->IsConnected() == FALSE ||
        m_pOutput->IsConnected() == FALSE) {
                m_State = State_Stopped;
                m_bEOSDelivered = FALSE;
                return NOERROR;
    }

    ASSERT(m_pInput);
    ASSERT(m_pOutput);

    
    m_pInput->Inactive();

    

    CAutoLock lck2(&m_csReceive);
    m_pOutput->Inactive();

    
    

    HRESULT hr = StopStreaming();
    if (SUCCEEDED(hr)) {
	
	m_State = State_Stopped;
	m_bEOSDelivered = FALSE;
    }
    return hr;
}  

STDMETHODIMP
CTransformFilter::Pause()
{
    CAutoLock lck(&m_csFilter);
    HRESULT hr = NOERROR;

    if (m_State == State_Paused) {
        
    }

    
    
    
    

    else if (m_pInput == NULL || m_pInput->IsConnected() == FALSE) {
        if (m_pOutput && m_bEOSDelivered == FALSE) {
            m_pOutput->DeliverEndOfStream();
            m_bEOSDelivered = TRUE;
        }
        m_State = State_Paused;
    }

    
    

    else if (m_pOutput->IsConnected() == FALSE) {
        m_State = State_Paused;
    }

    else {
	if (m_State == State_Stopped) {
	    
	    
            CAutoLock lck2(&m_csReceive);
	    hr = StartStreaming();
	}
	if (SUCCEEDED(hr)) {
	    hr = CBaseFilter::Pause();
	}
    }

    m_bSampleSkipped = FALSE;
    m_bQualityChanged = FALSE;
    return hr;
}

HRESULT
CTransformFilter::NewSegment(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop,
    double dRate)
{
    if (m_pOutput != NULL) {
        return m_pOutput->DeliverNewSegment(tStart, tStop, dRate);
    }
    return S_OK;
}  

HRESULT
CTransformInputPin::CheckStreaming()
{
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    } else {
        
        ASSERT(IsConnected());

        
        if (m_bFlushing) {
            return S_FALSE;
        }
        
        if (IsStopped()) {
            return VFW_E_WRONG_STATE;
        }
        if (m_bRunTimeError) {
    	    return VFW_E_RUNTIME_ERROR;
        }
        return S_OK;
    }
}                

CTransformInputPin::CTransformInputPin(
    TCHAR *pObjectName,
    CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pName)
    : CBaseInputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pName)
{
    DbgLog((LOG_TRACE,2,TEXT("CTransformInputPin::CTransformInputPin")));
    m_pTransformFilter = pTransformFilter;
}

#ifdef UNICODE
CTransformInputPin::CTransformInputPin(
    CHAR *pObjectName,
    CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pName)
    : CBaseInputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pName)
{
    DbgLog((LOG_TRACE,2,TEXT("CTransformInputPin::CTransformInputPin")));
    m_pTransformFilter = pTransformFilter;
}
#endif    

HRESULT
CTransformInputPin::CheckConnect(IPin *pPin)
{
    HRESULT hr = m_pTransformFilter->CheckConnect(PINDIR_INPUT,pPin);
    if (FAILED(hr)) {
    	return hr;
    }
    return CBaseInputPin::CheckConnect(pPin);
}      

HRESULT
CTransformInputPin::BreakConnect()
{
    
    ASSERT(IsStopped());
    m_pTransformFilter->BreakConnect(PINDIR_INPUT);
    return CBaseInputPin::BreakConnect();
}      

HRESULT
CTransformInputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = m_pTransformFilter->CompleteConnect(PINDIR_INPUT,pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    return CBaseInputPin::CompleteConnect(pReceivePin);
}      

HRESULT
CTransformInputPin::CheckMediaType(const CMediaType* pmt)
{
    

    HRESULT hr = m_pTransformFilter->CheckInputType(pmt);
    if (S_OK != hr) {
        return hr;
    }

    
    

    if ((m_pTransformFilter->m_pOutput != NULL) &&
        (m_pTransformFilter->m_pOutput->IsConnected())) {
            return m_pTransformFilter->CheckTransform(
                      pmt,
		      &m_pTransformFilter->m_pOutput->CurrentMediaType());
    } else {
        return hr;
    }
}      

HRESULT
CTransformInputPin::SetMediaType(const CMediaType* mtIn)
{
    
    HRESULT hr = CBasePin::SetMediaType(mtIn);
    ASSERT(SUCCEEDED(hr));

    
    ASSERT(SUCCEEDED(m_pTransformFilter->CheckInputType(mtIn)));

    m_pTransformFilter->SetMediaType(PINDIR_INPUT,mtIn);
    return NOERROR;
}                

STDMETHODIMP
CTransformInputPin::EndOfStream(void)
{
    CAutoLock lck(&m_pTransformFilter->m_csReceive);
    HRESULT hr = CheckStreaming();
    if (S_OK == hr) {
       hr = m_pTransformFilter->EndOfStream();
    }
    return hr;
}      

STDMETHODIMP
CTransformInputPin::BeginFlush(void)
{
    CAutoLock lck(&m_pTransformFilter->m_csFilter);
    
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!IsConnected() ||
        !m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }
    HRESULT hr = CBaseInputPin::BeginFlush();
    if (FAILED(hr)) {
    	return hr;
    }

    return m_pTransformFilter->BeginFlush();
}        

STDMETHODIMP
CTransformInputPin::EndFlush(void)
{
    CAutoLock lck(&m_pTransformFilter->m_csFilter);
    
    ASSERT(m_pTransformFilter->m_pOutput != NULL);
    if (!IsConnected() ||
        !m_pTransformFilter->m_pOutput->IsConnected()) {
        return VFW_E_NOT_CONNECTED;
    }

    HRESULT hr = m_pTransformFilter->EndFlush();
    if (FAILED(hr)) {
        return hr;
    }

    return CBaseInputPin::EndFlush();
}          

HRESULT
CTransformInputPin::Receive(IMediaSample * pSample)
{
    HRESULT hr;
    CAutoLock lck(&m_pTransformFilter->m_csReceive);
    ASSERT(pSample);

    
    hr = CBaseInputPin::Receive(pSample);
    if (S_OK == hr) {
        hr = m_pTransformFilter->Receive(pSample);
    }
    return hr;
}        

STDMETHODIMP
CTransformInputPin::NewSegment(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop,
    double dRate)
{
    
    CBasePin::NewSegment(tStart, tStop, dRate);
    return m_pTransformFilter->NewSegment(tStart, tStop, dRate);
}                    

CTransformOutputPin::CTransformOutputPin(
    TCHAR *pObjectName,
    CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : CBaseOutputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pPinName),
      m_pPosition(NULL)
{
    DbgLog((LOG_TRACE,2,TEXT("CTransformOutputPin::CTransformOutputPin")));
    m_pTransformFilter = pTransformFilter;

}

#ifdef UNICODE
CTransformOutputPin::CTransformOutputPin(
    CHAR *pObjectName,
    CTransformFilter *pTransformFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : CBaseOutputPin(pObjectName, pTransformFilter, &pTransformFilter->m_csFilter, phr, pPinName),
      m_pPosition(NULL)
{
    DbgLog((LOG_TRACE,2,TEXT("CTransformOutputPin::CTransformOutputPin")));
    m_pTransformFilter = pTransformFilter;

}
#endif    

CTransformOutputPin::~CTransformOutputPin()
{
    DbgLog((LOG_TRACE,2,TEXT("CTransformOutputPin::~CTransformOutputPin")));

    if (m_pPosition) m_pPosition->Release();
}      

STDMETHODIMP
CTransformOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    ValidateReadWritePtr(ppv,sizeof(PVOID));
    *ppv = NULL;

    if (riid == IID_IMediaPosition || riid == IID_IMediaSeeking) {

        

        ASSERT(m_pTransformFilter->m_pInput != NULL);

        if (m_pPosition == NULL) {

            HRESULT hr = CreatePosPassThru(
                             GetOwner(),
                             FALSE,
                             (IPin *)m_pTransformFilter->m_pInput,
                             &m_pPosition);
            if (FAILED(hr)) {
                return hr;
            }
        }
        return m_pPosition->QueryInterface(riid, ppv);
    } else {
        return CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
    }
}      

HRESULT
CTransformOutputPin::CheckConnect(IPin *pPin)
{
    

    ASSERT(m_pTransformFilter->m_pInput != NULL);
    if ((m_pTransformFilter->m_pInput->IsConnected() == FALSE)) {
	    return E_UNEXPECTED;
    }

    HRESULT hr = m_pTransformFilter->CheckConnect(PINDIR_OUTPUT,pPin);
    if (FAILED(hr)) {
	    return hr;
    }
    return CBaseOutputPin::CheckConnect(pPin);
}      

HRESULT
CTransformOutputPin::BreakConnect()
{
    
    ASSERT(IsStopped());
    m_pTransformFilter->BreakConnect(PINDIR_OUTPUT);
    return CBaseOutputPin::BreakConnect();
}      

HRESULT
CTransformOutputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = m_pTransformFilter->CompleteConnect(PINDIR_OUTPUT,pReceivePin);
    if (FAILED(hr)) {
        return hr;
    }
    return CBaseOutputPin::CompleteConnect(pReceivePin);
}      

HRESULT
CTransformOutputPin::CheckMediaType(const CMediaType* pmtOut)
{
    
    ASSERT(m_pTransformFilter->m_pInput != NULL);
    if ((m_pTransformFilter->m_pInput->IsConnected() == FALSE)) {
	        return E_INVALIDARG;
    }

    return m_pTransformFilter->CheckTransform(
				    &m_pTransformFilter->m_pInput->CurrentMediaType(),
				    pmtOut);
}        

HRESULT
CTransformOutputPin::SetMediaType(const CMediaType* pmtOut)
{
    HRESULT hr = NOERROR;
    ASSERT(m_pTransformFilter->m_pInput != NULL);

    ASSERT(m_pTransformFilter->m_pInput->CurrentMediaType().IsValid());

    
    hr = CBasePin::SetMediaType(pmtOut);
    ASSERT(SUCCEEDED(hr));

#ifdef DEBUG
    if (FAILED(m_pTransformFilter->CheckTransform(&m_pTransformFilter->
					m_pInput->CurrentMediaType(),pmtOut))) {
	DbgLog((LOG_ERROR,0,TEXT("*** This filter is accepting an output media type")));
	DbgLog((LOG_ERROR,0,TEXT("    that it can't currently transform to.  I hope")));
	DbgLog((LOG_ERROR,0,TEXT("    it's smart enough to reconnect its input.")));
    }
#endif

    m_pTransformFilter->SetMediaType(PINDIR_OUTPUT,pmtOut);
    return NOERROR;
}      

HRESULT
CTransformOutputPin::DecideBufferSize(
    IMemAllocator * pAllocator,
    ALLOCATOR_PROPERTIES* pProp)
{
    return m_pTransformFilter->DecideBufferSize(pAllocator, pProp);
}        

HRESULT
CTransformOutputPin::GetMediaType(
    int iPosition,
    CMediaType *pMediaType)
{
    ASSERT(m_pTransformFilter->m_pInput != NULL);

    

    if (m_pTransformFilter->m_pInput->IsConnected()) {
        return m_pTransformFilter->GetMediaType(iPosition,pMediaType);
    } else {
        return VFW_S_NO_MORE_ITEMS;
    }
}            

STDMETHODIMP
CTransformOutputPin::Notify(IBaseFilter * pSender, Quality q)
{
    UNREFERENCED_PARAMETER(pSender);
    ValidateReadPtr(pSender,sizeof(IBaseFilter));

    
    HRESULT hr = m_pTransformFilter->AlterQuality(q);
    if (hr!=S_FALSE) {
        return hr;        
    }

    
    

    ASSERT(m_pTransformFilter->m_pInput != NULL);

    return m_pTransformFilter->m_pInput->PassNotify(q);

}       

#pragma warning(disable:4514)
