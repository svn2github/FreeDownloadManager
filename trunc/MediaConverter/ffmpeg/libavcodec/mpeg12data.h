/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPEG12DATA_H
#define AVCODEC_MPEG12DATA_H

#include <stdint.h>
#include "libavutil/rational.h"
#include "rl.h"

extern const uint16_t ff_mpeg1_default_intra_matrix[64];
extern const uint16_t ff_mpeg1_default_non_intra_matrix[64];

extern const uint16_t ff_mpeg12_vlc_dc_lum_code[12];
extern const unsigned char ff_mpeg12_vlc_dc_lum_bits[12];
extern const uint16_t ff_mpeg12_vlc_dc_chroma_code[12];
extern const unsigned char ff_mpeg12_vlc_dc_chroma_bits[12];

extern RLTable ff_rl_mpeg1;
extern RLTable ff_rl_mpeg2;

extern const uint8_t ff_mpeg12_mbAddrIncrTable[36][2];
extern const uint8_t ff_mpeg12_mbPatTable[64][2];

extern const uint8_t ff_mpeg12_mbMotionVectorTable[17][2];

extern const AVRational ff_frame_rate_tab[];

extern const float ff_mpeg1_aspect[16];
extern const AVRational ff_mpeg2_aspect[16];

#endif 
