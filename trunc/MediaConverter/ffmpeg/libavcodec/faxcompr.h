/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef AVCODEC_FAXCOMPR_H
#define AVCODEC_FAXCOMPR_H

#include "avcodec.h"
#include "tiff.h"


void ff_ccitt_unpack_init(void);


int ff_ccitt_unpack(AVCodecContext *avctx,
                    const uint8_t *src, int srcsize,
                    uint8_t *dst, int height, int stride,
                    enum TiffCompr compr, int opts);

#endif 
