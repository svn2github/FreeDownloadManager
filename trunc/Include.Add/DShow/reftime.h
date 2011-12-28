/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __REFTIME__
#define __REFTIME__

const LONGLONG MILLISECONDS = (1000);            
const LONGLONG NANOSECONDS = (1000000000);       
const LONGLONG UNITS = (NANOSECONDS / 100);      

#define MILLISECONDS_TO_100NS_UNITS(lMs) \
    Int32x32To64((lMs), (UNITS / MILLISECONDS))

class CRefTime
{
public:

    
    
    

    REFERENCE_TIME m_time;

    inline CRefTime()
    {
        
        m_time = 0;
    };

    inline CRefTime(LONG msecs)
    {
        m_time = MILLISECONDS_TO_100NS_UNITS(msecs);
    };

    inline CRefTime(REFERENCE_TIME rt)
    {
        m_time = rt;
    };

    inline operator REFERENCE_TIME() const
    {
        return m_time;
    };

    inline CRefTime& operator=(const CRefTime& rt)
    {
        m_time = rt.m_time;
        return *this;
    };

    inline CRefTime& operator=(const LONGLONG ll)
    {
        m_time = ll;
        return *this;
    };

    inline CRefTime& operator+=(const CRefTime& rt)
    {
        return (*this = *this + rt);
    };

    inline CRefTime& operator-=(const CRefTime& rt)
    {
        return (*this = *this - rt);
    };

    inline LONG Millisecs(void)
    {
        return (LONG)(m_time / (UNITS / MILLISECONDS));
    };

    inline LONGLONG GetUnits(void)
    {
        return m_time;
    };
};

const LONGLONG TimeZero = 0;

#endif 

