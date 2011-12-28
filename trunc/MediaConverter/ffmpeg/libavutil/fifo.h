/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_FIFO_H
#define AVUTIL_FIFO_H

#include <stdint.h>

typedef struct AVFifoBuffer {
    uint8_t *buffer;
    uint8_t *rptr, *wptr, *end;
    uint32_t rndx, wndx;
} AVFifoBuffer;


AVFifoBuffer *av_fifo_alloc(unsigned int size);


void av_fifo_free(AVFifoBuffer *f);


void av_fifo_reset(AVFifoBuffer *f);


int av_fifo_size(AVFifoBuffer *f);


int av_fifo_space(AVFifoBuffer *f);


int av_fifo_generic_read(AVFifoBuffer *f, void *dest, int buf_size, void (*func)(void*, void*, int));


int av_fifo_generic_write(AVFifoBuffer *f, void *src, int size, int (*func)(void*, void*, int));


int av_fifo_realloc2(AVFifoBuffer *f, unsigned int size);


void av_fifo_drain(AVFifoBuffer *f, int size);

static inline uint8_t av_fifo_peek(AVFifoBuffer *f, int offs)
{
    uint8_t *ptr = f->rptr + offs;
    if (ptr >= f->end)
        ptr -= f->end - f->buffer;
    return *ptr;
}
#endif 
