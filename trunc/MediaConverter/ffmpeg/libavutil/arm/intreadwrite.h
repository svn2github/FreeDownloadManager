/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_ARM_INTREADWRITE_H
#define AVUTIL_ARM_INTREADWRITE_H

#include <stdint.h>
#include "config.h"

#if HAVE_FAST_UNALIGNED && HAVE_INLINE_ASM

#define AV_RN16 AV_RN16
static av_always_inline uint16_t AV_RN16(const void *p)
{
    uint16_t v;
    __asm__ ("ldrh %0, %1" : "=r"(v) : "m"(*(const uint16_t *)p));
    return v;
}

#define AV_WN16 AV_WN16
static av_always_inline void AV_WN16(void *p, uint16_t v)
{
    __asm__ ("strh %1, %0" : "=m"(*(uint16_t *)p) : "r"(v));
}

#define AV_RN32 AV_RN32
static av_always_inline uint32_t AV_RN32(const void *p)
{
    uint32_t v;
    __asm__ ("ldr  %0, %1" : "=r"(v) : "m"(*(const uint32_t *)p));
    return v;
}

#define AV_WN32 AV_WN32
static av_always_inline void AV_WN32(void *p, uint32_t v)
{
    __asm__ ("str  %1, %0" : "=m"(*(uint32_t *)p) : "r"(v));
}

#define AV_RN64 AV_RN64
static av_always_inline uint64_t AV_RN64(const void *p)
{
    union { uint64_t v; uint32_t hl[2]; } v;
    __asm__ ("ldr   %0, %2  \n\t"
             "ldr   %1, %3  \n\t"
             : "=&r"(v.hl[0]), "=r"(v.hl[1])
             : "m"(*(const uint32_t*)p), "m"(*((const uint32_t*)p+1)));
    return v.v;
}

#define AV_WN64 AV_WN64
static av_always_inline void AV_WN64(void *p, uint64_t v)
{
    union { uint64_t v; uint32_t hl[2]; } vv = { v };
    __asm__ ("str  %2, %0  \n\t"
             "str  %3, %1  \n\t"
             : "=m"(*(uint32_t*)p), "=m"(*((uint32_t*)p+1))
             : "r"(vv.hl[0]), "r"(vv.hl[1]));
}

#endif 

#endif 
