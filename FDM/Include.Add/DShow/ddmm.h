/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/                    

#ifdef __cplusplus
extern "C" {            
#endif    

#ifndef DDENUM_ATTACHEDSECONDARYDEVICES
#define DDENUM_ATTACHEDSECONDARYDEVICES     0x00000001L
#endif

typedef HRESULT (*PDRAWCREATE)(IID *,LPDIRECTDRAW *,LPUNKNOWN);
typedef HRESULT (*PDRAWENUM)(LPDDENUMCALLBACKA, LPVOID);

IDirectDraw * DirectDrawCreateFromDevice(LPSTR, PDRAWCREATE, PDRAWENUM);
IDirectDraw * DirectDrawCreateFromDeviceEx(LPSTR, PDRAWCREATE, LPDIRECTDRAWENUMERATEEXA);

#ifdef __cplusplus
}
#endif	
