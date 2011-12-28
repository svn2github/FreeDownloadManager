/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ARM_MPEGVIDEO_H
#define AVCODEC_ARM_MPEGVIDEO_H

#include "libavcodec/mpegvideo.h"

void MPV_common_init_iwmmxt(MpegEncContext *s);
void MPV_common_init_armv5te(MpegEncContext *s);

#endif
