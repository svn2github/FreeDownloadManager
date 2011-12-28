/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_INTMATH_H
#define AVUTIL_INTMATH_H

#include <stdint.h>
#include "config.h"
#include "attributes.h"

extern const uint32_t ff_inverse[257];

#if   ARCH_ARM
#   include "arm/intmath.h"
#elif ARCH_X86
#   include "x86/intmath.h"
#endif

#if HAVE_FAST_CLZ && AV_GCC_VERSION_AT_LEAST(3,4)

#ifndef av_log2

#define av_log2(x) (31 - __builtin_clz((x)|1))

#ifndef av_log2_16bit
#define av_log2_16bit av_log2
#endif

#endif 

#endif 

#ifndef FASTDIV

#if CONFIG_FASTDIV
#    define FASTDIV(a,b)   ((uint32_t)((((uint64_t)a) * ff_inverse[b]) >> 32))
#else
#    define FASTDIV(a,b)   ((a) / (b))
#endif

#endif 



#include "common.h"

#ifndef av_log2
#   define av_log2       av_log2_c
#endif
#ifndef av_log2_16bit
#   define av_log2_16bit av_log2_16bit_c
#endif

extern const uint8_t ff_sqrt_tab[256];

static inline av_const unsigned int ff_sqrt(unsigned int a)
{
    unsigned int b;

    if (a < 255) return (ff_sqrt_tab[a + 1] - 1) >> 4;
    else if (a < (1 << 12)) b = ff_sqrt_tab[a >> 4] >> 2;
#if !CONFIG_SMALL
    else if (a < (1 << 14)) b = ff_sqrt_tab[a >> 6] >> 1;
    else if (a < (1 << 16)) b = ff_sqrt_tab[a >> 8]   ;
#endif
    else {
        int s = av_log2_16bit(a >> 16) >> 1;
        unsigned int c = a >> (s + 2);
        b = ff_sqrt_tab[c >> (s + 8)];
        b = FASTDIV(c,b) + (b << s);
    }

    return b - (a < b * b);
}

#endif 
