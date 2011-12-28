/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AC3TAB_H
#define AVCODEC_AC3TAB_H

#include "libavutil/common.h"

extern const uint16_t ff_ac3_frame_size_tab[38][3];
extern const uint8_t  ff_ac3_channels_tab[8];
extern const uint16_t ff_ac3_channel_layout_tab[8];
extern const uint8_t  ff_ac3_enc_channel_map[8][2][6];
extern const uint8_t  ff_ac3_dec_channel_map[8][2][6];
extern const uint16_t ff_ac3_sample_rate_tab[3];
extern const uint16_t ff_ac3_bitrate_tab[19];
extern const int16_t  ff_ac3_window[256];
extern const uint8_t  ff_ac3_log_add_tab[260];
extern const uint16_t ff_ac3_hearing_threshold_tab[50][3];
extern const uint8_t  ff_ac3_bap_tab[64];
extern const uint8_t  ff_ac3_slow_decay_tab[4];
extern const uint8_t  ff_ac3_fast_decay_tab[4];
extern const uint16_t ff_ac3_slow_gain_tab[4];
extern const uint16_t ff_ac3_db_per_bit_tab[4];
extern const int16_t  ff_ac3_floor_tab[8];
extern const uint16_t ff_ac3_fast_gain_tab[8];
extern const uint8_t  ff_ac3_critical_band_size_tab[50];
extern const uint16_t ff_eac3_default_chmap[8];


enum CustomChannelMapLocation{
    AC3_CHMAP_L=        1<<(15-0),
    AC3_CHMAP_C=        1<<(15-1),
    AC3_CHMAP_R=        1<<(15-2),
    AC3_CHMAP_L_SUR=    1<<(15-3),
    AC3_CHMAP_R_SUR =   1<<(15-4),
    AC3_CHMAP_C_SUR=    1<<(15-7),
    AC3_CHMAP_LFE =     1<<(15-15)
};

#endif 
