/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PPC_DSPUTIL_ALTIVEC_H
#define AVCODEC_PPC_DSPUTIL_ALTIVEC_H

#include <stdint.h>
#include "libavcodec/dsputil.h"

void put_pixels16_altivec(uint8_t *block, const uint8_t *pixels, int line_size, int h);

void avg_pixels16_altivec(uint8_t *block, const uint8_t *pixels, int line_size, int h);

int has_altivec(void);

void fdct_altivec(int16_t *block);
void gmc1_altivec(uint8_t *dst, uint8_t *src, int stride, int h,
                  int x16, int y16, int rounder);
void idct_put_altivec(uint8_t *dest, int line_size, int16_t *block);
void idct_add_altivec(uint8_t *dest, int line_size, int16_t *block);

void ff_vp3_idct_altivec(DCTELEM *block);
void ff_vp3_idct_put_altivec(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_add_altivec(uint8_t *dest, int line_size, DCTELEM *block);

void dsputil_h264_init_ppc(DSPContext* c, AVCodecContext *avctx);

void dsputil_init_altivec(DSPContext* c, AVCodecContext *avctx);
void vc1dsp_init_altivec(DSPContext* c, AVCodecContext *avctx);
void float_init_altivec(DSPContext* c, AVCodecContext *avctx);
void int_init_altivec(DSPContext* c, AVCodecContext *avctx);

#endif 
