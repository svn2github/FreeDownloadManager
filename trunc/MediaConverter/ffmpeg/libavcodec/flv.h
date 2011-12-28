/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_FLV_H
#define AVCODEC_FLV_H

#include "mpegvideo.h"
#include "get_bits.h"
#include "put_bits.h"

void ff_flv_encode_picture_header(MpegEncContext * s, int picture_number);
void ff_flv2_encode_ac_esc(PutBitContext *pb, int slevel, int level, int run, int last);

int ff_flv_decode_picture_header(MpegEncContext *s);
void ff_flv2_decode_ac_esc(GetBitContext *gb, int *level, int *run, int *last);

#endif

