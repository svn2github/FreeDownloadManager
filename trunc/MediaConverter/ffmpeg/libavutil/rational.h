/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_RATIONAL_H
#define AVUTIL_RATIONAL_H

#include <stdint.h>
#include "attributes.h"


typedef struct AVRational{
    int num; 
    int den; 
} AVRational;


static inline int av_cmp_q(AVRational a, AVRational b){
    const int64_t tmp= a.num * (int64_t)b.den - b.num * (int64_t)a.den;

    if(tmp) return (tmp>>63)|1;
    else    return 0;
}


static inline double av_q2d(AVRational a){
    return a.num / (double) a.den;
}


int av_reduce(int *dst_num, int *dst_den, int64_t num, int64_t den, int64_t max);


AVRational av_mul_q(AVRational b, AVRational c) av_const;


AVRational av_div_q(AVRational b, AVRational c) av_const;


AVRational av_add_q(AVRational b, AVRational c) av_const;


AVRational av_sub_q(AVRational b, AVRational c) av_const;


AVRational av_d2q(double d, int max) av_const;


int av_nearer_q(AVRational q, AVRational q1, AVRational q2);


int av_find_nearest_q_idx(AVRational q, const AVRational* q_list);

#endif 
