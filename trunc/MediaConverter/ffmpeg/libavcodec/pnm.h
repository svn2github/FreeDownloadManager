/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PNM_H
#define AVCODEC_PNM_H

#include "avcodec.h"

typedef struct PNMContext {
    uint8_t *bytestream;
    uint8_t *bytestream_start;
    uint8_t *bytestream_end;
    AVFrame picture;
    int maxval;                 
    int type;
} PNMContext;

int ff_pnm_decode_header(AVCodecContext *avctx, PNMContext * const s);
av_cold int ff_pnm_end(AVCodecContext *avctx);
av_cold int ff_pnm_init(AVCodecContext *avctx);

#endif 
