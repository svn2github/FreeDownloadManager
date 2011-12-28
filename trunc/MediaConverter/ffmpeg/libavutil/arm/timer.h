/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_ARM_TIMER_H
#define AVUTIL_ARM_TIMER_H

#include <stdint.h>
#include "config.h"

#if HAVE_INLINE_ASM && defined(__ARM_ARCH_7A__)

#define AV_READ_TIME read_time

static inline uint64_t read_time(void)
{
    unsigned cc;
    __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r"(cc));
    return cc;
}

#endif 

#endif 
