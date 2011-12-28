/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_X86_IDCT_XVID_H
#define AVCODEC_X86_IDCT_XVID_H

#include <stdint.h>

void ff_idct_xvid_mmx(short *block);
void ff_idct_xvid_mmx2(short *block);
void ff_idct_xvid_sse2(short *block);
void ff_idct_xvid_sse2_put(uint8_t *dest, int line_size, short *block);
void ff_idct_xvid_sse2_add(uint8_t *dest, int line_size, short *block);

#endif 
