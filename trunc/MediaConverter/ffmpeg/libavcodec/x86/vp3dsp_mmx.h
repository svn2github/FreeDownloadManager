/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_X86_VP3DSP_MMX_H
#define AVCODEC_X86_VP3DSP_MMX_H

#include <stdint.h>
#include "libavcodec/dsputil.h"

void ff_vp3_idct_mmx(int16_t *data);
void ff_vp3_idct_put_mmx(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_add_mmx(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_dc_add_mmx2(uint8_t *dest, int line_size, const DCTELEM *block);

void ff_vp3_v_loop_filter_mmx2(uint8_t *src, int stride, int *bounding_values);
void ff_vp3_h_loop_filter_mmx2(uint8_t *src, int stride, int *bounding_values);

#endif 
