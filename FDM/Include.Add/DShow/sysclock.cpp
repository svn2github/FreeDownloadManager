/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#include <streams.h>
#include <limits.h>  

#ifdef FILTER_DLL    

CFactoryTemplate g_Templates[1] = {
    {&CLSID_SystemClock, CSystemClock::CreateInstance}
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);
#endif  

CUnknown * WINAPI CSystemClock::CreateInstance(LPUNKNOWN pUnk,HRESULT *phr)
{
    return new CSystemClock(NAME("System reference clock"),pUnk, phr);
}  

CSystemClock::CSystemClock(TCHAR *pName,LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseReferenceClock(pName, pUnk, phr)
{
}

STDMETHODIMP CSystemClock::NonDelegatingQueryInterface(
    REFIID riid,
    void ** ppv)
{
    if (riid == IID_IPersist)
    {
        return GetInterface(static_cast<IPersist *>(this), ppv);
    }
    else if (riid == IID_IAMClockAdjust)
    {
        return GetInterface(static_cast<IAMClockAdjust *>(this), ppv);
    }
    else
    {
        return CBaseReferenceClock::NonDelegatingQueryInterface(riid, ppv);
    }
}  

STDMETHODIMP
CSystemClock::GetClassID(CLSID *pClsID)
{
    CheckPointer(pClsID,E_POINTER);
    ValidateReadWritePtr(pClsID,sizeof(CLSID));
    *pClsID = CLSID_SystemClock;
    return NOERROR;
}  

STDMETHODIMP 
CSystemClock::SetClockDelta(REFERENCE_TIME rtDelta)
{
    return SetTimeDelta(rtDelta);
}
