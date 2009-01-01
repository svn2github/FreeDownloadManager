/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>
#include "seekpt.h"              

CUnknown * CSeekingPassThru::CreateInstance(LPUNKNOWN pUnk, HRESULT *phr)
{
    return new CSeekingPassThru(NAME("Silly Seeking Thing"),pUnk, phr);
}  

STDMETHODIMP CSeekingPassThru::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    if (riid == IID_ISeekingPassThru) {
        return GetInterface((ISeekingPassThru *) this, ppv);
    } else {
        if (m_pPosPassThru &&
            (riid == IID_IMediaSeeking ||
             riid == IID_IMediaPosition)) {
            return m_pPosPassThru->NonDelegatingQueryInterface(riid,ppv);
        } else {
            return CUnknown::NonDelegatingQueryInterface(riid, ppv);
        }
    }
}  

CSeekingPassThru::CSeekingPassThru( TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr )
                            : CUnknown(pName, pUnk, phr),
                            m_pPosPassThru(NULL)
{
}  

CSeekingPassThru::~CSeekingPassThru()
{
    delete m_pPosPassThru;
}

STDMETHODIMP CSeekingPassThru::Init(BOOL bRendererSeeking, IPin *pPin)
{
    HRESULT hr = NOERROR;
    if (m_pPosPassThru) {
        hr = E_FAIL;
    } else {
        m_pPosPassThru =
            bRendererSeeking ?
                new CRendererPosPassThru(
                    NAME("Render Seeking COM object"),
                    (IUnknown *)this,
                    &hr,
                    pPin) :
                new CPosPassThru(
                    NAME("Render Seeking COM object"),
                    (IUnknown *)this,
                    &hr,
                    pPin);
        if (!m_pPosPassThru) {
            hr = E_OUTOFMEMORY;
        } else {
            if (FAILED(hr)) {
                delete m_pPosPassThru;
                m_pPosPassThru = NULL;
            }
        }
    }
    return hr;
}

