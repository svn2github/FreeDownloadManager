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

#ifndef __iecooks_h__
#define __iecooks_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IFDMIEBHO_FWD_DEFINED__
#define __IFDMIEBHO_FWD_DEFINED__
typedef interface IFDMIEBHO IFDMIEBHO;
#endif 	  

#ifndef __IFDMIEStat_FWD_DEFINED__
#define __IFDMIEStat_FWD_DEFINED__
typedef interface IFDMIEStat IFDMIEStat;
#endif 	  

#ifndef __FDMIEBHO_FWD_DEFINED__
#define __FDMIEBHO_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMIEBHO FDMIEBHO;
#else
typedef struct FDMIEBHO FDMIEBHO;
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

#ifndef __IFDMIEBHO_INTERFACE_DEFINED__
#define __IFDMIEBHO_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMIEBHO;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40D3F599-74F0-44D3-B059-76C0F12C0D6E")
    IFDMIEBHO : public IDispatch
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
        
        virtual  HRESULT STDMETHODCALLTYPE DocumentComplete( 
             IDispatch __RPC_FAR *pDisp,
             VARIANT __RPC_FAR *URL) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE WindowStateChanged( 
            DWORD dwFlags,
            DWORD dwValidFlagsMask) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE NavigateComplete2( 
             IDispatch __RPC_FAR *pDisp,
             VARIANT __RPC_FAR *URL) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE StatusTextChange( 
             BSTR bstrText) = 0;
        
    };
    
#else 	

    typedef struct IFDMIEBHOVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMIEBHO __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMIEBHO __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMIEBHO __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFDMIEBHO __RPC_FAR * This,
             UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFDMIEBHO __RPC_FAR * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFDMIEBHO __RPC_FAR * This,
             REFIID riid,
             LPOLESTR __RPC_FAR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID __RPC_FAR *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFDMIEBHO __RPC_FAR * This,
             DISPID dispIdMember,
             REFIID riid,
             LCID lcid,
             WORD wFlags,
             DISPPARAMS __RPC_FAR *pDispParams,
             VARIANT __RPC_FAR *pVarResult,
             EXCEPINFO __RPC_FAR *pExcepInfo,
             UINT __RPC_FAR *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ProgressChange )( 
            IFDMIEBHO __RPC_FAR * This,
            long Progress,
            long ProgressMax);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *BeforeNavigate2 )( 
            IFDMIEBHO __RPC_FAR * This,
            IDispatch __RPC_FAR *__MIDL_0011,
            VARIANT __RPC_FAR *url,
            VARIANT __RPC_FAR *flags,
            VARIANT __RPC_FAR *tfn,
            VARIANT __RPC_FAR *pd,
            VARIANT __RPC_FAR *headers,
            VARIANT_BOOL __RPC_FAR *bCancel);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadBegin )( 
            IFDMIEBHO __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DownloadComplete )( 
            IFDMIEBHO __RPC_FAR * This);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DocumentComplete )( 
            IFDMIEBHO __RPC_FAR * This,
             IDispatch __RPC_FAR *pDisp,
             VARIANT __RPC_FAR *URL);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WindowStateChanged )( 
            IFDMIEBHO __RPC_FAR * This,
            DWORD dwFlags,
            DWORD dwValidFlagsMask);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *NavigateComplete2 )( 
            IFDMIEBHO __RPC_FAR * This,
             IDispatch __RPC_FAR *pDisp,
             VARIANT __RPC_FAR *URL);
        
         HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StatusTextChange )( 
            IFDMIEBHO __RPC_FAR * This,
             BSTR bstrText);
        
        END_INTERFACE
    } IFDMIEBHOVtbl;

    interface IFDMIEBHO
    {
        CONST_VTBL struct IFDMIEBHOVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMIEBHO_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMIEBHO_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMIEBHO_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#define IFDMIEBHO_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFDMIEBHO_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFDMIEBHO_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFDMIEBHO_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  

#define IFDMIEBHO_ProgressChange(This,Progress,ProgressMax)	\
    (This)->lpVtbl -> ProgressChange(This,Progress,ProgressMax)

#define IFDMIEBHO_BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)	\
    (This)->lpVtbl -> BeforeNavigate2(This,__MIDL_0011,url,flags,tfn,pd,headers,bCancel)

#define IFDMIEBHO_DownloadBegin(This)	\
    (This)->lpVtbl -> DownloadBegin(This)

#define IFDMIEBHO_DownloadComplete(This)	\
    (This)->lpVtbl -> DownloadComplete(This)

#define IFDMIEBHO_DocumentComplete(This,pDisp,URL)	\
    (This)->lpVtbl -> DocumentComplete(This,pDisp,URL)

#define IFDMIEBHO_WindowStateChanged(This,dwFlags,dwValidFlagsMask)	\
    (This)->lpVtbl -> WindowStateChanged(This,dwFlags,dwValidFlagsMask)

#define IFDMIEBHO_NavigateComplete2(This,pDisp,URL)	\
    (This)->lpVtbl -> NavigateComplete2(This,pDisp,URL)

#define IFDMIEBHO_StatusTextChange(This,bstrText)	\
    (This)->lpVtbl -> StatusTextChange(This,bstrText)

#endif   

#endif 	    

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_ProgressChange_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
    long Progress,
    long ProgressMax);  

void __RPC_STUB IFDMIEBHO_ProgressChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_BeforeNavigate2_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
    IDispatch __RPC_FAR *__MIDL_0011,
    VARIANT __RPC_FAR *url,
    VARIANT __RPC_FAR *flags,
    VARIANT __RPC_FAR *tfn,
    VARIANT __RPC_FAR *pd,
    VARIANT __RPC_FAR *headers,
    VARIANT_BOOL __RPC_FAR *bCancel);  

void __RPC_STUB IFDMIEBHO_BeforeNavigate2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_DownloadBegin_Proxy( 
    IFDMIEBHO __RPC_FAR * This);  

void __RPC_STUB IFDMIEBHO_DownloadBegin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_DownloadComplete_Proxy( 
    IFDMIEBHO __RPC_FAR * This);  

void __RPC_STUB IFDMIEBHO_DownloadComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_DocumentComplete_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
     IDispatch __RPC_FAR *pDisp,
     VARIANT __RPC_FAR *URL);  

void __RPC_STUB IFDMIEBHO_DocumentComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_WindowStateChanged_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
    DWORD dwFlags,
    DWORD dwValidFlagsMask);  

void __RPC_STUB IFDMIEBHO_WindowStateChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_NavigateComplete2_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
     IDispatch __RPC_FAR *pDisp,
     VARIANT __RPC_FAR *URL);  

void __RPC_STUB IFDMIEBHO_NavigateComplete2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);  

 HRESULT STDMETHODCALLTYPE IFDMIEBHO_StatusTextChange_Proxy( 
    IFDMIEBHO __RPC_FAR * This,
     BSTR bstrText);  

void __RPC_STUB IFDMIEBHO_StatusTextChange_Stub(
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

#ifndef __IEFDMBHOLib_LIBRARY_DEFINED__
#define __IEFDMBHOLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_IEFDMBHOLib;

EXTERN_C const CLSID CLSID_FDMIEBHO;

#ifdef __cplusplus

class DECLSPEC_UUID("CC59E0F9-7E43-44FA-9FAA-8377850BF205")
FDMIEBHO;
#endif

EXTERN_C const CLSID CLSID_FDMIEStat;

#ifdef __cplusplus

class DECLSPEC_UUID("D4D6EAB8-009E-4EED-814E-4891D7E1741B")
FDMIEStat;
#endif
#endif     

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * );     

#ifdef __cplusplus
}
#endif

#endif
