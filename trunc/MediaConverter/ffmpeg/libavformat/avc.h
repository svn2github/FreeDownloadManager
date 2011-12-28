/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_AVC_H
#define AVFORMAT_AVC_H

#include <stdint.h>
#include "avio.h"

int ff_avc_parse_nal_units(ByteIOContext *s, const uint8_t *buf, int size);
int ff_avc_parse_nal_units_buf(const uint8_t *buf_in, uint8_t **buf, int *size);
int ff_isom_write_avcc(ByteIOContext *pb, const uint8_t *data, int len);
const uint8_t *ff_avc_find_startcode(const uint8_t *p, const uint8_t *end);

#endif 
