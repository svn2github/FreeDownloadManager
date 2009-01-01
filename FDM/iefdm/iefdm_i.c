/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 03 23:35:39 2008
 */
/* Compiler settings for D:\VCW\FDM\FDM\iefdm\iefdm.idl:
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

const IID IID_IWgBHO = {0xDBDC6466,0x1232,0x4511,{0xB1,0xA5,0x0A,0xBC,0x79,0xEB,0x82,0x91}};


const IID LIBID_IEFDMLib = {0x591915B9,0x6A9B,0x415A,{0xB3,0x8F,0xD2,0x3F,0x6A,0xF0,0xB4,0x97}};


const CLSID CLSID_IEWGMonitor = {0x1985F8EF,0xB021,0x4A44,{0x9B,0xE1,0x34,0x3D,0x0B,0x4E,0x09,0x11}};


const CLSID CLSID_WgBHO = {0x67E9834D,0xB226,0x49E6,{0xB6,0xF6,0x85,0xAA,0x64,0xE1,0x4B,0xA3}};


#ifdef __cplusplus
}
#endif

