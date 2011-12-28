/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_X86_BSWAP_H
#define AVUTIL_X86_BSWAP_H

#include <stdint.h>
#include "config.h"
#include "libavutil/attributes.h"

#define bswap_16 bswap_16
static av_always_inline av_const uint16_t bswap_16(uint16_t x)
{
    __asm__("rorw $8, %0" : "+r"(x));
    return x;
}

#define bswap_32 bswap_32
static av_always_inline av_const uint32_t bswap_32(uint32_t x)
{
#if HAVE_BSWAP
    __asm__("bswap   %0" : "+r" (x));
#else
    __asm__("rorw    $8,  %w0 \n\t"
            "rorl    $16, %0  \n\t"
            "rorw    $8,  %w0"
            : "+r"(x));
#endif
    return x;
}

#if ARCH_X86_64
#define bswap_64 bswap_64
static inline uint64_t av_const bswap_64(uint64_t x)
{
    __asm__("bswap  %0": "=r" (x) : "0" (x));
    return x;
}
#endif

#endif 
