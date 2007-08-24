/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                  

#ifndef __ASYNCRDR_H__
#define __ASYNCRDR_H__                                

class CAsyncReader;    

class CAsyncOutputPin
  : public IAsyncReader,
    public CBasePin
{
protected:
    CAsyncReader* m_pReader;
    CAsyncIo * m_pIo;

    
    
    
    
    
    BOOL         m_bQueriedForAsyncReader;

    HRESULT InitAllocator(IMemAllocator **ppAlloc);

public:
    
    CAsyncOutputPin(
        HRESULT * phr,
	CAsyncReader *pReader,
	CAsyncIo *pIo,
        CCritSec * pLock);

    ~CAsyncOutputPin();

    

    
    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void**);

    
    STDMETHODIMP Connect(
        IPin * pReceivePin,
        const AM_MEDIA_TYPE *pmt   
    );

    

    
    
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

    
    HRESULT CheckMediaType(const CMediaType* pType);

    
    HRESULT CheckConnect(IPin *pPin)
    {
        m_bQueriedForAsyncReader = FALSE;
        return CBasePin::CheckConnect(pPin);
    }

    
    HRESULT CompleteConnect(IPin *pReceivePin)
    {
        if (m_bQueriedForAsyncReader) {
            return CBasePin::CompleteConnect(pReceivePin);
        } else {
#ifdef VFW_E_NO_TRANSPORT
            return VFW_E_NO_TRANSPORT;
#else
            return E_FAIL;
#endif
        }
    }

    
    HRESULT BreakConnect()
    {
        m_bQueriedForAsyncReader = FALSE;
        return CBasePin::BreakConnect();
    }

    
    
    
    
    
    
    STDMETHODIMP RequestAllocator(
                      IMemAllocator* pPreferred,
                      ALLOCATOR_PROPERTIES* pProps,
                      IMemAllocator ** ppActual);

    
    
    
    
    
    
    
    STDMETHODIMP Request(
                     IMediaSample* pSample,
                     DWORD dwUser);	        

    
    
    
    
    
    STDMETHODIMP WaitForNext(
                      DWORD dwTimeout,
                      IMediaSample** ppSample,  
                      DWORD * pdwUser);		

    
    
    
    
    STDMETHODIMP SyncReadAligned(
                      IMediaSample* pSample);  

    
    
    
    STDMETHODIMP SyncRead(
                      LONGLONG llPosition,	
                      LONG lLength,		
                      BYTE* pBuffer);		

    
    
    
    STDMETHODIMP Length(
                      LONGLONG* pTotal,
                      LONGLONG* pAvailable);

    
    
    
    STDMETHODIMP BeginFlush(void);
    STDMETHODIMP EndFlush(void);

};          

class CAsyncReader : public CBaseFilter
{

protected:
    
    CCritSec m_csFilter;

    
    CAsyncIo m_Io;

    
    CAsyncOutputPin m_OutputPin;

    
    CMediaType            m_mt;

public:
		
    

    CAsyncReader(
        TCHAR *pName,
        LPUNKNOWN pUnk,
        CAsyncStream *pStream,
        HRESULT *phr);
    ~CAsyncReader();  

    
    int GetPinCount();
    CBasePin *GetPin(int n);

    
    const CMediaType *LoadType() const
    {
        return &m_mt;
    }

    virtual HRESULT Connect(
        IPin * pReceivePin,
        const AM_MEDIA_TYPE *pmt   
    )
    {
        return m_OutputPin.CBasePin::Connect(pReceivePin, pmt);
    }
};    

#endif 
