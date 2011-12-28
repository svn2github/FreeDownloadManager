/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_WMV2_H
#define AVCODEC_WMV2_H

#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"
#include "intrax8.h"

#define SKIP_TYPE_NONE 0
#define SKIP_TYPE_MPEG 1
#define SKIP_TYPE_ROW  2
#define SKIP_TYPE_COL  3


typedef struct Wmv2Context{
    MpegEncContext s;
    IntraX8Context x8;
    int j_type_bit;
    int j_type;
    int abt_flag;
    int abt_type;
    int abt_type_table[6];
    int per_mb_abt;
    int per_block_abt;
    int mspel_bit;
    int cbp_table_index;
    int top_left_mv_flag;
    int per_mb_rl_bit;
    int skip_type;
    int hshift;

    ScanTable abt_scantable[2];
    DECLARE_ALIGNED(16, DCTELEM, abt_block2)[6][64];
}Wmv2Context;

void ff_wmv2_common_init(Wmv2Context * w);

#endif 
