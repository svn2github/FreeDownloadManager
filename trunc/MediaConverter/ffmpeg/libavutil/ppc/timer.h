/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_PPC_TIMER_H
#define AVUTIL_PPC_TIMER_H

#include <stdint.h>

#define AV_READ_TIME read_time

static inline uint64_t read_time(void)
{
    uint32_t tbu, tbl, temp;

     
     __asm__ volatile(
         "1:\n"
         "mftbu  %2\n"
         "mftb   %0\n"
         "mftbu  %1\n"
         "cmpw   %2,%1\n"
         "bne    1b\n"
     : "=r"(tbl), "=r"(tbu), "=r"(temp)
     :
     : "cc");

     return (((uint64_t)tbu)<<32) | (uint64_t)tbl;
}

#endif 
