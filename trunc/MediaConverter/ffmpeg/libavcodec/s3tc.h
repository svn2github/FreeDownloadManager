/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_S3TC_H
#define AVCODEC_S3TC_H

#include <stdint.h>

#define FF_S3TC_DXT1    0x31545844
#define FF_S3TC_DXT3    0x33545844


void ff_decode_dxt1(const uint8_t *src, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride);

void ff_decode_dxt3(const uint8_t *src, uint8_t *dst,
                    const unsigned int w, const unsigned int h,
                    const unsigned int stride);

#endif 
