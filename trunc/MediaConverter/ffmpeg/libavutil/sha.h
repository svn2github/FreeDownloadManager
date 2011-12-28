/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_SHA_H
#define AVUTIL_SHA_H

#include <stdint.h>

extern const int av_sha_size;

struct AVSHA;


int av_sha_init(struct AVSHA* context, int bits);


void av_sha_update(struct AVSHA* context, const uint8_t* data, unsigned int len);


void av_sha_final(struct AVSHA* context, uint8_t *digest);

#endif 
