/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ARM_DSPUTIL_H
#define AVCODEC_ARM_DSPUTIL_H

#include "libavcodec/avcodec.h"
#include "libavcodec/dsputil.h"

void ff_dsputil_init_armv5te(DSPContext* c, AVCodecContext *avctx);
void ff_dsputil_init_armv6(DSPContext* c, AVCodecContext *avctx);
void ff_dsputil_init_vfp(DSPContext* c, AVCodecContext *avctx);
void ff_dsputil_init_neon(DSPContext *c, AVCodecContext *avctx);
void ff_dsputil_init_iwmmxt(DSPContext* c, AVCodecContext *avctx);

#endif
