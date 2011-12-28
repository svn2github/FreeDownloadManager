/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ASFCRYPT_H
#define AVFORMAT_ASFCRYPT_H

#include <inttypes.h>

void ff_asfcrypt_dec(const uint8_t key[20], uint8_t *data, int len);

#endif 
