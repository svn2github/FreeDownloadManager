/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef RATIONAL_H
#define RATIONAL_H

typedef struct AVRational{
    int num; 
    int den; 
} AVRational;

static inline int av_cmp_q(AVRational a, AVRational b){
    const int64_t tmp= a.num * (int64_t)b.den - b.num * (int64_t)a.den;

    if(tmp) return (int)((tmp>>63)|1);
    else    return 0;
}

static inline double av_q2d(AVRational a){
    return a.num / (double) a.den;
}

int av_reduce(int *dst_nom, int *dst_den, int64_t nom, int64_t den, int64_t max);

AVRational av_mul_q(AVRational b, AVRational c);
AVRational av_div_q(AVRational b, AVRational c);
AVRational av_add_q(AVRational b, AVRational c);
AVRational av_sub_q(AVRational b, AVRational c);
AVRational av_d2q(double d, int max);

#endif 
