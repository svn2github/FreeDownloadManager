/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_IIRFILTER_H
#define AVCODEC_IIRFILTER_H

#include "avcodec.h"

struct FFIIRFilterCoeffs;
struct FFIIRFilterState;

enum IIRFilterType{
    FF_FILTER_TYPE_BESSEL,
    FF_FILTER_TYPE_BUTTERWORTH,
    FF_FILTER_TYPE_CHEBYSHEV,
    FF_FILTER_TYPE_ELLIPTIC,
};

enum IIRFilterMode{
    FF_FILTER_MODE_LOWPASS,
    FF_FILTER_MODE_HIGHPASS,
    FF_FILTER_MODE_BANDPASS,
    FF_FILTER_MODE_BANDSTOP,
};


struct FFIIRFilterCoeffs* ff_iir_filter_init_coeffs(enum IIRFilterType filt_type,
                                                    enum IIRFilterMode filt_mode,
                                                    int order, float cutoff_ratio,
                                                    float stopband, float ripple);


struct FFIIRFilterState* ff_iir_filter_init_state(int order);


void ff_iir_filter_free_coeffs(struct FFIIRFilterCoeffs *coeffs);


void ff_iir_filter_free_state(struct FFIIRFilterState *state);


void ff_iir_filter(const struct FFIIRFilterCoeffs *coeffs, struct FFIIRFilterState *state,
                   int size, const int16_t *src, int sstep, int16_t *dst, int dstep);

#endif 
