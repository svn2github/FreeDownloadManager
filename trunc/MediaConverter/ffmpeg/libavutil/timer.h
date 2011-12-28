/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_TIMER_H
#define AVUTIL_TIMER_H

#include <stdlib.h>
#include <stdint.h>
#include "config.h"

#if   ARCH_ARM
#   include "arm/timer.h"
#elif ARCH_BFIN
#   include "bfin/timer.h"
#elif ARCH_PPC
#   include "ppc/timer.h"
#elif ARCH_X86
#   include "x86/timer.h"
#endif

#if !defined(AV_READ_TIME) && HAVE_GETHRTIME
#   define AV_READ_TIME gethrtime
#endif

#ifdef AV_READ_TIME
#define START_TIMER \
uint64_t tend;\
uint64_t tstart= AV_READ_TIME();\

#define STOP_TIMER(id) \
tend= AV_READ_TIME();\
{\
    static uint64_t tsum=0;\
    static int tcount=0;\
    static int tskip_count=0;\
    if(tcount<2 || tend - tstart < 8*tsum/tcount || tend - tstart < 2000){\
        tsum+= tend - tstart;\
        tcount++;\
    }else\
        tskip_count++;\
    if(((tcount+tskip_count)&(tcount+tskip_count-1))==0){\
        av_log(NULL, AV_LOG_ERROR, "%"PRIu64" dezicycles in %s, %d runs, %d skips\n",\
               tsum*10/tcount, id, tcount, tskip_count);\
    }\
}
#else
#define START_TIMER
#define STOP_TIMER(id) {}
#endif

#endif 
