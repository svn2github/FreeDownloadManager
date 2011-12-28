/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PNG_H
#define AVCODEC_PNG_H

#include <stdint.h>

#define PNG_COLOR_MASK_PALETTE    1
#define PNG_COLOR_MASK_COLOR      2
#define PNG_COLOR_MASK_ALPHA      4

#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_PALETTE  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE)
#define PNG_COLOR_TYPE_RGB        (PNG_COLOR_MASK_COLOR)
#define PNG_COLOR_TYPE_RGB_ALPHA  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA)
#define PNG_COLOR_TYPE_GRAY_ALPHA (PNG_COLOR_MASK_ALPHA)

#define PNG_FILTER_TYPE_LOCO   64
#define PNG_FILTER_VALUE_NONE  0
#define PNG_FILTER_VALUE_SUB   1
#define PNG_FILTER_VALUE_UP    2
#define PNG_FILTER_VALUE_AVG   3
#define PNG_FILTER_VALUE_PAETH 4
#define PNG_FILTER_VALUE_MIXED 5

#define PNG_IHDR      0x0001
#define PNG_IDAT      0x0002
#define PNG_ALLIMAGE  0x0004
#define PNG_PLTE      0x0008

#define NB_PASSES 7

extern const uint8_t ff_pngsig[8];
extern const uint8_t ff_mngsig[8];


extern const uint8_t ff_png_pass_ymask[NB_PASSES];


extern const uint8_t ff_png_pass_xmin[NB_PASSES];


extern const uint8_t ff_png_pass_xshift[NB_PASSES];


extern const uint8_t ff_png_pass_mask[NB_PASSES];

void *ff_png_zalloc(void *opaque, unsigned int items, unsigned int size);

void ff_png_zfree(void *opaque, void *ptr);

int ff_png_get_nb_channels(int color_type);


int ff_png_pass_row_size(int pass, int bits_per_pixel, int width);

void ff_add_png_paeth_prediction(uint8_t *dst, uint8_t *src, uint8_t *top, int w, int bpp);

#endif 
