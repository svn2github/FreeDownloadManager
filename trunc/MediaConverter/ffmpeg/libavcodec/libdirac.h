/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_LIBDIRAC_H
#define AVCODEC_LIBDIRAC_H

#include "avcodec.h"
#include <libdirac_common/dirac_types.h>


static const struct {
    enum PixelFormat ff_pix_fmt;
    dirac_chroma_t dirac_pix_fmt;
} ffmpeg_dirac_pixel_format_map[] = {
    { PIX_FMT_YUV420P, format420 },
    { PIX_FMT_YUV422P, format422 },
    { PIX_FMT_YUV444P, format444 },
};

#endif 
