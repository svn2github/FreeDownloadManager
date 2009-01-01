/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                                          

#ifndef __TRANSIP__
#define __TRANSIP__          

class CTransInPlaceFilter;                    

class CTransInPlaceInputPin : public CTransformInputPin
{

protected:
    CTransInPlaceFilter * const m_pTIPFilter;    
    BOOL                 m_bReadOnly;    

public:

    CTransInPlaceInputPin(
        TCHAR               *pObjectName,
        CTransInPlaceFilter *pFilter,
        HRESULT             *phr,
        LPCWSTR              pName);

    

    
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );

    
    HRESULT CheckMediaType(const CMediaType* pmt);

    
    STDMETHODIMP GetAllocator(IMemAllocator ** ppAllocator);

    
    
    STDMETHODIMP NotifyAllocator(IMemAllocator * pAllocator,
                                 BOOL bReadOnly);

    
    
    IMemAllocator * PeekAllocator() const
        {  return m_pAllocator; }

    
    STDMETHODIMP
    CTransInPlaceInputPin::GetAllocatorRequirements(ALLOCATOR_PROPERTIES *pProps);

    inline const BOOL ReadOnly() { return m_bReadOnly ; }

};          

class CTransInPlaceOutputPin : public CTransformOutputPin
{

protected:
    
    CTransInPlaceFilter * const m_pTIPFilter;

public:

    CTransInPlaceOutputPin(
        TCHAR               *pObjectName,
        CTransInPlaceFilter *pFilter,
        HRESULT             *phr,
        LPCWSTR              pName);  

    

    
    
    virtual HRESULT DecideAllocator(IMemInputPin * pPin, IMemAllocator ** pAlloc);

    
    STDMETHODIMP EnumMediaTypes( IEnumMediaTypes **ppEnum );

    
    HRESULT CheckMediaType(const CMediaType* pmt);

    
    
    void SetAllocator(IMemAllocator * pAllocator);

    IMemInputPin * ConnectedIMemInputPin()
        { return m_pInputPin; }

    
    
    IMemAllocator * PeekAllocator() const
        {  return m_pAllocator; }
};    

class AM_NOVTABLE CTransInPlaceFilter : public CTransformFilter
{

public:

    
    

    virtual CBasePin *GetPin(int n);

public:

    
    
    

    CTransInPlaceFilter(TCHAR *, LPUNKNOWN, REFCLSID clsid, HRESULT *,
                        bool bModifiesData = true);
#ifdef UNICODE
    CTransInPlaceFilter(CHAR *, LPUNKNOWN, REFCLSID clsid, HRESULT *,
                        bool bModifiesData = true);
#endif
    
    

    
    
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType)
        {   DbgBreak("CTransInPlaceFilter::GetMediaType should never be called");
            return E_UNEXPECTED;
        }

    
    HRESULT DecideBufferSize(IMemAllocator*, ALLOCATOR_PROPERTIES *);

    
    
    
    
    HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
    {
        return S_OK;
    };  

    
    
    

    HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    
    virtual HRESULT Receive(IMediaSample *pSample);

    
    
    

    virtual HRESULT Transform(IMediaSample *pSample) PURE;

    
      

#ifdef PERF
    
    
    virtual void RegisterPerfId()
         {m_idTransInPlace = MSR_REGISTER(TEXT("TransInPlace"));}
#endif       

protected:

    IMediaSample * CTransInPlaceFilter::Copy(IMediaSample *pSource);

#ifdef PERF
    int m_idTransInPlace;                 
#endif 
    bool  m_bModifiesData;                

    

    friend class CTransInPlaceInputPin;
    friend class CTransInPlaceOutputPin;

    CTransInPlaceInputPin  *InputPin() const
    {
        return (CTransInPlaceInputPin *)m_pInput;
    };
    CTransInPlaceOutputPin *OutputPin() const
    {
        return (CTransInPlaceOutputPin *)m_pOutput;
    };

    
    BOOL TypesMatch()
    {
        return InputPin()->CurrentMediaType() ==
               OutputPin()->CurrentMediaType();
    }

    
    BOOL UsingDifferentAllocators() const
    {
        return InputPin()->PeekAllocator() != OutputPin()->PeekAllocator();
    }
}; 

#endif 

