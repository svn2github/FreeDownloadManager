/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VDPAU_INTERNAL_H
#define AVCODEC_VDPAU_INTERNAL_H

#include <stdint.h>
#include "mpegvideo.h"

void ff_vdpau_add_data_chunk(MpegEncContext *s, const uint8_t *buf,
                             int buf_size);

void ff_vdpau_mpeg_picture_complete(MpegEncContext *s, const uint8_t *buf,
                                    int buf_size, int slice_count);

void ff_vdpau_h264_picture_start(MpegEncContext *s);
void ff_vdpau_h264_set_reference_frames(MpegEncContext *s);
void ff_vdpau_h264_picture_complete(MpegEncContext *s);

void ff_vdpau_vc1_decode_picture(MpegEncContext *s, const uint8_t *buf,
                                 int buf_size);

void ff_vdpau_mpeg4_decode_picture(MpegEncContext *s, const uint8_t *buf,
                                   int buf_size);

#endif 
