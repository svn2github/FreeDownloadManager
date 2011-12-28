/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_CRC_H
#define AVUTIL_CRC_H

#include <stdint.h>
#include <stddef.h>
#include "attributes.h"

typedef uint32_t AVCRC;

typedef enum {
    AV_CRC_8_ATM,
    AV_CRC_16_ANSI,
    AV_CRC_16_CCITT,
    AV_CRC_32_IEEE,
    AV_CRC_32_IEEE_LE,  
    AV_CRC_MAX,         
}AVCRCId;

int av_crc_init(AVCRC *ctx, int le, int bits, uint32_t poly, int ctx_size);
const AVCRC *av_crc_get_table(AVCRCId crc_id);
uint32_t av_crc(const AVCRC *ctx, uint32_t start_crc, const uint8_t *buffer, size_t length) av_pure;

#endif 

