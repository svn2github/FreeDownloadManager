/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_BASE64_H
#define AVUTIL_BASE64_H

#include <stdint.h>


int av_base64_decode(uint8_t *out, const char *in, int out_size);


char *av_base64_encode(char *out, int out_size, const uint8_t *in, int in_size);

#endif 
