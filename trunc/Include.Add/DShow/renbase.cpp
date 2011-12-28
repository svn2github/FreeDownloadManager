/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#include <streams.h>        
#include <mmsystem.h>       
#include <limits.h>         
#include <measure.h>        

#pragma warning(disable:4355)

int inline TimeDiff(REFERENCE_TIME rt)
{
    if (rt < - (50 * UNITS)) {
        return -(50 * UNITS);
    } else
    if (rt > 50 * UNITS) {
        return 50 * UNITS;
    } else return (int)rt;
}

CBaseRenderer::CBaseRenderer(REFCLSID RenderClass, 
			     TCHAR *pName,         
			     LPUNKNOWN pUnk,       
			     HRESULT *phr) :       

    CBaseFilter(pName,pUnk,&m_InterfaceLock,RenderClass),
    m_evComplete(TRUE),
    m_bAbort(FALSE),
    m_pPosition(NULL),
    m_ThreadSignal(TRUE),
    m_bStreaming(FALSE),
    m_bEOS(FALSE),
    m_bEOSDelivered(FALSE),
    m_pMediaSample(NULL),
    m_dwAdvise(0),
    m_pQSink(NULL),
    m_pInputPin(NULL),
    m_bRepaintStatus(TRUE),
    m_SignalTime(0),
    m_bInReceive(FALSE),
    m_EndOfStreamTimer(0)
{
    Ready();
#ifdef PERF
    m_idBaseStamp = MSR_REGISTER("BaseRenderer: sample time stamp");
    m_idBaseRenderTime = MSR_REGISTER("BaseRenderer: draw time (msec)");
    m_idBaseAccuracy = MSR_REGISTER("BaseRenderer: Accuracy (msec)");
#endif
}

CBaseRenderer::~CBaseRenderer()
{
    ASSERT(m_bStreaming == FALSE);
    ASSERT(m_EndOfStreamTimer == 0);
    StopStreaming();
    ClearPendingSample();

    

    if (m_pPosition) {
	delete m_pPosition;
	m_pPosition = NULL;
    }

    

    if (m_pInputPin) {
	delete m_pInputPin;
	m_pInputPin = NULL;
    }

    

    ASSERT(m_pQSink == NULL);
}

HRESULT CBaseRenderer::GetMediaPositionInterface(REFIID riid,void **ppv)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    if (m_pPosition) {
	return m_pPosition->NonDelegatingQueryInterface(riid,ppv);
    }

    HRESULT hr = NOERROR;

    
    
    
    

    m_pPosition = new CRendererPosPassThru(NAME("Renderer CPosPassThru"),
					   CBaseFilter::GetOwner(),
					   (HRESULT *) &hr,
					   GetPin(0));
    if (m_pPosition == NULL) {
	return E_OUTOFMEMORY;
    }

    if (FAILED(hr)) {
	delete m_pPosition;
	m_pPosition = NULL;
	return E_NOINTERFACE;
    }
    return GetMediaPositionInterface(riid,ppv);
}

STDMETHODIMP CBaseRenderer::NonDelegatingQueryInterface(REFIID riid,void **ppv)
{
    

    if (riid == IID_IMediaPosition || riid == IID_IMediaSeeking) {
	return GetMediaPositionInterface(riid,ppv);
    } else {
	return CBaseFilter::NonDelegatingQueryInterface(riid,ppv);
    }
}

HRESULT CBaseRenderer::SourceThreadCanWait(BOOL bCanWait)
{
    if (bCanWait == TRUE) {
	m_ThreadSignal.Reset();
    } else {
	m_ThreadSignal.Set();
    }
    return NOERROR;
}

#ifdef DEBUG

void CBaseRenderer::DisplayRendererState()
{
    DbgLog((LOG_TIMING, 1, TEXT("\nTimed out in WaitForRenderTime")));

    

    BOOL bSignalled = m_ThreadSignal.Check();
    DbgLog((LOG_TIMING, 1, TEXT("Signal sanity check %d"),bSignalled));

    

    DbgLog((LOG_TIMING, 1, TEXT("Filter state %d"),m_State));

    DbgLog((LOG_TIMING, 1, TEXT("Abort flag %d"),m_bAbort));

    DbgLog((LOG_TIMING, 1, TEXT("Streaming flag %d"),m_bStreaming));

    DbgLog((LOG_TIMING, 1, TEXT("Clock advise link %d"),m_dwAdvise));

    DbgLog((LOG_TIMING, 1, TEXT("Current media sample %x"),m_pMediaSample));

    DbgLog((LOG_TIMING, 1, TEXT("EOS signalled %d"),m_bEOS));

    DbgLog((LOG_TIMING, 1, TEXT("EOS delivered %d"),m_bEOSDelivered));

    DbgLog((LOG_TIMING, 1, TEXT("Repaint status %d"),m_bRepaintStatus));

    

    DbgLog((LOG_TIMING, 1, TEXT("End of stream timer %x"),m_EndOfStreamTimer));

    DbgLog((LOG_TIMING, 1, TEXT("Deliver time %s"),CDisp((LONGLONG)m_SignalTime)));

    

    BOOL bFlushing = m_pInputPin->IsFlushing();
    DbgLog((LOG_TIMING, 1, TEXT("Flushing sanity check %d"),bFlushing));

    
    DbgLog((LOG_TIMING, 1, TEXT("Last run time %s"),CDisp((LONGLONG)m_tStart.m_time)));

    
    if (m_pClock == NULL) return;

    

    CRefTime CurrentTime,StartTime,EndTime;
    m_pClock->GetTime((REFERENCE_TIME*) &CurrentTime);
    CRefTime Offset = CurrentTime - m_tStart;

    

    DbgLog((LOG_TIMING, 1, TEXT("Clock time %s"),CDisp((LONGLONG)CurrentTime.m_time)));

    DbgLog((LOG_TIMING, 1, TEXT("Time difference %dms"),Offset.Millisecs()));

    
    if (m_pMediaSample == NULL) return;

    m_pMediaSample->GetTime((REFERENCE_TIME*)&StartTime, (REFERENCE_TIME*)&EndTime);
    DbgLog((LOG_TIMING, 1, TEXT("Next sample stream times (Start %d End %d ms)"),
	   StartTime.Millisecs(),EndTime.Millisecs()));

    
    CRefTime Wait = (m_tStart + StartTime) - CurrentTime;
    DbgLog((LOG_TIMING, 1, TEXT("Wait required %d ms"),Wait.Millisecs()));
}
#endif

#define RENDER_TIMEOUT 10000

HRESULT CBaseRenderer::WaitForRenderTime()
{
    HANDLE WaitObjects[] = { m_ThreadSignal, m_RenderEvent };
    DWORD Result = WAIT_TIMEOUT;

    

    OnWaitStart();
    while (Result == WAIT_TIMEOUT) {
	Result = WaitForMultipleObjects(2,WaitObjects,FALSE,RENDER_TIMEOUT);

#ifdef DEBUG
	if (Result == WAIT_TIMEOUT) DisplayRendererState();
#endif

    }
    OnWaitEnd();

    

    if (Result == WAIT_OBJECT_0) {
	return VFW_E_STATE_CHANGED;
    }

    SignalTimerFired();
    return NOERROR;
}

void CBaseRenderer::WaitForReceiveToComplete()
{
    for (;;) {
	if (!m_bInReceive) {
	    break;
	}

	MSG msg;
	
	PeekMessage(&msg, NULL, WM_NULL, WM_NULL, PM_NOREMOVE);

	Sleep(1);
    }

    
    
    
    
    if (HIWORD(GetQueueStatus(QS_POSTMESSAGE)) & QS_POSTMESSAGE) {
	
	PostThreadMessage(GetCurrentThreadId(), WM_NULL, 0, 0);
    }
}

FILTER_STATE CBaseRenderer::GetRealState() {
    return m_State;
}

STDMETHODIMP CBaseRenderer::GetState(DWORD dwMSecs,FILTER_STATE *State)
{
    CheckPointer(State,E_POINTER);

    if (WaitDispatchingMessages(m_evComplete, dwMSecs) == WAIT_TIMEOUT) {
	*State = m_State;
	return VFW_S_STATE_INTERMEDIATE;
    }
    *State = m_State;
    return NOERROR;
}

HRESULT CBaseRenderer::CompleteStateChange(FILTER_STATE OldState)
{
    

    if (m_pInputPin->IsConnected() == FALSE) {
	Ready();
	return S_OK;
    }

    

    if (IsEndOfStream() == TRUE) {
	Ready();
	return S_OK;
    }

    

    if (HaveCurrentSample() == TRUE) {
	if (OldState != State_Stopped) {
	    Ready();
	    return S_OK;
	}
    }
    NotReady();
    return S_FALSE;
}

STDMETHODIMP CBaseRenderer::Stop()
{
    CAutoLock cRendererLock(&m_InterfaceLock);

    

    if (m_State == State_Stopped) {
	return NOERROR;
    }

    

    if (m_pInputPin->IsConnected() == FALSE) {
	NOTE("Input pin is not connected");
	m_State = State_Stopped;
	return NOERROR;
    }

    CBaseFilter::Stop();

    
    
    

    if (m_pInputPin->Allocator()) {
	m_pInputPin->Allocator()->Decommit();
    }

    

    SetRepaintStatus(TRUE);
    StopStreaming();
    SourceThreadCanWait(FALSE);
    ResetEndOfStream();
    CancelNotification();

    
    ASSERT(CancelNotification() == S_FALSE);
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));
    ASSERT(m_EndOfStreamTimer == 0);

    Ready();
    WaitForReceiveToComplete();
    m_bAbort = FALSE;
    return NOERROR;
}

STDMETHODIMP CBaseRenderer::Pause()
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    FILTER_STATE OldState = m_State;
    ASSERT(m_pInputPin->IsFlushing() == FALSE);

    

    if (m_State == State_Paused) {
	return CompleteStateChange(State_Paused);
    }

    

    if (m_pInputPin->IsConnected() == FALSE) {
	NOTE("Input pin is not connected");
	m_State = State_Paused;
	return CompleteStateChange(State_Paused);
    }

    

    HRESULT hr = CBaseFilter::Pause();
    if (FAILED(hr)) {
	NOTE("Pause failed");
	return hr;
    }

    

    SetRepaintStatus(TRUE);
    StopStreaming();
    SourceThreadCanWait(TRUE);
    CancelNotification();
    ResetEndOfStreamTimer();

    
    
    

    if (m_pInputPin->Allocator()) {
	m_pInputPin->Allocator()->Commit();
    }

    
    ASSERT(CancelNotification() == S_FALSE);
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));
    ASSERT(m_EndOfStreamTimer == 0);
    ASSERT(m_pInputPin->IsFlushing() == FALSE);

    
    
    
    
    

    if (OldState == State_Stopped) {
	m_bAbort = FALSE;
	ClearPendingSample();
    }
    return CompleteStateChange(OldState);
}

STDMETHODIMP CBaseRenderer::Run(REFERENCE_TIME StartTime)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    FILTER_STATE OldState = m_State;

    

    if (m_State == State_Running) {
	return NOERROR;
    }

    

    if (m_pInputPin->IsConnected() == FALSE) {
	NotifyEvent(EC_COMPLETE,S_OK,(LONG_PTR)(IBaseFilter *)this);
	m_State = State_Running;
	return NOERROR;
    }

    Ready();

    

    HRESULT hr = CBaseFilter::Run(StartTime);
    if (FAILED(hr)) {
	NOTE("Run failed");
	return hr;
    }

    
    ASSERT(m_pInputPin->IsFlushing() == FALSE);
    SourceThreadCanWait(TRUE);
    SetRepaintStatus(FALSE);

    
    ASSERT(CancelNotification() == S_FALSE);
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));
    ASSERT(m_EndOfStreamTimer == 0);
    ASSERT(m_pInputPin->IsFlushing() == FALSE);

    
    
    

    if (m_pInputPin->Allocator()) {
	m_pInputPin->Allocator()->Commit();
    }

    
    
    
    
    

    if (OldState == State_Stopped) {
	m_bAbort = FALSE;
	ClearPendingSample();
    }
    return StartStreaming();
}

int CBaseRenderer::GetPinCount()
{
    return 1;
}

CBasePin *CBaseRenderer::GetPin(int n)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    HRESULT hr = NOERROR;
    ASSERT(n == 0);

    

    if (n != 0) {
	return NULL;
    }

    

    if (m_pInputPin == NULL) {
	m_pInputPin = new CRendererInputPin(this,&hr,L"In");
    }
    return m_pInputPin;
}

STDMETHODIMP CBaseRenderer::FindPin(LPCWSTR Id, IPin **ppPin)
{
    CheckPointer(ppPin,E_POINTER);

    if (0==lstrcmpW(Id,L"In")) {
	*ppPin = GetPin(0);
	ASSERT(*ppPin);
	(*ppPin)->AddRef();
    } else {
	*ppPin = NULL;
	return VFW_E_NOT_FOUND;
    }
    return NOERROR;
}

HRESULT CBaseRenderer::EndOfStream()
{
    

    if (m_State == State_Stopped) {
	return NOERROR;
    }

    

    m_bEOS = TRUE;
    if (m_pMediaSample) {
	return NOERROR;
    }

    
    
    

    Ready();

    
    
    

    if (m_bStreaming) {
	SendEndOfStream();
    }
    return NOERROR;
}

HRESULT CBaseRenderer::BeginFlush()
{
    

    if (m_State == State_Paused) {
	NotReady();
    }

    SourceThreadCanWait(FALSE);
    CancelNotification();
    ClearPendingSample();
    
    WaitForReceiveToComplete();
    return NOERROR;
}

HRESULT CBaseRenderer::EndFlush()
{
    
    if (m_pPosition) m_pPosition->ResetMediaTime();

    

    ASSERT(CancelNotification() == S_FALSE);
    SourceThreadCanWait(TRUE);
    return NOERROR;
}

HRESULT CBaseRenderer::CompleteConnect(IPin *pReceivePin)
{
    SetRepaintStatus(TRUE);
    m_bAbort = FALSE;
    return NOERROR;
}

HRESULT CBaseRenderer::Active()
{
    return NOERROR;
}

HRESULT CBaseRenderer::Inactive()
{
    if (m_pPosition) {
	m_pPosition->ResetMediaTime();
    }
    
    
    ClearPendingSample();
    return NOERROR;
}

HRESULT CBaseRenderer::SetMediaType(const CMediaType *pmt)
{
    return NOERROR;
}

HRESULT CBaseRenderer::BreakConnect()
{
    

    if (m_pQSink) {
	    m_pQSink->Release();
	    m_pQSink = NULL;
    }

    

    if (m_pInputPin->IsConnected() == FALSE) {
	    return S_FALSE;
    }

    
    if (m_State != State_Stopped && !m_pInputPin->CanReconnectWhenActive()) {
	    return VFW_E_NOT_STOPPED;
    }

    SetRepaintStatus(FALSE);
    ResetEndOfStream();
    ClearPendingSample();
    m_bAbort = FALSE;
    return NOERROR;
}

HRESULT CBaseRenderer::GetSampleTimes(IMediaSample *pMediaSample,
				      REFERENCE_TIME *pStartTime,
				      REFERENCE_TIME *pEndTime)
{
    ASSERT(m_dwAdvise == 0);
    ASSERT(pMediaSample);

    
    
    

    if (SUCCEEDED(pMediaSample->GetTime(pStartTime, pEndTime))) {
	if (*pEndTime < *pStartTime) {
	    return VFW_E_START_TIME_AFTER_END;
	}
    } else {
	
	return S_OK;
    }

    
    
    

    if (m_pClock == NULL) {
	return S_OK;
    }
    return ShouldDrawSampleNow(pMediaSample,pStartTime,pEndTime);
}

HRESULT CBaseRenderer::ShouldDrawSampleNow(IMediaSample *pMediaSample,
					   REFERENCE_TIME *ptrStart,
					   REFERENCE_TIME *ptrEnd)
{
    return S_FALSE;
}

void CBaseRenderer::SignalTimerFired()
{
    m_dwAdvise = 0;
}

HRESULT CBaseRenderer::CancelNotification()
{
    ASSERT(m_dwAdvise == 0 || m_pClock);
    DWORD_PTR dwAdvise = m_dwAdvise;

    

    if (m_dwAdvise) {
	m_pClock->Unadvise(m_dwAdvise);
	SignalTimerFired();
	ASSERT(m_dwAdvise == 0);
    }

    

    m_RenderEvent.Reset();
    return (dwAdvise ? S_OK : S_FALSE);
}

BOOL CBaseRenderer::ScheduleSample(IMediaSample *pMediaSample)
{
    REFERENCE_TIME StartSample, EndSample;

    

    if (pMediaSample == NULL) {
	return FALSE;
    }

    
    
    
    

    HRESULT hr = GetSampleTimes(pMediaSample, &StartSample, &EndSample);
    if (FAILED(hr)) {
	return FALSE;
    }

    
    
    

    if (hr == S_OK) {
	EXECUTE_ASSERT(SetEvent((HANDLE) m_RenderEvent));
	return TRUE;
    }

    ASSERT(m_dwAdvise == 0);
    ASSERT(m_pClock);
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));

    
    
    
    

    hr = m_pClock->AdviseTime(
	    (REFERENCE_TIME) m_tStart,          
	    StartSample,                        
	    (HEVENT)(HANDLE) m_RenderEvent,     
	    &m_dwAdvise);                       

    if (SUCCEEDED(hr)) {
	return TRUE;
    }

    
    
    

    ASSERT(m_dwAdvise == 0);
    return FALSE;
}

HRESULT CBaseRenderer::Render(IMediaSample *pMediaSample)
{
    
    
    

    if (pMediaSample == NULL) {
	return S_FALSE;
    }

    
    
    

    if (m_bStreaming == FALSE) {
	return S_FALSE;
    }

    

    OnRenderStart(pMediaSample);
    DoRenderSample(pMediaSample);
    OnRenderEnd(pMediaSample);

    return NOERROR;
}

BOOL CBaseRenderer::HaveCurrentSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    return (m_pMediaSample == NULL ? FALSE : TRUE);
}

IMediaSample *CBaseRenderer::GetCurrentSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_pMediaSample) {
	m_pMediaSample->AddRef();
    }
    return m_pMediaSample;
}

HRESULT CBaseRenderer::PrepareReceive(IMediaSample *pMediaSample)
{
    CAutoLock cRendererLock(&m_InterfaceLock);
    m_bInReceive = TRUE;

    

    HRESULT hr = m_pInputPin->CBaseInputPin::Receive(pMediaSample);

    if (hr != NOERROR) {
	m_bInReceive = FALSE;
	return E_FAIL;
    }

    
    
    
    
    
    
    if (m_pInputPin->SampleProps()->pMediaType) {
	m_pInputPin->SetMediaType(
	    (CMediaType *)m_pInputPin->SampleProps()->pMediaType);
    }

    CAutoLock cSampleLock(&m_RendererLock);

    ASSERT(IsActive() == TRUE);
    ASSERT(m_pInputPin->IsFlushing() == FALSE);
    ASSERT(m_pInputPin->IsConnected() == TRUE);
    ASSERT(m_pMediaSample == NULL);

    
    
    

    if (m_pMediaSample || m_bEOS || m_bAbort) {
	Ready();
	m_bInReceive = FALSE;
	return E_UNEXPECTED;
    }

    
    if (m_pPosition) m_pPosition->RegisterMediaTime(pMediaSample);

    

    if ((m_bStreaming == TRUE) && (ScheduleSample(pMediaSample) == FALSE)) {
	ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));
	ASSERT(CancelNotification() == S_FALSE);
	m_bInReceive = FALSE;
	return VFW_E_SAMPLE_REJECTED;
    }

    
    m_SignalTime = m_pInputPin->SampleProps()->tStop;

    
    
    
    
    

    m_pMediaSample = pMediaSample;
    m_pMediaSample->AddRef();

    if (m_bStreaming == FALSE) {
	SetRepaintStatus(TRUE);
    }
    return NOERROR;
}

HRESULT CBaseRenderer::Receive(IMediaSample *pSample)
{
    ASSERT(pSample);

    

    HRESULT hr = PrepareReceive(pSample);
    ASSERT(m_bInReceive == SUCCEEDED(hr));
    if (FAILED(hr)) {
	if (hr == VFW_E_SAMPLE_REJECTED) {
	    return NOERROR;
	}
	return hr;
    }

    
    
    if (m_State == State_Paused) {
	PrepareRender();
	
	m_bInReceive = FALSE;
	{
	    
	    CAutoLock cRendererLock(&m_InterfaceLock);
	    if (m_State == State_Stopped)
		return NOERROR;
	    m_bInReceive = TRUE;
	    CAutoLock cSampleLock(&m_RendererLock);
	    OnReceiveFirstSample(pSample);
	}
	Ready();
    }
    
    
    

    hr = WaitForRenderTime();
    if (FAILED(hr)) {
	m_bInReceive = FALSE;
	return NOERROR;
    }

    PrepareRender();

    
    
    
    
    m_bInReceive = FALSE;

    
    CAutoLock cRendererLock(&m_InterfaceLock);

    
    
    if (m_State == State_Stopped)
	return NOERROR;

    CAutoLock cSampleLock(&m_RendererLock);

    

    Render(m_pMediaSample);
    ClearPendingSample();
    SendEndOfStream();
    CancelNotification();
    return NOERROR;
}

HRESULT CBaseRenderer::ClearPendingSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_pMediaSample) {
	m_pMediaSample->Release();
	m_pMediaSample = NULL;
    }
    return NOERROR;
}

void CALLBACK EndOfStreamTimer(UINT uID,        
			       UINT uMsg,       
			       DWORD_PTR dwUser,
			       DWORD_PTR dw1,   
			       DWORD_PTR dw2)   
{
    CBaseRenderer *pRenderer = (CBaseRenderer *) dwUser;
    NOTE1("EndOfStreamTimer called (%d)",uID);
    pRenderer->TimerCallback();
}

void CBaseRenderer::TimerCallback()
{
    
    
    CAutoLock cRendererLock(&m_RendererLock);

    

    if (m_EndOfStreamTimer) {
	m_EndOfStreamTimer = 0;
	SendEndOfStream();
    }
}

#define TIMEOUT_DELIVERYWAIT 50
#define TIMEOUT_RESOLUTION 10

HRESULT CBaseRenderer::SendEndOfStream()
{
    ASSERT(CritCheckIn(&m_RendererLock));
    if (m_bEOS == FALSE || m_bEOSDelivered || m_EndOfStreamTimer) {
	return NOERROR;
    }

    
    if (m_pClock == NULL) {
	return NotifyEndOfStream();
    }

    

    REFERENCE_TIME Signal = m_tStart + m_SignalTime;
    REFERENCE_TIME CurrentTime;
    m_pClock->GetTime(&CurrentTime);
    LONG Delay = LONG((Signal - CurrentTime) / 10000);

    

    NOTE1("Delay until end of stream delivery %d",Delay);
    NOTE1("Current %s",(LPCTSTR)CDisp((LONGLONG)CurrentTime));
    NOTE1("Signal %s",(LPCTSTR)CDisp((LONGLONG)Signal));

    

    if (Delay < TIMEOUT_DELIVERYWAIT) {
	return NotifyEndOfStream();
    }

    

    m_EndOfStreamTimer = timeSetEvent((UINT) Delay,       
				      TIMEOUT_RESOLUTION, 
				      EndOfStreamTimer,   
				      DWORD_PTR(this),    
				      TIME_ONESHOT);      
    if (m_EndOfStreamTimer == 0) {
	return NotifyEndOfStream();
    }
    return NOERROR;
}

HRESULT CBaseRenderer::NotifyEndOfStream()
{
    CAutoLock cRendererLock(&m_RendererLock);
    ASSERT(m_bEOS == TRUE);
    ASSERT(m_bEOSDelivered == FALSE);
    ASSERT(m_EndOfStreamTimer == 0);

    

    if (m_bStreaming == FALSE) {
	ASSERT(m_EndOfStreamTimer == 0);
	return NOERROR;
    }

    
    m_EndOfStreamTimer = 0;

    
    
    
    

    if (m_pPosition) m_pPosition->EOS();
    m_bEOSDelivered = TRUE;
    NOTE("Sending EC_COMPLETE...");
    return NotifyEvent(EC_COMPLETE,S_OK,(LONG_PTR)(IBaseFilter *)this);
}

HRESULT CBaseRenderer::ResetEndOfStream()
{
    ResetEndOfStreamTimer();
    CAutoLock cRendererLock(&m_RendererLock);

    m_bEOS = FALSE;
    m_bEOSDelivered = FALSE;
    m_SignalTime = 0;

    return NOERROR;
}

void CBaseRenderer::ResetEndOfStreamTimer()
{
    ASSERT(CritCheckOut(&m_RendererLock));
    if (m_EndOfStreamTimer) {
	timeKillEvent(m_EndOfStreamTimer);
	m_EndOfStreamTimer = 0;
    }
}

HRESULT CBaseRenderer::StartStreaming()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_bStreaming == TRUE) {
	return NOERROR;
    }

    

    m_bStreaming = TRUE;
    timeBeginPeriod(1);
    OnStartStreaming();

    
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject((HANDLE)m_RenderEvent,0));
    ASSERT(CancelNotification() == S_FALSE);

    

    if (m_pMediaSample == NULL) {
	return SendEndOfStream();
    }

    

    ASSERT(m_pMediaSample);
    if (!ScheduleSample(m_pMediaSample))
	m_RenderEvent.Set();
    
    return NOERROR;
}

HRESULT CBaseRenderer::StopStreaming()
{
    CAutoLock cRendererLock(&m_RendererLock);
    m_bEOSDelivered = FALSE;

    if (m_bStreaming == TRUE) {
	m_bStreaming = FALSE;
	OnStopStreaming();
	timeEndPeriod(1);
    }
    return NOERROR;
}

void CBaseRenderer::SetRepaintStatus(BOOL bRepaint)
{
    CAutoLock cSampleLock(&m_RendererLock);
    m_bRepaintStatus = bRepaint;
}

void CBaseRenderer::SendNotifyWindow(IPin *pPin,HWND hwnd)
{
    IMediaEventSink *pSink;

    
    HRESULT hr = pPin->QueryInterface(IID_IMediaEventSink,(void **)&pSink);
    if (SUCCEEDED(hr)) {
	pSink->Notify(EC_NOTIFY_WINDOW,LONG_PTR(hwnd),0);
	pSink->Release();
    }
    NotifyEvent(EC_NOTIFY_WINDOW,LONG_PTR(hwnd),0);
}

#define RLOG(_x_) DbgLog((LOG_TRACE,1,TEXT(_x_)));

void CBaseRenderer::SendRepaint()
{
    CAutoLock cSampleLock(&m_RendererLock);
    ASSERT(m_pInputPin);

    
    
    
    
    
    

    if (m_bAbort == FALSE) {
	if (m_pInputPin->IsConnected() == TRUE) {
	    if (m_pInputPin->IsFlushing() == FALSE) {
		if (IsEndOfStream() == FALSE) {
		    if (m_bRepaintStatus == TRUE) {
			IPin *pPin = (IPin *) m_pInputPin;
			NotifyEvent(EC_REPAINT,(LONG_PTR) pPin,0);
			SetRepaintStatus(FALSE);
			RLOG("Sending repaint");
		    }
		}
	    }
	}
    }
}

BOOL CBaseRenderer::OnDisplayChange()
{
    

    CAutoLock cSampleLock(&m_RendererLock);
    if (m_pInputPin->IsConnected() == FALSE) {
	return FALSE;
    }

    RLOG("Notification of EC_DISPLAY_CHANGE");

    

    IPin *pPin = (IPin *) m_pInputPin;
    m_pInputPin->AddRef();
    NotifyEvent(EC_DISPLAY_CHANGED,(LONG_PTR) pPin,0);
    SetAbortSignal(TRUE);
    ClearPendingSample();
    m_pInputPin->Release();

    return TRUE;
}

void CBaseRenderer::OnRenderStart(IMediaSample *pMediaSample)
{
#ifdef PERF
    REFERENCE_TIME trStart, trEnd;
    pMediaSample->GetTime(&trStart, &trEnd);

    MSR_INTEGER(m_idBaseStamp, (int)trStart);     

    m_pClock->GetTime(&m_trRenderStart);
    MSR_INTEGER(0, (int)m_trRenderStart);
    REFERENCE_TIME trStream;
    trStream = m_trRenderStart-m_tStart;     
    MSR_INTEGER(0,(int)trStream);

    const int trLate = (int)(trStream - trStart);
    MSR_INTEGER(m_idBaseAccuracy, trLate/10000);  
#endif

} 

void CBaseRenderer::OnRenderEnd(IMediaSample *pMediaSample)
{
#ifdef PERF
    REFERENCE_TIME trNow;
    m_pClock->GetTime(&trNow);
    MSR_INTEGER(0,(int)trNow);
    int t = (int)((trNow - m_trRenderStart)/10000);   
    MSR_INTEGER(m_idBaseRenderTime, t);
#endif
} 

CRendererInputPin::CRendererInputPin(CBaseRenderer *pRenderer,
				     HRESULT *phr,
				     LPCWSTR pPinName) :
    CBaseInputPin(NAME("Renderer pin"),
		  pRenderer,
		  &pRenderer->m_InterfaceLock,
		  (HRESULT *) phr,
		  pPinName)
{
    m_pRenderer = pRenderer;
    ASSERT(m_pRenderer);
}

STDMETHODIMP CRendererInputPin::EndOfStream()
{
    CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
    CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);

    

    HRESULT hr = CheckStreaming();
    if (hr != NOERROR) {
	return hr;
    }

    

    hr = m_pRenderer->EndOfStream();
    if (SUCCEEDED(hr)) {
	hr = CBaseInputPin::EndOfStream();
    }
    return hr;
}

STDMETHODIMP CRendererInputPin::BeginFlush()
{
    CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
    {
	CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);
	CBaseInputPin::BeginFlush();
	m_pRenderer->BeginFlush();
    }
    return m_pRenderer->ResetEndOfStream();
}

STDMETHODIMP CRendererInputPin::EndFlush()
{
    CAutoLock cRendererLock(&m_pRenderer->m_InterfaceLock);
    CAutoLock cSampleLock(&m_pRenderer->m_RendererLock);

    HRESULT hr = m_pRenderer->EndFlush();
    if (SUCCEEDED(hr)) {
	hr = CBaseInputPin::EndFlush();
    }
    return hr;
}

STDMETHODIMP CRendererInputPin::Receive(IMediaSample *pSample)
{
    return m_pRenderer->Receive(pSample);
}

HRESULT CRendererInputPin::BreakConnect()
{
    HRESULT hr = m_pRenderer->BreakConnect();
    if (FAILED(hr)) {
	return hr;
    }
    return CBaseInputPin::BreakConnect();
}

HRESULT CRendererInputPin::CompleteConnect(IPin *pReceivePin)
{
    HRESULT hr = m_pRenderer->CompleteConnect(pReceivePin);
    if (FAILED(hr)) {
	return hr;
    }
    return CBaseInputPin::CompleteConnect(pReceivePin);
}

STDMETHODIMP CRendererInputPin::QueryId(LPWSTR *Id)
{
    CheckPointer(Id,E_POINTER);

    *Id = (LPWSTR)CoTaskMemAlloc(8);
    if (*Id == NULL) {
       return E_OUTOFMEMORY;
    }
    lstrcpyW(*Id, L"In");
    return NOERROR;
}

HRESULT CRendererInputPin::CheckMediaType(const CMediaType *pmt)
{
    return m_pRenderer->CheckMediaType(pmt);
}

HRESULT CRendererInputPin::Active()
{
    return m_pRenderer->Active();
}

HRESULT CRendererInputPin::Inactive()
{
    return m_pRenderer->Inactive();
}

HRESULT CRendererInputPin::SetMediaType(const CMediaType *pmt)
{
    HRESULT hr = CBaseInputPin::SetMediaType(pmt);
    if (FAILED(hr)) {
	return hr;
    }
    return m_pRenderer->SetMediaType(pmt);
}

const TCHAR AMQUALITY[] = TEXT("ActiveMovie");
const TCHAR DRAWLATEFRAMES[] = TEXT("DrawLateFrames");

CBaseVideoRenderer::CBaseVideoRenderer(
      REFCLSID RenderClass, 
      TCHAR *pName,         
      LPUNKNOWN pUnk,       
      HRESULT *phr) :       

    CBaseRenderer(RenderClass,pName,pUnk,phr),
    m_cFramesDropped(0),
    m_cFramesDrawn(0),
    m_bSupplierHandlingQuality(FALSE)
{
    ResetStreamingTimes();

#ifdef PERF
    m_idTimeStamp       = MSR_REGISTER("Frame time stamp");
    m_idEarliness       = MSR_REGISTER("Earliness fudge");
    m_idTarget          = MSR_REGISTER("Target (mSec)");
    m_idSchLateTime     = MSR_REGISTER("mSec late when scheduled");
    m_idDecision        = MSR_REGISTER("Scheduler decision code");
    m_idQualityRate     = MSR_REGISTER("Quality rate sent");
    m_idQualityTime     = MSR_REGISTER("Quality time sent");
    m_idWaitReal        = MSR_REGISTER("Render wait");
    
    m_idFrameAccuracy   = MSR_REGISTER("Frame accuracy (msecs)");
    m_bDrawLateFrames = GetProfileInt(AMQUALITY, DRAWLATEFRAMES, FALSE);
    

    m_idRenderAvg       = MSR_REGISTER("Render draw time Avg");
    m_idFrameAvg        = MSR_REGISTER("FrameAvg");
    m_idWaitAvg         = MSR_REGISTER("WaitAvg");
    m_idDuration        = MSR_REGISTER("Duration");
    m_idThrottle        = MSR_REGISTER("Audio-video throttle wait");
    
#endif 
} 

CBaseVideoRenderer::~CBaseVideoRenderer()
{
    ASSERT(m_dwAdvise == 0);
}

HRESULT CBaseVideoRenderer::ResetStreamingTimes()
{
    m_trLastDraw = -1000;     
    m_tStreamingStart = timeGetTime();
    m_trRenderAvg = 0;
    m_trFrameAvg = -1;        
    m_trDuration = 0;         
    m_trRenderLast = 0;
    m_trWaitAvg = 0;
    m_tRenderStart = 0;
    m_cFramesDrawn = 0;
    m_cFramesDropped = 0;
    m_iTotAcc = 0;
    m_iSumSqAcc = 0;
    m_iSumSqFrameTime = 0;
    m_trFrame = 0;          
    m_trLate = 0;           
    m_iSumFrameTime = 0;
    m_nNormal = 0;
    m_trEarliness = 0;
    m_trTarget = -300000;  
    m_trThrottle = 0;
    m_trRememberStampForPerf = 0;

#ifdef PERF
    m_trRememberFrameForPerf = 0;
#endif

    return NOERROR;
} 

HRESULT CBaseVideoRenderer::OnStartStreaming()
{
    ResetStreamingTimes();
    return NOERROR;
} 

HRESULT CBaseVideoRenderer::OnStopStreaming()
{
    m_tStreamingStart = timeGetTime()-m_tStreamingStart;
    return NOERROR;
} 

void CBaseVideoRenderer::OnWaitStart()
{
    MSR_START(m_idWaitReal);
} 

void CBaseVideoRenderer::OnWaitEnd()
{
#ifdef PERF
    MSR_STOP(m_idWaitReal);
    
    

    REFERENCE_TIME trRealStream;     
#if 0
    m_pClock->GetTime(&trRealStream); 
#else
    
    
    
    REFERENCE_TIME tr = timeGetTime()*10000;
    trRealStream = tr + m_llTimeOffset;
#endif
    trRealStream -= m_tStart;     

    if (m_trRememberStampForPerf==0) {
	
	
	
	
	PreparePerformanceData(0, 0);
    } else {
	int trLate = (int)(trRealStream - m_trRememberStampForPerf);
	int trFrame = (int)(tr - m_trRememberFrameForPerf);
	PreparePerformanceData(trLate, trFrame);
    }
    m_trRememberFrameForPerf = tr;
#endif 
} 

void CBaseVideoRenderer::PreparePerformanceData(int trLate, int trFrame)
{
    m_trLate = trLate;
    m_trFrame = trFrame;
} 

void CBaseVideoRenderer::RecordFrameLateness(int trLate, int trFrame)
{
    
    int tLate = trLate/10000;

    
    
    
    
    MSR_INTEGER( m_idFrameAccuracy, tLate );

    
    
    
    if (tLate>1000 || tLate<-1000) {
	if (m_cFramesDrawn<=1) {
	    tLate = 0;
	} else if (tLate>0) {
	    tLate = 1000;
	} else {
	    tLate = -1000;
	}
    }
    
    
    if (m_cFramesDrawn>1) {
	m_iTotAcc += tLate;
	m_iSumSqAcc += (tLate*tLate);
    }

    
    
    if (m_cFramesDrawn>2) {
	int tFrame = trFrame/10000;    

	
	
	
	if (tFrame>1000||tFrame<0) tFrame = 1000;
	m_iSumSqFrameTime += tFrame*tFrame;
	ASSERT(m_iSumSqFrameTime>=0);
	m_iSumFrameTime += tFrame;
    }
    ++m_cFramesDrawn;

} 

void CBaseVideoRenderer::ThrottleWait()
{
    if (m_trThrottle>0) {
	int iThrottle = m_trThrottle/10000;    
	MSR_INTEGER( m_idThrottle, iThrottle);
        DbgLog((LOG_TRACE, 0, TEXT("Throttle %d ms"), iThrottle));
	Sleep(iThrottle);
    } else {
        Sleep(0);
    }
} 

void CBaseVideoRenderer::OnDirectRender(IMediaSample *pMediaSample)
{
    int time = 0;
    m_trRenderAvg = 0;
    m_trRenderLast = 5000000;  
			       
			       
    
    RecordFrameLateness(m_trLate, m_trFrame);
    ThrottleWait();
} 

void CBaseVideoRenderer::OnRenderStart(IMediaSample *pMediaSample)
{

    RecordFrameLateness(m_trLate, m_trFrame);
    m_tRenderStart = timeGetTime();
} 

void CBaseVideoRenderer::OnRenderEnd(IMediaSample *pMediaSample)
{

    
    
    

    int tr = (timeGetTime() - m_tRenderStart)*10000;   
    if (tr < m_trRenderAvg*2 || tr < 2 * m_trRenderLast) {
	
	m_trRenderAvg = (tr + (AVGPERIOD-1)*m_trRenderAvg)/AVGPERIOD;
    }
    m_trRenderLast = tr;
    ThrottleWait();
} 

STDMETHODIMP CBaseVideoRenderer::SetSink( IQualityControl * piqc)
{

    m_pQSink = piqc;

    return NOERROR;
} 

STDMETHODIMP CBaseVideoRenderer::Notify( IBaseFilter * pSelf, Quality q)
{
    
    
    
    
    
    

    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    
    
    
    

    if (q.Proportion>=1000) { m_trThrottle = 0; }
    else {
	
	
	m_trThrottle = -330000 + (388880000/(q.Proportion+167));
    }
    return NOERROR;
} 

HRESULT CBaseVideoRenderer::SendQuality(REFERENCE_TIME trLate,
					REFERENCE_TIME trRealStream)
{
    Quality q;
    HRESULT hr;

    
    
    
    
    
    
    
    
    
    
    

    q.TimeStamp = (REFERENCE_TIME)trRealStream;

    if (m_trFrameAvg<0) {
	q.Type = Famine;      
    }
    
    else if (m_trFrameAvg > 2*m_trRenderAvg) {
	q.Type = Famine;                        
    } else {
	q.Type = Flood;                         
    }

    q.Proportion = 1000;               

    if (m_trFrameAvg<0) {
	
    }
    else if ( trLate> 0 ) {
	
	
	

	q.Proportion = 1000 - (int)((trLate)/(UNITS/1000));
	if (q.Proportion<500) {
	   q.Proportion = 500;      
	} else {
	}

    } else if (  m_trWaitAvg>20000
	      && trLate<-20000
	      ){
	
	if (m_trWaitAvg>=m_trFrameAvg) {
	    
	    
	    
	    
	    q.Proportion = 2000;    
	} else {
	    if (m_trFrameAvg+20000 > m_trWaitAvg) {
		q.Proportion
		    = 1000 * (m_trFrameAvg / (m_trFrameAvg + 20000 - m_trWaitAvg));
	    } else {
		
		
		
		
		q.Proportion = 2000;
	    }
	}

	if (q.Proportion>2000) {
	    q.Proportion = 2000;    
	}
    }

    
    
    
    
    
    
    
    
    
    
    
    

    q.Late = trLate + m_trRenderAvg/2;

    
    MSR_INTEGER(m_idQualityRate, q.Proportion);
    MSR_INTEGER( m_idQualityTime, (int)q.Late / 10000 );

    

    if (m_pQSink==NULL) {
	
	
	

	IQualityControl *pQC = NULL;
	IPin *pOutputPin = m_pInputPin->GetConnected();
	ASSERT(pOutputPin != NULL);

	

	hr = pOutputPin->QueryInterface(IID_IQualityControl,(void**) &pQC);
	if (SUCCEEDED(hr)) {
	    m_pQSink = pQC;
	}
    }
    if (m_pQSink) {
	return m_pQSink->Notify(this,q);
    }

    return S_FALSE;

} 

HRESULT CBaseVideoRenderer::ShouldDrawSampleNow(IMediaSample *pMediaSample,
						REFERENCE_TIME *ptrStart,
						REFERENCE_TIME *ptrEnd)
{

    
    ASSERT(m_pClock);

    MSR_INTEGER(m_idTimeStamp, (int)((*ptrStart)>>32));   
    MSR_INTEGER(m_idTimeStamp, (int)(*ptrStart));         

    
    
    
    
    
    if (*ptrStart>=80000) {
	*ptrStart -= 80000;
	*ptrEnd -= 80000;       
    }

    
    
    m_trRememberStampForPerf = *ptrStart;

    
    REFERENCE_TIME trRealStream;     
    m_pClock->GetTime(&trRealStream);
#ifdef PERF
    
    
    
    
    m_llTimeOffset = trRealStream-timeGetTime()*10000;
#endif
    trRealStream -= m_tStart;     

    
    
    
    
    
    
    
    const int trTrueLate = TimeDiff(trRealStream - *ptrStart);
    const int trLate = trTrueLate;

    MSR_INTEGER(m_idSchLateTime, trTrueLate/10000);

    
    HRESULT hr = SendQuality(trLate, trRealStream);
    
    m_bSupplierHandlingQuality = (hr==S_OK);

    

    const int trDuration = (int)(*ptrEnd - *ptrStart);
    {
	
	
	
	
	
	int t = m_trDuration/32;
	if (  trDuration > m_trDuration+t
	   || trDuration < m_trDuration-t
	   ) {
	    
	    
	    
	    m_trFrameAvg = trDuration;
	    m_trDuration = trDuration;
	}
    }

    MSR_INTEGER(m_idEarliness, m_trEarliness/10000);
    MSR_INTEGER(m_idRenderAvg, m_trRenderAvg/10000);
    MSR_INTEGER(m_idFrameAvg, m_trFrameAvg/10000);
    MSR_INTEGER(m_idWaitAvg, m_trWaitAvg/10000);
    MSR_INTEGER(m_idDuration, trDuration/10000);

#ifdef PERF
    if (S_OK==pMediaSample->IsDiscontinuity()) {
	MSR_INTEGER(m_idDecision, 9000);
    }
#endif

    
    
    
    
    
    BOOL bJustDroppedFrame
	= (  m_bSupplierHandlingQuality
	  
	  
	  && (S_OK == pMediaSample->IsDiscontinuity())          
	  )
       || (m_nNormal==-1);                          

    
    if (trLate>0) {
	m_trEarliness = 0;   
    } else if (  (trLate>=m_trEarliness) || bJustDroppedFrame) {
	m_trEarliness = trLate;  
    } else {
	m_trEarliness = m_trEarliness - m_trEarliness/8;  
    }

    
    
    int trWaitAvg;
    {
	
	
	int trL = trLate<0 ? -trLate : 0;
	trWaitAvg = (trL + m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;
    }

    int trFrame;
    {
	REFERENCE_TIME tr = trRealStream - m_trLastDraw; 
	if (tr>10000000) {
	    tr = 10000000;   
	}
	trFrame = int(tr);
    }

    
    if (
	  
	  
	  (3*m_trRenderAvg <= m_trFrameAvg)

	 
	 
	 
       || ( m_bSupplierHandlingQuality
	  ? (trLate <= trDuration*4)
	  : (trLate+trLate < trDuration)
	  )

	  
	  
       || (m_trWaitAvg > 80000)

	  
	  
	  
       || ((trRealStream - m_trLastDraw) > UNITS)

    ) {
	HRESULT Result;

	
	
	
	
	

	BOOL bPlayASAP = FALSE;

	

	    
	if ( bJustDroppedFrame) {
	    bPlayASAP = TRUE;
	    MSR_INTEGER(m_idDecision, 9001);
	}

	    
	    
	    
	else if (  (m_trFrameAvg > trDuration + trDuration/16)

		   
		   
		   
		&& (trLate > - trDuration*10)
		){
	    bPlayASAP = TRUE;
	    MSR_INTEGER(m_idDecision, 9002);
	}
#if 0
	    
	else if (  (trLate + trDuration > 0)
		&& (m_trWaitAvg<=20000)
		) {
	    bPlayASAP = TRUE;
	    MSR_INTEGER(m_idDecision, 9003);
	}
#endif
	
	
	
	
	if (trLate<-9000000) {
	    bPlayASAP = FALSE;
	}

	if (bPlayASAP) {

	    m_nNormal = 0;
	    MSR_INTEGER(m_idDecision, 0);
	    
	    
	    
	    
	    
	    m_trWaitAvg = (m_trWaitAvg*(AVGPERIOD-1))/AVGPERIOD;

	    
	    m_trFrameAvg = (trFrame + m_trFrameAvg*(AVGPERIOD-1))/AVGPERIOD;
#ifndef PERF
	    
	    
	    
	    
	    
	    
	    PreparePerformanceData(trTrueLate, trFrame);
#endif
	    m_trLastDraw = trRealStream;
	    if (m_trEarliness > trLate) {
		m_trEarliness = trLate;  
	    }
	    Result = S_OK;                   

	} else {
	    ++m_nNormal;
	    
	    
	    
	    
	    
	    
	    m_trFrameAvg = trDuration;
	    MSR_INTEGER(m_idDecision, 1);

	    

	    {
		int trE = m_trEarliness;
                if (trE < -m_trFrameAvg) {
                    trE = -m_trFrameAvg;
                }
		*ptrStart += trE;           
	    }

	    int Delay = -trTrueLate;
	    Result = Delay<=0 ? S_OK : S_FALSE;     

	    m_trWaitAvg = trWaitAvg;

	    

	    if (Result==S_FALSE) {   
		trFrame = TimeDiff(*ptrStart-m_trLastDraw);
		m_trLastDraw = *ptrStart;
	    } else {
		
		m_trLastDraw = trRealStream;
	    }
#ifndef PERF
	    int iAccuracy;
	    if (Delay>0) {
		
		iAccuracy = TimeDiff(*ptrStart-m_trRememberStampForPerf);
	    } else {
		
		iAccuracy = trTrueLate;     
	    }
	    PreparePerformanceData(iAccuracy, trFrame);
#endif
	}
	return Result;
    }

    
    

    
    m_trWaitAvg = trWaitAvg;

#ifdef PERF
    
    if (m_bDrawLateFrames) {
       return S_OK;                        
    }                                      
#endif

    
    
    

    MSR_INTEGER(m_idDecision, 2);
    m_nNormal = -1;
    return E_FAIL;                         

} 

BOOL CBaseVideoRenderer::ScheduleSample(IMediaSample *pMediaSample)
{
    

    BOOL bDrawImage = CBaseRenderer::ScheduleSample(pMediaSample);
    if (bDrawImage == FALSE) {

	++m_cFramesDropped;
	return FALSE;
    }

    
    
    return TRUE;
}

STDMETHODIMP CBaseVideoRenderer::get_FramesDroppedInRenderer(int *pcFramesDropped)
{
    CheckPointer(pcFramesDropped,E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    *pcFramesDropped = m_cFramesDropped;
    return NOERROR;
} 

STDMETHODIMP CBaseVideoRenderer::get_FramesDrawn( int *pcFramesDrawn)
{
    CheckPointer(pcFramesDrawn,E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    *pcFramesDrawn = m_cFramesDrawn;
    return NOERROR;
} 

STDMETHODIMP CBaseVideoRenderer::get_AvgFrameRate( int *piAvgFrameRate)
{
    CheckPointer(piAvgFrameRate,E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);

    int t;
    if (m_bStreaming) {
	t = timeGetTime()-m_tStreamingStart;
    } else {
	t = m_tStreamingStart;
    }

    if (t<=0) {
	*piAvgFrameRate = 0;
	ASSERT(m_cFramesDrawn == 0);
    } else {
	
	*piAvgFrameRate = MulDiv(100000, m_cFramesDrawn, t);
    }
    return NOERROR;
} 

STDMETHODIMP CBaseVideoRenderer::get_AvgSyncOffset( int *piAvg)
{
    CheckPointer(piAvg,E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);

    if (NULL==m_pClock) {
	*piAvg = 0;
	return NOERROR;
    }

    
    
    if (m_cFramesDrawn<=1) {
	*piAvg = 0;
    } else {
	*piAvg = (int)(m_iTotAcc / (m_cFramesDrawn-1));
    }
    return NOERROR;
} 

int isqrt(int x)
{
    int s = 1;
    
    if (x > 0x40000000) {
       s = 0x8000;     
    } else {
	while (s*s<x) {    
	    s = 2*s;       
	}
	
	if (x==0) {
	   s= 0; 
		 
	} else {
	    s = (s*s+x)/(2*s);
	    if (s>=0) s = (s*s+x)/(2*s);
	    if (s>=0) s = (s*s+x)/(2*s);
	}
    }
    return s;
}

HRESULT CBaseVideoRenderer::GetStdDev(
    int nSamples,
    int *piResult,
    LONGLONG llSumSq,
    LONGLONG iTot
)
{
    CheckPointer(piResult,E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);

    if (NULL==m_pClock) {
	*piResult = 0;
	return NOERROR;
    }

    
    
    
    

    if (nSamples<=1) {
	*piResult = 0;
    } else {
	LONGLONG x;
	
	

	
	x = llSumSq - llMulDiv(iTot, iTot, nSamples, 0);
	x = x / (nSamples-1);
	ASSERT(x>=0);
	*piResult = isqrt((LONG)x);
    }
    return NOERROR;
}

STDMETHODIMP CBaseVideoRenderer::get_DevSyncOffset( int *piDev)
{
    
    
    return GetStdDev(m_cFramesDrawn - 1,
		     piDev,
		     m_iSumSqAcc,
		     m_iTotAcc);
} 

STDMETHODIMP CBaseVideoRenderer::get_Jitter( int *piJitter)
{
    
    
    
    return GetStdDev(m_cFramesDrawn - 2,
		     piJitter,
		     m_iSumSqFrameTime,
		     m_iSumFrameTime);
} 

STDMETHODIMP
CBaseVideoRenderer::NonDelegatingQueryInterface(REFIID riid,VOID **ppv)
{
    

    if (riid == IID_IQualProp) {
	return GetInterface( (IQualProp *)this, ppv);
    } else if (riid == IID_IQualityControl) {
	return GetInterface( (IQualityControl *)this, ppv);
    }
    return CBaseRenderer::NonDelegatingQueryInterface(riid,ppv);
}

STDMETHODIMP
CBaseVideoRenderer::JoinFilterGraph(IFilterGraph *pGraph,LPCWSTR pName)
{
    
    
    
    if (!pGraph && m_pGraph) {

	
	
	IBaseFilter* pFilter;
	QueryInterface(IID_IBaseFilter,(void **) &pFilter);
	NotifyEvent(EC_WINDOW_DESTROYED, (LPARAM) pFilter, 0);
	pFilter->Release();
    }
    return CBaseFilter::JoinFilterGraph(pGraph, pName);
}

#pragma warning(disable: 4514)

