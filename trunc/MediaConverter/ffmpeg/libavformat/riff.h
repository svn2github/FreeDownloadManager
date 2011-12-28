/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVFORMAT_RIFF_H
#define AVFORMAT_RIFF_H

#include "libavcodec/avcodec.h"
#include "avio.h"

int64_t ff_start_tag(ByteIOContext *pb, const char *tag);
void ff_end_tag(ByteIOContext *pb, int64_t start);

typedef struct AVCodecTag {
    enum CodecID id;
    unsigned int tag;
} AVCodecTag;

void ff_put_bmp_header(ByteIOContext *pb, AVCodecContext *enc, const AVCodecTag *tags, int for_asf);
int ff_put_wav_header(ByteIOContext *pb, AVCodecContext *enc);
enum CodecID ff_wav_codec_get_id(unsigned int tag, int bps);
void ff_get_wav_header(ByteIOContext *pb, AVCodecContext *codec, int size);

extern const AVCodecTag ff_codec_bmp_tags[];
extern const AVCodecTag ff_codec_wav_tags[];

unsigned int ff_codec_get_tag(const AVCodecTag *tags, int id);
enum CodecID ff_codec_get_id(const AVCodecTag *tags, unsigned int tag);
void ff_parse_specific_params(AVCodecContext *stream, int *au_rate, int *au_ssize, int *au_scale);

#endif 
