/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_ATRAC_H
#define AVCODEC_ATRAC_H


extern float sf_table[64];
extern float qmf_window[48];

void atrac_generate_tables(void);
void atrac_iqmf (float *inlo, float *inhi, unsigned int nIn, float *pOut, float *delayBuf, float *temp);

#endif 
