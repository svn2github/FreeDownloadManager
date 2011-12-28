/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>
#include "pullpin.h"

CPullPin::CPullPin()
  : m_pReader(NULL),
    m_pAlloc(NULL),
    m_State(TM_Exit)
{

}

CPullPin::~CPullPin()
{
    Disconnect();

}

HRESULT
CPullPin::Connect(IUnknown* pUnk, IMemAllocator* pAlloc, BOOL bSync)
{
    CAutoLock lock(&m_AccessLock);

    if (m_pReader) {
	return VFW_E_ALREADY_CONNECTED;
    }

    HRESULT hr = pUnk->QueryInterface(IID_IAsyncReader, (void**)&m_pReader);
    if (FAILED(hr)) {

	return(hr);
    }

    hr = DecideAllocator(pAlloc, NULL);
    if (FAILED(hr)) {
	Disconnect();

	return hr;
    }

    LONGLONG llTotal, llAvail;
    hr = m_pReader->Length(&llTotal, &llAvail);
    if (FAILED(hr)) {
	Disconnect();

	return hr;
    }

    
    m_tDuration = llTotal * UNITS;
    m_tStop = m_tDuration;
    m_tStart = 0;

    m_bSync = bSync;

    return S_OK;
}

HRESULT
CPullPin::Disconnect()
{
    CAutoLock lock(&m_AccessLock);

    StopThread();

    if (m_pReader) {
	m_pReader->Release();
	m_pReader = NULL;
    }

    if (m_pAlloc) {
	m_pAlloc->Release();
	m_pAlloc = NULL;
    }

    return S_OK;
}

HRESULT
CPullPin::DecideAllocator(
    IMemAllocator * pAlloc,
    ALLOCATOR_PROPERTIES * pProps)
{
    ALLOCATOR_PROPERTIES *pRequest;
    ALLOCATOR_PROPERTIES Request;
    if (pProps == NULL) {
	Request.cBuffers = 3;
	Request.cbBuffer = 64*1024;
	Request.cbAlign = 0;
	Request.cbPrefix = 0;
	pRequest = &Request;
    } else {
	pRequest = pProps;
    }
    HRESULT hr = m_pReader->RequestAllocator(
		    pAlloc,
		    pRequest,
		    &m_pAlloc);
    return hr;
}

HRESULT
CPullPin::Active(void)
{
    ASSERT(!ThreadExists());
    return StartThread();
}

HRESULT
CPullPin::Inactive(void)
{
    StopThread();

    return S_OK;
}

HRESULT
CPullPin::Seek(REFERENCE_TIME tStart, REFERENCE_TIME tStop)
{
    CAutoLock lock(&m_AccessLock);

    ThreadMsg AtStart = m_State;

    if (AtStart == TM_Start) {
	BeginFlush();
	PauseThread();
	EndFlush();
    }

    m_tStart = tStart;
    m_tStop = tStop;

    HRESULT hr = S_OK;
    if (AtStart == TM_Start) {
	hr = StartThread();
    }

    return hr;
}

HRESULT
CPullPin::Duration(REFERENCE_TIME* ptDuration)
{
    *ptDuration = m_tDuration;
    return S_OK;
}

HRESULT
CPullPin::StartThread()
{
    CAutoLock lock(&m_AccessLock);

    if (!m_pAlloc || !m_pReader) {
	return E_UNEXPECTED;
    }

    HRESULT hr;
    if (!ThreadExists()) {

	
	hr = m_pAlloc->Commit();
	if (FAILED(hr)) {
	    return hr;
	}

	
	if (!Create()) {
	    return E_FAIL;
	}
    }

    m_State = TM_Start;
    hr = (HRESULT) CallWorker(m_State);
    return hr;
}

HRESULT
CPullPin::PauseThread()
{
    CAutoLock lock(&m_AccessLock);

    if (!ThreadExists()) {
	return E_UNEXPECTED;
    }

    
    
    HRESULT hr = m_pReader->BeginFlush();
    if (FAILED(hr)) {
	return hr;
    }

    m_State = TM_Pause;
    hr = CallWorker(TM_Pause);

    m_pReader->EndFlush();
    return hr;
}

HRESULT
CPullPin::StopThread()
{
    CAutoLock lock(&m_AccessLock);

    if (!ThreadExists()) {
	return S_FALSE;
    }

    
    
    HRESULT hr = m_pReader->BeginFlush();
    if (FAILED(hr)) {
	return hr;
    }

    m_State = TM_Exit;
    hr = CallWorker(TM_Exit);

    m_pReader->EndFlush();

    
    Close();

    
    if (m_pAlloc) {
	m_pAlloc->Decommit();
    }

    return S_OK;
}

DWORD
CPullPin::ThreadProc(void)
{
    while(1) {
	DWORD cmd = GetRequest();
	switch(cmd) {
	case TM_Exit:
	    Reply(S_OK);
	    return 0;

	case TM_Pause:
	    
	    Reply(S_OK);
	    break;

	case TM_Start:
	    Reply(S_OK);
	    Process();
	    break;
	}

	
	
	
	
	
	
	m_pReader->BeginFlush();
	CleanupCancelled();
	m_pReader->EndFlush();
    }
}

HRESULT
CPullPin::QueueSample(
    REFERENCE_TIME& tCurrent,
    REFERENCE_TIME tAlignStop,
    BOOL bDiscontinuity
    )
{
    IMediaSample* pSample;

    HRESULT hr = m_pAlloc->GetBuffer(&pSample, NULL, NULL, 0);
    if (FAILED(hr)) {
	return hr;
    }

    LONGLONG tStopThis = tCurrent + (pSample->GetSize() * UNITS);
    if (tStopThis > tAlignStop) {
	tStopThis = tAlignStop;
    }
    pSample->SetTime(&tCurrent, &tStopThis);
    tCurrent = tStopThis;

    pSample->SetDiscontinuity(bDiscontinuity);

    hr = m_pReader->Request(
			pSample,
			0);
    if (FAILED(hr)) {
	pSample->Release();

	CleanupCancelled();
	OnError(hr);
    }
    return hr;
}

HRESULT
CPullPin::CollectAndDeliver(
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop)
{
    IMediaSample* pSample = NULL;   
    DWORD_PTR dwUnused;
    HRESULT hr = m_pReader->WaitForNext(
			INFINITE,
			&pSample,
			&dwUnused);
    if (FAILED(hr)) {
	if (pSample) {
	    pSample->Release();
	}
    } else {
	hr = DeliverSample(pSample, tStart, tStop);
    }
    if (FAILED(hr)) {
	CleanupCancelled();
	OnError(hr);
    }
    return hr;

}

HRESULT
CPullPin::DeliverSample(
    IMediaSample* pSample,
    REFERENCE_TIME tStart,
    REFERENCE_TIME tStop
    )
{
    
    REFERENCE_TIME t1, t2;
    pSample->GetTime(&t1, &t2);
    if (t2 > tStop) {
	t2 = tStop;
    }

    
    t1 -= tStart;
    t2 -= tStart;
    pSample->SetTime(&t1, &t2);

    HRESULT hr = Receive(pSample);
    pSample->Release();
    return hr;
}

void
CPullPin::Process(void)
{
    
    if (m_tStop <= m_tStart) {
	EndOfStream();
	return;
    }

    BOOL bDiscontinuity = TRUE;

    
    
    
    ALLOCATOR_PROPERTIES Actual;
    HRESULT hr = m_pAlloc->GetProperties(&Actual);

    
    REFERENCE_TIME tStart = AlignDown(m_tStart / UNITS, Actual.cbAlign) * UNITS;
    REFERENCE_TIME tCurrent = tStart;

    REFERENCE_TIME tStop = m_tStop;
    if (tStop > m_tDuration) {
	tStop = m_tDuration;
    }

    
    
    REFERENCE_TIME tAlignStop = AlignUp(tStop / UNITS, Actual.cbAlign) * UNITS;

    DWORD dwRequest;

    if (!m_bSync) {

	
	
	while (tCurrent < tAlignStop) {

	    
	    
	    if (CheckRequest(&dwRequest)) {
		return;
	    }

	    
	    if (Actual.cBuffers > 1) {

		hr = QueueSample(tCurrent, tAlignStop, TRUE);
		bDiscontinuity = FALSE;

		if (FAILED(hr)) {
		    return;
		}
	    }

	    
	    while (tCurrent < tAlignStop) {

		hr = QueueSample(tCurrent, tAlignStop, bDiscontinuity);
		bDiscontinuity = FALSE;

		if (FAILED(hr)) {
		    return;
		}

		hr = CollectAndDeliver(tStart, tStop);
		if (S_OK != hr) {

		    
		    
		    return;
		}
	    }

	    if (Actual.cBuffers > 1) {
		hr = CollectAndDeliver(tStart, tStop);
		if (FAILED(hr)) {
		    return;
		}
	    }
	}
    } else {

	
	while (tCurrent < tAlignStop) {

	    
	    
	    if (CheckRequest(&dwRequest)) {
		return;
	    }

	    IMediaSample* pSample;

	    hr = m_pAlloc->GetBuffer(&pSample, NULL, NULL, 0);
	    if (FAILED(hr)) {
		OnError(hr);
		return;
	    }

	    LONGLONG tStopThis = tCurrent + (pSample->GetSize() * UNITS);
	    if (tStopThis > tAlignStop) {
		tStopThis = tAlignStop;
	    }
	    pSample->SetTime(&tCurrent, &tStopThis);
	    tCurrent = tStopThis;

	    if (bDiscontinuity) {
		pSample->SetDiscontinuity(TRUE);
		bDiscontinuity = FALSE;
	    }

	    hr = m_pReader->SyncReadAligned(pSample);

	    if (FAILED(hr)) {
		pSample->Release();
		OnError(hr);
		return;
	    }

	    hr = DeliverSample(pSample, tStart, tStop);
	    if (hr != S_OK) {
		if (FAILED(hr)) {
		    OnError(hr);
		}
		return;
	    }
	}
    }

    EndOfStream();
}

void
CPullPin::CleanupCancelled(void)
{
    while (1) {
	IMediaSample * pSample;
	DWORD_PTR dwUnused;

	HRESULT hr = m_pReader->WaitForNext(
			    0,          
			    &pSample,
			    &dwUnused);
	if(pSample) {
	    pSample->Release();
	} else {
	    
	    return;
	}
    }
}
