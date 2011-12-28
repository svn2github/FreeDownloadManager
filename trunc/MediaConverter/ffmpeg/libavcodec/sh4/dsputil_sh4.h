/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_SH4_DSPUTIL_SH4_H
#define AVCODEC_SH4_DSPUTIL_SH4_H

#include "libavcodec/avcodec.h"
#include "libavcodec/dsputil.h"

void idct_sh4(DCTELEM *block);
void dsputil_init_align(DSPContext* c, AVCodecContext *avctx);

#endif
