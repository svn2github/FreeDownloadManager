/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __CAMSchedule__
#define __CAMSchedule__

class CAMSchedule : private CBaseObject
{
public:
    virtual ~CAMSchedule();
    
    CAMSchedule( HANDLE ev );

    DWORD GetAdviseCount();
    REFERENCE_TIME GetNextAdviseTime();

    
    DWORD_PTR AddAdvisePacket( const REFERENCE_TIME & time1, const REFERENCE_TIME & time2, HANDLE h, BOOL periodic );
    
    HRESULT Unadvise(DWORD_PTR dwAdviseCookie);

    
    
    
    REFERENCE_TIME Advise( const REFERENCE_TIME & rtTime );

    
    HANDLE GetEvent() const { return m_ev; }

private:
    
    
    
    class CAdvisePacket
    {
    public:
        CAdvisePacket()
        {}

        CAdvisePacket * m_next;
        DWORD_PTR       m_dwAdviseCookie;
        REFERENCE_TIME  m_rtEventTime;      
        REFERENCE_TIME  m_rtPeriod;         
        HANDLE          m_hNotify;          
        BOOL            m_bPeriodic;        

        CAdvisePacket( CAdvisePacket * next, LONGLONG time ) : m_next(next), m_rtEventTime(time)
        {}

        void InsertAfter( CAdvisePacket * p )
        {
            p->m_next = m_next;
            m_next    = p;
        }

        int IsZ() const 
        { return m_next == 0; }

        CAdvisePacket * RemoveNext()
        {
            CAdvisePacket *const next = m_next;
            CAdvisePacket *const new_next = next->m_next;
            m_next = new_next;
            return next;
        }

        void DeleteNext()
        {
            delete RemoveNext();
        }

        CAdvisePacket * Next() const
        {
            CAdvisePacket * result = m_next;
            if (result->IsZ()) result = 0;
            return result;
        }

        DWORD_PTR Cookie() const
        { return m_dwAdviseCookie; }
    };

    
    
    
    
    
    CAdvisePacket   head, z;            

    volatile DWORD_PTR  m_dwNextCookie;     
    volatile DWORD  m_dwAdviseCount;    

    CCritSec        m_Serialize;

    
    DWORD_PTR AddAdvisePacket( CAdvisePacket * pPacket );
    
    const HANDLE m_ev;

    
    
    
    void ShuntHead();

    
    CAdvisePacket * m_pAdviseCache;
    DWORD           m_dwCacheCount;
    enum { dwCacheMax = 5 };             

    void Delete( CAdvisePacket * pLink );

public:
#ifdef DEBUG
    void DumpLinkedList();
#else
    void DumpLinkedList() {}
#endif

};

#endif 
