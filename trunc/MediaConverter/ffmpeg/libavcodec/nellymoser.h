/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_NELLYMOSER_H
#define AVCODEC_NELLYMOSER_H

#include "avcodec.h"

#define NELLY_BANDS       23
#define NELLY_BLOCK_LEN   64
#define NELLY_HEADER_BITS 116
#define NELLY_DETAIL_BITS 198
#define NELLY_BUF_LEN     128
#define NELLY_FILL_LEN    124
#define NELLY_BIT_CAP     6
#define NELLY_BASE_OFF    4228
#define NELLY_BASE_SHIFT  19
#define NELLY_SAMPLES     (2 * NELLY_BUF_LEN)

extern const float    ff_nelly_dequantization_table[127];
extern const uint8_t  ff_nelly_band_sizes_table[NELLY_BANDS];
extern const uint16_t ff_nelly_init_table[64];
extern const int16_t  ff_nelly_delta_table[32];

void ff_nelly_get_sample_bits(const float *buf, int *bits);

#endif
