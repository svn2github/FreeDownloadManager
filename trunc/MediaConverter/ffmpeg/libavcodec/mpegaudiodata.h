/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPEGAUDIODATA_H
#define AVCODEC_MPEGAUDIODATA_H

#include "libavutil/common.h"

#define MODE_EXT_MS_STEREO 2
#define MODE_EXT_I_STEREO  1

extern const uint16_t ff_mpa_bitrate_tab[2][3][15];
extern const uint16_t ff_mpa_freq_tab[3];
extern const int32_t ff_mpa_enwindow[257];
extern const int ff_mpa_sblimit_table[5];
extern const int ff_mpa_quant_steps[17];
extern const int ff_mpa_quant_bits[17];
extern const unsigned char * const ff_mpa_alloc_tables[5];

#endif 
