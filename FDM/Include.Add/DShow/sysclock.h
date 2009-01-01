/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#ifndef __SYSTEMCLOCK__
#define __SYSTEMCLOCK__            

class CSystemClock : public CBaseReferenceClock, public IAMClockAdjust, public IPersist
{
public:
    
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
    CSystemClock(TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr);

    DECLARE_IUNKNOWN

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,void ** ppv);

    
    
    STDMETHODIMP GetClassID(CLSID *pClsID);

    
    STDMETHODIMP SetClockDelta(REFERENCE_TIME rtDelta);
}; 

#endif 
