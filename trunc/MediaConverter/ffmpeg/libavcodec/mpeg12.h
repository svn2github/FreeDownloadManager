/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MPEG12_H
#define AVCODEC_MPEG12_H

#include "mpegvideo.h"

#define DC_VLC_BITS 9
#define TEX_VLC_BITS 9

static VLC dc_lum_vlc;
static VLC dc_chroma_vlc;

extern uint8_t ff_mpeg12_static_rl_table_store[2][2][2*MAX_RUN + MAX_LEVEL + 3];

void ff_mpeg12_common_init(MpegEncContext *s);
void ff_mpeg12_init_vlcs(void);

static inline int decode_dc(GetBitContext *gb, int component)
{
    int code, diff;

    if (component == 0) {
        code = get_vlc2(gb, dc_lum_vlc.table, DC_VLC_BITS, 2);
    } else {
        code = get_vlc2(gb, dc_chroma_vlc.table, DC_VLC_BITS, 2);
    }
    if (code < 0){
        av_log(NULL, AV_LOG_ERROR, "invalid dc code at\n");
        return 0xffff;
    }
    if (code == 0) {
        diff = 0;
    } else {
        diff = get_xbits(gb, code);
    }
    return diff;
}

extern int ff_mpeg1_decode_block_intra(MpegEncContext *s, DCTELEM *block, int n);

#endif 
