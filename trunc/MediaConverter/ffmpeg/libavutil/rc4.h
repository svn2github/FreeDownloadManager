/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_RC4_H
#define AVUTIL_RC4_H

#include <stdint.h>

struct AVRC4 {
    uint8_t state[256];
    int x, y;
};


int av_rc4_init(struct AVRC4 *d, const uint8_t *key, int key_bits, int decrypt);


void av_rc4_crypt(struct AVRC4 *d, uint8_t *dst, const uint8_t *src, int count, uint8_t *iv, int decrypt);

#endif 
