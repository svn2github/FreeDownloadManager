/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MJPEGENC_H
#define AVCODEC_MJPEGENC_H

#include "dsputil.h"
#include "mpegvideo.h"

typedef struct MJpegContext {
    uint8_t huff_size_dc_luminance[12]; 
    uint16_t huff_code_dc_luminance[12];
    uint8_t huff_size_dc_chrominance[12];
    uint16_t huff_code_dc_chrominance[12];

    uint8_t huff_size_ac_luminance[256];
    uint16_t huff_code_ac_luminance[256];
    uint8_t huff_size_ac_chrominance[256];
    uint16_t huff_code_ac_chrominance[256];
} MJpegContext;

int  ff_mjpeg_encode_init(MpegEncContext *s);
void ff_mjpeg_encode_close(MpegEncContext *s);
void ff_mjpeg_encode_picture_header(MpegEncContext *s);
void ff_mjpeg_encode_picture_trailer(MpegEncContext *s);
void ff_mjpeg_encode_stuffing(PutBitContext *pbc);
void ff_mjpeg_encode_dc(MpegEncContext *s, int val,
                        uint8_t *huff_size, uint16_t *huff_code);
void ff_mjpeg_encode_mb(MpegEncContext *s, DCTELEM block[6][64]);

#endif 
