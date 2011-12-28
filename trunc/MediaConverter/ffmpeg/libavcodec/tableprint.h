/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_TABLEPRINT_H
#define AVCODEC_TABLEPRINT_H

#include <stdint.h>
#include <stdio.h>

#define WRITE_1D_FUNC_ARGV(name, type, linebrk, fmtstr, ...)\
void write_##name##_array(const type *data, int len)\
{\
    int i;\
    printf("   ");\
    for (i = 0; i < len - 1; i++) {\
       printf(" "fmtstr",", __VA_ARGS__);\
       if ((i & linebrk) == linebrk) printf("\n   ");\
    }\
    printf(" "fmtstr"\n", __VA_ARGS__);\
}

#define WRITE_1D_FUNC(name, type, fmtstr, linebrk)\
    WRITE_1D_FUNC_ARGV(name, type, linebrk, fmtstr, data[i])

#define WRITE_2D_FUNC(name, type)\
void write_##name##_2d_array(const void *arg, int len, int len2)\
{\
    const type *data = arg;\
    int i;\
    printf("    {\n");\
    for (i = 0; i < len; i++) {\
        write_##name##_array(data + i * len2, len2);\
        printf(i == len - 1 ? "    }\n" : "    }, {\n");\
    }\
}


void write_int8_array     (const int8_t   *, int);
void write_uint8_array    (const uint8_t  *, int);
void write_uint16_array   (const uint16_t *, int);
void write_uint32_array   (const uint32_t *, int);
void write_float_array    (const float    *, int);
void write_int8_2d_array  (const void *, int, int);
void write_uint8_2d_array (const void *, int, int);
void write_uint32_2d_array(const void *, int, int);
 


void write_fileheader(void);

#endif 
