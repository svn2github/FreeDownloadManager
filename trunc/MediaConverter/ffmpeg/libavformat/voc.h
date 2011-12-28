/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_VOC_H
#define AVFORMAT_VOC_H

#include "avformat.h"
#include "riff.h"    

typedef struct voc_dec_context {
    int64_t remaining_size;
} VocDecContext;

typedef enum voc_type {
    VOC_TYPE_EOF              = 0x00,
    VOC_TYPE_VOICE_DATA       = 0x01,
    VOC_TYPE_VOICE_DATA_CONT  = 0x02,
    VOC_TYPE_SILENCE          = 0x03,
    VOC_TYPE_MARKER           = 0x04,
    VOC_TYPE_ASCII            = 0x05,
    VOC_TYPE_REPETITION_START = 0x06,
    VOC_TYPE_REPETITION_END   = 0x07,
    VOC_TYPE_EXTENDED         = 0x08,
    VOC_TYPE_NEW_VOICE_DATA   = 0x09,
} VocType;

extern const unsigned char ff_voc_magic[21];
extern const AVCodecTag ff_voc_codec_tags[];

int voc_get_packet(AVFormatContext *s, AVPacket *pkt,
                   AVStream *st, int max_size);

#endif 
