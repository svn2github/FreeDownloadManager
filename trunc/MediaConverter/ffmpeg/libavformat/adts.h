/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ADTS_H
#define AVFORMAT_ADTS_H

#include "avformat.h"
#include "libavcodec/mpeg4audio.h"

#define ADTS_HEADER_SIZE 7

typedef struct {
    int write_adts;
    int objecttype;
    int sample_rate_index;
    int channel_conf;
    int pce_size;
    uint8_t pce_data[MAX_PCE_SIZE];
} ADTSContext;

int ff_adts_write_frame_header(ADTSContext *ctx, uint8_t *buf,
                               int size, int pce_size);
int ff_adts_decode_extradata(AVFormatContext *s, ADTSContext *adts,
                             uint8_t *buf, int size);

#endif 
