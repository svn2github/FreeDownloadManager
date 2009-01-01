/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

#include <streams.h>
#include <measure.h>
#include <transip.h>          

CTransInPlaceFilter::CTransInPlaceFilter
   ( TCHAR     *pName,
     LPUNKNOWN  pUnk,
     REFCLSID   clsid,
     HRESULT   *phr,
     bool       bModifiesData
   )
   : CTransformFilter(pName, pUnk, clsid),
     m_bModifiesData(bModifiesData)
{
#ifdef PERF
    RegisterPerfId();
#endif 

} 

#ifdef UNICODE
CTransInPlaceFilter::CTransInPlaceFilter
   ( CHAR     *pName,
     LPUNKNOWN  pUnk,
     REFCLSID   clsid,
     HRESULT   *phr,
     bool       bModifiesData
   )
   : CTransformFilter(pName, pUnk, clsid),
     m_bModifiesData(bModifiesData)
{
#ifdef PERF
    RegisterPerfId();
#endif 

} 
#endif                    

CBasePin *
CTransInPlaceFilter::GetPin(int n)
{
    HRESULT hr = S_OK;

    

    if (m_pInput == NULL) {

        m_pInput = new CTransInPlaceInputPin( NAME("TransInPlace input pin")
                                            , this        
                                            , &hr         
                                            , L"Input"    
                                            );

        
        ASSERT(SUCCEEDED(hr));
    }

    

    if (m_pInput!=NULL && m_pOutput == NULL) {

        m_pOutput = new CTransInPlaceOutputPin( NAME("TransInPlace output pin")
                                              , this       
                                              , &hr        
                                              , L"Output"  
                                              );

        

        ASSERT(SUCCEEDED(hr));
        if (m_pOutput == NULL) {
            delete m_pInput;
            m_pInput = NULL;
        }
    }

    

    ASSERT (n>=0 && n<=1);
    if (n == 0) {
        return m_pInput;
    } else if (n==1) {
        return m_pOutput;
    } else {
        return NULL;
    }

}         

HRESULT CTransInPlaceFilter::CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin)
{
    UNREFERENCED_PARAMETER(pReceivePin);
    ASSERT(m_pInput);
    ASSERT(m_pOutput);

    
    
    if (!m_pGraph) {
        return VFW_E_NOT_IN_GRAPH;
    }

    
    
    
    
    
    
    
    
    

    if (dir == PINDIR_OUTPUT) {
        if( m_pInput->IsConnected() ) {
            return ReconnectPin( m_pInput, &m_pOutput->CurrentMediaType() );
        }
        return NOERROR;
    }

    ASSERT(dir == PINDIR_INPUT);

    

    if( m_pOutput->IsConnected() ) {

        if (  m_pInput->CurrentMediaType()
           != m_pOutput->CurrentMediaType()
           ) {
            return ReconnectPin( m_pOutput, &m_pInput->CurrentMediaType() );
        }
    }
    return NOERROR;

}                     

HRESULT CTransInPlaceFilter::DecideBufferSize
            ( IMemAllocator *pAlloc
            , ALLOCATOR_PROPERTIES *pProperties
            )
{
    ALLOCATOR_PROPERTIES Request, Actual;
    HRESULT hr;

    
    if (m_pInput->IsConnected()) {
        
        

        hr = InputPin()->PeekAllocator()->GetProperties(&Request);
        if (FAILED(hr)) {
            
            return hr;
        }
    } else {
        
        
        
        ZeroMemory(&Request, sizeof(Request));
        Request.cBuffers = 1;
        Request.cbBuffer = 1;
    }  

    DbgLog((LOG_MEMORY,1,TEXT("Setting Allocator Requirements")));
    DbgLog((LOG_MEMORY,1,TEXT("Count %d, Size %d"),
           Request.cBuffers, Request.cbBuffer));

    
    
    

    pProperties->cBuffers = Request.cBuffers;
    pProperties->cbBuffer = Request.cbBuffer;
    if (pProperties->cBuffers<=0) {pProperties->cBuffers = 1; }
    if (pProperties->cbBuffer<=0) {pProperties->cbBuffer = 1; }
    hr = pAlloc->SetProperties(pProperties, &Actual);

    if (FAILED(hr)) {
        return hr;
    }

    DbgLog((LOG_MEMORY,1,TEXT("Obtained Allocator Requirements")));
    DbgLog((LOG_MEMORY,1,TEXT("Count %d, Size %d, Alignment %d"),
           Actual.cBuffers, Actual.cbBuffer, Actual.cbAlign));

    

    if (  (Request.cBuffers > Actual.cBuffers)
       || (Request.cbBuffer > Actual.cbBuffer)
       || (Request.cbAlign  > Actual.cbAlign)
       ) {
        return E_FAIL;
    }
    return NOERROR;

}         

IMediaSample * CTransInPlaceFilter::Copy(IMediaSample *pSource)
{
    IMediaSample * pDest;

    HRESULT hr;
    REFERENCE_TIME tStart, tStop;
    const BOOL bTime = S_OK == pSource->GetTime( &tStart, &tStop);

    
    hr = OutputPin()->PeekAllocator()->GetBuffer(
              &pDest
              , bTime ? &tStart : NULL
              , bTime ? &tStop : NULL
              , m_bSampleSkipped ? AM_GBF_PREVFRAMESKIPPED : 0
              );

    if (FAILED(hr)) {
        return NULL;
    }

    ASSERT(pDest);
    IMediaSample2 *pSample2;
    if (SUCCEEDED(pDest->QueryInterface(IID_IMediaSample2, (void **)&pSample2))) {
        HRESULT hr = pSample2->SetProperties(
            FIELD_OFFSET(AM_SAMPLE2_PROPERTIES, pbBuffer),
            (PBYTE)m_pInput->SampleProps());
        pSample2->Release();
        if (FAILED(hr)) {
            pDest->Release();
            return NULL;
        }
    } else {
        if (bTime) {
            pDest->SetTime(&tStart, &tStop);
        }

        if (S_OK == pSource->IsSyncPoint()) {
            pDest->SetSyncPoint(TRUE);
        }
        if (S_OK == pSource->IsDiscontinuity() || m_bSampleSkipped) {
            pDest->SetDiscontinuity(TRUE);
        }
        if (S_OK == pSource->IsPreroll()) {
            pDest->SetPreroll(TRUE);
        }

        
        AM_MEDIA_TYPE *pMediaType;
        if (S_OK == pSource->GetMediaType(&pMediaType)) {
            pDest->SetMediaType(pMediaType);
            DeleteMediaType( pMediaType );
        }

    }

    m_bSampleSkipped = FALSE;

    
    REFERENCE_TIME TimeStart, TimeEnd;
    if (pSource->GetMediaTime(&TimeStart,&TimeEnd) == NOERROR) {
        pDest->SetMediaTime(&TimeStart,&TimeEnd);
    }

    
    {
        const long lDataLength = pSource->GetActualDataLength();
        pDest->SetActualDataLength(lDataLength);

        
        {
            BYTE *pSourceBuffer, *pDestBuffer;
            long lSourceSize  = pSource->GetSize();
            long lDestSize = pDest->GetSize();

            ASSERT(lDestSize >= lSourceSize && lDestSize >= lDataLength);

            pSource->GetPointer(&pSourceBuffer);
            pDest->GetPointer(&pDestBuffer);
            ASSERT(lDestSize == 0 || pSourceBuffer != NULL && pDestBuffer != NULL);

            CopyMemory( (PVOID) pDestBuffer, (PVOID) pSourceBuffer, lDataLength );
        }
    }

    return pDest;

}       

HRESULT
CTransInPlaceFilter::Receive(IMediaSample *pSample)
{
    
    AM_SAMPLE2_PROPERTIES * const pProps = m_pInput->SampleProps();
    if (pProps->dwStreamId != AM_STREAM_MEDIA) {
        return m_pOutput->Deliver(pSample);
    }
    HRESULT hr;

    
    MSR_START(m_idTransInPlace);

    if (UsingDifferentAllocators()) {

        

        pSample = Copy(pSample);

        if (pSample==NULL) {
            MSR_STOP(m_idTransInPlace);
            return E_UNEXPECTED;
        }
    }

    
    hr = Transform(pSample);

    
    MSR_STOP(m_idTransInPlace);

    if (FAILED(hr)) {
        DbgLog((LOG_TRACE, 1, TEXT("Error from TransInPlace")));
        if (UsingDifferentAllocators()) {
            pSample->Release();
        }
        return hr;
    }

    
    
    
    if (hr == NOERROR) {
        hr = m_pOutput->Deliver(pSample);
    } else {
        
        
        if (S_FALSE == hr) {
            
            
            
            
            m_bSampleSkipped = TRUE;
            if (!m_bQualityChanged) {
                NotifyEvent(EC_QUALITY_CHANGE,0,0);
                m_bQualityChanged = TRUE;
            }
            hr = NOERROR;
        }
    }

    
    
    if (UsingDifferentAllocators()) {
        pSample->Release();
    }

    return hr;

}                   

CTransInPlaceInputPin::CTransInPlaceInputPin
    ( TCHAR               *pObjectName
    , CTransInPlaceFilter *pFilter
    , HRESULT             *phr
    , LPCWSTR              pName
    )
    : CTransformInputPin(pObjectName,
                         pFilter,
                         phr,
                         pName)
    , m_bReadOnly(FALSE)
    , m_pTIPFilter(pFilter)
{
    DbgLog((LOG_TRACE, 2
           , TEXT("CTransInPlaceInputPin::CTransInPlaceInputPin")));

}                               

STDMETHODIMP CTransInPlaceInputPin::GetAllocator(IMemAllocator ** ppAllocator)
{
    CheckPointer(ppAllocator,E_POINTER);
    ValidateReadWritePtr(ppAllocator,sizeof(IMemAllocator *));
    CAutoLock cObjectLock(m_pLock);

    HRESULT hr;

    if ( m_pTIPFilter->m_pOutput->IsConnected() ) {
        
        hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                        ->GetAllocator( ppAllocator );
        if (SUCCEEDED(hr)) {
            m_pTIPFilter->OutputPin()->SetAllocator( *ppAllocator );
        }
    }
    else {
        
        
        
        
        hr = CTransformInputPin::GetAllocator( ppAllocator );
    }
    return hr;

}           

STDMETHODIMP
CTransInPlaceInputPin::NotifyAllocator(
    IMemAllocator * pAllocator,
    BOOL bReadOnly)
{
    HRESULT hr;
    CheckPointer(pAllocator,E_POINTER);
    ValidateReadPtr(pAllocator,sizeof(IMemAllocator));

    CAutoLock cObjectLock(m_pLock);

    m_bReadOnly = bReadOnly;
    
    

    
    
    
    if (!m_pTIPFilter->OutputPin()->IsConnected()) {
        return CTransformInputPin::NotifyAllocator(pAllocator, bReadOnly);
    }

    
    
    
    if (bReadOnly && m_pTIPFilter->m_bModifiesData) {
        IMemAllocator *pOutputAllocator =
            m_pTIPFilter->OutputPin()->PeekAllocator();

        
        if (pOutputAllocator == NULL) {
            hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()->
                                      GetAllocator(&pOutputAllocator);
            if (SUCCEEDED(hr)) {
                m_pTIPFilter->OutputPin()->SetAllocator(pOutputAllocator);
                pOutputAllocator->Release();
            }
        }
        if (pAllocator == pOutputAllocator) {
            hr = E_FAIL;
        } else {
            
            ALLOCATOR_PROPERTIES Props, Actual;
            hr = pAllocator->GetProperties(&Props);
            if (SUCCEEDED(hr)) {
                hr = pOutputAllocator->SetProperties(&Props, &Actual);
            }
            if (SUCCEEDED(hr)) {
                if (  (Props.cBuffers > Actual.cBuffers)
                   || (Props.cbBuffer > Actual.cbBuffer)
                   || (Props.cbAlign  > Actual.cbAlign)
                   ) {
                    hr =  E_FAIL;
                }
            }

            
            if (SUCCEEDED(hr)) {
                hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                       ->NotifyAllocator( pOutputAllocator, FALSE );
            }
        }
    } else {
        hr = m_pTIPFilter->OutputPin()->ConnectedIMemInputPin()
                                   ->NotifyAllocator( pAllocator, bReadOnly );
        if (SUCCEEDED(hr)) {
            m_pTIPFilter->OutputPin()->SetAllocator( pAllocator );
        }
    }

    if (SUCCEEDED(hr)) {

        
        
        pAllocator->AddRef();

        if( m_pAllocator != NULL )
            m_pAllocator->Release();

        m_pAllocator = pAllocator;    
    }

    return hr;

}       

STDMETHODIMP CTransInPlaceInputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
    
    if( !m_pTIPFilter->m_pOutput->IsConnected() )
        return VFW_E_NOT_CONNECTED;

    return m_pTIPFilter->m_pOutput->GetConnected()->EnumMediaTypes( ppEnum );

}             

HRESULT CTransInPlaceInputPin::CheckMediaType(const CMediaType *pmt )
{
    HRESULT hr = m_pTIPFilter->CheckInputType(pmt);
    if (hr!=S_OK) return hr;

    if( m_pTIPFilter->m_pOutput->IsConnected() )
        return m_pTIPFilter->m_pOutput->GetConnected()->QueryAccept( pmt );
    else
        return S_OK;

}       

STDMETHODIMP
CTransInPlaceInputPin::GetAllocatorRequirements(ALLOCATOR_PROPERTIES *pProps)
{

    if( m_pTIPFilter->m_pOutput->IsConnected() )
        return m_pTIPFilter->OutputPin()
               ->ConnectedIMemInputPin()->GetAllocatorRequirements( pProps );
    else
        return E_NOTIMPL;

}                   

CTransInPlaceOutputPin::CTransInPlaceOutputPin(
    TCHAR *pObjectName,
    CTransInPlaceFilter *pFilter,
    HRESULT * phr,
    LPCWSTR pPinName)
    : CTransformOutputPin( pObjectName
                         , pFilter
                         , phr
                         , pPinName),
      m_pTIPFilter(pFilter)
{
    DbgLog(( LOG_TRACE, 2
           , TEXT("CTransInPlaceOutputPin::CTransInPlaceOutputPin")));

}       

STDMETHODIMP CTransInPlaceOutputPin::EnumMediaTypes( IEnumMediaTypes **ppEnum )
{
    
    if( ! m_pTIPFilter->m_pInput->IsConnected() )
        return VFW_E_NOT_CONNECTED;

    return m_pTIPFilter->m_pInput->GetConnected()->EnumMediaTypes( ppEnum );

}             

HRESULT CTransInPlaceOutputPin::CheckMediaType(const CMediaType *pmt )
{
    
    
    
    if (m_pTIPFilter->UsingDifferentAllocators() && !m_pFilter->IsStopped()) {
        if (*pmt == m_mt) {
            return S_OK;
        } else {
            return VFW_E_TYPE_NOT_ACCEPTED;
        }
    }

    
    
    HRESULT hr = m_pTIPFilter->CheckInputType(pmt);
    if (hr!=S_OK) return hr;

    if( m_pTIPFilter->m_pInput->IsConnected() )
        return m_pTIPFilter->m_pInput->GetConnected()->QueryAccept( pmt );
    else
        return S_OK;

}             

HRESULT
CTransInPlaceOutputPin::DecideAllocator(IMemInputPin *pPin, IMemAllocator **ppAlloc)
{
    
    return S_OK;
}     

void
CTransInPlaceOutputPin::SetAllocator(IMemAllocator * pAllocator)
{
    pAllocator->AddRef();
    if (m_pAllocator) {
        m_pAllocator->Release();
    }
    m_pAllocator = pAllocator;
} 
