/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MIPS_MATHOPS_H
#define AVCODEC_MIPS_MATHOPS_H

#include <stdint.h>
#include "config.h"
#include "libavutil/common.h"

#if HAVE_LOONGSON

static inline av_const int64_t MAC64(int64_t d, int a, int b)
{
    int64_t m;
    __asm__ ("dmult.g %1, %2, %3 \n\t"
             "daddu   %0, %0, %1 \n\t"
             : "+r"(d), "=&r"(m) : "r"(a), "r"(b));
    return d;
}
#define MAC64(d, a, b) ((d) = MAC64(d, a, b))

static inline av_const int64_t MLS64(int64_t d, int a, int b)
{
    int64_t m;
    __asm__ ("dmult.g %1, %2, %3 \n\t"
             "dsubu   %0, %0, %1 \n\t"
             : "+r"(d), "=&r"(m) : "r"(a), "r"(b));
    return d;
}
#define MLS64(d, a, b) ((d) = MLS64(d, a, b))

#elif ARCH_MIPS64

static inline av_const int64_t MAC64(int64_t d, int a, int b)
{
    int64_t m;
    __asm__ ("dmult %2, %3     \n\t"
             "mflo  %1         \n\t"
             "daddu %0, %0, %1 \n\t"
             : "+r"(d), "=&r"(m) : "r"(a), "r"(b));
    return d;
}
#define MAC64(d, a, b) ((d) = MAC64(d, a, b))

static inline av_const int64_t MLS64(int64_t d, int a, int b)
{
    int64_t m;
    __asm__ ("dmult %2, %3     \n\t"
             "mflo  %1         \n\t"
             "dsubu %0, %0, %1 \n\t"
             : "+r"(d), "=&r"(m) : "r"(a), "r"(b));
    return d;
}
#define MLS64(d, a, b) ((d) = MLS64(d, a, b))

#endif

#endif 
