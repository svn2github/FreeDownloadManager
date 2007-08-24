/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Aug 21 23:44:20 2007
 */
/* Compiler settings for D:\VCW\FDM\FDM\iefdm\iefdmdm\iefdmdm.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_IEFDMDMLib = {0x752D3C5E,0x1EF8,0x4FA9,{0xAC,0x6E,0xB5,0xE0,0x95,0xA4,0xD4,0xAF}};


const CLSID CLSID_IEWGDM = {0x57BDEE5A,0x1E29,0x4CFD,{0xAE,0xE7,0xEF,0x32,0x11,0x8E,0xB6,0xD6}};


#ifdef __cplusplus
}
#endif

