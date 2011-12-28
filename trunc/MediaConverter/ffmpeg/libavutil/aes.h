/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_AES_H
#define AVUTIL_AES_H

#include <stdint.h>

extern const int av_aes_size;

struct AVAES;


int av_aes_init(struct AVAES *a, const uint8_t *key, int key_bits, int decrypt);


void av_aes_crypt(struct AVAES *a, uint8_t *dst, const uint8_t *src, int count, uint8_t *iv, int decrypt);

#endif 
