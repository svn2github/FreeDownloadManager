/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef AVCODEC_TIFF_H
#define AVCODEC_TIFF_H

#include <stdint.h>


enum TiffTags{
    TIFF_SUBFILE = 0xfe,
    TIFF_WIDTH = 0x100,
    TIFF_HEIGHT,
    TIFF_BPP,
    TIFF_COMPR,
    TIFF_INVERT = 0x106,
    TIFF_FILL_ORDER = 0x10A,
    TIFF_STRIP_OFFS = 0x111,
    TIFF_SAMPLES_PER_PIXEL = 0x115,
    TIFF_ROWSPERSTRIP = 0x116,
    TIFF_STRIP_SIZE,
    TIFF_XRES = 0x11A,
    TIFF_YRES = 0x11B,
    TIFF_PLANAR = 0x11C,
    TIFF_XPOS = 0x11E,
    TIFF_YPOS = 0x11F,
    TIFF_T4OPTIONS = 0x124,
    TIFF_T6OPTIONS,
    TIFF_RES_UNIT = 0x128,
    TIFF_SOFTWARE_NAME = 0x131,
    TIFF_PREDICTOR = 0x13D,
    TIFF_PAL = 0x140,
    TIFF_YCBCR_COEFFICIENTS = 0x211,
    TIFF_YCBCR_SUBSAMPLING = 0x212,
    TIFF_YCBCR_POSITIONING = 0x213,
    TIFF_REFERENCE_BW = 0x214,
};


enum TiffCompr{
    TIFF_RAW = 1,
    TIFF_CCITT_RLE,
    TIFF_G3,
    TIFF_G4,
    TIFF_LZW,
    TIFF_JPEG,
    TIFF_NEWJPEG,
    TIFF_ADOBE_DEFLATE,
    TIFF_PACKBITS = 0x8005,
    TIFF_DEFLATE = 0x80B2
};

enum TiffTypes{
    TIFF_BYTE = 1,
    TIFF_STRING,
    TIFF_SHORT,
    TIFF_LONG,
    TIFF_RATIONAL,
};


static const uint8_t type_sizes[6] = {
    0, 1, 100, 2, 4, 8
};

#endif 
