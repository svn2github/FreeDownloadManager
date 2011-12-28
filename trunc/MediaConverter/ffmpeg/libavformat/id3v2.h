/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ID3V2_H
#define AVFORMAT_ID3V2_H

#include <stdint.h>
#include "avformat.h"
#include "metadata.h"

#define ID3v2_HEADER_SIZE 10


int ff_id3v2_match(const uint8_t *buf);


int ff_id3v2_tag_len(const uint8_t *buf);


void ff_id3v2_parse(AVFormatContext *s, int len, uint8_t version, uint8_t flags);


void ff_id3v2_read(AVFormatContext *s);

extern const AVMetadataConv ff_id3v2_metadata_conv[];


extern const char ff_id3v2_tags[][4];

#endif 
