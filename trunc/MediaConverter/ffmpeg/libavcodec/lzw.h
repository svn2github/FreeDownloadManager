/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_LZW_H
#define AVCODEC_LZW_H

#include <stdint.h>

struct PutBitContext;

enum FF_LZW_MODES{
    FF_LZW_GIF,
    FF_LZW_TIFF
};


typedef void LZWState;


void ff_lzw_decode_open(LZWState **p);
void ff_lzw_decode_close(LZWState **p);
int ff_lzw_decode_init(LZWState *s, int csize, const uint8_t *buf, int buf_size, int mode);
int ff_lzw_decode(LZWState *s, uint8_t *buf, int len);
const uint8_t* ff_lzw_cur_ptr(LZWState *lzw);
void ff_lzw_decode_tail(LZWState *lzw);


struct LZWEncodeState;
extern const int ff_lzw_encode_state_size;

void ff_lzw_encode_init(struct LZWEncodeState *s, uint8_t *outbuf, int outsize,
                        int maxbits, enum FF_LZW_MODES mode,
                        void (*lzw_put_bits)(struct PutBitContext *, int, unsigned int));
int ff_lzw_encode(struct LZWEncodeState * s, const uint8_t * inbuf, int insize);
int ff_lzw_encode_flush(struct LZWEncodeState *s,
                        void (*lzw_flush_put_bits)(struct PutBitContext *));

#endif 
