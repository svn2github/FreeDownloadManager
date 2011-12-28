/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_ATRAC1DATA_H
#define AVCODEC_ATRAC1DATA_H

#include <stdint.h>

static const uint8_t bfu_amount_tab1[8] = {20,  28,  32,  36, 40, 44, 48, 52};
static const uint8_t bfu_amount_tab2[4] = { 0, 112, 176, 208};
static const uint8_t bfu_amount_tab3[8] = { 0,  24,  36,  48, 72, 108, 132, 156};


static const uint8_t bfu_bands_t[4]  = {0, 20, 36, 52};


static const uint8_t specs_per_bfu[52] = {
     8,  8,  8,  8,  4,  4,  4,  4,  8,  8,  8,  8,  6,  6,  6,  6, 6, 6, 6, 6, 
     6,  6,  6,  6,  7,  7,  7,  7,  9,  9,  9,  9, 10, 10, 10, 10,             
    12, 12, 12, 12, 12, 12, 12, 12, 20, 20, 20, 20, 20, 20, 20, 20              
};


static const uint16_t bfu_start_long[52] = {
      0,   8,  16,  24,  32,  36,  40,  44,  48,  56,  64,  72,  80,  86,  92,  98, 104, 110, 116, 122,
    128, 134, 140, 146, 152, 159, 166, 173, 180, 189, 198, 207, 216, 226, 236, 246,
    256, 268, 280, 292, 304, 316, 328, 340, 352, 372, 392, 412, 432, 452, 472, 492,
};


static const uint16_t bfu_start_short[52] = {
      0,  32,  64,  96,   8,  40,  72, 104,  12,  44,  76, 108,  20,  52,  84, 116, 26, 58, 90, 122,
    128, 160, 192, 224, 134, 166, 198, 230, 141, 173, 205, 237, 150, 182, 214, 246,
    256, 288, 320, 352, 384, 416, 448, 480, 268, 300, 332, 364, 396, 428, 460, 492
};

#endif 
