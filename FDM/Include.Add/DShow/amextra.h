/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#ifndef __AMEXTRA__
#define __AMEXTRA__                

class CRenderedInputPin : public CBaseInputPin
{
public:

    CRenderedInputPin(TCHAR *pObjectName,
                      CBaseFilter *pFilter,
                      CCritSec *pLock,
                      HRESULT *phr,
                      LPCWSTR pName);
#ifdef UNICODE
    CRenderedInputPin(CHAR *pObjectName,
                      CBaseFilter *pFilter,
                      CCritSec *pLock,
                      HRESULT *phr,
                      LPCWSTR pName);
#endif
    
    
    STDMETHODIMP EndOfStream();
    STDMETHODIMP EndFlush();

    HRESULT Active();
    HRESULT Run(REFERENCE_TIME tStart);

protected:

    
    BOOL m_bAtEndOfStream;      
    BOOL m_bCompleteNotified;   

private:
    void DoCompleteHandling();
};

#endif 

