/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_INTERNAL_H
#define AVCODEC_INTERNAL_H

#include <stdint.h>
#include "avcodec.h"


int ff_is_hwaccel_pix_fmt(enum PixelFormat pix_fmt);


AVHWAccel *ff_find_hwaccel(enum CodecID codec_id, enum PixelFormat pix_fmt);


int ff_match_2uint16(const uint16_t (*tab)[2], int size, int a, int b);

#endif 
