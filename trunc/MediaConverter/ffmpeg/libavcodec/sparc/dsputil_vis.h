/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_SPARC_DSPUTIL_VIS_H
#define AVCODEC_SPARC_DSPUTIL_VIS_H

#include <stdint.h>
#include "libavcodec/dsputil.h"

void ff_simple_idct_put_vis(uint8_t *dest, int line_size, DCTELEM *data);
void ff_simple_idct_add_vis(uint8_t *dest, int line_size, DCTELEM *data);
void ff_simple_idct_vis(DCTELEM *data);

#endif
