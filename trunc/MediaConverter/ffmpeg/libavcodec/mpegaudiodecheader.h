/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPEGAUDIODECHEADER_H
#define AVCODEC_MPEGAUDIODECHEADER_H

#include "libavutil/common.h"
#include "mpegaudio.h"



int ff_mpegaudio_decode_header(MPADecodeHeader *s, uint32_t header);

#endif 
