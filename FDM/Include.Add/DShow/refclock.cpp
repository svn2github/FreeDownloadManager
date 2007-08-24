/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#include <streams.h>
#include <limits.h>      

#pragma warning(disable:4355)  

STDMETHODIMP CBaseReferenceClock::NonDelegatingQueryInterface(
    REFIID riid,
    void ** ppv)
{
    HRESULT hr;

    if (riid == IID_IReferenceClock)
    {
        hr = GetInterface((IReferenceClock *) this, ppv);
    }
    else
    {
        hr = CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
    return hr;
}

CBaseReferenceClock::~CBaseReferenceClock()
{

    if (m_TimerResolution) timeEndPeriod(m_TimerResolution);

    m_pSchedule->DumpLinkedList();

    if (m_hThread)
    {
        m_bAbort = TRUE;
        TriggerThread();
        WaitForSingleObject( m_hThread, INFINITE );
        EXECUTE_ASSERT( CloseHandle(m_hThread) );
        m_hThread = 0;
        EXECUTE_ASSERT( CloseHandle(m_pSchedule->GetEvent()) );
	delete m_pSchedule;
    }
}      

CBaseReferenceClock::CBaseReferenceClock( TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr, CAMSchedule * pShed )
: CUnknown( pName, pUnk )
, m_rtLastGotTime(0)
, m_TimerResolution(0)
, m_bAbort( FALSE )
, m_pSchedule( pShed ? pShed : new CAMSchedule(CreateEvent(NULL, FALSE, FALSE, NULL)) )
, m_hThread(0)
{  

    ASSERT(m_pSchedule);
    if (!m_pSchedule)
    {
	*phr = E_OUTOFMEMORY;
    }
    else
    {
	
	TIMECAPS tc;
	m_TimerResolution = (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(tc)))
			    ? tc.wPeriodMin
			    : 1;

	timeBeginPeriod(m_TimerResolution);

	
	m_dwPrevSystemTime = timeGetTime();
	m_rtPrivateTime = (UNITS / MILLISECONDS) * m_dwPrevSystemTime;

	#ifdef PERF
	    m_idGetSystemTime = MSR_REGISTER("CBaseReferenceClock::GetTime");
	#endif

	if ( !pShed )
	{
	    DWORD ThreadID;
	    m_hThread = ::CreateThread(NULL,                  
				       (DWORD) 0,             
				       AdviseThreadFunction,  
				       (LPVOID) this,         
				       (DWORD) 0,             
				       &ThreadID);            

	    if (m_hThread)
	    {
		SetThreadPriority( m_hThread, THREAD_PRIORITY_TIME_CRITICAL );
	    }
	    else
	    {
		*phr = E_FAIL;
		EXECUTE_ASSERT( CloseHandle(m_pSchedule->GetEvent()) );
		delete m_pSchedule;
	    }
	}
    }
}

STDMETHODIMP CBaseReferenceClock::GetTime(REFERENCE_TIME *pTime)
{
    HRESULT hr;
    if (pTime)
    {
        REFERENCE_TIME rtNow;
        Lock();
        rtNow = GetPrivateTime();
        if (rtNow > m_rtLastGotTime)
        {
            m_rtLastGotTime = rtNow;
            hr = S_OK;
        }
        else
        {
            hr = S_FALSE;
        }
        *pTime = m_rtLastGotTime;
        Unlock();
        MSR_INTEGER(m_idGetSystemTime, LONG((*pTime) / (UNITS/MILLISECONDS)) );  

    }
    else hr = E_POINTER;

    return hr;
}    

STDMETHODIMP CBaseReferenceClock::AdviseTime(
    REFERENCE_TIME baseTime,         
    REFERENCE_TIME streamTime,       
    HEVENT hEvent,                  
    DWORD_PTR *pdwAdviseCookie)         
{
    CheckPointer(pdwAdviseCookie, E_POINTER);
    *pdwAdviseCookie = 0;

    
    ASSERT(WAIT_TIMEOUT == WaitForSingleObject(HANDLE(hEvent),0));

    HRESULT hr;

    const REFERENCE_TIME lRefTime = baseTime + streamTime;
    if ( lRefTime <= 0 || lRefTime == MAX_TIME )
    {
        hr = E_INVALIDARG;
    }
    else
    {
        *pdwAdviseCookie = m_pSchedule->AddAdvisePacket( lRefTime, 0, HANDLE(hEvent), FALSE );
        hr = *pdwAdviseCookie ? NOERROR : E_OUTOFMEMORY;
    }
    return hr;
}      

STDMETHODIMP CBaseReferenceClock::AdvisePeriodic(
    REFERENCE_TIME StartTime,         
    REFERENCE_TIME PeriodTime,        
    HSEMAPHORE hSemaphore,           
    DWORD_PTR *pdwAdviseCookie)          
{
    CheckPointer(pdwAdviseCookie, E_POINTER);
    *pdwAdviseCookie = 0;

    HRESULT hr;
    if (StartTime > 0 && PeriodTime > 0 && StartTime != MAX_TIME )
    {
        *pdwAdviseCookie = m_pSchedule->AddAdvisePacket( StartTime, PeriodTime, HANDLE(hSemaphore), TRUE );
        hr = *pdwAdviseCookie ? NOERROR : E_OUTOFMEMORY;
    }
    else hr = E_INVALIDARG;

    return hr;
}  

STDMETHODIMP CBaseReferenceClock::Unadvise(DWORD_PTR dwAdviseCookie)
{
    return m_pSchedule->Unadvise(dwAdviseCookie);
}  

REFERENCE_TIME CBaseReferenceClock::GetPrivateTime()
{
    CAutoLock cObjectLock(this);  

    

    DWORD dwTime = timeGetTime();
    {
        m_rtPrivateTime += Int32x32To64(UNITS / MILLISECONDS, (DWORD)(dwTime - m_dwPrevSystemTime));
        m_dwPrevSystemTime = dwTime;
    }

    return m_rtPrivateTime;
}      

STDMETHODIMP CBaseReferenceClock::SetTimeDelta(const REFERENCE_TIME & TimeDelta)
{
#ifdef DEBUG

    
    LONGLONG llDelta = TimeDelta > 0 ? TimeDelta : -TimeDelta;
    if (llDelta > UNITS * 1000) {
        DbgLog((LOG_TRACE, 0, TEXT("Bad Time Delta")));
        DebugBreak();
    }

    
    
    
    const LONG usDelta = LONG(TimeDelta/10);      

    DWORD delta        = abs(usDelta);            
    
    int   Severity     = 8;
    while ( delta > 0 )
    {
        delta >>= 3;                              
        Severity--;
    }

    
    DbgLog((LOG_TIMING, Severity < 0 ? 0 : Severity,
        TEXT("Sev %2i: CSystemClock::SetTimeDelta(%8ld us) %lu -> %lu ms."),
        Severity, usDelta, DWORD(ConvertToMilliseconds(m_rtPrivateTime)),
        DWORD(ConvertToMilliseconds(TimeDelta+m_rtPrivateTime)) ));

    
    #ifdef BREAK_ON_SEVERE_TIME_DELTA
        if (Severity < 0)
            DbgBreakPoint(TEXT("SetTimeDelta > 16 seconds!"),
                          TEXT(__FILE__),__LINE__);
    #endif

#endif

    CAutoLock cObjectLock(this);
    m_rtPrivateTime += TimeDelta;
    
    
    
    
    
    
    
    if ( TimeDelta > 5000 && m_pSchedule->GetAdviseCount() > 0 ) TriggerThread();
    return NOERROR;
}    

DWORD __stdcall CBaseReferenceClock::AdviseThreadFunction(LPVOID p)
{
    return DWORD(reinterpret_cast<CBaseReferenceClock*>(p)->AdviseThread());
}

HRESULT CBaseReferenceClock::AdviseThread()
{
    DWORD dwWait = INFINITE;

    
    
    
    
    

    while ( !m_bAbort )
    {
        
        DbgLog((LOG_TIMING, 3, TEXT("CBaseRefClock::AdviseThread() Delay: %lu ms"), dwWait ));
        WaitForSingleObject(m_pSchedule->GetEvent(), dwWait);
        if (m_bAbort) break;

        
        
        
        
        
        const REFERENCE_TIME  rtNow = GetPrivateTime();

        DbgLog((LOG_TIMING, 3,
              TEXT("CBaseRefClock::AdviseThread() Woke at = %lu ms"),
              ConvertToMilliseconds(rtNow) ));

        
        
        
        m_rtNextAdvise = m_pSchedule->Advise( 10000 + rtNow );
        LONGLONG llWait = m_rtNextAdvise - rtNow;

        ASSERT( llWait > 0 );

        llWait = ConvertToMilliseconds(llWait);
        
        dwWait = (llWait > REFERENCE_TIME(UINT_MAX)) ? UINT_MAX : DWORD(llWait);
    };
    return NOERROR;
}
