/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MSMPEG4DATA_H
#define AVCODEC_MSMPEG4DATA_H

#include "libavutil/common.h"
#include "get_bits.h"
#include "rl.h"


typedef struct MVTable {
    int n;
    const uint16_t *table_mv_code;
    const uint8_t *table_mv_bits;
    const uint8_t *table_mvx;
    const uint8_t *table_mvy;
    uint16_t *table_mv_index; 
    VLC vlc;                
} MVTable;

extern VLC ff_msmp4_mb_i_vlc;
extern VLC ff_msmp4_dc_luma_vlc[2];
extern VLC ff_msmp4_dc_chroma_vlc[2];


extern const uint16_t ff_msmp4_mb_i_table[64][2];

#define WMV1_SCANTABLE_COUNT 4

extern const uint8_t wmv1_scantable[WMV1_SCANTABLE_COUNT][64];

#define NB_RL_TABLES  6

extern RLTable rl_table[NB_RL_TABLES];

extern const uint8_t wmv1_y_dc_scale_table[32];
extern const uint8_t wmv1_c_dc_scale_table[32];
extern const uint8_t old_ff_y_dc_scale_table[32];

extern MVTable mv_tables[2];

extern const uint8_t v2_mb_type[8][2];
extern const uint8_t v2_intra_cbpc[4][2];

extern const uint32_t table_mb_non_intra[128][2];
extern const uint8_t  table_inter_intra[4][2];

extern const uint32_t ff_table0_dc_lum[120][2];
extern const uint32_t ff_table1_dc_lum[120][2];
extern const uint32_t ff_table0_dc_chroma[120][2];
extern const uint32_t ff_table1_dc_chroma[120][2];

#define WMV2_INTER_CBP_TABLE_COUNT 4
extern const uint32_t (* const wmv2_inter_table[WMV2_INTER_CBP_TABLE_COUNT])[2];

extern const uint8_t wmv2_scantableA[64];
extern const uint8_t wmv2_scantableB[64];

#endif 
