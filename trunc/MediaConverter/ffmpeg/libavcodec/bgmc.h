/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/






#ifndef AVCODEC_BGMC_H
#define AVCODEC_BGMC_H


#include "avcodec.h"
#include "get_bits.h"


int ff_bgmc_init(AVCodecContext *avctx, uint8_t **cf_lut, unsigned int **cf_lut_status);


void ff_bgmc_end(uint8_t **cf_lut, unsigned int **cf_lut_status);


void ff_bgmc_decode_init(GetBitContext *gb,
                      unsigned int *h, unsigned int *l, unsigned int *v);


void ff_bgmc_decode_end(GetBitContext *gb);


void ff_bgmc_decode(GetBitContext *gb, unsigned int num, int32_t *dst,
                 unsigned int delta, unsigned int sx,
                 unsigned int *h, unsigned int *l, unsigned int *v,
                 uint8_t *cf_lut, unsigned int *cf_lut_status);


#endif 
