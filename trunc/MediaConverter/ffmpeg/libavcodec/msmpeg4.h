/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MSMPEG4_H
#define AVCODEC_MSMPEG4_H

#include "config.h"
#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"

#define INTER_INTRA_VLC_BITS 3
#define MB_NON_INTRA_VLC_BITS 9
#define MB_INTRA_VLC_BITS 9

extern VLC ff_mb_non_intra_vlc[4];
extern VLC ff_inter_intra_vlc;

void ff_msmpeg4_code012(PutBitContext *pb, int n);
void ff_msmpeg4_encode_block(MpegEncContext * s, DCTELEM * block, int n);
void ff_msmpeg4_handle_slices(MpegEncContext *s);
void ff_msmpeg4_encode_motion(MpegEncContext * s, int mx, int my);
int ff_msmpeg4_coded_block_pred(MpegEncContext * s, int n,
                                uint8_t **coded_block_ptr);
int ff_msmpeg4_decode_motion(MpegEncContext * s, int *mx_ptr, int *my_ptr);
int ff_msmpeg4_decode_block(MpegEncContext * s, DCTELEM * block,
                            int n, int coded, const uint8_t *scan_table);
int ff_wmv2_decode_mb(MpegEncContext *s, DCTELEM block[6][64]);

#define CONFIG_MSMPEG4_DECODER (CONFIG_MSMPEG4V1_DECODER || \
                                CONFIG_MSMPEG4V2_DECODER || \
                                CONFIG_MSMPEG4V3_DECODER || \
                                CONFIG_WMV2_DECODER      || \
                                CONFIG_VC1_DECODER)
#define CONFIG_MSMPEG4_ENCODER (CONFIG_MSMPEG4V1_ENCODER || \
                                CONFIG_MSMPEG4V2_ENCODER || \
                                CONFIG_MSMPEG4V3_ENCODER || \
                                CONFIG_WMV2_ENCODER)

#endif 
