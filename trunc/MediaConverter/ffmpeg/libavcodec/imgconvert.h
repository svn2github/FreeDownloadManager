/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_IMGCONVERT_H
#define AVCODEC_IMGCONVERT_H

#include <stdint.h>
#include "avcodec.h"

int ff_fill_linesize(AVPicture *picture, enum PixelFormat pix_fmt, int width);

int ff_fill_pointer(AVPicture *picture, uint8_t *ptr, enum PixelFormat pix_fmt, int height);

int ff_get_plane_bytewidth(enum PixelFormat pix_fmt, int width, int plane);

int ff_set_systematic_pal(uint32_t pal[256], enum PixelFormat pix_fmt);

#endif 
