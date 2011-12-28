/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __strmctl_h__
#define __strmctl_h__

class CBaseStreamControl : public IAMStreamControl
{
public:
    
    enum StreamControlState
    { STREAM_FLOWING = 0x1000,
      STREAM_DISCARDING
    };

private:
    enum StreamControlState	m_StreamState;		
    enum StreamControlState	m_StreamStateOnStop;	
						

    REFERENCE_TIME	m_tStartTime;	    
    REFERENCE_TIME	m_tStopTime;	    
    DWORD		m_dwStartCookie;    
    DWORD		m_dwStopCookie;	    
    volatile BOOL       m_bIsFlushing;        
    volatile BOOL	m_bStopSendExtra;   
    volatile BOOL	m_bStopExtraSent;   

    CCritSec		m_CritSec;	    

    
    
    
    
    CAMEvent			m_StreamEvent;

    
    
    void ExecuteStop();
    void ExecuteStart();
    void CancelStop();
    void CancelStart();

    
    
    
    IReferenceClock *	m_pRefClock;	    
					    
					    
    IMediaEventSink *   m_pSink;            
					    
					    
    FILTER_STATE	m_FilterState;	    
					    
					    
    REFERENCE_TIME	m_tRunStart;	    

    
    
    
    
    
    
    
    
    
    
    enum StreamControlState CheckSampleTimes( const REFERENCE_TIME * pSampleStart,
					      const REFERENCE_TIME * pSampleStop );

public:
    
    
    
    
    CBaseStreamControl();
    ~CBaseStreamControl();

    
    
    
    

    
    
    
    
    
    
    
    
    
    void SetSyncSource( IReferenceClock * pRefClock )
    {
	CAutoLock lck(&m_CritSec);
	if (m_pRefClock) m_pRefClock->Release();
	m_pRefClock = pRefClock;
	if (m_pRefClock) m_pRefClock->AddRef();
    }

    
    
    
    
    void SetFilterGraph( IMediaEventSink *pSink ) {
        m_pSink = pSink;
    }

    
    
    
    
    void NotifyFilterState( FILTER_STATE new_state, REFERENCE_TIME tStart = 0 );

    
    
    
    void Flushing( BOOL bInProgress );

    

    
    

    STDMETHODIMP StopAt( const REFERENCE_TIME * ptStop = NULL,
			 BOOL bSendExtra = FALSE,
			 DWORD dwCookie = 0 );
    STDMETHODIMP StartAt( const REFERENCE_TIME * ptStart = NULL,
		    	  DWORD dwCookie = 0 );
    STDMETHODIMP GetInfo( AM_STREAM_INFO *pInfo);

    
    
    
    
    
    
    
    enum StreamControlState CheckStreamState( IMediaSample * pSample );

private:
    
    
    
    HANDLE GetStreamEventHandle() const { return m_StreamEvent; }
    enum StreamControlState GetStreamState() const { return m_StreamState; }
    BOOL IsStreaming() const { return m_StreamState == STREAM_FLOWING; }
};

#endif
