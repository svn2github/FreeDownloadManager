/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_JPEGLS_H
#define AVCODEC_JPEGLS_H

#include "avcodec.h"

typedef struct JpeglsContext{
    AVCodecContext *avctx;
    AVFrame picture;
}JpeglsContext;

typedef struct JLSState{
    int T1, T2, T3;
    int A[367], B[367], C[365], N[367];
    int limit, reset, bpp, qbpp, maxval, range;
    int near, twonear;
    int run_index[3];
}JLSState;

extern const uint8_t ff_log2_run[32];


void ff_jpegls_init_state(JLSState *state);


static inline int ff_jpegls_quantize(JLSState *s, int v){ 
    if(v==0) return 0;
    if(v < 0){
        if(v <= -s->T3) return -4;
        if(v <= -s->T2) return -3;
        if(v <= -s->T1) return -2;
        if(v <  -s->near) return -1;
        return 0;
    }else{
        if(v <= s->near) return 0;
        if(v <  s->T1) return 1;
        if(v <  s->T2) return 2;
        if(v <  s->T3) return 3;
        return 4;
    }
}


void ff_jpegls_reset_coding_parameters(JLSState *s, int reset_all);


static inline void ff_jpegls_downscale_state(JLSState *state, int Q){
    if(state->N[Q] == state->reset){
        state->A[Q] >>=1;
        state->B[Q] >>=1;
        state->N[Q] >>=1;
    }
    state->N[Q]++;
}

static inline int ff_jpegls_update_state_regular(JLSState *state, int Q, int err){
    state->A[Q] += FFABS(err);
    err *= state->twonear;
    state->B[Q] += err;

    ff_jpegls_downscale_state(state, Q);

    if(state->B[Q] <= -state->N[Q]) {
        state->B[Q]= FFMAX(state->B[Q] + state->N[Q], 1-state->N[Q]);
        if(state->C[Q] > -128)
            state->C[Q]--;
    }else if(state->B[Q] > 0){
        state->B[Q]= FFMIN(state->B[Q] - state->N[Q], 0);
        if(state->C[Q] < 127)
            state->C[Q]++;
    }

    return err;
}

#define R(a, i   ) (bits == 8 ?  ((uint8_t*)(a))[i]    :  ((uint16_t*)(a))[i]  )
#define W(a, i, v) (bits == 8 ? (((uint8_t*)(a))[i]=v) : (((uint16_t*)(a))[i]=v))

#endif 
