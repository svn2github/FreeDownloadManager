/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MSRLEDEC_H
#define AVCODEC_MSRLEDEC_H

#include "avcodec.h"


int ff_msrle_decode(AVCodecContext *avctx, AVPicture *pic, int depth,
                    const uint8_t* data, int data_size);

#endif 

