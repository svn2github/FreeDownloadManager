/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_SIMPLE_IDCT_H
#define AVCODEC_SIMPLE_IDCT_H

#include <stdint.h>
#include "dsputil.h"

void ff_simple_idct_put(uint8_t *dest, int line_size, DCTELEM *block);
void ff_simple_idct_add(uint8_t *dest, int line_size, DCTELEM *block);
void ff_simple_idct_mmx(int16_t *block);
void ff_simple_idct_add_mmx(uint8_t *dest, int line_size, int16_t *block);
void ff_simple_idct_put_mmx(uint8_t *dest, int line_size, int16_t *block);
void ff_simple_idct(DCTELEM *block);

void ff_simple_idct248_put(uint8_t *dest, int line_size, DCTELEM *block);

void ff_simple_idct84_add(uint8_t *dest, int line_size, DCTELEM *block);
void ff_simple_idct48_add(uint8_t *dest, int line_size, DCTELEM *block);
void ff_simple_idct44_add(uint8_t *dest, int line_size, DCTELEM *block);

#endif 
