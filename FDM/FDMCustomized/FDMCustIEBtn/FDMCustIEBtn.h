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

#ifndef __FDMCustIEBtn_h__
#define __FDMCustIEBtn_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IFDMCustButton_FWD_DEFINED__
#define __IFDMCustButton_FWD_DEFINED__
typedef interface IFDMCustButton IFDMCustButton;
#endif 	  

#ifndef __FDMCustButton_FWD_DEFINED__
#define __FDMCustButton_FWD_DEFINED__

#ifdef __cplusplus
typedef class FDMCustButton FDMCustButton;
#else
typedef struct FDMCustButton FDMCustButton;
#endif 

#endif 	    

#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFDMCustButton_INTERFACE_DEFINED__
#define __IFDMCustButton_INTERFACE_DEFINED__  

   

EXTERN_C const IID IID_IFDMCustButton;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EAC5AE32-9C01-4F1D-9E01-5F0F4D310014")
    IFDMCustButton : public IUnknown
    {
    public:
    };
    
#else 	

    typedef struct IFDMCustButtonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFDMCustButton __RPC_FAR * This,
             REFIID riid,
             void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFDMCustButton __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFDMCustButton __RPC_FAR * This);
        
        END_INTERFACE
    } IFDMCustButtonVtbl;

    interface IFDMCustButton
    {
        CONST_VTBL struct IFDMCustButtonVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS  

#define IFDMCustButton_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFDMCustButton_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFDMCustButton_Release(This)	\
    (This)->lpVtbl -> Release(This)  

#endif   

#endif 	      

#endif 	    

#ifndef __FDMCUSTIEBTNLib_LIBRARY_DEFINED__
#define __FDMCUSTIEBTNLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_FDMCUSTIEBTNLib;

EXTERN_C const CLSID CLSID_FDMCustButton;

#ifdef __cplusplus

class DECLSPEC_UUID("DAC360AF-9FD0-432D-B2F2-ED3220F4CAD9")
FDMCustButton;
#endif
#endif         

#ifdef __cplusplus
}
#endif

#endif
