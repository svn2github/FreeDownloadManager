/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_X86_FFT_H
#define AVCODEC_X86_FFT_H

#include "libavcodec/fft.h"

void ff_fft_permute_sse(FFTContext *s, FFTComplex *z);
void ff_fft_calc_sse(FFTContext *s, FFTComplex *z);
void ff_fft_calc_3dn(FFTContext *s, FFTComplex *z);
void ff_fft_calc_3dn2(FFTContext *s, FFTComplex *z);

void ff_imdct_calc_3dn(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_half_3dn(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_calc_3dn2(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_half_3dn2(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_calc_sse(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_half_sse(FFTContext *s, FFTSample *output, const FFTSample *input);

#endif
