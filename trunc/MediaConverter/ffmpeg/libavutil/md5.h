/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_MD5_H
#define AVUTIL_MD5_H

#include <stdint.h>

extern const int av_md5_size;

struct AVMD5;

void av_md5_init(struct AVMD5 *ctx);
void av_md5_update(struct AVMD5 *ctx, const uint8_t *src, const int len);
void av_md5_final(struct AVMD5 *ctx, uint8_t *dst);
void av_md5_sum(uint8_t *dst, const uint8_t *src, const int len);

#endif 

