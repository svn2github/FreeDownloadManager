/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_H264DSP_H
#define AVCODEC_H264DSP_H

#include <stdint.h>
#include "dsputil.h"


typedef void (*h264_weight_func)(uint8_t *block, int stride, int log2_denom, int weight, int offset);
typedef void (*h264_biweight_func)(uint8_t *dst, uint8_t *src, int stride, int log2_denom, int weightd, int weights, int offset);


typedef struct H264DSPContext{
    
    h264_weight_func weight_h264_pixels_tab[10];
    h264_biweight_func biweight_h264_pixels_tab[10];

    
    void (*h264_v_loop_filter_luma)(uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0);
    void (*h264_h_loop_filter_luma)(uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0);
    
    void (*h264_v_loop_filter_luma_intra)(uint8_t *pix, int stride, int alpha, int beta);
    void (*h264_h_loop_filter_luma_intra)(uint8_t *pix, int stride, int alpha, int beta);
    void (*h264_v_loop_filter_chroma)(uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0);
    void (*h264_h_loop_filter_chroma)(uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0);
    void (*h264_v_loop_filter_chroma_intra)(uint8_t *pix, int stride, int alpha, int beta);
    void (*h264_h_loop_filter_chroma_intra)(uint8_t *pix, int stride, int alpha, int beta);
    
    void (*h264_loop_filter_strength)(int16_t bS[2][4][4], uint8_t nnz[40], int8_t ref[2][40], int16_t mv[2][40][2],
                                      int bidir, int edges, int step, int mask_mv0, int mask_mv1, int field);

    
    
    void (*h264_idct_add)(uint8_t *dst, DCTELEM *block, int stride);
    void (*h264_idct8_add)(uint8_t *dst, DCTELEM *block, int stride);
    void (*h264_idct_dc_add)(uint8_t *dst, DCTELEM *block, int stride);
    void (*h264_idct8_dc_add)(uint8_t *dst, DCTELEM *block, int stride);
    void (*h264_dct)(DCTELEM block[4][4]);
    void (*h264_idct_add16)(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
    void (*h264_idct8_add4)(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
    void (*h264_idct_add8)(uint8_t **dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
    void (*h264_idct_add16intra)(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
}H264DSPContext;

void ff_h264dsp_init(H264DSPContext *c);
void ff_h264dsp_init_arm(H264DSPContext *c);
void ff_h264dsp_init_ppc(H264DSPContext *c);
void ff_h264dsp_init_x86(H264DSPContext *c);

#endif 
