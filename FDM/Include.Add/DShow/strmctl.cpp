/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>
#include <strmctl.h>

CBaseStreamControl::CBaseStreamControl()
: m_StreamState(STREAM_FLOWING)
, m_StreamStateOnStop(STREAM_FLOWING) 
, m_tStartTime(MAX_TIME)
, m_tStopTime(MAX_TIME)
, m_dwStartCookie(0)
, m_dwStopCookie(0)
, m_pRefClock(NULL)
, m_FilterState(State_Stopped)
, m_bIsFlushing(FALSE)
, m_bStopSendExtra(FALSE)
{}

CBaseStreamControl::~CBaseStreamControl()
{
    
    SetSyncSource(NULL);
    return;
}  

STDMETHODIMP CBaseStreamControl::StopAt(const REFERENCE_TIME * ptStop, BOOL bSendExtra, DWORD dwCookie)
{
    CAutoLock lck(&m_CritSec);
    m_bStopSendExtra = FALSE;	
    m_bStopExtraSent = FALSE;
    if (ptStop)
    {
        if (*ptStop == MAX_TIME)
        {
            DbgLog((LOG_TRACE,2,TEXT("StopAt: Cancel stop")));
            CancelStop();
	    
	    
	    if (m_FilterState == State_Stopped && m_tStartTime < MAX_TIME) {
	        m_StreamState = STREAM_DISCARDING;
                DbgLog((LOG_TRACE,2,TEXT("graph will begin by DISCARDING")));
	    }
            return NOERROR;
        }
        DbgLog((LOG_TRACE,2,TEXT("StopAt: %dms extra=%d"),
				(int)(*ptStop/10000), bSendExtra));
	
        
	if (m_FilterState == State_Stopped && m_tStartTime > *ptStop) {
	    m_StreamState = STREAM_FLOWING;
            DbgLog((LOG_TRACE,2,TEXT("graph will begin by FLOWING")));
	}
        m_bStopSendExtra = bSendExtra;
        m_tStopTime = *ptStop;
        m_dwStopCookie = dwCookie;
        m_StreamStateOnStop = STREAM_DISCARDING;
    }
    else
    {
        DbgLog((LOG_TRACE,2,TEXT("StopAt: now")));
	
        m_bStopSendExtra = FALSE;
        m_tStopTime = MAX_TIME;
        m_dwStopCookie = 0;
        m_StreamState = STREAM_DISCARDING;
        m_StreamStateOnStop = STREAM_FLOWING;	
    }
    
    m_StreamEvent.Set();
    return NOERROR;
}

STDMETHODIMP CBaseStreamControl::StartAt
( const REFERENCE_TIME *ptStart, DWORD dwCookie )
{
    CAutoLock lck(&m_CritSec);
    if (ptStart)
    {
        if (*ptStart == MAX_TIME)
        {
            DbgLog((LOG_TRACE,2,TEXT("StartAt: Cancel start")));
            CancelStart();
	    
	    
	    if (m_FilterState == State_Stopped && m_tStopTime < MAX_TIME) {
                DbgLog((LOG_TRACE,2,TEXT("graph will begin by FLOWING")));
	        m_StreamState = STREAM_FLOWING;
	    }
            return NOERROR;
        }
        DbgLog((LOG_TRACE,2,TEXT("StartAt: %dms"), (int)(*ptStart/10000)));
	
        
	if (m_FilterState == State_Stopped && m_tStopTime >= *ptStart) {
            DbgLog((LOG_TRACE,2,TEXT("graph will begin by DISCARDING")));
	    m_StreamState = STREAM_DISCARDING;
	}
        m_tStartTime = *ptStart;
        m_dwStartCookie = dwCookie;
        
    }
    else
    {
        DbgLog((LOG_TRACE,2,TEXT("StartAt: now")));
        m_tStartTime = MAX_TIME;
        m_dwStartCookie = 0;
        m_StreamState = STREAM_FLOWING;
    }
    
    m_StreamEvent.Set();
    return NOERROR;
}  

STDMETHODIMP CBaseStreamControl::GetInfo(AM_STREAM_INFO *pInfo)
{
    if (pInfo == NULL)
	return E_POINTER;

    pInfo->tStart = m_tStartTime;
    pInfo->tStop  = m_tStopTime;
    pInfo->dwStartCookie = m_dwStartCookie;
    pInfo->dwStopCookie  = m_dwStopCookie;
    pInfo->dwFlags = m_bStopSendExtra ? AM_STREAM_INFO_STOP_SEND_EXTRA : 0;
    pInfo->dwFlags |= m_tStartTime == MAX_TIME ? 0 : AM_STREAM_INFO_START_DEFINED;
    pInfo->dwFlags |= m_tStopTime == MAX_TIME ? 0 : AM_STREAM_INFO_STOP_DEFINED;
    switch (m_StreamState) {
    default:
        DbgBreak("Invalid stream state");
    case STREAM_FLOWING:
        break;
    case STREAM_DISCARDING:
        pInfo->dwFlags |= AM_STREAM_INFO_DISCARDING;
        break;
    }
    return S_OK;
}  

void CBaseStreamControl::ExecuteStop()
{
    ASSERT(CritCheckIn(&m_CritSec));
    m_StreamState = m_StreamStateOnStop;
    if (m_dwStopCookie && m_pSink) {
	DbgLog((LOG_TRACE,2,TEXT("*sending EC_STREAM_CONTROL_STOPPED (%d)"),
							m_dwStopCookie));
        m_pSink->Notify(EC_STREAM_CONTROL_STOPPED, (LONG_PTR)this, m_dwStopCookie);
    }
    CancelStop(); 
}

void CBaseStreamControl::ExecuteStart()
{
    ASSERT(CritCheckIn(&m_CritSec));
    m_StreamState = STREAM_FLOWING;
    if (m_dwStartCookie) {
	DbgLog((LOG_TRACE,2,TEXT("*sending EC_STREAM_CONTROL_STARTED (%d)"),
							m_dwStartCookie));
        m_pSink->Notify(EC_STREAM_CONTROL_STARTED, (LONG_PTR)this, m_dwStartCookie);
    }
    CancelStart(); 
}

void CBaseStreamControl::CancelStop()
{
    ASSERT(CritCheckIn(&m_CritSec));
    m_tStopTime = MAX_TIME;
    m_dwStopCookie = 0;
    m_StreamStateOnStop = STREAM_FLOWING;
}

void CBaseStreamControl::CancelStart()
{
    ASSERT(CritCheckIn(&m_CritSec));
    m_tStartTime = MAX_TIME;
    m_dwStartCookie = 0;
}                                                                                            

enum CBaseStreamControl::StreamControlState CBaseStreamControl::CheckSampleTimes
( const REFERENCE_TIME * pSampleStart, const REFERENCE_TIME * pSampleStop )
{
    CAutoLock lck(&m_CritSec);

    ASSERT(!m_bIsFlushing);
    ASSERT(pSampleStart && pSampleStop);

    
    

    if (m_tStopTime >= *pSampleStart)
    {
        if (m_tStartTime >= *pSampleStop)
	    return m_StreamState;		
	if (m_tStopTime < m_tStartTime)
	    ExecuteStop();			
	ExecuteStart();                         
	return m_StreamState;
    }

    if (m_tStartTime >= *pSampleStop)
    {
        ExecuteStop();                          
        return m_StreamState;
    }

    if (m_tStartTime <= m_tStopTime)
    {
	ExecuteStart();
	ExecuteStop();
        return m_StreamState;		
    }
    else
    {
	ExecuteStop();
	ExecuteStart();
        return m_StreamState;		
    }
}  

enum CBaseStreamControl::StreamControlState CBaseStreamControl::CheckStreamState( IMediaSample * pSample )
{

    REFERENCE_TIME rtBufferStart, rtBufferStop;
    const BOOL bNoBufferTimes =
              pSample == NULL ||
              FAILED(pSample->GetTime(&rtBufferStart, &rtBufferStop));

    StreamControlState state;
    LONG lWait;

    do
        {
 	    
            if (m_bIsFlushing || m_FilterState == State_Stopped)
		return STREAM_DISCARDING;

            if (bNoBufferTimes) {
                
                state = m_StreamState;
                break;
            } else {
                state = CheckSampleTimes( &rtBufferStart, &rtBufferStop );
                if (state == STREAM_FLOWING)
		    break;

		
		
		
		if (m_bStopSendExtra && !m_bStopExtraSent &&
					m_tStopTime == MAX_TIME &&
					m_FilterState != State_Stopped) {
		    m_bStopExtraSent = TRUE;
		    DbgLog((LOG_TRACE,2,TEXT("%d sending an EXTRA frame"),
							    m_dwStopCookie));
		    state = STREAM_FLOWING;
		    break;
		}
            }

            

            
            if (!m_pRefClock) {
		break;

	    
	    
	    
	    
	    } else if (m_FilterState == State_Paused) {
		lWait = INFINITE;

	    } else {
	        
	        
	        REFERENCE_TIME rtNow;
                EXECUTE_ASSERT(SUCCEEDED(m_pRefClock->GetTime(&rtNow)));
                rtNow -= m_tRunStart;   
                lWait = LONG((rtBufferStart - rtNow)/10000); 
                if (lWait < 10) break; 
	    }

    } while(WaitForSingleObject(GetStreamEventHandle(), lWait) != WAIT_TIMEOUT);

    return state;
}  

void CBaseStreamControl::NotifyFilterState( FILTER_STATE new_state, REFERENCE_TIME tStart )
{
    CAutoLock lck(&m_CritSec);

    
    if (m_FilterState == new_state)
	return;

    switch (new_state)
    {
        case State_Stopped:

            DbgLog((LOG_TRACE,2,TEXT("Filter is STOPPED")));

	    
	    
	    

	    if (m_tStartTime != MAX_TIME && m_tStopTime == MAX_TIME) {
		ExecuteStart();
	    } else if (m_tStopTime != MAX_TIME && m_tStartTime == MAX_TIME) {
		ExecuteStop();
	    } else if (m_tStopTime != MAX_TIME && m_tStartTime != MAX_TIME) {
		if (m_tStartTime <= m_tStopTime) {
		    ExecuteStart();
		    ExecuteStop();
		} else {
		    ExecuteStop();
		    ExecuteStart();
		}
	    }
	    
	    
	    m_StreamState = STREAM_FLOWING;
            m_FilterState = new_state;
            break;

        case State_Running:

            DbgLog((LOG_TRACE,2,TEXT("Filter is RUNNING")));

            m_tRunStart = tStart;
            

        default: 
            m_FilterState = new_state;
    }
    
    m_StreamEvent.Set();
}  

void CBaseStreamControl::Flushing(BOOL bInProgress)
{
    CAutoLock lck(&m_CritSec);
    m_bIsFlushing = bInProgress;
    m_StreamEvent.Set();
}
