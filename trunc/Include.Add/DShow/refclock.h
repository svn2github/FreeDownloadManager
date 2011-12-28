/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __BASEREFCLOCK__
#define __BASEREFCLOCK__

#include <Schedule.h>

const UINT RESOLUTION = 1;                      
const INT ADVISE_CACHE = 4;                     
const LONGLONG MAX_TIME = 0x7FFFFFFFFFFFFFFF;   

inline LONGLONG WINAPI ConvertToMilliseconds(const REFERENCE_TIME& RT)
{
    

    return (RT / (UNITS / MILLISECONDS));
}

class CBaseReferenceClock
: public CUnknown, public IReferenceClock, public CCritSec
{
protected:
    virtual ~CBaseReferenceClock();     
public:
    CBaseReferenceClock(TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr, CAMSchedule * pSched = 0 );

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void ** ppv);

    DECLARE_IUNKNOWN

    
    
    
    
    
    
    
    STDMETHODIMP GetTime(REFERENCE_TIME *pTime);
    

    

    
    STDMETHODIMP AdviseTime(
        REFERENCE_TIME baseTime,        
        REFERENCE_TIME streamTime,      
        HEVENT hEvent,                  
        DWORD_PTR *pdwAdviseCookie          
    );

    
    STDMETHODIMP AdvisePeriodic(
        REFERENCE_TIME StartTime,       
        REFERENCE_TIME PeriodTime,      
        HSEMAPHORE hSemaphore,          
        DWORD_PTR *pdwAdviseCookie          
    );

    

    STDMETHODIMP Unadvise(DWORD_PTR dwAdviseCookie);

    

    
    
    
    
    
    
    virtual REFERENCE_TIME GetPrivateTime();

    
    STDMETHODIMP SetTimeDelta( const REFERENCE_TIME& TimeDelta );

    CAMSchedule * GetSchedule() const { return m_pSchedule; }

private:
    REFERENCE_TIME m_rtPrivateTime;     
    DWORD          m_dwPrevSystemTime;  
    REFERENCE_TIME m_rtLastGotTime;     
    REFERENCE_TIME m_rtNextAdvise;      
    UINT           m_TimerResolution;

#ifdef PERF
    int m_idGetSystemTime;
#endif

public:
    void TriggerThread()                	
    {						
	EXECUTE_ASSERT(SetEvent(m_pSchedule->GetEvent()));
    }

private:
    BOOL           m_bAbort;            
    HANDLE         m_hThread;           

    HRESULT AdviseThread();             
    static DWORD __stdcall AdviseThreadFunction(LPVOID); 

protected:
    CAMSchedule * const m_pSchedule;
};

#endif

