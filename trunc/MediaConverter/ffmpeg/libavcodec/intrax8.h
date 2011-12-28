/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_INTRAX8_H
#define AVCODEC_INTRAX8_H

#include "get_bits.h"
#include "mpegvideo.h"

typedef struct{
    VLC * j_ac_vlc[4];
    VLC * j_orient_vlc;
    VLC * j_dc_vlc[3];

    int use_quant_matrix;

    uint8_t * prediction_table;
    ScanTable scantable[3];

    MpegEncContext * s;
    int quant;
    int dquant;
    int qsum;

    int quant_dc_chroma;
    int divide_quant_dc_luma;
    int divide_quant_dc_chroma;

    int edges;
    int flat_dc;
    int predicted_dc;
    int raw_orient;
    int chroma_orient;
    int orient;
    int est_run;
} IntraX8Context;

void ff_intrax8_common_init(IntraX8Context * w, MpegEncContext * const s);
void ff_intrax8_common_end(IntraX8Context * w);
int  ff_intrax8_decode_picture(IntraX8Context * w, int quant, int halfpq);

#endif 
