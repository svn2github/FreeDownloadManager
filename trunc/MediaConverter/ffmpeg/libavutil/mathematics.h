/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVUTIL_MATHEMATICS_H
#define AVUTIL_MATHEMATICS_H

#include <stdint.h>
#include <math.h>
#include "attributes.h"
#include "rational.h"

#ifndef M_E
#define M_E            2.7182818284590452354   
#endif
#ifndef M_LN2
#define M_LN2          0.69314718055994530942  
#endif
#ifndef M_LN10
#define M_LN10         2.30258509299404568402  
#endif
#ifndef M_LOG2_10
#define M_LOG2_10      3.32192809488736234787  
#endif
#ifndef M_PI
#define M_PI           3.14159265358979323846  
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2      0.70710678118654752440  
#endif
#ifndef M_SQRT2
#define M_SQRT2        1.41421356237309504880  
#endif
#ifndef NAN
#define NAN            (0.0/0.0)
#endif
#ifndef INFINITY
#define INFINITY       (1.0/0.0)
#endif

enum AVRounding {
    AV_ROUND_ZERO     = 0, 
    AV_ROUND_INF      = 1, 
    AV_ROUND_DOWN     = 2, 
    AV_ROUND_UP       = 3, 
    AV_ROUND_NEAR_INF = 5, 
};


int64_t av_const av_gcd(int64_t a, int64_t b);


int64_t av_rescale(int64_t a, int64_t b, int64_t c) av_const;


int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c, enum AVRounding) av_const;


int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq) av_const;


int av_compare_ts(int64_t ts_a, AVRational tb_a, int64_t ts_b, AVRational tb_b);


#endif 
