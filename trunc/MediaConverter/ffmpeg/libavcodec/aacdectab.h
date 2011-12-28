/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_AACDECTAB_H
#define AVCODEC_AACDECTAB_H

#include "aac.h"

#include <stdint.h>


static const float tns_tmp2_map_1_3[4] = {
     0.00000000, -0.43388373,  0.64278758,  0.34202015,
};

static const float tns_tmp2_map_0_3[8] = {
     0.00000000, -0.43388373, -0.78183150, -0.97492790,
     0.98480773,  0.86602539,  0.64278758,  0.34202015,
};

static const float tns_tmp2_map_1_4[8] = {
     0.00000000, -0.20791170, -0.40673664, -0.58778524,
     0.67369562,  0.52643216,  0.36124167,  0.18374951,
};

static const float tns_tmp2_map_0_4[16] = {
     0.00000000, -0.20791170, -0.40673664, -0.58778524,
    -0.74314481, -0.86602539, -0.95105654, -0.99452192,
     0.99573416,  0.96182561,  0.89516330,  0.79801720,
     0.67369562,  0.52643216,  0.36124167,  0.18374951,
};

static const float * const tns_tmp2_map[4] = {
    tns_tmp2_map_0_3,
    tns_tmp2_map_0_4,
    tns_tmp2_map_1_3,
    tns_tmp2_map_1_4
};


static const int8_t tags_per_config[16] = { 0, 1, 1, 2, 3, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0 };

static const uint8_t aac_channel_layout_map[7][5][2] = {
    { { TYPE_SCE, 0 }, },
    { { TYPE_CPE, 0 }, },
    { { TYPE_CPE, 0 }, { TYPE_SCE, 0 }, },
    { { TYPE_CPE, 0 }, { TYPE_SCE, 0 }, { TYPE_SCE, 1 }, },
    { { TYPE_CPE, 0 }, { TYPE_SCE, 0 }, { TYPE_CPE, 1 }, },
    { { TYPE_CPE, 0 }, { TYPE_SCE, 0 }, { TYPE_LFE, 0 }, { TYPE_CPE, 1 }, },
    { { TYPE_CPE, 0 }, { TYPE_SCE, 0 }, { TYPE_LFE, 0 }, { TYPE_CPE, 2 }, { TYPE_CPE, 1 }, },
};

static const int64_t aac_channel_layout[8] = {
    CH_LAYOUT_MONO,
    CH_LAYOUT_STEREO,
    CH_LAYOUT_SURROUND,
    CH_LAYOUT_4POINT0,
    CH_LAYOUT_5POINT0_BACK,
    CH_LAYOUT_5POINT1_BACK,
    CH_LAYOUT_7POINT1_WIDE,
    0,
};

#endif 
