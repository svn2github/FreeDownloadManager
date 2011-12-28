/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PARSER_H
#define AVCODEC_PARSER_H

#include "avcodec.h"

typedef struct ParseContext{
    uint8_t *buffer;
    int index;
    int last_index;
    unsigned int buffer_size;
    uint32_t state;             
    int frame_start_found;
    int overread;               
    int overread_index;         
    uint64_t state64;           
} ParseContext;

struct MpegEncContext;

typedef struct ParseContext1{
    ParseContext pc;

    
    AVRational frame_rate;
    int progressive_sequence;
    int width, height;

    
    struct MpegEncContext *enc;
    int first_picture;
} ParseContext1;

#define END_NOT_FOUND (-100)

int ff_combine_frame(ParseContext *pc, int next, const uint8_t **buf, int *buf_size);
int ff_mpeg4video_split(AVCodecContext *avctx, const uint8_t *buf,
                        int buf_size);
void ff_parse_close(AVCodecParserContext *s);
void ff_parse1_close(AVCodecParserContext *s);


void ff_fetch_timestamp(AVCodecParserContext *s, int off, int remove);

#endif 
