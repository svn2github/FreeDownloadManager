/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_INTERNAL_XVMC_H
#define AVCODEC_INTERNAL_XVMC_H

#include "avcodec.h"
#include "mpegvideo.h"

void ff_xvmc_init_block(MpegEncContext *s);
void ff_xvmc_pack_pblocks(MpegEncContext *s, int cbp);
int  ff_xvmc_field_start(MpegEncContext*s, AVCodecContext *avctx);
void ff_xvmc_field_end(MpegEncContext *s);
void ff_xvmc_decode_mb(MpegEncContext *s);

#endif 
