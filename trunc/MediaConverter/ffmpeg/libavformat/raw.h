/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_RAW_H
#define AVFORMAT_RAW_H

#include "avformat.h"

int pcm_read_seek(AVFormatContext *s,
                  int stream_index, int64_t timestamp, int flags);

int ff_raw_read_partial_packet(AVFormatContext *s, AVPacket *pkt);

#endif 
