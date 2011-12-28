/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_H261_H
#define AVCODEC_H261_H

#include "mpegvideo.h"


typedef struct H261Context{
    MpegEncContext s;

    int current_mba;
    int previous_mba;
    int mba_diff;
    int mtype;
    int current_mv_x;
    int current_mv_y;
    int gob_number;
    int gob_start_code_skipped; 
}H261Context;

#define MB_TYPE_H261_FIL 0x800000

#endif 
