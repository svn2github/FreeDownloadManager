/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#include <streams.h>

#ifdef PERF
#include <measure.h>
#endif        

CPersistStream::CPersistStream(IUnknown *punk, HRESULT *phr)
    : mPS_fDirty(FALSE)
{
    mPS_dwFileVersion = GetSoftwareVersion();
}        

CPersistStream::~CPersistStream() {
    
}

#if 0
SAMPLE CODE TO COPY - not active at the moment        

STDMETHODIMP CPersistStream::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    if (riid == IID_IPersist) {
        return GetInterface((IPersist *) this, ppv);      
    }
    else if (riid == IID_IPersistStream) {
        return GetInterface((IPersistStream *) this, ppv);
    }
    else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}
#endif          

HRESULT CPersistStream::WriteToStream(IStream *pStream)
{
    
    

    return NOERROR;
}    

HRESULT CPersistStream::ReadFromStream(IStream * pStream)
{
    
    

    return NOERROR;
}          

STDMETHODIMP CPersistStream::Load(LPSTREAM pStm)
{
    HRESULT hr;
    
    mPS_dwFileVersion = ReadInt(pStm, hr);
    if (FAILED(hr)) {
        return hr;
    }

    return ReadFromStream(pStm);
}              

STDMETHODIMP CPersistStream::Save(LPSTREAM pStm, BOOL fClearDirty)
{

    HRESULT hr = WriteInt(pStm, GetSoftwareVersion());
    if (FAILED(hr)) {
        return hr;
    }

    hr = WriteToStream(pStm);
    if (FAILED(hr)) {
        return hr;
    }

    mPS_fDirty = !fClearDirty;

    return hr;
}                   

STDAPI WriteInt(IStream *pIStream, int n)
{
    WCHAR Buff[13];  
    wsprintfW(Buff, L"%011d ",n);
    return pIStream->Write(&(Buff[0]), 12*sizeof(WCHAR), NULL);
}                 

STDAPI_(int) ReadInt(IStream *pIStream, HRESULT &hr)
{

    int Sign = 1;
    unsigned int n = 0;    
    WCHAR wch;

    hr = pIStream->Read( &wch, sizeof(wch), NULL);
    if (FAILED(hr)) {
        return 0;
    }

    if (wch==L'-'){
        Sign = -1;
        hr = pIStream->Read( &wch, sizeof(wch), NULL);
        if (FAILED(hr)) {
            return 0;
        }
    }

    for( ; ; ) {
        if (wch>=L'0' && wch<=L'9') {
            n = 10*n+(int)(wch-L'0');
        } else if (  wch == L' '
                  || wch == L'\t'
                  || wch == L'\r'
                  || wch == L'\n'
                  || wch == L'\0'
                  ) {
            break;
        } else {
            hr = VFW_E_INVALID_FILE_FORMAT;
            return 0;
        }

        hr = pIStream->Read( &wch, sizeof(wch), NULL);
        if (FAILED(hr)) {
            return 0;
        }
    }

    if (n==0x80000000 && Sign==-1) {
        
        return (int)n;
    }
    else return (int)n * Sign;
}           

#pragma warning(disable: 4514)
