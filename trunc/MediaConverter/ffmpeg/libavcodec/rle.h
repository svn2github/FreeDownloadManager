/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_RLE_H
#define AVCODEC_RLE_H

#include <stdint.h>


int ff_rle_encode(uint8_t *outbuf, int out_size, const uint8_t *inbuf, int bpp, int w,
                  int add_rep, int xor_rep, int add_raw, int xor_raw);

#endif 
