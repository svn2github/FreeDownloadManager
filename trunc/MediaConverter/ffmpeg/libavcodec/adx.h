/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_ADX_H
#define AVCODEC_ADX_H

typedef struct {
    int s1,s2;
} PREV;

typedef struct {
    PREV prev[2];
    int header_parsed;
    unsigned char dec_temp[18*2];
    int in_temp;
} ADXContext;

#define    BASEVOL   0x4000
#define    SCALE1    0x7298
#define    SCALE2    0x3350

#endif 
