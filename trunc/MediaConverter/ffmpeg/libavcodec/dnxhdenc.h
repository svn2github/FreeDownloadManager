/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DNXHDENC_H
#define AVCODEC_DNXHDENC_H

#include <stdint.h>
#include "libavcodec/mpegvideo.h"
#include "libavcodec/dnxhddata.h"

typedef struct {
    uint16_t mb;
    int value;
} RCCMPEntry;

typedef struct {
    int ssd;
    int bits;
} RCEntry;

typedef struct DNXHDEncContext {
    MpegEncContext m; 

    AVFrame frame;
    int cid;
    const CIDEntry *cid_table;
    uint8_t *msip; 
    uint32_t *slice_size;
    uint32_t *slice_offs;

    struct DNXHDEncContext *thread[MAX_THREADS];

    unsigned dct_y_offset;
    unsigned dct_uv_offset;
    int interlaced;
    int cur_field;

    DECLARE_ALIGNED(16, DCTELEM, blocks)[8][64];

    int      (*qmatrix_c)     [64];
    int      (*qmatrix_l)     [64];
    uint16_t (*qmatrix_l16)[2][64];
    uint16_t (*qmatrix_c16)[2][64];

    unsigned frame_bits;
    uint8_t *src[3];

    uint32_t *vlc_codes;
    uint8_t  *vlc_bits;
    uint16_t *run_codes;
    uint8_t  *run_bits;

    
    unsigned slice_bits;
    unsigned qscale;
    unsigned lambda;

    unsigned thread_size;

    uint16_t *mb_bits;
    uint8_t  *mb_qscale;

    RCCMPEntry *mb_cmp;
    RCEntry   (*mb_rc)[8160];

    void (*get_pixels_8x4_sym)(DCTELEM *, const uint8_t *, int);
} DNXHDEncContext;

void ff_dnxhd_init_mmx(DNXHDEncContext *ctx);

#endif 
