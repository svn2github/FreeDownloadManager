/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_DES_H
#define AVUTIL_DES_H

#include <stdint.h>

struct AVDES {
    uint64_t round_keys[3][16];
    int triple_des;
};


int av_des_init(struct AVDES *d, const uint8_t *key, int key_bits, int decrypt);


void av_des_crypt(struct AVDES *d, uint8_t *dst, const uint8_t *src, int count, uint8_t *iv, int decrypt);

#endif 
