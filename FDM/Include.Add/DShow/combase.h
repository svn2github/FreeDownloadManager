/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                        

#ifndef __COMBASE__
#define __COMBASE__    

typedef REGPINTYPES
AMOVIESETUP_MEDIATYPE, * PAMOVIESETUP_MEDIATYPE, * FAR LPAMOVIESETUP_MEDIATYPE;

typedef REGFILTERPINS
AMOVIESETUP_PIN, * PAMOVIESETUP_PIN, * FAR LPAMOVIESETUP_PIN;

typedef struct _AMOVIESETUP_FILTER
{
  const CLSID * clsID;
  const WCHAR * strName;
  DWORD      dwMerit;
  UINT       nPins;
  const AMOVIESETUP_PIN * lpPin;
}
AMOVIESETUP_FILTER, * PAMOVIESETUP_FILTER, * FAR LPAMOVIESETUP_FILTER;    

extern HINSTANCE g_hInst;    

extern DWORD g_amPlatform;
extern OSVERSIONINFO g_osInfo;         

#ifndef INONDELEGATINGUNKNOWN_DEFINED
DECLARE_INTERFACE(INonDelegatingUnknown)
{
    STDMETHOD(NonDelegatingQueryInterface) (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG, NonDelegatingAddRef)(THIS) PURE;
    STDMETHOD_(ULONG, NonDelegatingRelease)(THIS) PURE;
};
#define INONDELEGATINGUNKNOWN_DEFINED
#endif

typedef INonDelegatingUnknown *PNDUNKNOWN;      

class CBaseObject
{

private:

    
    
    
    CBaseObject(const CBaseObject& objectSrc);          
    void operator=(const CBaseObject& objectSrc);       

private:
    static LONG m_cObjects;     

protected:
#ifdef DEBUG
    DWORD m_dwCookie;           
#endif  

public:

    

    CBaseObject(const TCHAR *pName);
#ifdef UNICODE
    CBaseObject(const char *pName);
#endif
    ~CBaseObject();

    

    static LONG ObjectsActive() {
        return m_cObjects;
    };
};      

class AM_NOVTABLE CUnknown : public INonDelegatingUnknown,
                 public CBaseObject
{
private:
    const LPUNKNOWN m_pUnknown; 

protected:                      
    volatile LONG m_cRef;       

public:

    CUnknown(const TCHAR *pName, LPUNKNOWN pUnk);
    virtual ~CUnknown() {};

    
    
    CUnknown(TCHAR *pName, LPUNKNOWN pUnk,HRESULT *phr);
#ifdef UNICODE
    CUnknown(const char *pName, LPUNKNOWN pUnk);
    CUnknown(char *pName, LPUNKNOWN pUnk,HRESULT *phr);
#endif

    

    LPUNKNOWN GetOwner() const {
        return m_pUnknown;
    };

    

    

    

    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void **);
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
};    

STDAPI GetInterface(LPUNKNOWN pUnk, void **ppv);            

typedef CUnknown *(CALLBACK *LPFNNewCOMObject)(LPUNKNOWN pUnkOuter, HRESULT *phr);  

typedef void (CALLBACK *LPFNInitRoutine)(BOOL bLoading, const CLSID *rclsid);    

class CFactoryTemplate {

public:

    const WCHAR *              m_Name;
    const CLSID *              m_ClsID;
    LPFNNewCOMObject           m_lpfnNew;
    LPFNInitRoutine            m_lpfnInit;
    const AMOVIESETUP_FILTER * m_pAMovieSetup_Filter;

    BOOL IsClassID(REFCLSID rclsid) const {
        return (IsEqualCLSID(*m_ClsID,rclsid));
    };

    CUnknown *CreateInstance(LPUNKNOWN pUnk, HRESULT *phr) const {
        CheckPointer(phr,NULL);
        return m_lpfnNew(pUnk, phr);
    };
};      

#define DECLARE_IUNKNOWN                                        \
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {      \
        return GetOwner()->QueryInterface(riid,ppv);            \
    };                                                          \
    STDMETHODIMP_(ULONG) AddRef() {                             \
        return GetOwner()->AddRef();                            \
    };                                                          \
    STDMETHODIMP_(ULONG) Release() {                            \
        return GetOwner()->Release();                           \
    };    

HINSTANCE	LoadOLEAut32();  

#endif       

