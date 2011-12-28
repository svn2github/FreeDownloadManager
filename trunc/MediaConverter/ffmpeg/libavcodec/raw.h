/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_RAW_H
#define AVCODEC_RAW_H

#include "avcodec.h"

typedef struct PixelFormatTag {
    enum PixelFormat pix_fmt;
    unsigned int fourcc;
} PixelFormatTag;

extern const PixelFormatTag ff_raw_pixelFormatTags[];

#endif 
