/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_X86_VP3DSP_SSE2_H
#define AVCODEC_X86_VP3DSP_SSE2_H

#include "libavcodec/dsputil.h"

void ff_vp3_idct_sse2(int16_t *input_data);
void ff_vp3_idct_put_sse2(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_add_sse2(uint8_t *dest, int line_size, DCTELEM *block);

#endif 
