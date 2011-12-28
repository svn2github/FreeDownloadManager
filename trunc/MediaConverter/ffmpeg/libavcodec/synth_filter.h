/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_SYNTH_FILTER_H
#define AVCODEC_SYNTH_FILTER_H

#include "fft.h"

typedef struct SynthFilterContext {
    void (*synth_filter_float)(FFTContext *imdct,
                               float *synth_buf_ptr, int *synth_buf_offset,
                               float synth_buf2[32], const float window[512],
                               float out[32], const float in[32],
                               float scale, float bias);
} SynthFilterContext;

void ff_synth_filter_init(SynthFilterContext *c);
void ff_synth_filter_init_arm(SynthFilterContext *c);

#endif 
