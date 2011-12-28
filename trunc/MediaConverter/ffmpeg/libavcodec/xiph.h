/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_XIPH_H
#define AVCODEC_XIPH_H

#include "libavutil/common.h"


int ff_split_xiph_headers(uint8_t *extradata, int extradata_size,
                          int first_header_size, uint8_t *header_start[3],
                          int header_len[3]);

#endif 
