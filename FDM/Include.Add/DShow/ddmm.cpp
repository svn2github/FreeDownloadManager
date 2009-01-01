/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                    

#include <streams.h>
#include <ddraw.h>
#include "ddmm.h"  

typedef struct {
	LPSTR   szDevice;
	GUID*   lpGUID;
	GUID    GUID;
	BOOL    fFound;
}   FindDeviceData;

BOOL CALLBACK FindDeviceCallback(GUID* lpGUID, LPSTR szName, LPSTR szDevice, LPVOID lParam)
{
	FindDeviceData *p = (FindDeviceData*)lParam;

	if (lstrcmpiA(p->szDevice, szDevice) == 0) {
	    if (lpGUID) {
		p->GUID = *lpGUID;
		p->lpGUID = &p->GUID;
	    } else {
		p->lpGUID = NULL;
	    }
	    p->fFound = TRUE;
	    return FALSE;
	}
	return TRUE;
}  

BOOL CALLBACK FindDeviceCallbackEx(GUID* lpGUID, LPSTR szName, LPSTR szDevice, LPVOID lParam, HMONITOR hMonitor)
{
	FindDeviceData *p = (FindDeviceData*)lParam;

	if (lstrcmpiA(p->szDevice, szDevice) == 0) {
	    if (lpGUID) {
		p->GUID = *lpGUID;
		p->lpGUID = &p->GUID;
	    } else {
		p->lpGUID = NULL;
	    }
	    p->fFound = TRUE;
	    return FALSE;
	}
	return TRUE;
}    

IDirectDraw * DirectDrawCreateFromDevice(LPSTR szDevice, PDRAWCREATE DirectDrawCreateP, PDRAWENUM DirectDrawEnumerateP)
{
	IDirectDraw*    pdd = NULL;
	FindDeviceData  find;

	if (szDevice == NULL) {
		DirectDrawCreateP(NULL, &pdd, NULL);
		return pdd;
	}

	find.szDevice = szDevice;
	find.fFound   = FALSE;
	DirectDrawEnumerateP(FindDeviceCallback, (LPVOID)&find);

	if (find.fFound)
	{
		
		
		
		
		
		UINT ErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
		DirectDrawCreateP(find.lpGUID, &pdd, NULL);
		SetErrorMode(ErrorMode);
	}

	return pdd;
}    

IDirectDraw * DirectDrawCreateFromDeviceEx(LPSTR szDevice, PDRAWCREATE DirectDrawCreateP, LPDIRECTDRAWENUMERATEEXA DirectDrawEnumerateExP)
{
	IDirectDraw*    pdd = NULL;
	FindDeviceData  find;

	if (szDevice == NULL) {
		DirectDrawCreateP(NULL, &pdd, NULL);
		return pdd;
	}

	find.szDevice = szDevice;
	find.fFound   = FALSE;
	DirectDrawEnumerateExP(FindDeviceCallbackEx, (LPVOID)&find,
					DDENUM_ATTACHEDSECONDARYDEVICES);

	if (find.fFound)
	{
		
		
		
		
		
		UINT ErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
		DirectDrawCreateP(find.lpGUID, &pdd, NULL);
		SetErrorMode(ErrorMode);
	}

	return pdd;
}
