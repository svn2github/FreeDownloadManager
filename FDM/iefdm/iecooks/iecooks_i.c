/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 06 00:31:02 2007
 */
/* Compiler settings for D:\VCW\FDM\FDM\iefdm\iecooks\iecooks.idl:
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

const IID IID_IFDMIECookiesBHO = {0x40D3F599,0x74F0,0x44D3,{0xB0,0x59,0x76,0xC0,0xF1,0x2C,0x0D,0x6E}};


const IID IID_IFDMIEStat = {0x64844F4D,0x492F,0x429E,{0x88,0x1C,0xD7,0xF1,0x06,0x25,0x97,0x38}};


const IID LIBID_IECOOKSLib = {0x351A47E5,0xB59E,0x4CF2,{0xB8,0x1A,0xB6,0x51,0xD7,0x5F,0xE9,0x44}};


const CLSID CLSID_FDMIECookiesBHO = {0xCC59E0F9,0x7E43,0x44FA,{0x9F,0xAA,0x83,0x77,0x85,0x0B,0xF2,0x05}};


const CLSID CLSID_FDMIEStat = {0xD4D6EAB8,0x009E,0x4EED,{0x81,0x4E,0x48,0x91,0xD7,0xE1,0x74,0x1B}};


#ifdef __cplusplus
}
#endif

