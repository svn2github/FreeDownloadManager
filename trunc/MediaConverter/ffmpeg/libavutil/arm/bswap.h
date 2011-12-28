/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_ARM_BSWAP_H
#define AVUTIL_ARM_BSWAP_H

#include <stdint.h>
#include "config.h"
#include "libavutil/attributes.h"

#ifdef __ARMCC_VERSION

#if HAVE_ARMV6
#define bswap_16 bswap_16
static av_always_inline av_const unsigned bswap_16(unsigned x)
{
    __asm { rev16 x, x }
    return x;
}

#define bswap_32 bswap_32
static av_always_inline av_const uint32_t bswap_32(uint32_t x)
{
    return __rev(x);
}
#endif 

#elif HAVE_INLINE_ASM

#if HAVE_ARMV6
#define bswap_16 bswap_16
static av_always_inline av_const unsigned bswap_16(unsigned x)
{
    __asm__("rev16 %0, %0" : "+r"(x));
    return x;
}
#endif

#define bswap_32 bswap_32
static av_always_inline av_const uint32_t bswap_32(uint32_t x)
{
#if HAVE_ARMV6
    __asm__("rev %0, %0" : "+r"(x));
#else
    uint32_t t;
    __asm__ ("eor %1, %0, %0, ror #16 \n\t"
             "bic %1, %1, #0xFF0000   \n\t"
             "mov %0, %0, ror #8      \n\t"
             "eor %0, %0, %1, lsr #8  \n\t"
             : "+r"(x), "=&r"(t));
#endif 
    return x;
}

#endif 

#endif 
