/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_LSP_H
#define AVCODEC_LSP_H

#include <stdint.h>




void ff_acelp_reorder_lsf(int16_t* lsfq, int lsfq_min_distance, int lsfq_min, int lsfq_max, int lp_order);


void ff_set_min_dist_lsf(float *lsf, double min_spacing, int order);


void ff_acelp_lsf2lsp(int16_t *lsp, const int16_t *lsf, int lp_order);


void ff_acelp_lsp2lpc(int16_t* lp, const int16_t* lsp, int lp_half_order);


void ff_acelp_lp_decode(int16_t* lp_1st, int16_t* lp_2nd, const int16_t* lsp_2nd, const int16_t* lsp_prev, int lp_order);


#define MAX_LP_HALF_ORDER 8


void ff_acelp_lspd2lpc(const double *lsp, float *lpc, int lp_half_order);


void ff_sort_nearly_sorted_floats(float *vals, int len);


void ff_lsp2polyf(const double *lsp, double *f, int lp_half_order);

#endif 
