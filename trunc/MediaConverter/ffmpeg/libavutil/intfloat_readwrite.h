/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_INTFLOAT_READWRITE_H
#define AVUTIL_INTFLOAT_READWRITE_H

#include <stdint.h>
#include "attributes.h"


typedef struct AVExtFloat  {
    uint8_t exponent[2];
    uint8_t mantissa[8];
} AVExtFloat;

double av_int2dbl(int64_t v) av_const;
float av_int2flt(int32_t v) av_const;
double av_ext2dbl(const AVExtFloat ext) av_const;
int64_t av_dbl2int(double d) av_const;
int32_t av_flt2int(float d) av_const;
AVExtFloat av_dbl2ext(double d) av_const;

#endif 
