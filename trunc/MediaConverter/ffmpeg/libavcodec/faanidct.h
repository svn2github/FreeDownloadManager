/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_FAANIDCT_H
#define AVCODEC_FAANIDCT_H

#include <stdint.h>
#include "dsputil.h"

void ff_faanidct(DCTELEM block[64]);
void ff_faanidct_add(uint8_t *dest, int line_size, DCTELEM block[64]);
void ff_faanidct_put(uint8_t *dest, int line_size, DCTELEM block[64]);

#endif 
