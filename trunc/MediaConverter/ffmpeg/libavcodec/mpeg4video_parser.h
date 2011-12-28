/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MPEG4VIDEO_PARSER_H
#define AVCODEC_MPEG4VIDEO_PARSER_H

#include "parser.h"


int ff_mpeg4_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size);

#endif 
