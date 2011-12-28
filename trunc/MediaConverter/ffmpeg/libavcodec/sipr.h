/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_SIPR_H
#define AVCODEC_SIPR_H

#include "avcodec.h"
#include "dsputil.h"
#include "acelp_pitch_delay.h"

#define LP_FILTER_ORDER_16k  16
#define L_SUBFR_16k          80
#define PITCH_MIN            30
#define PITCH_MAX            281

#define LSFQ_DIFF_MIN        (0.0125 * M_PI)

#define LP_FILTER_ORDER      10


#define L_INTERPOL           (LP_FILTER_ORDER + 1)


#define SUBFR_SIZE           48

#define SUBFRAME_COUNT_16k   2

typedef enum {
    MODE_16k,
    MODE_8k5,
    MODE_6k5,
    MODE_5k0,
    MODE_COUNT
} SiprMode;

typedef struct {
    AVCodecContext *avctx;
    DSPContext dsp;

    SiprMode mode;

    float past_pitch_gain;
    float lsf_history[LP_FILTER_ORDER_16k];

    float excitation[L_INTERPOL + PITCH_MAX + 2 * L_SUBFR_16k];

    DECLARE_ALIGNED(16, float, synth_buf)[LP_FILTER_ORDER + 5*SUBFR_SIZE + 6];

    float lsp_history[LP_FILTER_ORDER];
    float gain_mem;
    float energy_history[4];
    float highpass_filt_mem[2];
    float postfilter_mem[PITCH_DELAY_MAX + LP_FILTER_ORDER];

    
    float tilt_mem;
    float postfilter_agc;
    float postfilter_mem5k0[PITCH_DELAY_MAX + LP_FILTER_ORDER];
    float postfilter_syn5k0[LP_FILTER_ORDER + SUBFR_SIZE*5];

    
    int pitch_lag_prev;
    float iir_mem[LP_FILTER_ORDER_16k+1];
    float filt_buf[2][LP_FILTER_ORDER_16k+1];
    float *filt_mem[2];
    float mem_preemph[LP_FILTER_ORDER_16k];
    float synth[LP_FILTER_ORDER_16k];
    double lsp_history_16k[16];
} SiprContext;

typedef struct {
    int ma_pred_switch;        
    int vq_indexes[5];
    int pitch_delay[5];        
    int gp_index[5];           
    int16_t fc_indexes[5][10]; 
    int gc_index[5];           
} SiprParameters;

extern const float ff_pow_0_5[16];

void ff_sipr_init_16k(SiprContext *ctx);

void ff_sipr_decode_frame_16k(SiprContext *ctx, SiprParameters *params,
                              float *out_data);

#endif 
