/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_MIPS_INTREADWRITE_H
#define AVUTIL_MIPS_INTREADWRITE_H

#include <stdint.h>
#include "config.h"

#define AV_RN32 AV_RN32
static av_always_inline uint32_t AV_RN32(const void *p)
{
    uint32_t v;
    __asm__ ("lwl %0, %1  \n\t"
             "lwr %0, %2  \n\t"
             : "=&r"(v)
             : "m"(*(const uint32_t *)((const uint8_t *)p+3*!HAVE_BIGENDIAN)),
               "m"(*(const uint32_t *)((const uint8_t *)p+3*HAVE_BIGENDIAN)));
    return v;
}

#define AV_WN32 AV_WN32
static av_always_inline void AV_WN32(void *p, uint32_t v)
{
    __asm__ ("swl %2, %0  \n\t"
             "swr %2, %1  \n\t"
             : "=m"(*(uint32_t *)((uint8_t *)p+3*!HAVE_BIGENDIAN)),
               "=m"(*(uint32_t *)((uint8_t *)p+3*HAVE_BIGENDIAN))
             : "r"(v));
}

#if ARCH_MIPS64

#define AV_RN64 AV_RN64
static av_always_inline uint64_t AV_RN64(const void *p)
{
    uint64_t v;
    __asm__ ("ldl %0, %1  \n\t"
             "ldr %0, %2  \n\t"
             : "=&r"(v)
             : "m"(*(const uint64_t *)((const uint8_t *)p+7*!HAVE_BIGENDIAN)),
               "m"(*(const uint64_t *)((const uint8_t *)p+7*HAVE_BIGENDIAN)));
    return v;
}

#define AV_WN64 AV_WN64
static av_always_inline void AV_WN64(void *p, uint64_t v)
{
    __asm__ ("sdl %2, %0  \n\t"
             "sdr %2, %1  \n\t"
             : "=m"(*(uint64_t *)((uint8_t *)p+7*!HAVE_BIGENDIAN)),
               "=m"(*(uint64_t *)((uint8_t *)p+7*HAVE_BIGENDIAN))
             : "r"(v));
}

#else

#define AV_RN64 AV_RN64
static av_always_inline uint64_t AV_RN64(const void *p)
{
    union { uint64_t v; uint32_t hl[2]; } v;
    v.hl[0] = AV_RN32(p);
    v.hl[1] = AV_RN32((const uint8_t *)p + 4);
    return v.v;
}

#define AV_WN64 AV_WN64
static av_always_inline void AV_WN64(void *p, uint64_t v)
{
    union { uint64_t v; uint32_t hl[2]; } vv = { v };
    AV_WN32(p, vv.hl[0]);
    AV_WN32((uint8_t *)p + 4, vv.hl[1]);
}

#endif 

#endif 
