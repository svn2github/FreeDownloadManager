/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "rational.h"

enum AVRounding {
    AV_ROUND_ZERO     = 0, 
    AV_ROUND_INF      = 1, 
    AV_ROUND_DOWN     = 2, 
    AV_ROUND_UP       = 3, 
    AV_ROUND_NEAR_INF = 5, 
};  

int64_t av_rescale(int64_t a, int64_t b, int64_t c);  

int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c, enum AVRounding);  

int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq);

#endif 
