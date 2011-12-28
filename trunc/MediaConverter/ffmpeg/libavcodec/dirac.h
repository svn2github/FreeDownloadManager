/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DIRAC_H
#define AVCODEC_DIRAC_H



#include "avcodec.h"
#include "get_bits.h"

typedef struct {
    unsigned width;
    unsigned height;
    uint8_t chroma_format;          

    uint8_t interlaced;
    uint8_t top_field_first;

    uint8_t frame_rate_index;       
    uint8_t aspect_ratio_index;     

    uint16_t clean_width;
    uint16_t clean_height;
    uint16_t clean_left_offset;
    uint16_t clean_right_offset;

    uint8_t pixel_range_index;      
    uint8_t color_spec_index;       
} dirac_source_params;

int ff_dirac_parse_sequence_header(AVCodecContext *avctx, GetBitContext *gb,
                                   dirac_source_params *source);

#endif 
