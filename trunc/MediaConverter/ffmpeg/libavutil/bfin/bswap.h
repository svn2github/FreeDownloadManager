/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_BFIN_BSWAP_H
#define AVUTIL_BFIN_BSWAP_H

#include <stdint.h>
#include "config.h"
#include "libavutil/attributes.h"

#define bswap_32 bswap_32
static av_always_inline av_const uint32_t bswap_32(uint32_t x)
{
    unsigned tmp;
    __asm__("%1 = %0 >> 8 (V);      \n\t"
            "%0 = %0 << 8 (V);      \n\t"
            "%0 = %0 | %1;          \n\t"
            "%0 = PACK(%0.L, %0.H); \n\t"
            : "+d"(x), "=&d"(tmp));
    return x;
}

#endif 
