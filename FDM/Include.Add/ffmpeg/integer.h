/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/        

#ifndef INTEGER_H
#define INTEGER_H

#define AV_INTEGER_SIZE 8

typedef struct AVInteger{
    uint16_t v[AV_INTEGER_SIZE];
} AVInteger;

AVInteger av_add_i(AVInteger a, AVInteger b);
AVInteger av_sub_i(AVInteger a, AVInteger b);
int av_log2_i(AVInteger a);
AVInteger av_mul_i(AVInteger a, AVInteger b);
int av_cmp_i(AVInteger a, AVInteger b);
AVInteger av_shr_i(AVInteger a, int s);
AVInteger av_mod_i(AVInteger *quot, AVInteger a, AVInteger b);
AVInteger av_div_i(AVInteger a, AVInteger b);
AVInteger av_int2i(int64_t a);
int64_t av_i2int(AVInteger a);

#endif 
