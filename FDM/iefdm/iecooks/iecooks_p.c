/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Dec 30 01:00:19 2008
 */
/* Compiler settings for D:\VCW\FDM\FDM\iefdm\iecooks\iecooks.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "iecooks.h"

#define TYPE_FORMAT_STRING_SIZE   1019                              
#define PROC_FORMAT_STRING_SIZE   205                               

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IFDMIECookiesBHO, ver. 0.0,
   GUID={0x40D3F599,0x74F0,0x44D3,{0xB0,0x59,0x76,0xC0,0xF1,0x2C,0x0D,0x6E}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IFDMIECookiesBHO_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IFDMIECookiesBHO_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    34,
    98,
    120,
    142
    };

static const MIDL_SERVER_INFO IFDMIECookiesBHO_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IFDMIECookiesBHO_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IFDMIECookiesBHO_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IFDMIECookiesBHO_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(12) _IFDMIECookiesBHOProxyVtbl = 
{
    &IFDMIECookiesBHO_ProxyInfo,
    &IID_IFDMIECookiesBHO,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IFDMIECookiesBHO::ProgressChange */ ,
    (void *)-1 /* IFDMIECookiesBHO::BeforeNavigate2 */ ,
    (void *)-1 /* IFDMIECookiesBHO::DownloadBegin */ ,
    (void *)-1 /* IFDMIECookiesBHO::DownloadComplete */ ,
    (void *)-1 /* IFDMIECookiesBHO::DocumentComplete */
};


static const PRPC_STUB_FUNCTION IFDMIECookiesBHO_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IFDMIECookiesBHOStubVtbl =
{
    &IID_IFDMIECookiesBHO,
    &IFDMIECookiesBHO_ServerInfo,
    12,
    &IFDMIECookiesBHO_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IFDMIEStat, ver. 0.0,
   GUID={0x64844F4D,0x492F,0x429E,{0x88,0x1C,0xD7,0xF1,0x06,0x25,0x97,0x38}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IFDMIEStat_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static const unsigned short IFDMIEStat_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    176
    };

static const MIDL_SERVER_INFO IFDMIEStat_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IFDMIEStat_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IFDMIEStat_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IFDMIEStat_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(8) _IFDMIEStatProxyVtbl = 
{
    &IFDMIEStat_ProxyInfo,
    &IID_IFDMIEStat,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IFDMIEStat::get_DownloadCount */
};


static const PRPC_STUB_FUNCTION IFDMIEStat_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IFDMIEStatStubVtbl =
{
    &IID_IFDMIEStat,
    &IFDMIEStat_ServerInfo,
    8,
    &IFDMIEStat_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure ProgressChange */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 10 */	NdrFcShort( 0x10 ),	/* 16 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter Progress */

/* 16 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ProgressMax */

/* 22 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 28 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 30 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 32 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure BeforeNavigate2 */

/* 34 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 36 */	NdrFcLong( 0x0 ),	/* 0 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 42 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 44 */	NdrFcShort( 0x6 ),	/* 6 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
/* 48 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter __MIDL_0011 */

/* 50 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 52 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 54 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter url */

/* 56 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 60 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Parameter flags */

/* 62 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 64 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 66 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Parameter tfn */

/* 68 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 70 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 72 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Parameter pd */

/* 74 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 76 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 78 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Parameter headers */

/* 80 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 82 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 84 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Parameter bCancel */

/* 86 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
/* 88 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 90 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 92 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 94 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 96 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DownloadBegin */

/* 98 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 100 */	NdrFcLong( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 106 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 112 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 116 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DownloadComplete */

/* 120 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 128 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 134 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 136 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 138 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 140 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DocumentComplete */

/* 142 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 144 */	NdrFcLong( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 150 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 154 */	NdrFcShort( 0x8 ),	/* 8 */
/* 156 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pDisp */

/* 158 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 160 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 162 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter URL */

/* 164 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 166 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 168 */	NdrFcShort( 0x3e8 ),	/* Type Offset=1000 */

	/* Return value */

/* 170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 172 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_DownloadCount */

/* 176 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 184 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	NdrFcShort( 0x10 ),	/* 16 */
/* 190 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 192 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 194 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 196 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 200 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0x20400 ),	/* 132096 */
/*  8 */	NdrFcShort( 0x0 ),	/* 0 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 14 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 16 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 18 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 20 */	
			0x11, 0x0,	/* FC_RP */
/* 22 */	NdrFcShort( 0x3d2 ),	/* Offset= 978 (1000) */
/* 24 */	
			0x12, 0x0,	/* FC_UP */
/* 26 */	NdrFcShort( 0x3ba ),	/* Offset= 954 (980) */
/* 28 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 30 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 32 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 34 */	NdrFcShort( 0x2 ),	/* Offset= 2 (36) */
/* 36 */	NdrFcShort( 0x10 ),	/* 16 */
/* 38 */	NdrFcShort( 0x2f ),	/* 47 */
/* 40 */	NdrFcLong( 0x14 ),	/* 20 */
/* 44 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 46 */	NdrFcLong( 0x3 ),	/* 3 */
/* 50 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 52 */	NdrFcLong( 0x11 ),	/* 17 */
/* 56 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 58 */	NdrFcLong( 0x2 ),	/* 2 */
/* 62 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 64 */	NdrFcLong( 0x4 ),	/* 4 */
/* 68 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 70 */	NdrFcLong( 0x5 ),	/* 5 */
/* 74 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 76 */	NdrFcLong( 0xb ),	/* 11 */
/* 80 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 82 */	NdrFcLong( 0xa ),	/* 10 */
/* 86 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 88 */	NdrFcLong( 0x6 ),	/* 6 */
/* 92 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (324) */
/* 94 */	NdrFcLong( 0x7 ),	/* 7 */
/* 98 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 100 */	NdrFcLong( 0x8 ),	/* 8 */
/* 104 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (330) */
/* 106 */	NdrFcLong( 0xd ),	/* 13 */
/* 110 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (354) */
/* 112 */	NdrFcLong( 0x9 ),	/* 9 */
/* 116 */	NdrFcShort( 0xffffff8e ),	/* Offset= -114 (2) */
/* 118 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 122 */	NdrFcShort( 0xfa ),	/* Offset= 250 (372) */
/* 124 */	NdrFcLong( 0x24 ),	/* 36 */
/* 128 */	NdrFcShort( 0x308 ),	/* Offset= 776 (904) */
/* 130 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 134 */	NdrFcShort( 0x302 ),	/* Offset= 770 (904) */
/* 136 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 140 */	NdrFcShort( 0x300 ),	/* Offset= 768 (908) */
/* 142 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 146 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (912) */
/* 148 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 152 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (916) */
/* 154 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 158 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (920) */
/* 160 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 164 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (924) */
/* 166 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 170 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (928) */
/* 172 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 176 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (912) */
/* 178 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 182 */	NdrFcShort( 0x2de ),	/* Offset= 734 (916) */
/* 184 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 188 */	NdrFcShort( 0x2e8 ),	/* Offset= 744 (932) */
/* 190 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 194 */	NdrFcShort( 0x2de ),	/* Offset= 734 (928) */
/* 196 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 200 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (936) */
/* 202 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 206 */	NdrFcShort( 0x2de ),	/* Offset= 734 (940) */
/* 208 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 212 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (944) */
/* 214 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 218 */	NdrFcShort( 0x2da ),	/* Offset= 730 (948) */
/* 220 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 224 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (952) */
/* 226 */	NdrFcLong( 0x10 ),	/* 16 */
/* 230 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 232 */	NdrFcLong( 0x12 ),	/* 18 */
/* 236 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 238 */	NdrFcLong( 0x13 ),	/* 19 */
/* 242 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 244 */	NdrFcLong( 0x15 ),	/* 21 */
/* 248 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 250 */	NdrFcLong( 0x16 ),	/* 22 */
/* 254 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 256 */	NdrFcLong( 0x17 ),	/* 23 */
/* 260 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 262 */	NdrFcLong( 0xe ),	/* 14 */
/* 266 */	NdrFcShort( 0x2b6 ),	/* Offset= 694 (960) */
/* 268 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 272 */	NdrFcShort( 0x2bc ),	/* Offset= 700 (972) */
/* 274 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 278 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (976) */
/* 280 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 284 */	NdrFcShort( 0x274 ),	/* Offset= 628 (912) */
/* 286 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 290 */	NdrFcShort( 0x272 ),	/* Offset= 626 (916) */
/* 292 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 296 */	NdrFcShort( 0x270 ),	/* Offset= 624 (920) */
/* 298 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 302 */	NdrFcShort( 0x266 ),	/* Offset= 614 (916) */
/* 304 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 308 */	NdrFcShort( 0x260 ),	/* Offset= 608 (916) */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* Offset= 0 (314) */
/* 316 */	NdrFcLong( 0x1 ),	/* 1 */
/* 320 */	NdrFcShort( 0x0 ),	/* Offset= 0 (320) */
/* 322 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (321) */
/* 324 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 328 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 330 */	
			0x12, 0x0,	/* FC_UP */
/* 332 */	NdrFcShort( 0xc ),	/* Offset= 12 (344) */
/* 334 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 336 */	NdrFcShort( 0x2 ),	/* 2 */
/* 338 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 340 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 342 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 344 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 346 */	NdrFcShort( 0x8 ),	/* 8 */
/* 348 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (334) */
/* 350 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 352 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 354 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 356 */	NdrFcLong( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 364 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 366 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 368 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 370 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 372 */	
			0x12, 0x10,	/* FC_UP */
/* 374 */	NdrFcShort( 0x2 ),	/* Offset= 2 (376) */
/* 376 */	
			0x12, 0x0,	/* FC_UP */
/* 378 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (886) */
/* 380 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 382 */	NdrFcShort( 0x18 ),	/* 24 */
/* 384 */	NdrFcShort( 0xa ),	/* 10 */
/* 386 */	NdrFcLong( 0x8 ),	/* 8 */
/* 390 */	NdrFcShort( 0x58 ),	/* Offset= 88 (478) */
/* 392 */	NdrFcLong( 0xd ),	/* 13 */
/* 396 */	NdrFcShort( 0x78 ),	/* Offset= 120 (516) */
/* 398 */	NdrFcLong( 0x9 ),	/* 9 */
/* 402 */	NdrFcShort( 0x94 ),	/* Offset= 148 (550) */
/* 404 */	NdrFcLong( 0xc ),	/* 12 */
/* 408 */	NdrFcShort( 0xbc ),	/* Offset= 188 (596) */
/* 410 */	NdrFcLong( 0x24 ),	/* 36 */
/* 414 */	NdrFcShort( 0x114 ),	/* Offset= 276 (690) */
/* 416 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 420 */	NdrFcShort( 0x130 ),	/* Offset= 304 (724) */
/* 422 */	NdrFcLong( 0x10 ),	/* 16 */
/* 426 */	NdrFcShort( 0x148 ),	/* Offset= 328 (754) */
/* 428 */	NdrFcLong( 0x2 ),	/* 2 */
/* 432 */	NdrFcShort( 0x160 ),	/* Offset= 352 (784) */
/* 434 */	NdrFcLong( 0x3 ),	/* 3 */
/* 438 */	NdrFcShort( 0x178 ),	/* Offset= 376 (814) */
/* 440 */	NdrFcLong( 0x14 ),	/* 20 */
/* 444 */	NdrFcShort( 0x190 ),	/* Offset= 400 (844) */
/* 446 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (445) */
/* 448 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 450 */	NdrFcShort( 0x4 ),	/* 4 */
/* 452 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 458 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 460 */	NdrFcShort( 0x4 ),	/* 4 */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x1 ),	/* 1 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	0x12, 0x0,	/* FC_UP */
/* 472 */	NdrFcShort( 0xffffff80 ),	/* Offset= -128 (344) */
/* 474 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 476 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 478 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 480 */	NdrFcShort( 0x8 ),	/* 8 */
/* 482 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 484 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 486 */	NdrFcShort( 0x4 ),	/* 4 */
/* 488 */	NdrFcShort( 0x4 ),	/* 4 */
/* 490 */	0x11, 0x0,	/* FC_RP */
/* 492 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (448) */
/* 494 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 496 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 498 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 502 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 510 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 512 */	NdrFcShort( 0xffffff62 ),	/* Offset= -158 (354) */
/* 514 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 516 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 518 */	NdrFcShort( 0x8 ),	/* 8 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x6 ),	/* Offset= 6 (528) */
/* 524 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 526 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 528 */	
			0x11, 0x0,	/* FC_RP */
/* 530 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (498) */
/* 532 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 534 */	NdrFcShort( 0x0 ),	/* 0 */
/* 536 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 544 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 546 */	NdrFcShort( 0xfffffde0 ),	/* Offset= -544 (2) */
/* 548 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 550 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	NdrFcShort( 0x6 ),	/* Offset= 6 (562) */
/* 558 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 560 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 562 */	
			0x11, 0x0,	/* FC_RP */
/* 564 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (532) */
/* 566 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 568 */	NdrFcShort( 0x4 ),	/* 4 */
/* 570 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 576 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 578 */	NdrFcShort( 0x4 ),	/* 4 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x1 ),	/* 1 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	0x12, 0x0,	/* FC_UP */
/* 590 */	NdrFcShort( 0xfffffdb2 ),	/* Offset= -590 (0) */
/* 592 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 594 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 596 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x6 ),	/* Offset= 6 (608) */
/* 604 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 606 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 608 */	
			0x11, 0x0,	/* FC_RP */
/* 610 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (566) */
/* 612 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 614 */	NdrFcLong( 0x2f ),	/* 47 */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 624 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 626 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 628 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 630 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 632 */	NdrFcShort( 0x1 ),	/* 1 */
/* 634 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 636 */	NdrFcShort( 0x4 ),	/* 4 */
/* 638 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 640 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 642 */	NdrFcShort( 0x10 ),	/* 16 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	NdrFcShort( 0xa ),	/* Offset= 10 (656) */
/* 648 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 650 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 652 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (612) */
/* 654 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 656 */	
			0x12, 0x0,	/* FC_UP */
/* 658 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (630) */
/* 660 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 662 */	NdrFcShort( 0x4 ),	/* 4 */
/* 664 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 670 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 672 */	NdrFcShort( 0x4 ),	/* 4 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x1 ),	/* 1 */
/* 678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 682 */	0x12, 0x0,	/* FC_UP */
/* 684 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (640) */
/* 686 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 688 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 690 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x6 ),	/* Offset= 6 (702) */
/* 698 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 700 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 702 */	
			0x11, 0x0,	/* FC_RP */
/* 704 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (660) */
/* 706 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 708 */	NdrFcShort( 0x8 ),	/* 8 */
/* 710 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 712 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 714 */	NdrFcShort( 0x10 ),	/* 16 */
/* 716 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 718 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 720 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (706) */
			0x5b,		/* FC_END */
/* 724 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 726 */	NdrFcShort( 0x18 ),	/* 24 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0xa ),	/* Offset= 10 (740) */
/* 732 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 734 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 736 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (712) */
/* 738 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 740 */	
			0x11, 0x0,	/* FC_RP */
/* 742 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (498) */
/* 744 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 746 */	NdrFcShort( 0x1 ),	/* 1 */
/* 748 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */
/* 752 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 754 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 760 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 762 */	NdrFcShort( 0x4 ),	/* 4 */
/* 764 */	NdrFcShort( 0x4 ),	/* 4 */
/* 766 */	0x12, 0x0,	/* FC_UP */
/* 768 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (744) */
/* 770 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 772 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 774 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 776 */	NdrFcShort( 0x2 ),	/* 2 */
/* 778 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 784 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 790 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	NdrFcShort( 0x4 ),	/* 4 */
/* 796 */	0x12, 0x0,	/* FC_UP */
/* 798 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (774) */
/* 800 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 802 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 804 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 806 */	NdrFcShort( 0x4 ),	/* 4 */
/* 808 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 812 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 814 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 818 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 820 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	NdrFcShort( 0x4 ),	/* 4 */
/* 826 */	0x12, 0x0,	/* FC_UP */
/* 828 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (804) */
/* 830 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 832 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 834 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 838 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 844 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 848 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 850 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 852 */	NdrFcShort( 0x4 ),	/* 4 */
/* 854 */	NdrFcShort( 0x4 ),	/* 4 */
/* 856 */	0x12, 0x0,	/* FC_UP */
/* 858 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (834) */
/* 860 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 862 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 864 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 868 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 870 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 872 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 874 */	NdrFcShort( 0x8 ),	/* 8 */
/* 876 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 878 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 880 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 882 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (864) */
/* 884 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 886 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 888 */	NdrFcShort( 0x28 ),	/* 40 */
/* 890 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (872) */
/* 892 */	NdrFcShort( 0x0 ),	/* Offset= 0 (892) */
/* 894 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 896 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 898 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 900 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (380) */
			0x5b,		/* FC_END */
/* 904 */	
			0x12, 0x0,	/* FC_UP */
/* 906 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (640) */
/* 908 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 910 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 912 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 914 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 916 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 918 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 920 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 922 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 924 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 926 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 928 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 930 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 932 */	
			0x12, 0x0,	/* FC_UP */
/* 934 */	NdrFcShort( 0xfffffd9e ),	/* Offset= -610 (324) */
/* 936 */	
			0x12, 0x10,	/* FC_UP */
/* 938 */	NdrFcShort( 0xfffffda0 ),	/* Offset= -608 (330) */
/* 940 */	
			0x12, 0x10,	/* FC_UP */
/* 942 */	NdrFcShort( 0xfffffdb4 ),	/* Offset= -588 (354) */
/* 944 */	
			0x12, 0x10,	/* FC_UP */
/* 946 */	NdrFcShort( 0xfffffc50 ),	/* Offset= -944 (2) */
/* 948 */	
			0x12, 0x10,	/* FC_UP */
/* 950 */	NdrFcShort( 0xfffffdbe ),	/* Offset= -578 (372) */
/* 952 */	
			0x12, 0x10,	/* FC_UP */
/* 954 */	NdrFcShort( 0x2 ),	/* Offset= 2 (956) */
/* 956 */	
			0x12, 0x0,	/* FC_UP */
/* 958 */	NdrFcShort( 0xfffffc42 ),	/* Offset= -958 (0) */
/* 960 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 962 */	NdrFcShort( 0x10 ),	/* 16 */
/* 964 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 966 */	0x1,		/* FC_BYTE */
			0x38,		/* FC_ALIGNM4 */
/* 968 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 970 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 972 */	
			0x12, 0x0,	/* FC_UP */
/* 974 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (960) */
/* 976 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 978 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 980 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 982 */	NdrFcShort( 0x20 ),	/* 32 */
/* 984 */	NdrFcShort( 0x0 ),	/* 0 */
/* 986 */	NdrFcShort( 0x0 ),	/* Offset= 0 (986) */
/* 988 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 990 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 992 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 994 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 996 */	NdrFcShort( 0xfffffc38 ),	/* Offset= -968 (28) */
/* 998 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1000 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1008 */	NdrFcShort( 0xfffffc28 ),	/* Offset= -984 (24) */
/* 1010 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1012 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1014 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1016 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

const CInterfaceProxyVtbl * _iecooks_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IFDMIEStatProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IFDMIECookiesBHOProxyVtbl,
    0
};

const CInterfaceStubVtbl * _iecooks_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IFDMIEStatStubVtbl,
    ( CInterfaceStubVtbl *) &_IFDMIECookiesBHOStubVtbl,
    0
};

PCInterfaceName const _iecooks_InterfaceNamesList[] = 
{
    "IFDMIEStat",
    "IFDMIECookiesBHO",
    0
};

const IID *  _iecooks_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _iecooks_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _iecooks, pIID, n)

int __stdcall _iecooks_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _iecooks, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _iecooks, 2, *pIndex )
    
}

const ExtendedProxyFileInfo iecooks_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _iecooks_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _iecooks_StubVtblList,
    (const PCInterfaceName * ) & _iecooks_InterfaceNamesList,
    (const IID ** ) & _iecooks_BaseIIDList,
    & _iecooks_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
