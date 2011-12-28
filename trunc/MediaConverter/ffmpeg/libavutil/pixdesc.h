/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_PIXDESC_H
#define AVUTIL_PIXDESC_H

#include <inttypes.h>

typedef struct AVComponentDescriptor{
    uint16_t plane        :2;            

    
    uint16_t step_minus1  :3;

    
    uint16_t offset_plus1 :3;
    uint16_t shift        :3;            
    uint16_t depth_minus1 :4;            
}AVComponentDescriptor;


typedef struct AVPixFmtDescriptor{
    const char *name;
    uint8_t nb_components;      

    
    uint8_t log2_chroma_w;      

    
    uint8_t log2_chroma_h;
    uint8_t flags;

    
    AVComponentDescriptor comp[4];
}AVPixFmtDescriptor;

#define PIX_FMT_BE        1 
#define PIX_FMT_PAL       2 
#define PIX_FMT_BITSTREAM 4 
#define PIX_FMT_HWACCEL   8 


extern const AVPixFmtDescriptor av_pix_fmt_descriptors[];


void read_line(uint16_t *dst, const uint8_t *data[4], const int linesize[4],
               const AVPixFmtDescriptor *desc, int x, int y, int c, int w, int read_pal_component);


void write_line(const uint16_t *src, uint8_t *data[4], const int linesize[4],
                const AVPixFmtDescriptor *desc, int x, int y, int c, int w);


enum PixelFormat av_get_pix_fmt(const char *name);


int av_get_bits_per_pixel(const AVPixFmtDescriptor *pixdesc);

#endif 
