/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_ARM_INTMATH_H
#define AVUTIL_ARM_INTMATH_H

#include "config.h"
#include "libavutil/attributes.h"

#if HAVE_INLINE_ASM

#if HAVE_ARMV6
static inline av_const int FASTDIV(int a, int b)
{
    int r, t;
    __asm__ volatile("cmp     %3, #2               \n\t"
                     "ldr     %1, [%4, %3, lsl #2] \n\t"
                     "lsrle   %0, %2, #1           \n\t"
                     "smmulgt %0, %1, %2           \n\t"
                     : "=&r"(r), "=&r"(t) : "r"(a), "r"(b), "r"(ff_inverse));
    return r;
}
#else
static inline av_const int FASTDIV(int a, int b)
{
    int r, t;
    __asm__ volatile("umull %1, %0, %2, %3"
                     : "=&r"(r), "=&r"(t) : "r"(a), "r"(ff_inverse[b]));
    return r;
}
#endif

#define FASTDIV FASTDIV

#endif 

#endif 
