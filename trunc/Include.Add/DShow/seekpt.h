/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __seekpt_h__
#define __seekpt_h__

class CSeekingPassThru : public ISeekingPassThru, public CUnknown
{
public:
    static CUnknown *CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
    CSeekingPassThru(TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr);
    ~CSeekingPassThru();

    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

    STDMETHODIMP Init(BOOL bSupportRendering, IPin *pPin);

private:
    CPosPassThru              *m_pPosPassThru;
};

#endif
