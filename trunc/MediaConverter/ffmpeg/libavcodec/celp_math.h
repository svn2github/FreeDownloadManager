/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_CELP_MATH_H
#define AVCODEC_CELP_MATH_H

#include <stdint.h>


int16_t ff_cos(uint16_t arg);


int ff_exp2(uint16_t power);


int ff_log2(uint32_t value);


static inline int bidir_sal(int value, int offset)
{
    if(offset < 0) return value >> -offset;
    else           return value <<  offset;
}


float ff_dot_productf(const float* a, const float* b, int length);

#endif 
