/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AAC_PARSER_H
#define AVCODEC_AAC_PARSER_H

#include <stdint.h>
#include "aac_ac3_parser.h"
#include "get_bits.h"

#define AAC_ADTS_HEADER_SIZE 7

typedef struct {
    uint32_t sample_rate;
    uint32_t samples;
    uint32_t bit_rate;
    uint8_t  crc_absent;
    uint8_t  object_type;
    uint8_t  sampling_index;
    uint8_t  chan_config;
    uint8_t  num_aac_frames;
} AACADTSHeaderInfo;


int ff_aac_parse_header(GetBitContext *gbc, AACADTSHeaderInfo *hdr);

#endif 
