/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef INTFLOAT_READWRITE_H
#define INTFLOAT_READWRITE_H

#include "common.h"  

typedef struct AVExtFloat  {
    uint8_t exponent[2];
    uint8_t mantissa[8];
} AVExtFloat;

double av_int2dbl(int64_t v);
float av_int2flt(int32_t v);
double av_ext2dbl(const AVExtFloat ext);
int64_t av_dbl2int(double d);
int32_t av_flt2int(float d);
AVExtFloat av_dbl2ext(double d);

#endif 
