/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DNXHDDATA_H
#define AVCODEC_DNXHDDATA_H

#include <stdint.h>
#include "avcodec.h"

typedef struct {
    int cid;
    unsigned int width, height;
    int interlaced;
    unsigned int frame_size;
    unsigned int coding_unit_size;
    int index_bits;
    int bit_depth;
    const uint8_t *luma_weight, *chroma_weight;
    const uint8_t *dc_codes, *dc_bits;
    const uint16_t *ac_codes;
    const uint8_t *ac_bits, *ac_level;
    const uint8_t *ac_run_flag, *ac_index_flag;
    const uint16_t *run_codes;
    const uint8_t *run_bits, *run;
    int bit_rates[5]; 
} CIDEntry;

extern const CIDEntry ff_dnxhd_cid_table[];

int ff_dnxhd_get_cid_table(int cid);
int ff_dnxhd_find_cid(AVCodecContext *avctx);

#endif 
