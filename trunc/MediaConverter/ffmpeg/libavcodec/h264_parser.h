/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_H264_PARSER_H
#define AVCODEC_H264_PARSER_H

#include "h264.h"


int ff_h264_find_frame_end(H264Context *h, const uint8_t *buf, int buf_size);

#endif 
