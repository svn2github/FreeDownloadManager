/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_CELP_FILTERS_H
#define AVCODEC_CELP_FILTERS_H

#include <stdint.h>


void ff_celp_convolve_circ(int16_t *fc_out, const int16_t *fc_in,
                           const int16_t *filter, int len);


void ff_celp_circ_addf(float *out, const float *in,
                       const float *lagged, int lag, float fac, int n);


int ff_celp_lp_synthesis_filter(int16_t *out, const int16_t *filter_coeffs,
                                const int16_t *in, int buffer_length,
                                int filter_length, int stop_on_overflow,
                                int rounder);


void ff_celp_lp_synthesis_filterf(float *out, const float *filter_coeffs,
                                  const float *in, int buffer_length,
                                  int filter_length);


void ff_celp_lp_zero_synthesis_filterf(float *out, const float *filter_coeffs,
                                       const float *in, int buffer_length,
                                       int filter_length);

#endif 
