/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#include <streams.h>                

CAMSchedule::CAMSchedule( HANDLE ev )
: CBaseObject(TEXT("CAMSchedule"))
, head(&z, 0), z(0, MAX_TIME)
, m_dwNextCookie(0), m_dwAdviseCount(0)
, m_pAdviseCache(0), m_dwCacheCount(0)
, m_ev( ev )
{
    head.m_dwAdviseCookie = z.m_dwAdviseCookie = 0;
}

CAMSchedule::~CAMSchedule()
{
    m_Serialize.Lock();

    
    CAdvisePacket * p = m_pAdviseCache;
    while (p)
    {
        CAdvisePacket *const p_next = p->m_next;
        delete p;
        p = p_next;
    }

    ASSERT( m_dwAdviseCount == 0 );
    
    if ( m_dwAdviseCount > 0 )
    {
        DumpLinkedList();
        while ( !head.m_next->IsZ() )
        {
            head.DeleteNext();
            --m_dwAdviseCount;
        }
    }

    
    
    
    ASSERT( m_dwAdviseCount == 0 );

    m_Serialize.Unlock();
}    

DWORD CAMSchedule::GetAdviseCount()
{
    
    return m_dwAdviseCount;
}

REFERENCE_TIME CAMSchedule::GetNextAdviseTime()
{
    CAutoLock lck(&m_Serialize); 
    return head.m_next->m_rtEventTime;
}

DWORD_PTR CAMSchedule::AddAdvisePacket
( const REFERENCE_TIME & time1
, const REFERENCE_TIME & time2
, HANDLE h, BOOL periodic
)
{
    
    
    ASSERT( time1 < MAX_TIME );
    DWORD_PTR Result;
    CAdvisePacket * p;

    m_Serialize.Lock();

    if (m_pAdviseCache)
    {
        p = m_pAdviseCache;
        m_pAdviseCache = p->m_next;
        --m_dwCacheCount;
    }
    else
    {
        p = new CAdvisePacket();
    }
    if (p)
    {
        p->m_rtEventTime = time1; p->m_rtPeriod = time2;
        p->m_hNotify = h; p->m_bPeriodic = periodic;
        Result = AddAdvisePacket( p );
    }
    else Result = 0;

    m_Serialize.Unlock();

    return Result;
}

HRESULT CAMSchedule::Unadvise(DWORD_PTR dwAdviseCookie)
{
    HRESULT hr = S_FALSE;
    CAdvisePacket * p_prev = &head;
    CAdvisePacket * p_n;
    m_Serialize.Lock();
    while ( p_n = p_prev->Next() ) 
    {
        if ( p_n->m_dwAdviseCookie == dwAdviseCookie )
        {
            Delete( p_prev->RemoveNext() );
            --m_dwAdviseCount;
            hr = S_OK;
	    
            #ifdef DEBUG
	       while (p_n = p_prev->Next())
               {
                   ASSERT(p_n->m_dwAdviseCookie != dwAdviseCookie);
                   p_prev = p_n;
               }
            #endif
            break;
        }
        p_prev = p_n;
    };
    m_Serialize.Unlock();
    return hr;
}

REFERENCE_TIME CAMSchedule::Advise( const REFERENCE_TIME & rtTime )
{
    REFERENCE_TIME  rtNextTime;
    CAdvisePacket * pAdvise;

    DbgLog((LOG_TIMING, 2,
        TEXT("CAMSchedule::Advise( %lu ms )"), ULONG(rtTime / (UNITS / MILLISECONDS))));

    CAutoLock lck(&m_Serialize);

    #ifdef DEBUG
        if (DbgCheckModuleLevel(LOG_TIMING, 4)) DumpLinkedList();
    #endif

    
    while ( rtTime >= (rtNextTime = (pAdvise=head.m_next)->m_rtEventTime) &&
            !pAdvise->IsZ() )
    {
        ASSERT(pAdvise->m_dwAdviseCookie); 

        ASSERT(pAdvise->m_hNotify != INVALID_HANDLE_VALUE);

        if (pAdvise->m_bPeriodic == TRUE)
        {
            EXECUTE_ASSERT(ReleaseSemaphore(pAdvise->m_hNotify,1,NULL));
            pAdvise->m_rtEventTime += pAdvise->m_rtPeriod;
            ShuntHead();
        }
        else
        {
            ASSERT( pAdvise->m_bPeriodic == FALSE );
            EXECUTE_ASSERT(SetEvent(pAdvise->m_hNotify));
            --m_dwAdviseCount;
            Delete( head.RemoveNext() );
        }

    }

    DbgLog((LOG_TIMING, 3,
            TEXT("CAMSchedule::Advise() Next time stamp: %lu ms, for advise %lu."),
            DWORD(rtNextTime / (UNITS / MILLISECONDS)), pAdvise->m_dwAdviseCookie ));

    return rtNextTime;
}    

DWORD_PTR CAMSchedule::AddAdvisePacket( CAdvisePacket * pPacket )
{
    ASSERT(pPacket->m_rtEventTime >= 0 && pPacket->m_rtEventTime < MAX_TIME);
    ASSERT(CritCheckIn(&m_Serialize));

    CAdvisePacket * p_prev = &head;
    CAdvisePacket * p_n;

    const DWORD_PTR Result = pPacket->m_dwAdviseCookie = ++m_dwNextCookie;
    
    for(;;p_prev = p_n)
    {
        p_n = p_prev->m_next;
        if ( p_n->m_rtEventTime >= pPacket->m_rtEventTime ) break;
    }
    p_prev->InsertAfter( pPacket );
    ++m_dwAdviseCount;

    DbgLog((LOG_TIMING, 2, TEXT("Added advise %lu, for thread 0x%02X, scheduled at %lu"),
    	pPacket->m_dwAdviseCookie, GetCurrentThreadId(), (pPacket->m_rtEventTime / (UNITS / MILLISECONDS)) ));

    
    if ( p_prev == &head ) SetEvent( m_ev );

    return Result;
}

void CAMSchedule::Delete( CAdvisePacket * pPacket )
{
    if ( m_dwCacheCount >= dwCacheMax ) delete pPacket;
    else
    {
        m_Serialize.Lock();
        pPacket->m_next = m_pAdviseCache;
        m_pAdviseCache = pPacket;
        ++m_dwCacheCount;
        m_Serialize.Unlock();
    }
}    

void CAMSchedule::ShuntHead()
{
    CAdvisePacket * p_prev = &head;
    CAdvisePacket * p_n;

    m_Serialize.Lock();
    CAdvisePacket *const pPacket = head.m_next;

    
    
    
    ASSERT( pPacket->m_rtEventTime < MAX_TIME );

    
    for(;;p_prev = p_n)
    {
        p_n = p_prev->m_next;
        if ( p_n->m_rtEventTime > pPacket->m_rtEventTime ) break;
    }
    
    if (p_prev != pPacket)
    {
        head.m_next = pPacket->m_next;
        (p_prev->m_next = pPacket)->m_next = p_n;
    }
    #ifdef DEBUG
        DbgLog((LOG_TIMING, 2, TEXT("Periodic advise %lu, shunted to %lu"),
    	    pPacket->m_dwAdviseCookie, (pPacket->m_rtEventTime / (UNITS / MILLISECONDS)) ));
    #endif
    m_Serialize.Unlock();
}  

#ifdef DEBUG
void CAMSchedule::DumpLinkedList()
{
    m_Serialize.Lock();
    int i=0;
    DbgLog((LOG_TIMING, 1, TEXT("CAMSchedule::DumpLinkedList() this = 0x%p"), this));
    for ( CAdvisePacket * p = &head
        ; p
        ; p = p->m_next         , i++
        )	
    {
        DbgLog((LOG_TIMING, 1, TEXT("Advise List # %lu, Cookie %d,  RefTime %lu"),
            i,
	    p->m_dwAdviseCookie,
	    p->m_rtEventTime / (UNITS / MILLISECONDS)
            ));
    }
    m_Serialize.Unlock();
}
#endif
