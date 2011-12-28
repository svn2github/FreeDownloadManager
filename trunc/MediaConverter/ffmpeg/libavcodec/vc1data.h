/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_VC1DATA_H
#define AVCODEC_VC1DATA_H

#include <stdint.h>
#include "libavutil/rational.h"
#include "get_bits.h"


extern const int ff_vc1_ttblk_to_tt[3][8];

extern const int ff_vc1_ttfrm_to_tt[4];


extern const uint8_t ff_vc1_mv_pmode_table[2][5];
extern const uint8_t ff_vc1_mv_pmode_table2[2][4];

extern const int ff_vc1_fps_nr[5], ff_vc1_fps_dr[2];
extern const uint8_t ff_vc1_pquant_table[3][32];



#define VC1_BFRACTION_VLC_BITS 7
extern VLC ff_vc1_bfraction_vlc;
#define VC1_IMODE_VLC_BITS 4
extern VLC ff_vc1_imode_vlc;
#define VC1_NORM2_VLC_BITS 3
extern VLC ff_vc1_norm2_vlc;
#define VC1_NORM6_VLC_BITS 9
extern VLC ff_vc1_norm6_vlc;

#define VC1_TTMB_VLC_BITS 9 
extern VLC ff_vc1_ttmb_vlc[3];
#define VC1_MV_DIFF_VLC_BITS 9 
extern VLC ff_vc1_mv_diff_vlc[4];
#define VC1_CBPCY_P_VLC_BITS 9 
extern VLC ff_vc1_cbpcy_p_vlc[4];
#define VC1_4MV_BLOCK_PATTERN_VLC_BITS 6
extern VLC ff_vc1_4mv_block_pattern_vlc[4];
#define VC1_TTBLK_VLC_BITS 5
extern VLC ff_vc1_ttblk_vlc[3];
#define VC1_SUBBLKPAT_VLC_BITS 6
extern VLC ff_vc1_subblkpat_vlc[3];

extern VLC ff_vc1_ac_coeff_table[8];



#if 0 

#define B_FRACTION_DEN  840


extern const int16_t ff_vc1_bfraction_lut[23];
#else

#define B_FRACTION_DEN  256


extern const int16_t ff_vc1_bfraction_lut[23];
#endif

extern const uint8_t ff_vc1_bfraction_bits[23];
extern const uint8_t ff_vc1_bfraction_codes[23];


extern const AVRational ff_vc1_pixel_aspect[16];


extern const uint8_t ff_vc1_imode_codes[7];
extern const uint8_t ff_vc1_imode_bits[7];


extern const uint8_t ff_vc1_norm2_codes[4];
extern const uint8_t ff_vc1_norm2_bits[4];
extern const uint16_t ff_vc1_norm6_codes[64];
extern const uint8_t ff_vc1_norm6_bits[64];

extern const uint8_t ff_vc1_norm6_spec[64][5];


extern const uint8_t ff_vc1_4mv_block_pattern_codes[4][16];
extern const uint8_t ff_vc1_4mv_block_pattern_bits[4][16];

extern const uint8_t wmv3_dc_scale_table[32];


extern const uint16_t ff_vc1_cbpcy_p_codes[4][64];
extern const uint8_t ff_vc1_cbpcy_p_bits[4][64];


extern const uint16_t ff_vc1_ttmb_codes[3][16];

extern const uint8_t ff_vc1_ttmb_bits[3][16];


extern const uint8_t ff_vc1_ttblk_codes[3][8];
extern const uint8_t ff_vc1_ttblk_bits[3][8];


extern const uint8_t ff_vc1_subblkpat_codes[3][15];
extern const uint8_t ff_vc1_subblkpat_bits[3][15];


extern const uint16_t ff_vc1_mv_diff_codes[4][73];
extern const uint8_t ff_vc1_mv_diff_bits[4][73];




extern const int8_t ff_vc1_simple_progressive_4x4_zz [16];
extern const int8_t ff_vc1_adv_progressive_8x4_zz [32];
extern const int8_t ff_vc1_adv_progressive_4x8_zz [32];
extern const int8_t ff_vc1_adv_interlaced_8x8_zz [64];
extern const int8_t ff_vc1_adv_interlaced_8x4_zz [32];
extern const int8_t ff_vc1_adv_interlaced_4x8_zz [32];
extern const int8_t ff_vc1_adv_interlaced_4x4_zz [16];


extern const int32_t ff_vc1_dqscale[63];

#endif 
