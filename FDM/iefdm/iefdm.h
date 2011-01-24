/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/                    

#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif 

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif 

#ifndef __iefdm_h__
#define __iefdm_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IWgBHO_FWD_DEFINED__
#define __IWgBHO_FWD_DEFINED__
typedef interface IWgBHO IWgBHO;
#endif 	  

#ifndef __IEWGMonitor_FWD_DEFINED__
#define __IEWGMonitor_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEWGMonitor IEWGMonitor;
#else
typedef struct IEWGMonitor IEWGMonitor;
#endif 

#endif 	  

#ifndef __WgBHO_FWD_DEFINED__
#define __WgBHO_FWD_DEFINED__

#ifdef __cplusplus
typedef class WgBHO WgBHO;
#else
typedef struct WgBHO WgBHO;
#endif 

#endif 	    

#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IWgBHO_INTERFACE_DEFINED__
#define __IWgBHO_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IWgBHO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DBDC6466-1232-4511-B1A5-0ABC79EB8291")
    IWgBHO : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE BeforeNavigate2( 
            IDispatch __RPC_FAR *__MIDL_0011,
            VARIANT __RPC_FAR *url,
            VARIANT __RPC_FAR *flags,
            VARIANT __RPC_FAR *tfn,
            VARIANT __RPC_FAR *pd,
            VARIANT __RPC_FAR *headers,
            VARIANT_BOOL __RPC_FAR *bCancel) = 0;
        
    };
    
#else 	

    typedef struct IWgBHOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWgBHO __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWgBHO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWgBHO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWgBHO __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWgBHO __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWgBHO __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWgBHO __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeforeNavigate2 )( 
            IWgBHO __RPC_FAR * This,
            IDispatch __RPC_FAR *__MIDL_0011,
            VARIANT __RPC_FAR *url,
            VARIANT __RPC_FAR *flags,
            VARIANT __RPC_FAR *tfn,
            VARIANT __RPC_FAR *pd,
            VARIANT __RPC_FAR *headers,
            VARIANT_BOOL __RPC_FAR *bCancel);
        
        END_INTERFACE
    } IWgBHOVtbl;

    interface IWgBHO
    {
        CONST_VTBL struct IWgBHOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IWgBHO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWgBHO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWgBHO_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IWgBHO_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWgBHO_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWgBHO_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWgBHO_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IWgBHO_BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)	\
    (This)->lpVtbl -> BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IWgBHO_BeforeNavigate2_Proxy( 
    IWgBHO __RPC_FAR * This,
    IDispatch __RPC_FAR *__MIDL_0011,
    VARIANT __RPC_FAR *url,
    VARIANT __RPC_FAR *flags,
    VARIANT __RPC_FAR *tfn,
    VARIANT __RPC_FAR *pd,
    VARIANT __RPC_FAR *headers,
    VARIANT_BOOL __RPC_FAR *bCancel);  

void __RPC_STUB IWgBHO_BeforeNavigate2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	    

#ifndef __IEFDMLib_LIBRARY_DEFINED__
#define __IEFDMLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_IEFDMLib;

EXTERN_C const CLSID CLSID_IEWGMonitor;

#ifdef __cplusplus

class DECLSPEC_UUID("1985F8EF-B021-4A44-9BE1-343D0B4E0911")
IEWGMonitor;
#endif

EXTERN_C const CLSID CLSID_WgBHO;

#ifdef __cplusplus

class DECLSPEC_UUID("67E9834D-B226-49E6-B6F6-85AA64E14BA3")
WgBHO;
#endif
#endif     

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * );     

#ifdef __cplusplus
}
#endif

#endif
