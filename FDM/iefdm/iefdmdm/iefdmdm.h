/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/                    

#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __iefdmdm_h__
#define __iefdmdm_h__

#ifdef __cplusplus
extern "C"{
#endif 

 

#ifndef __IEWGDM_FWD_DEFINED__
#define __IEWGDM_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEWGDM IEWGDM;
#else
typedef struct IEWGDM IEWGDM;
#endif 

#endif 	    

#include "oaidl.h"
#include "ocidl.h"
#include "downloadmgr.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );   

#ifndef __IEFDMDMLib_LIBRARY_DEFINED__
#define __IEFDMDMLib_LIBRARY_DEFINED__  

   

EXTERN_C const IID LIBID_IEFDMDMLib;

EXTERN_C const CLSID CLSID_IEWGDM;

#ifdef __cplusplus

class DECLSPEC_UUID("57BDEE5A-1E29-4CFD-AEE7-EF32118EB6D6")
IEWGDM;
#endif
#endif         

#ifdef __cplusplus
}
#endif

#endif
