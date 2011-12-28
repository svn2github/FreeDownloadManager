/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_LIBSCHROEDINGER_H
#define AVCODEC_LIBSCHROEDINGER_H

#include <schroedinger/schrobitstream.h>
#include <schroedinger/schroframe.h>
#include "avcodec.h"

static const struct {
    enum PixelFormat  ff_pix_fmt;
    SchroChromaFormat schro_pix_fmt;
    SchroFrameFormat  schro_frame_fmt;
} ffmpeg_schro_pixel_format_map[] = {
    { PIX_FMT_YUV420P, SCHRO_CHROMA_420, SCHRO_FRAME_FORMAT_U8_420 },
    { PIX_FMT_YUV422P, SCHRO_CHROMA_422, SCHRO_FRAME_FORMAT_U8_422 },
    { PIX_FMT_YUV444P, SCHRO_CHROMA_444, SCHRO_FRAME_FORMAT_U8_444 },
};


SchroVideoFormatEnum ff_get_schro_video_format_preset (AVCodecContext *avccontext);


int ff_get_schro_frame_format(SchroChromaFormat schro_chroma_fmt,
                              SchroFrameFormat  *schro_frame_fmt);


SchroFrame *ff_create_schro_frame(AVCodecContext *avccontext,
                                  SchroFrameFormat schro_frame_fmt);

#endif 
