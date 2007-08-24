/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
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

#ifndef __iecooks_h__
#define __iecooks_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IFDMIECookiesBHO_FWD_DEFINED__
#define __IFDMIECookiesBHO_FWD_DEFINED__
typedef interface IFDMIECookiesBHO IFDMIECookiesBHO;
#endif 	  

#ifndef __IFDMIEStat_FWD_DEFINED__
#define __IFDMIEStat_FWD_DEFINED__
typedef interface IFDMIEStat IFDMIEStat;
#endif 	  

#ifndef __FDMIECookiesBHO_FWD_DEFINED__
#define __FDMIECookiesBHO_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMIECookiesBHO FDMIECookiesBHO;
#else
typedef struct FDMIECookiesBHO FDMIECookiesBHO;
#endif 

#endif 	  

#ifndef __FDMIEStat_FWD_DEFINED__
#define __FDMIEStat_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMIEStat FDMIEStat;
#else
typedef struct FDMIEStat FDMIEStat;
#endif 

#endif 	    

#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFDMIECookiesBHO_INTERFACE_DEFINED__
#define __IFDMIECookiesBHO_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMIECookiesBHO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40D3F599-74F0-44D3-B059-76C0F12C0D6E")
    IFDMIECookiesBHO : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE ProgressChange( 
            long Progress,
            long ProgressMax) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE BeforeNavigate2( 
            IDispatch __RPC_FAR *__MIDL_0011,
            VARIANT __RPC_FAR *url,
            VARIANT __RPC_FAR *flags,
            VARIANT __RPC_FAR *tfn,
            VARIANT __RPC_FAR *pd,
            VARIANT __RPC_FAR *headers,
            VARIANT_BOOL __RPC_FAR *bCancel) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE DownloadBegin( void) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE DownloadComplete( void) = 0;
        
    };
    
#else 	

    typedef struct IFDMIECookiesBHOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMIECookiesBHO __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMIECookiesBHO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMIECookiesBHO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMIECookiesBHO __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMIECookiesBHO __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMIECookiesBHO __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMIECookiesBHO __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProgressChange )( 
            IFDMIECookiesBHO __RPC_FAR * This,
            long Progress,
            long ProgressMax);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeforeNavigate2 )( 
            IFDMIECookiesBHO __RPC_FAR * This,
            IDispatch __RPC_FAR *__MIDL_0011,
            VARIANT __RPC_FAR *url,
            VARIANT __RPC_FAR *flags,
            VARIANT __RPC_FAR *tfn,
            VARIANT __RPC_FAR *pd,
            VARIANT __RPC_FAR *headers,
            VARIANT_BOOL __RPC_FAR *bCancel);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadBegin )( 
            IFDMIECookiesBHO __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadComplete )( 
            IFDMIECookiesBHO __RPC_FAR * This);
        
        END_INTERFACE
    } IFDMIECookiesBHOVtbl;

    interface IFDMIECookiesBHO
    {
        CONST_VTBL struct IFDMIECookiesBHOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMIECookiesBHO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMIECookiesBHO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMIECookiesBHO_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMIECookiesBHO_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMIECookiesBHO_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMIECookiesBHO_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMIECookiesBHO_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMIECookiesBHO_ProgressChange(This,Progress,ProgressMax)	\
    (This)->lpVtbl -> ProgressChange(This,Progress,ProgressMax)

#define IFDMIECookiesBHO_BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)	\
    (This)->lpVtbl -> BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)

#define IFDMIECookiesBHO_DownloadBegin(This)	\
    (This)->lpVtbl -> DownloadBegin(This)

#define IFDMIECookiesBHO_DownloadComplete(This)	\
    (This)->lpVtbl -> DownloadComplete(This)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMIECookiesBHO_ProgressChange_Proxy( 
    IFDMIECookiesBHO __RPC_FAR * This,
    long Progress,
    long ProgressMax);  

void __RPC_STUB IFDMIECookiesBHO_ProgressChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIECookiesBHO_BeforeNavigate2_Proxy( 
    IFDMIECookiesBHO __RPC_FAR * This,
    IDispatch __RPC_FAR *__MIDL_0011,
    VARIANT __RPC_FAR *url,
    VARIANT __RPC_FAR *flags,
    VARIANT __RPC_FAR *tfn,
    VARIANT __RPC_FAR *pd,
    VARIANT __RPC_FAR *headers,
    VARIANT_BOOL __RPC_FAR *bCancel);  

void __RPC_STUB IFDMIECookiesBHO_BeforeNavigate2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIECookiesBHO_DownloadBegin_Proxy( 
    IFDMIECookiesBHO __RPC_FAR * This);  

void __RPC_STUB IFDMIECookiesBHO_DownloadBegin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIECookiesBHO_DownloadComplete_Proxy( 
    IFDMIECookiesBHO __RPC_FAR * This);  

void __RPC_STUB IFDMIECookiesBHO_DownloadComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	  

#ifndef __IFDMIEStat_INTERFACE_DEFINED__
#define __IFDMIEStat_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMIEStat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("64844F4D-492F-429E-881C-D7F106259738")
    IFDMIEStat : public IDispatch
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE get_DownloadCount( 
             long __RPC_FAR *pVal) = 0;
        
    };
    
#else 	

    typedef struct IFDMIEStatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMIEStat __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMIEStat __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMIEStat __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMIEStat __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMIEStat __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMIEStat __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMIEStat __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DownloadCount )( 
            IFDMIEStat __RPC_FAR * This,
             long __RPC_FAR *pVal);
        
        END_INTERFACE
    } IFDMIEStatVtbl;

    interface IFDMIEStat
    {
        CONST_VTBL struct IFDMIEStatVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMIEStat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMIEStat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMIEStat_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMIEStat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMIEStat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMIEStat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMIEStat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMIEStat_get_DownloadCount(This,pVal)	\
    (This)->lpVtbl -> get_DownloadCount(This,pVal)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMIEStat_get_DownloadCount_Proxy( 
    IFDMIEStat __RPC_FAR * This,
     long __RPC_FAR *pVal);  

void __RPC_STUB IFDMIEStat_get_DownloadCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);    

#endif 	    

#ifndef __IECOOKSLib_LIBRARY_DEFINED__
#define __IECOOKSLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_IECOOKSLib;

EXTERN_C const CLSID CLSID_FDMIECookiesBHO;

#ifdef __cplusplus

class DECLSPEC_UUID("CC59E0F9-7E43-44FA-9FAA-8377850BF205")
FDMIECookiesBHO;
#endif

EXTERN_C const CLSID CLSID_FDMIEStat;

#ifdef __cplusplus

class DECLSPEC_UUID("D4D6EAB8-009E-4EED-814E-4891D7E1741B")
FDMIEStat;
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
