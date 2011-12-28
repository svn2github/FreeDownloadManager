/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_FLACENC_H
#define AVFORMAT_FLACENC_H

#include "libavcodec/flac.h"
#include "libavcodec/bytestream.h"
#include "avformat.h"

int ff_flac_write_header(ByteIOContext *pb, AVCodecContext *codec,
                         int last_block);

#endif 
