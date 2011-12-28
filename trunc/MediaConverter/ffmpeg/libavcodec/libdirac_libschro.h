/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_LIBDIRAC_LIBSCHRO_H
#define AVCODEC_LIBDIRAC_LIBSCHRO_H

#include "avcodec.h"

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t frame_rate_num;
    uint16_t frame_rate_denom;
} FfmpegDiracSchroVideoFormatInfo;


unsigned int ff_dirac_schro_get_video_format_idx(AVCodecContext *avccontext);


typedef struct FfmpegDiracSchroEncodedFrame {
    
    uint8_t *p_encbuf;

    
    uint32_t size;

    
    uint32_t frame_num;

    
    uint16_t key_frame;
} FfmpegDiracSchroEncodedFrame;


typedef struct FfmpegDiracSchroQueueElement {
    
    void *data;
    
    struct FfmpegDiracSchroQueueElement *next;
} FfmpegDiracSchroQueueElement;



typedef struct FfmpegDiracSchroQueue {
    
    FfmpegDiracSchroQueueElement *p_head;
    
    FfmpegDiracSchroQueueElement *p_tail;
    
    int size;
} FfmpegDiracSchroQueue;


void ff_dirac_schro_queue_init(FfmpegDiracSchroQueue *queue);


int ff_dirac_schro_queue_push_back(FfmpegDiracSchroQueue *queue, void *p_data);


void *ff_dirac_schro_queue_pop(FfmpegDiracSchroQueue *queue);


void ff_dirac_schro_queue_free(FfmpegDiracSchroQueue *queue,
                               void (*free_func)(void *));
#endif 
