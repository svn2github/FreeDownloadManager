/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVCODEC_MATHOPS_H
#define AVCODEC_MATHOPS_H

#include "libavutil/common.h"

#if   ARCH_ARM
#   include "arm/mathops.h"
#elif ARCH_AVR32
#   include "avr32/mathops.h"
#elif ARCH_BFIN
#   include "bfin/mathops.h"
#elif ARCH_MIPS
#   include "mips/mathops.h"
#elif ARCH_PPC
#   include "ppc/mathops.h"
#elif ARCH_X86
#   include "x86/mathops.h"
#endif



#ifndef MULL
#   define MULL(a,b,s) (((int64_t)(a) * (int64_t)(b)) >> (s))
#endif

#ifndef MULH



static av_always_inline int MULH(int a, int b){
    return ((int64_t)(a) * (int64_t)(b))>>32;
}
#endif

#ifndef UMULH
static av_always_inline unsigned UMULH(unsigned a, unsigned b){
    return ((uint64_t)(a) * (uint64_t)(b))>>32;
}
#endif

#ifndef MUL64
#   define MUL64(a,b) ((int64_t)(a) * (int64_t)(b))
#endif

#ifndef MAC64
#   define MAC64(d, a, b) ((d) += MUL64(a, b))
#endif

#ifndef MLS64
#   define MLS64(d, a, b) ((d) -= MUL64(a, b))
#endif


#ifndef MAC16
#   define MAC16(rt, ra, rb) rt += (ra) * (rb)
#endif


#ifndef MUL16
#   define MUL16(ra, rb) ((ra) * (rb))
#endif

#ifndef MLS16
#   define MLS16(rt, ra, rb) ((rt) -= (ra) * (rb))
#endif


#ifndef mid_pred
#define mid_pred mid_pred
static inline av_const int mid_pred(int a, int b, int c)
{
#if 0
    int t= (a-b)&((a-b)>>31);
    a-=t;
    b+=t;
    b-= (b-c)&((b-c)>>31);
    b+= (a-b)&((a-b)>>31);

    return b;
#else
    if(a>b){
        if(c>b){
            if(c>a) b=a;
            else    b=c;
        }
    }else{
        if(b>c){
            if(c>a) b=c;
            else    b=a;
        }
    }
    return b;
#endif
}
#endif

#ifndef sign_extend
static inline av_const int sign_extend(int val, unsigned bits)
{
    return (val << (INT_BIT - bits)) >> (INT_BIT - bits);
}
#endif

#ifndef zero_extend
static inline av_const unsigned zero_extend(unsigned val, unsigned bits)
{
    return (val << (INT_BIT - bits)) >> (INT_BIT - bits);
}
#endif

#ifndef COPY3_IF_LT
#define COPY3_IF_LT(x, y, a, b, c, d)\
if ((y) < (x)) {\
    (x) = (y);\
    (a) = (b);\
    (c) = (d);\
}
#endif

#ifndef NEG_SSR32
#   define NEG_SSR32(a,s) ((( int32_t)(a))>>(32-(s)))
#endif

#ifndef NEG_USR32
#   define NEG_USR32(a,s) (((uint32_t)(a))>>(32-(s)))
#endif

#endif 

