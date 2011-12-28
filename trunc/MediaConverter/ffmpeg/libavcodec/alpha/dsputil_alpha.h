/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ALPHA_DSPUTIL_ALPHA_H
#define AVCODEC_ALPHA_DSPUTIL_ALPHA_H

#include "libavcodec/dsputil.h"

void ff_simple_idct_axp(DCTELEM *block);
void ff_simple_idct_put_axp(uint8_t *dest, int line_size, DCTELEM *block);
void ff_simple_idct_add_axp(uint8_t *dest, int line_size, DCTELEM *block);

void put_pixels_axp_asm(uint8_t *block, const uint8_t *pixels,
                        int line_size, int h);
void put_pixels_clamped_mvi_asm(const DCTELEM *block, uint8_t *pixels,
                                int line_size);
void add_pixels_clamped_mvi_asm(const DCTELEM *block, uint8_t *pixels,
                                int line_size);
extern void (*put_pixels_clamped_axp_p)(const DCTELEM *block, uint8_t *pixels,
                                        int line_size);
extern void (*add_pixels_clamped_axp_p)(const DCTELEM *block, uint8_t *pixels,
                                        int line_size);

void get_pixels_mvi(DCTELEM *restrict block,
                    const uint8_t *restrict pixels, int line_size);
void diff_pixels_mvi(DCTELEM *block, const uint8_t *s1, const uint8_t *s2,
                     int stride);
int pix_abs8x8_mvi(void *v, uint8_t *pix1, uint8_t *pix2, int line_size, int h);
int pix_abs16x16_mvi_asm(void *v, uint8_t *pix1, uint8_t *pix2, int line_size, int h);
int pix_abs16x16_x2_mvi(void *v, uint8_t *pix1, uint8_t *pix2, int line_size, int h);
int pix_abs16x16_y2_mvi(void *v, uint8_t *pix1, uint8_t *pix2, int line_size, int h);
int pix_abs16x16_xy2_mvi(void *v, uint8_t *pix1, uint8_t *pix2, int line_size, int h);


#endif 
