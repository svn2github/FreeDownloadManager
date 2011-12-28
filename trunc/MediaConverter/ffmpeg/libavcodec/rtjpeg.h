/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_RTJPEG_H
#define AVCODEC_RTJPEG_H

#include <stdint.h>
#include "dsputil.h"

typedef struct {
    int w, h;
    DSPContext *dsp;
    uint8_t scan[64];
    uint32_t lquant[64];
    uint32_t cquant[64];
    DECLARE_ALIGNED(16, DCTELEM, block)[64];
} RTJpegContext;

void rtjpeg_decode_init(RTJpegContext *c, DSPContext *dsp,
                        int width, int height,
                        const uint32_t *lquant, const uint32_t *cquant);

int rtjpeg_decode_frame_yuv420(RTJpegContext *c, AVFrame *f,
                               const uint8_t *buf, int buf_size);
#endif 
