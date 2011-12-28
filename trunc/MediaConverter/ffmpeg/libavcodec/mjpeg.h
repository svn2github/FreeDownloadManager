/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MJPEG_H
#define AVCODEC_MJPEG_H

#include "avcodec.h"
#include "put_bits.h"



typedef enum {
    
    SOF0  = 0xc0,       
    SOF1  = 0xc1,       
    SOF2  = 0xc2,       
    SOF3  = 0xc3,       

    SOF5  = 0xc5,       
    SOF6  = 0xc6,       
    SOF7  = 0xc7,       
    JPG   = 0xc8,       
    SOF9  = 0xc9,       
    SOF10 = 0xca,       
    SOF11 = 0xcb,       

    SOF13 = 0xcd,       
    SOF14 = 0xce,       
    SOF15 = 0xcf,       

    DHT   = 0xc4,       

    DAC   = 0xcc,       

    
    RST0  = 0xd0,
    RST1  = 0xd1,
    RST2  = 0xd2,
    RST3  = 0xd3,
    RST4  = 0xd4,
    RST5  = 0xd5,
    RST6  = 0xd6,
    RST7  = 0xd7,

    SOI   = 0xd8,       
    EOI   = 0xd9,       
    SOS   = 0xda,       
    DQT   = 0xdb,       
    DNL   = 0xdc,       
    DRI   = 0xdd,       
    DHP   = 0xde,       
    EXP   = 0xdf,       

    APP0  = 0xe0,
    APP1  = 0xe1,
    APP2  = 0xe2,
    APP3  = 0xe3,
    APP4  = 0xe4,
    APP5  = 0xe5,
    APP6  = 0xe6,
    APP7  = 0xe7,
    APP8  = 0xe8,
    APP9  = 0xe9,
    APP10 = 0xea,
    APP11 = 0xeb,
    APP12 = 0xec,
    APP13 = 0xed,
    APP14 = 0xee,
    APP15 = 0xef,

    JPG0  = 0xf0,
    JPG1  = 0xf1,
    JPG2  = 0xf2,
    JPG3  = 0xf3,
    JPG4  = 0xf4,
    JPG5  = 0xf5,
    JPG6  = 0xf6,
    SOF48 = 0xf7,       
    LSE   = 0xf8,       
    JPG9  = 0xf9,
    JPG10 = 0xfa,
    JPG11 = 0xfb,
    JPG12 = 0xfc,
    JPG13 = 0xfd,

    COM   = 0xfe,       

    TEM   = 0x01,       

    
} JPEG_MARKER;

static inline void put_marker(PutBitContext *p, int code)
{
    put_bits(p, 8, 0xff);
    put_bits(p, 8, code);
}

#define PREDICT(ret, topleft, top, left, predictor)\
    switch(predictor){\
        case 1: ret= left; break;\
        case 2: ret= top; break;\
        case 3: ret= topleft; break;\
        case 4: ret= left   +   top - topleft; break;\
        case 5: ret= left   + ((top - topleft)>>1); break;\
        case 6: ret= top + ((left   - topleft)>>1); break;\
        default:\
        case 7: ret= (left + top)>>1; break;\
    }

extern const uint8_t ff_mjpeg_bits_dc_luminance[];
extern const uint8_t ff_mjpeg_val_dc[];

extern const uint8_t ff_mjpeg_bits_dc_chrominance[];

extern const uint8_t ff_mjpeg_bits_ac_luminance[];
extern const uint8_t ff_mjpeg_val_ac_luminance[];

extern const uint8_t ff_mjpeg_bits_ac_chrominance[];
extern const uint8_t ff_mjpeg_val_ac_chrominance[];

void ff_mjpeg_build_huffman_codes(uint8_t *huff_size, uint16_t *huff_code,
                                  const uint8_t *bits_table,
                                  const uint8_t *val_table);

#endif 
