/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_FLAC_H
#define AVCODEC_FLAC_H

#include "avcodec.h"

#define FLAC_STREAMINFO_SIZE   34
#define FLAC_MAX_CHANNELS       8
#define FLAC_MIN_BLOCKSIZE     16
#define FLAC_MAX_BLOCKSIZE  65535

enum {
    FLAC_CHMODE_INDEPENDENT =  0,
    FLAC_CHMODE_LEFT_SIDE   =  8,
    FLAC_CHMODE_RIGHT_SIDE  =  9,
    FLAC_CHMODE_MID_SIDE    = 10,
};

enum {
    FLAC_METADATA_TYPE_STREAMINFO = 0,
    FLAC_METADATA_TYPE_PADDING,
    FLAC_METADATA_TYPE_APPLICATION,
    FLAC_METADATA_TYPE_SEEKTABLE,
    FLAC_METADATA_TYPE_VORBIS_COMMENT,
    FLAC_METADATA_TYPE_CUESHEET,
    FLAC_METADATA_TYPE_PICTURE,
    FLAC_METADATA_TYPE_INVALID = 127
};

enum FLACExtradataFormat {
    FLAC_EXTRADATA_FORMAT_STREAMINFO  = 0,
    FLAC_EXTRADATA_FORMAT_FULL_HEADER = 1
};

#define FLACCOMMONINFO \
    int samplerate;         \
    int channels;           \
    int bps;                \


#define FLACSTREAMINFO \
    FLACCOMMONINFO \
    int max_blocksize;      \
    int max_framesize;      \
    int64_t samples;        \

typedef struct FLACStreaminfo {
    FLACSTREAMINFO
} FLACStreaminfo;

typedef struct FLACFrameInfo {
    FLACCOMMONINFO
    int blocksize;          
    int ch_mode;            
} FLACFrameInfo;


void ff_flac_parse_streaminfo(AVCodecContext *avctx, struct FLACStreaminfo *s,
                              const uint8_t *buffer);


int ff_flac_is_extradata_valid(AVCodecContext *avctx,
                               enum FLACExtradataFormat *format,
                               uint8_t **streaminfo_start);


void ff_flac_parse_block_header(const uint8_t *block_header,
                                int *last, int *type, int *size);


int ff_flac_get_max_frame_size(int blocksize, int ch, int bps);

#endif 
