/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPEG12DECDATA_H
#define AVCODEC_MPEG12DECDATA_H

#include <stdint.h>
#include "mpegvideo.h"


#define MB_TYPE_ZERO_MV   0x20000000
#define IS_ZERO_MV(a)   ((a)&MB_TYPE_ZERO_MV)

static const uint8_t table_mb_ptype[7][2] = {
    { 3, 5 }, 
    { 1, 2 }, 
    { 1, 3 }, 
    { 1, 1 }, 
    { 1, 6 }, 
    { 1, 5 }, 
    { 2, 5 }, 
};

static const uint32_t ptype2mb_type[7] = {
                    MB_TYPE_INTRA,
                    MB_TYPE_L0 | MB_TYPE_CBP | MB_TYPE_ZERO_MV | MB_TYPE_16x16,
                    MB_TYPE_L0,
                    MB_TYPE_L0 | MB_TYPE_CBP,
    MB_TYPE_QUANT | MB_TYPE_INTRA,
    MB_TYPE_QUANT | MB_TYPE_L0 | MB_TYPE_CBP | MB_TYPE_ZERO_MV | MB_TYPE_16x16,
    MB_TYPE_QUANT | MB_TYPE_L0 | MB_TYPE_CBP,
};

static const uint8_t table_mb_btype[11][2] = {
    { 3, 5 }, 
    { 2, 3 }, 
    { 3, 3 }, 
    { 2, 4 }, 
    { 3, 4 }, 
    { 2, 2 }, 
    { 3, 2 }, 
    { 1, 6 }, 
    { 2, 6 }, 
    { 3, 6 }, 
    { 2, 5 }, 
};

static const uint32_t btype2mb_type[11] = {
                    MB_TYPE_INTRA,
                    MB_TYPE_L1,
                    MB_TYPE_L1   | MB_TYPE_CBP,
                    MB_TYPE_L0,
                    MB_TYPE_L0   | MB_TYPE_CBP,
                    MB_TYPE_L0L1,
                    MB_TYPE_L0L1 | MB_TYPE_CBP,
    MB_TYPE_QUANT | MB_TYPE_INTRA,
    MB_TYPE_QUANT | MB_TYPE_L1   | MB_TYPE_CBP,
    MB_TYPE_QUANT | MB_TYPE_L0   | MB_TYPE_CBP,
    MB_TYPE_QUANT | MB_TYPE_L0L1 | MB_TYPE_CBP,
};

static const uint8_t non_linear_qscale[32] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8,10,12,14,16,18,20,22,
    24,28,32,36,40,44,48,52,
    56,64,72,80,88,96,104,112,
};

#endif 
