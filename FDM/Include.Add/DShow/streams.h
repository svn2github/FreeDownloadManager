/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

                  

#ifndef __STREAMS__
#define __STREAMS__

#ifdef	_MSC_VER

#pragma warning(disable:4100) 
#pragma warning(disable:4201) 
#pragma warning(disable:4511) 
#pragma warning(disable:4512) 
#pragma warning(disable:4514) 

#if _MSC_VER>=1100
#define AM_NOVTABLE __declspec(novtable)
#else
#define AM_NOVTABLE
#endif
#endif	  

#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include <ddraw.h>
#include <mmsystem.h>

#ifndef NUMELMS
   #define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))
#endif        

#ifndef InterlockedExchangePointer
	#define InterlockedExchangePointer(Target, Value) \
   (PVOID)InterlockedExchange((PLONG)(Target), (LONG)(Value))
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       
        WORD wSamplesPerBlock;          
        WORD wReserved;                 
    } Samples;
    DWORD           dwChannelMask;      
                                        
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif 

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif 

#ifndef GetWindowLongPtr
  #define GetWindowLongPtrA   GetWindowLongA
  #define GetWindowLongPtrW   GetWindowLongW
  #ifdef UNICODE
    #define GetWindowLongPtr  GetWindowLongPtrW
  #else
    #define GetWindowLongPtr  GetWindowLongPtrA
  #endif 
#endif 

#ifndef SetWindowLongPtr
  #define SetWindowLongPtrA   SetWindowLongA
  #define SetWindowLongPtrW   SetWindowLongW
  #ifdef UNICODE
    #define SetWindowLongPtr  SetWindowLongPtrW
  #else
    #define SetWindowLongPtr  SetWindowLongPtrA
  #endif 
#endif 

#ifndef GWLP_WNDPROC
  #define GWLP_WNDPROC        (-4)
#endif
#ifndef GWLP_HINSTANCE
  #define GWLP_HINSTANCE      (-6)
#endif
#ifndef GWLP_HWNDPARENT
  #define GWLP_HWNDPARENT     (-8)
#endif
#ifndef GWLP_USERDATA
  #define GWLP_USERDATA       (-21)
#endif
#ifndef GWLP_ID
  #define GWLP_ID             (-12)
#endif
#ifndef DWLP_MSGRESULT
  #define DWLP_MSGRESULT  0
#endif
#ifndef DWLP_DLGPROC 
  #define DWLP_DLGPROC    DWLP_MSGRESULT + sizeof(LRESULT)
#endif
#ifndef DWLP_USER
  #define DWLP_USER       DWLP_DLGPROC + sizeof(DLGPROC)
#endif        

#include <strmif.h>     

#include <reftime.h>    
#include <wxdebug.h>    
#include <amvideo.h>      

#include <wxutil.h>     
#include <combase.h>    
#include <dllsetup.h>   
#include <measure.h>    
#include <comlite.h>    

#include <cache.h>      
#include <wxlist.h>     
#include <msgthrd.h>	
#include <mtype.h>      
#include <fourcc.h>     
#include <control.h>    
#include <ctlutil.h>    
#include <evcode.h>     
#include <amfilter.h>   
#include <transfrm.h>   
#include <transip.h>    
#include <uuids.h>      
#include <source.h>	
#include <outputq.h>    
#include <errors.h>     
#include <renbase.h>    
#include <winutil.h>    
#include <winctrl.h>    
#include <videoctl.h>   
#include <refclock.h>	
#include <sysclock.h>	
#include <pstream.h>    
#include <vtrans.h>     
#include <amextra.h>
#include <cprop.h>      
#include <strmctl.h>    
#include <edevdefs.h>   
#include <audevcod.h>   

#else
    #ifdef DEBUG
    #pragma message("STREAMS.H included TWICE")
    #endif
#endif 

