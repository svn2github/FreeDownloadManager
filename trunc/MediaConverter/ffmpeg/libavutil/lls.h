/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_LLS_H
#define AVUTIL_LLS_H

#define MAX_VARS 32




typedef struct LLSModel{
    double covariance[MAX_VARS+1][MAX_VARS+1];
    double coeff[MAX_VARS][MAX_VARS];
    double variance[MAX_VARS];
    int indep_count;
}LLSModel;

void av_init_lls(LLSModel *m, int indep_count);
void av_update_lls(LLSModel *m, double *param, double decay);
void av_solve_lls(LLSModel *m, double threshold, int min_order);
double av_evaluate_lls(LLSModel *m, double *param, int order);

#endif 
