/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef SWSCALE_RGB2RGB_H
#define SWSCALE_RGB2RGB_H

#include <inttypes.h>


extern void (*rgb24tobgr32)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb24tobgr16)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb24tobgr15)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32tobgr24)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32to16)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32to15)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb15to16)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb15tobgr24)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb15to32)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb16to15)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb16tobgr24)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb16to32)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb24tobgr24)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb24to16)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb24to15)   (const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32tobgr32)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32tobgr16)(const uint8_t *src, uint8_t *dst, long src_size);
extern void (*rgb32tobgr15)(const uint8_t *src, uint8_t *dst, long src_size);

void rgb24to32   (const uint8_t *src, uint8_t *dst, long src_size);
void rgb32to24   (const uint8_t *src, uint8_t *dst, long src_size);
void rgb16tobgr32(const uint8_t *src, uint8_t *dst, long src_size);
void rgb16to24   (const uint8_t *src, uint8_t *dst, long src_size);
void rgb16tobgr16(const uint8_t *src, uint8_t *dst, long src_size);
void rgb16tobgr15(const uint8_t *src, uint8_t *dst, long src_size);
void rgb15tobgr32(const uint8_t *src, uint8_t *dst, long src_size);
void rgb15to24   (const uint8_t *src, uint8_t *dst, long src_size);
void rgb15tobgr16(const uint8_t *src, uint8_t *dst, long src_size);
void rgb15tobgr15(const uint8_t *src, uint8_t *dst, long src_size);
void bgr8torgb8  (const uint8_t *src, uint8_t *dst, long src_size);

void shuffle_bytes_0321(const uint8_t *src, uint8_t *dst, long src_size);
void shuffle_bytes_1230(const uint8_t *src, uint8_t *dst, long src_size);
void shuffle_bytes_2103(const uint8_t *src, uint8_t *dst, long src_size);
void shuffle_bytes_3012(const uint8_t *src, uint8_t *dst, long src_size);
void shuffle_bytes_3210(const uint8_t *src, uint8_t *dst, long src_size);

void palette8topacked32(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);
void palette8topacked24(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);
void palette8torgb16(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);
void palette8tobgr16(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);
void palette8torgb15(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);
void palette8tobgr15(const uint8_t *src, uint8_t *dst, long num_pixels, const uint8_t *palette);





extern void (*yv12toyuy2)(const uint8_t *ysrc, const uint8_t *usrc, const uint8_t *vsrc, uint8_t *dst,
                          long width, long height,
                          long lumStride, long chromStride, long dstStride);


extern void (*yuv422ptoyuy2)(const uint8_t *ysrc, const uint8_t *usrc, const uint8_t *vsrc, uint8_t *dst,
                             long width, long height,
                             long lumStride, long chromStride, long dstStride);


extern void (*yuy2toyv12)(const uint8_t *src, uint8_t *ydst, uint8_t *udst, uint8_t *vdst,
                          long width, long height,
                          long lumStride, long chromStride, long srcStride);


extern void (*yv12touyvy)(const uint8_t *ysrc, const uint8_t *usrc, const uint8_t *vsrc, uint8_t *dst,
                          long width, long height,
                          long lumStride, long chromStride, long dstStride);


extern void (*yuv422ptouyvy)(const uint8_t *ysrc, const uint8_t *usrc, const uint8_t *vsrc, uint8_t *dst,
                             long width, long height,
                             long lumStride, long chromStride, long dstStride);


extern void (*rgb24toyv12)(const uint8_t *src, uint8_t *ydst, uint8_t *udst, uint8_t *vdst,
                           long width, long height,
                           long lumStride, long chromStride, long srcStride);
extern void (*planar2x)(const uint8_t *src, uint8_t *dst, long width, long height,
                        long srcStride, long dstStride);

extern void (*interleaveBytes)(const uint8_t *src1, const uint8_t *src2, uint8_t *dst,
                               long width, long height, long src1Stride,
                               long src2Stride, long dstStride);

extern void (*vu9_to_vu12)(const uint8_t *src1, const uint8_t *src2,
                           uint8_t *dst1, uint8_t *dst2,
                           long width, long height,
                           long srcStride1, long srcStride2,
                           long dstStride1, long dstStride2);

extern void (*yvu9_to_yuy2)(const uint8_t *src1, const uint8_t *src2, const uint8_t *src3,
                            uint8_t *dst,
                            long width, long height,
                            long srcStride1, long srcStride2,
                            long srcStride3, long dstStride);


extern void (*uyvytoyuv420)(uint8_t *ydst, uint8_t *udst, uint8_t *vdst, const uint8_t *src,
                            long width, long height,
                            long lumStride, long chromStride, long srcStride);
extern void (*uyvytoyuv422)(uint8_t *ydst, uint8_t *udst, uint8_t *vdst, const uint8_t *src,
                            long width, long height,
                            long lumStride, long chromStride, long srcStride);
extern void (*yuyvtoyuv420)(uint8_t *ydst, uint8_t *udst, uint8_t *vdst, const uint8_t *src,
                            long width, long height,
                            long lumStride, long chromStride, long srcStride);
extern void (*yuyvtoyuv422)(uint8_t *ydst, uint8_t *udst, uint8_t *vdst, const uint8_t *src,
                            long width, long height,
                            long lumStride, long chromStride, long srcStride);

void sws_rgb2rgb_init(int flags);

#endif 
