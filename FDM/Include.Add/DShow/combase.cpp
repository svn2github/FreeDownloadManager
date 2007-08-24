/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#include <streams.h>
#pragma warning( disable : 4514 )         

LONG CBaseObject::m_cObjects = 0;      

CBaseObject::CBaseObject(const TCHAR *pName)
{
    
    InterlockedIncrement(&m_cObjects);

#ifdef DEBUG

#ifdef UNICODE
    m_dwCookie = DbgRegisterObjectCreation(0, pName);
#else
    m_dwCookie = DbgRegisterObjectCreation(pName, 0);
#endif

#endif
}

#ifdef UNICODE
CBaseObject::CBaseObject(const char *pName)
{
    
    InterlockedIncrement(&m_cObjects);

#ifdef DEBUG
    m_dwCookie = DbgRegisterObjectCreation(pName, 0);
#endif
}
#endif

HINSTANCE	hlibOLEAut32;    

CBaseObject::~CBaseObject()
{
    
    if (InterlockedDecrement(&m_cObjects) == 0) {
	if (hlibOLEAut32) {
	    FreeLibrary(hlibOLEAut32);

	    hlibOLEAut32 = 0;
	}
    };  

#ifdef DEBUG
    DbgRegisterObjectDestruction(m_dwCookie);
#endif
}

static const TCHAR szOle32Aut[]   = TEXT("OleAut32.dll");

HINSTANCE LoadOLEAut32()
{
    if (hlibOLEAut32 == 0) {

	hlibOLEAut32 = LoadLibrary(szOle32Aut);
    }

    return hlibOLEAut32;
}        

#pragma warning( disable : 4355 4100 )
CUnknown::CUnknown(const TCHAR *pName, LPUNKNOWN pUnk)
: CBaseObject(pName)      

, m_cRef(0)      

, m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>(this) ) )
 
 
 
 
{
    
}    

CUnknown::CUnknown(TCHAR *pName, LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseObject(pName),
    m_cRef(0),
    m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>(this) ) )
{
}

#ifdef UNICODE
CUnknown::CUnknown(const CHAR *pName, LPUNKNOWN pUnk)
: CBaseObject(pName), m_cRef(0),
    m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>(this) ) )
{ }

CUnknown::CUnknown(CHAR *pName, LPUNKNOWN pUnk,HRESULT *phr) :
    CBaseObject(pName), m_cRef(0),
    m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<LPUNKNOWN>( static_cast<PNDUNKNOWN>(this) ) )
{ }

#endif

#pragma warning( default : 4355 4100 )      

STDMETHODIMP CUnknown::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    CheckPointer(ppv,E_POINTER);
    ValidateReadWritePtr(ppv,sizeof(PVOID));

    

    if (riid == IID_IUnknown) {
        GetInterface((LPUNKNOWN) (PNDUNKNOWN) this, ppv);
        return NOERROR;
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}          

template<class T> inline static T ourmax( const T & a, const T & b )
{
    return a > b ? a : b;
}    

STDMETHODIMP_(ULONG) CUnknown::NonDelegatingAddRef()
{
    LONG lRef = InterlockedIncrement( &m_cRef );
    ASSERT(lRef > 0);
    DbgLog((LOG_MEMORY,3,TEXT("    Obj %d ref++ = %d"),
           m_dwCookie, m_cRef));
    return ourmax(ULONG(m_cRef), 1ul);
}      

STDMETHODIMP_(ULONG) CUnknown::NonDelegatingRelease()
{
    

    LONG lRef = InterlockedDecrement( &m_cRef );
    ASSERT(lRef >= 0);

    DbgLog((LOG_MEMORY,3,TEXT("    Object %d ref-- = %d"),
	    m_dwCookie, m_cRef));
    if (lRef == 0) {

        
        
        
        
        
        
        
        
        
        
        

        m_cRef++;

        delete this;
        return ULONG(0);
    } else {
        return ourmax(ULONG(m_cRef), 1ul);
    }
}      

STDAPI GetInterface(LPUNKNOWN pUnk, void **ppv)
{
    CheckPointer(ppv, E_POINTER);
    *ppv = pUnk;
    pUnk->AddRef();
    return NOERROR;
}      

BOOL WINAPI IsEqualObject(IUnknown *pFirst, IUnknown *pSecond)
{
    
    if (pFirst == pSecond) {
        return TRUE;
    }
    
    LPUNKNOWN pUnknown1;     
    LPUNKNOWN pUnknown2;     
    HRESULT hr;              

    ASSERT(pFirst);
    ASSERT(pSecond);

    

    hr = pFirst->QueryInterface(IID_IUnknown,(void **) &pUnknown1);
    ASSERT(SUCCEEDED(hr));
    ASSERT(pUnknown1);

    hr = pSecond->QueryInterface(IID_IUnknown,(void **) &pUnknown2);
    ASSERT(SUCCEEDED(hr));
    ASSERT(pUnknown2);

    

    pUnknown1->Release();
    pUnknown2->Release();
    return (pUnknown1 == pUnknown2);
}

