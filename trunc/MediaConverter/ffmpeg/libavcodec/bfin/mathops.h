/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVCODEC_BFIN_MATHOPS_H
#define AVCODEC_BFIN_MATHOPS_H

#include "config.h"

#if CONFIG_MPEGAUDIO_HP
#define MULH(X,Y) ({ int xxo;                           \
    __asm__ (                                               \
        "a1 = %2.L * %1.L (FU);\n\t"                    \
        "a1 = a1 >> 16;\n\t"                            \
        "a1 += %2.H * %1.L (IS,M);\n\t"                 \
        "a0 = %1.H * %2.H, a1+= %1.H * %2.L (IS,M);\n\t"\
        "a1 = a1 >>> 16;\n\t"                           \
        "%0 = (a0 += a1);\n\t"                          \
        : "=d" (xxo) : "d" (X), "d" (Y) : "A0","A1"); xxo; })
#else
#define MULH(X,Y) ({ int xxo;                           \
    __asm__ (                                               \
        "a1 = %2.H * %1.L (IS,M);\n\t"                  \
        "a0 = %1.H * %2.H, a1+= %1.H * %2.L (IS,M);\n\t"\
        "a1 = a1 >>> 16;\n\t"                           \
        "%0 = (a0 += a1);\n\t"                          \
        : "=d" (xxo) : "d" (X), "d" (Y) : "A0","A1"); xxo; })
#endif


#define MUL16(a, b) ({ int xxo;                         \
    __asm__ (                                               \
       "%0 = %1.l*%2.l (is);\n\t"                       \
       : "=W" (xxo) : "d" (a), "d" (b) : "A1");         \
    xxo; })

#endif 
