/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_BMP_H
#define AVCODEC_BMP_H

#include "avcodec.h"

typedef struct BMPContext {
    AVFrame picture;
} BMPContext;

typedef enum {
    BMP_RGB         =0,
    BMP_RLE8        =1,
    BMP_RLE4        =2,
    BMP_BITFIELDS   =3,
} BiCompression;

#endif 
