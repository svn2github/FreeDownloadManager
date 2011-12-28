/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_X86_VP6DSP_SSE2_H
#define AVCODEC_X86_VP6DSP_SSE2_H

#include <stdint.h>

void ff_vp6_filter_diag4_sse2(uint8_t *dst, uint8_t *src, int stride,
                             const int16_t *h_weights,const int16_t *v_weights);

#endif 
