/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>        
#include <mmsystem.h>       
#include <limits.h>         
#include <measure.h>        

#include "amextra.h"

#pragma warning(disable:4355)    

CRenderedInputPin::CRenderedInputPin(TCHAR *pObjectName,
                                     CBaseFilter *pFilter,
                                     CCritSec *pLock,
                                     HRESULT *phr,
                                     LPCWSTR pName) :
    CBaseInputPin(pObjectName, pFilter, pLock, phr, pName),
    m_bAtEndOfStream(FALSE),
    m_bCompleteNotified(FALSE)
{
}
#ifdef UNICODE
CRenderedInputPin::CRenderedInputPin(CHAR *pObjectName,
                                     CBaseFilter *pFilter,
                                     CCritSec *pLock,
                                     HRESULT *phr,
                                     LPCWSTR pName) :
    CBaseInputPin(pObjectName, pFilter, pLock, phr, pName),
    m_bAtEndOfStream(FALSE),
    m_bCompleteNotified(FALSE)
{
}
#endif      

STDMETHODIMP CRenderedInputPin::EndOfStream()
{
    HRESULT hr = CheckStreaming();

    
    if (S_OK == hr  && !m_bAtEndOfStream) {
        m_bAtEndOfStream = TRUE;
        FILTER_STATE fs;
        EXECUTE_ASSERT(SUCCEEDED(m_pFilter->GetState(0, &fs)));
        if (fs == State_Running) {
            DoCompleteHandling();
        }
    }
    return hr;
}      

STDMETHODIMP CRenderedInputPin::EndFlush()
{
    CAutoLock lck(m_pLock);

    
    m_bAtEndOfStream = FALSE;
    m_bCompleteNotified = FALSE;

    return CBaseInputPin::EndFlush();
}      

HRESULT CRenderedInputPin::Run(REFERENCE_TIME tStart)
{
    UNREFERENCED_PARAMETER(tStart);
    m_bCompleteNotified = FALSE;
    if (m_bAtEndOfStream) {
        DoCompleteHandling();
    }
    return S_OK;
}      

HRESULT CRenderedInputPin::Active()
{
    m_bAtEndOfStream = FALSE;
    m_bCompleteNotified = FALSE;
    return CBaseInputPin::Active();
}      

void CRenderedInputPin::DoCompleteHandling()
{
    ASSERT(m_bAtEndOfStream);
    if (!m_bCompleteNotified) {
        m_bCompleteNotified = TRUE;
        m_pFilter->NotifyEvent(EC_COMPLETE, S_OK, (LONG_PTR)(IBaseFilter *)m_pFilter);
    }
}

