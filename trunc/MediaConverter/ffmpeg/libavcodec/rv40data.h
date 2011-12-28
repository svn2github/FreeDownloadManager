/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_RV40DATA_H
#define AVCODEC_RV40DATA_H

#include <stdint.h>



static const int rv40_standard_widths[]   = { 160, 172, 240, 320, 352, 640, 704, 0};
static const int rv40_standard_heights[]  = { 120, 132, 144, 240, 288, 480, -8, -10, 180, 360, 576, 0};


#define MODE2_PATTERNS_NUM 20

static const uint16_t rv40_aic_table_index[MODE2_PATTERNS_NUM] = {
 0x000, 0x100, 0x200,
 0x011, 0x111, 0x211, 0x511, 0x611,
 0x022, 0x122, 0x222, 0x722,
 0x272, 0x227,
 0x822, 0x282, 0x228,
 0x112, 0x116, 0x221
};


static const uint8_t rv40_luma_dc_quant[2][32] = {
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 22, 22, 22, 22 },
 {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 20, 21, 21, 22, 23, 23, 23, 24, 24, 24, 24 }
};



static const uint8_t rv40_dither_l[16] = {
    0x40, 0x50, 0x20, 0x60, 0x30, 0x50, 0x40, 0x30,
    0x50, 0x40, 0x50, 0x30, 0x60, 0x20, 0x50, 0x40
};

static const uint8_t rv40_dither_r[16] = {
    0x40, 0x30, 0x60, 0x20, 0x50, 0x30, 0x30, 0x40,
    0x40, 0x40, 0x50, 0x30, 0x20, 0x60, 0x30, 0x40
};


static const uint8_t rv40_alpha_tab[32] = {
    128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 122,  96,  75,  59,  47,  37,
     29,  23,  18,  15,  13,  11,  10,   9,
      8,   7,   6,   5,   4,   3,   2,   1
};

static const uint8_t rv40_beta_tab[32] = {
     0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  3,  4,  4,  4,  6,  6,
     6,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 13, 14, 15, 16, 17
};

static const uint8_t rv40_filter_clip_tbl[3][32] = {
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 5, 5
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 7, 8, 9
    }
};
 

#endif 
