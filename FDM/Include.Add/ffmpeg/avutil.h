/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef AVUTIL_H
#define AVUTIL_H      

#ifdef __cplusplus
extern "C" {
#endif

#define AV_STRINGIFY(s)         AV_TOSTRING(s)
#define AV_TOSTRING(s) #s

#define LIBAVUTIL_VERSION_INT   ((49<<16)+(2<<8)+0)
#define LIBAVUTIL_VERSION       49.2.0
#define LIBAVUTIL_BUILD         LIBAVUTIL_VERSION_INT

#define LIBAVUTIL_IDENT         "Lavu" AV_STRINGIFY(LIBAVUTIL_VERSION)  

#include "common.h"
#include "mathematics.h"
#include "rational.h"
#include "integer.h"
#include "intfloat_readwrite.h"
#include "log.h"  

enum PixelFormat {
    PIX_FMT_NONE= -1,
    PIX_FMT_YUV420P,   
    PIX_FMT_YUYV422,   
    PIX_FMT_RGB24,     
    PIX_FMT_BGR24,     
    PIX_FMT_YUV422P,   
    PIX_FMT_YUV444P,   
    PIX_FMT_RGB32,     
    PIX_FMT_YUV410P,   
    PIX_FMT_YUV411P,   
    PIX_FMT_RGB565,    
    PIX_FMT_RGB555,    
    PIX_FMT_GRAY8,     
    PIX_FMT_MONOWHITE, 
    PIX_FMT_MONOBLACK, 
    PIX_FMT_PAL8,      
    PIX_FMT_YUVJ420P,  
    PIX_FMT_YUVJ422P,  
    PIX_FMT_YUVJ444P,  
    PIX_FMT_XVMC_MPEG2_MC,
    PIX_FMT_XVMC_MPEG2_IDCT,
    PIX_FMT_UYVY422,   
    PIX_FMT_UYYVYY411, 
    PIX_FMT_BGR32,     
    PIX_FMT_BGR565,    
    PIX_FMT_BGR555,    
    PIX_FMT_BGR8,      
    PIX_FMT_BGR4,      
    PIX_FMT_BGR4_BYTE, 
    PIX_FMT_RGB8,      
    PIX_FMT_RGB4,      
    PIX_FMT_RGB4_BYTE, 
    PIX_FMT_NV12,      
    PIX_FMT_NV21,      

    PIX_FMT_RGB32_1,   
    PIX_FMT_BGR32_1,   

    PIX_FMT_GRAY16BE,  
    PIX_FMT_GRAY16LE,  
    PIX_FMT_NB,        
};

#ifdef WORDS_BIGENDIAN
#define PIX_FMT_RGBA PIX_FMT_RGB32_1
#define PIX_FMT_BGRA PIX_FMT_BGR32_1
#define PIX_FMT_ARGB PIX_FMT_RGB32
#define PIX_FMT_ABGR PIX_FMT_BGR32
#define PIX_FMT_GRAY16 PIX_FMT_GRAY16BE
#else
#define PIX_FMT_RGBA PIX_FMT_BGR32
#define PIX_FMT_BGRA PIX_FMT_RGB32
#define PIX_FMT_ARGB PIX_FMT_BGR32_1
#define PIX_FMT_ABGR PIX_FMT_RGB32_1
#define PIX_FMT_GRAY16 PIX_FMT_GRAY16LE
#endif

#if LIBAVUTIL_VERSION_INT < (50<<16)
#define PIX_FMT_UYVY411 PIX_FMT_UYYVYY411
#define PIX_FMT_RGBA32  PIX_FMT_RGB32
#define PIX_FMT_YUV422  PIX_FMT_YUYV422
#endif

#ifdef __cplusplus
}
#endif

#endif 
