/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ROQVIDEO_H
#define AVCODEC_ROQVIDEO_H

#include "libavutil/lfg.h"
#include "avcodec.h"
#include "dsputil.h"

typedef struct {
    unsigned char y[4];
    unsigned char u, v;
} roq_cell;

typedef struct {
    int idx[4];
} roq_qcell;

typedef struct {
    int d[2];
} motion_vect;

struct RoqTempData;

typedef struct RoqContext {

    AVCodecContext *avctx;
    DSPContext dsp;
    AVFrame frames[2];
    AVFrame *last_frame;
    AVFrame *current_frame;
    int first_frame;

    roq_cell cb2x2[256];
    roq_qcell cb4x4[256];

    const unsigned char *buf;
    int size;
    int width, height;

    
    AVLFG randctx;
    uint64_t lambda;

    motion_vect *this_motion4;
    motion_vect *last_motion4;

    motion_vect *this_motion8;
    motion_vect *last_motion8;

    unsigned int framesSinceKeyframe;

    AVFrame *frame_to_enc;
    uint8_t *out_buf;
    struct RoqTempData *tmpData;
} RoqContext;

#define RoQ_INFO              0x1001
#define RoQ_QUAD_CODEBOOK     0x1002
#define RoQ_QUAD_VQ           0x1011
#define RoQ_SOUND_MONO        0x1020
#define RoQ_SOUND_STEREO      0x1021

#define RoQ_ID_MOT              0x00
#define RoQ_ID_FCC              0x01
#define RoQ_ID_SLD              0x02
#define RoQ_ID_CCC              0x03

void ff_apply_vector_2x2(RoqContext *ri, int x, int y, roq_cell *cell);
void ff_apply_vector_4x4(RoqContext *ri, int x, int y, roq_cell *cell);

void ff_apply_motion_4x4(RoqContext *ri, int x, int y, int deltax, int deltay);

void ff_apply_motion_8x8(RoqContext *ri, int x, int y, int deltax, int deltay);

#endif 
