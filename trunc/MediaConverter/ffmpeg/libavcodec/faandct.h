/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_FAANDCT_H
#define AVCODEC_FAANDCT_H

#include "dsputil.h"

#define FAAN_POSTSCALE

void ff_faandct(DCTELEM * data);
void ff_faandct248(DCTELEM * data);

#endif 
