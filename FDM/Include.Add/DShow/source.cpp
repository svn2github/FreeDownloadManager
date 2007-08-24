/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                                        

#include <streams.h>            

CSource::CSource(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid)
    : CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
      m_iPins(0),
      m_paStreams(NULL)
{
}

CSource::CSource(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr)
    : CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
      m_iPins(0),
      m_paStreams(NULL)
{
    UNREFERENCED_PARAMETER(phr);
}

#ifdef UNICODE
CSource::CSource(CHAR *pName, LPUNKNOWN lpunk, CLSID clsid)
    : CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
      m_iPins(0),
      m_paStreams(NULL)
{
}

CSource::CSource(CHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *phr)
    : CBaseFilter(pName, lpunk, &m_cStateLock, clsid),
      m_iPins(0),
      m_paStreams(NULL)
{
    UNREFERENCED_PARAMETER(phr);
}
#endif      

CSource::~CSource()
{
    
    while (m_iPins != 0) {
	
	delete m_paStreams[m_iPins - 1];
    }

    ASSERT(m_paStreams == NULL);
}        

HRESULT CSource::AddPin(CSourceStream *pStream)
{
    CAutoLock lock(&m_cStateLock);

    
    CSourceStream **paStreams = new CSourceStream *[m_iPins + 1];
    if (paStreams == NULL) {
        return E_OUTOFMEMORY;
    }
    if (m_paStreams != NULL) {
        CopyMemory((PVOID)paStreams, (PVOID)m_paStreams,
                   m_iPins * sizeof(m_paStreams[0]));
        paStreams[m_iPins] = pStream;
        delete [] m_paStreams;
    }
    m_paStreams = paStreams;
    m_paStreams[m_iPins] = pStream;
    m_iPins++;
    return S_OK;
}      

HRESULT CSource::RemovePin(CSourceStream *pStream)
{
    int i;
    for (i = 0; i < m_iPins; i++) {
        if (m_paStreams[i] == pStream) {
            if (m_iPins == 1) {
                delete [] m_paStreams;
                m_paStreams = NULL;
            } else {
                
		while (++i < m_iPins)
		    m_paStreams[i - 1] = m_paStreams[i];
            }
            m_iPins--;
            return S_OK;
        }
    }
    return S_FALSE;
}          

STDMETHODIMP CSource::FindPin(LPCWSTR Id, IPin **ppPin)
{
    CheckPointer(ppPin,E_POINTER);
    ValidateReadWritePtr(ppPin,sizeof(IPin *));
    
    
    int i = WstrToInt(Id) -1;
    *ppPin = GetPin(i);
    if (*ppPin!=NULL){
        (*ppPin)->AddRef();
        return NOERROR;
    } else {
        return VFW_E_NOT_FOUND;
    }
}        

int CSource::FindPinNumber(IPin *iPin) {
    int i;
    for (i=0; i<m_iPins; ++i) {
        if ((IPin *)(m_paStreams[i])==iPin) {
            return i;
        }
    }
    return -1;
}        

int CSource::GetPinCount(void) {

    CAutoLock lock(&m_cStateLock);
    return m_iPins;
}            

CBasePin *CSource::GetPin(int n) {

    CAutoLock lock(&m_cStateLock);

    
    
    
    if ((n >= 0) && (n < m_iPins)) {

        ASSERT(m_paStreams[n]);
	return m_paStreams[n];
    }
    return NULL;
}                    

STDMETHODIMP CSourceStream::QueryId(LPWSTR *Id) {
    CheckPointer(Id,E_POINTER);
    ValidateReadWritePtr(Id,sizeof(LPWSTR));

    
    
    int i = 1+ m_pFilter->FindPinNumber(this);
    if (i<1) return VFW_E_NOT_FOUND;
    *Id = (LPWSTR)CoTaskMemAlloc(8);
    if (*Id==NULL) {
       return E_OUTOFMEMORY;
    }
    IntToWstr(i, *Id);
    return NOERROR;
}            

CSourceStream::CSourceStream(
    TCHAR *pObjectName,
    HRESULT *phr,
    CSource *ps,
    LPCWSTR pPinName)
    : CBaseOutputPin(pObjectName, ps, ps->pStateLock(), phr, pPinName),
      m_pFilter(ps) {

     *phr = m_pFilter->AddPin(this);
}

#ifdef UNICODE
CSourceStream::CSourceStream(
    char *pObjectName,
    HRESULT *phr,
    CSource *ps,
    LPCWSTR pPinName)
    : CBaseOutputPin(pObjectName, ps, ps->pStateLock(), phr, pPinName),
      m_pFilter(ps) {

     *phr = m_pFilter->AddPin(this);
}
#endif      

CSourceStream::~CSourceStream(void) {

     m_pFilter->RemovePin(this);
}          

HRESULT CSourceStream::CheckMediaType(const CMediaType *pMediaType) {

    CAutoLock lock(m_pFilter->pStateLock());

    CMediaType mt;
    GetMediaType(&mt);

    if (mt == *pMediaType) {
        return NOERROR;
    }

    return E_FAIL;
}            

HRESULT CSourceStream::GetMediaType(int iPosition, CMediaType *pMediaType) {

    CAutoLock lock(m_pFilter->pStateLock());

    if (iPosition<0) {
        return E_INVALIDARG;
    }
    if (iPosition>0) {
        return VFW_S_NO_MORE_ITEMS;
    }
    return GetMediaType(pMediaType);
}          

HRESULT CSourceStream::Active(void) {

    CAutoLock lock(m_pFilter->pStateLock());

    HRESULT hr;

    if (m_pFilter->IsActive()) {
	return S_FALSE;	
    }

    
    
    if (!IsConnected()) {
        return NOERROR;
    }

    hr = CBaseOutputPin::Active();
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT(!ThreadExists());

    
    if (!Create()) {
        return E_FAIL;
    }

    
    hr = Init();
    if (FAILED(hr))
	return hr;

    return Pause();
}            

HRESULT CSourceStream::Inactive(void) {

    CAutoLock lock(m_pFilter->pStateLock());

    HRESULT hr;

    
    
    if (!IsConnected()) {
        return NOERROR;
    }

    
    

    hr = CBaseOutputPin::Inactive();  
    if (FAILED(hr)) {
	return hr;
    }

    if (ThreadExists()) {
	hr = Stop();

	if (FAILED(hr)) {
	    return hr;
	}

	hr = Exit();
	if (FAILED(hr)) {
	    return hr;
	}

	Close();	
    }

    
    
    
    

    return NOERROR;
}            

DWORD CSourceStream::ThreadProc(void) {

    HRESULT hr;  
    Command com;

    do {
	com = GetRequest();
	if (com != CMD_INIT) {
	    DbgLog((LOG_ERROR, 1, TEXT("Thread expected init command")));
	    Reply((DWORD) E_UNEXPECTED);
	}
    } while (com != CMD_INIT);

    DbgLog((LOG_TRACE, 1, TEXT("CSourceStream worker thread initializing")));

    hr = OnThreadCreate(); 
    if (FAILED(hr)) {
        DbgLog((LOG_ERROR, 1, TEXT("CSourceStream::OnThreadCreate failed. Aborting thread.")));
        OnThreadDestroy();
	Reply(hr);	
        return 1;
    }

    
    Reply(NOERROR);

    Command cmd;
    do {
	cmd = GetRequest();

	switch (cmd) {

	case CMD_EXIT:
	    Reply(NOERROR);
	    break;

	case CMD_RUN:
	    DbgLog((LOG_ERROR, 1, TEXT("CMD_RUN received before a CMD_PAUSE???")));
	    
	
	case CMD_PAUSE:
	    Reply(NOERROR);
	    DoBufferProcessingLoop();
	    break;

	case CMD_STOP:
	    Reply(NOERROR);
	    break;

	default:
	    DbgLog((LOG_ERROR, 1, TEXT("Unknown command %d received!"), cmd));
	    Reply((DWORD) E_NOTIMPL);
	    break;
	}
    } while (cmd != CMD_EXIT);

    hr = OnThreadDestroy();	
    if (FAILED(hr)) {
        DbgLog((LOG_ERROR, 1, TEXT("CSourceStream::OnThreadDestroy failed. Exiting thread.")));
        return 1;
    }

    DbgLog((LOG_TRACE, 1, TEXT("CSourceStream worker thread exiting")));
    return 0;
}            

HRESULT CSourceStream::DoBufferProcessingLoop(void) {

    Command com;

    OnThreadStartPlay();

    do {
	while (!CheckRequest(&com)) {

	    IMediaSample *pSample;

	    HRESULT hr = GetDeliveryBuffer(&pSample,NULL,NULL,0);
	    if (FAILED(hr)) {
                Sleep(1);
		continue;	
			    
			    
	    }

	    
	    hr = FillBuffer(pSample);

	    if (hr == S_OK) {
		hr = Deliver(pSample);
                pSample->Release();

                
                
                if(hr != S_OK)
                {
                  DbgLog((LOG_TRACE, 2, TEXT("Deliver() returned %08x; stopping"), hr));
                  return S_OK;
                }

	    } else if (hr == S_FALSE) {
                
		pSample->Release();
		DeliverEndOfStream();
		return S_OK;
	    } else {
                
                pSample->Release();
		DbgLog((LOG_ERROR, 1, TEXT("Error %08lX from FillBuffer!!!"), hr));
                DeliverEndOfStream();
                m_pFilter->NotifyEvent(EC_ERRORABORT, hr, 0);
                return hr;
	    }

            
	}

        

	if (com == CMD_RUN || com == CMD_PAUSE) {
	    Reply(NOERROR);
	} else if (com != CMD_STOP) {
	    Reply((DWORD) E_UNEXPECTED);
	    DbgLog((LOG_ERROR, 1, TEXT("Unexpected command!!!")));
	}
    } while (com != CMD_STOP);

    return S_FALSE;
}

