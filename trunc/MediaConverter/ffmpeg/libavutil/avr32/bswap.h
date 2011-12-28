/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_AVR32_BSWAP_H
#define AVUTIL_AVR32_BSWAP_H

#include <stdint.h>
#include "config.h"
#include "libavutil/attributes.h"

#if HAVE_INLINE_ASM

#define bswap_16 bswap_16
static av_always_inline av_const uint16_t bswap_16(uint16_t x)
{
    __asm__ ("swap.bh %0" : "+r"(x));
    return x;
}

#define bswap_32 bswap_32
static av_always_inline av_const uint32_t bswap_32(uint32_t x)
{
    __asm__ ("swap.b  %0" : "+r"(x));
    return x;
}

#endif 

#endif 
