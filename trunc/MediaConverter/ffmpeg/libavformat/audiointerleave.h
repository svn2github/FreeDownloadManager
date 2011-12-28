/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_AUDIOINTERLEAVE_H
#define AVFORMAT_AUDIOINTERLEAVE_H

#include "libavutil/fifo.h"
#include "avformat.h"

typedef struct {
    AVFifoBuffer *fifo;
    unsigned fifo_size;           
    uint64_t dts;                 
    int sample_size;              
    const int *samples_per_frame; 
    const int *samples;           
    AVRational time_base;         
} AudioInterleaveContext;

int ff_audio_interleave_init(AVFormatContext *s, const int *samples_per_frame, AVRational time_base);
void ff_audio_interleave_close(AVFormatContext *s);

int ff_interleave_compare_dts(AVFormatContext *s, AVPacket *next, AVPacket *pkt);

int ff_audio_rechunk_interleave(AVFormatContext *s, AVPacket *out, AVPacket *pkt, int flush,
                        int (*get_packet)(AVFormatContext *, AVPacket *, AVPacket *, int),
                        int (*compare_ts)(AVFormatContext *, AVPacket *, AVPacket *));

#endif 
