/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DCTREF_H
#define AVCODEC_DCTREF_H

#include "dsputil.h"

void ff_ref_fdct(DCTELEM *block);
void ff_ref_idct(DCTELEM *block);
void ff_ref_dct_init(void);

#endif
