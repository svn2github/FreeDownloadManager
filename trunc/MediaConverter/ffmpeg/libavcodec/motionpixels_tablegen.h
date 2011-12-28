/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef MOTIONPIXELS_TABLEGEN_H
#define MOTIONPIXELS_TABLEGEN_H

#include <stdint.h>

typedef struct YuvPixel {
    int8_t y, v, u;
} YuvPixel;

static int mp_yuv_to_rgb(int y, int v, int u, int clip_rgb) {
    static const uint8_t *cm = ff_cropTbl + MAX_NEG_CROP;
    int r, g, b;

    r = (1000 * y + 701 * v) / 1000;
    g = (1000 * y - 357 * v - 172 * u) / 1000;
    b = (1000 * y + 886 * u) / 1000;
    if (clip_rgb)
        return ((cm[r * 8] & 0xF8) << 7) | ((cm[g * 8] & 0xF8) << 2) | (cm[b * 8] >> 3);
    if ((unsigned)r < 32 && (unsigned)g < 32 && (unsigned)b < 32)
        return (r << 10) | (g << 5) | b;
    return 1 << 15;
}

#if CONFIG_HARDCODED_TABLES
#define motionpixels_tableinit()
#include "libavcodec/motionpixels_tables.h"
#else
static YuvPixel mp_rgb_yuv_table[1 << 15];

static void mp_set_zero_yuv(YuvPixel *p)
{
    int i, j;

    for (i = 0; i < 31; ++i) {
        for (j = 31; j > i; --j)
            if (!(p[j].u | p[j].v | p[j].y))
                p[j] = p[j - 1];
        for (j = 0; j < 31 - i; ++j)
            if (!(p[j].u | p[j].v | p[j].y))
                p[j] = p[j + 1];
    }
}

static void mp_build_rgb_yuv_table(YuvPixel *p)
{
    int y, v, u, i;

    for (y = 0; y <= 31; ++y)
        for (v = -31; v <= 31; ++v)
            for (u = -31; u <= 31; ++u) {
                i = mp_yuv_to_rgb(y, v, u, 0);
                if (i < (1 << 15) && !(p[i].u | p[i].v | p[i].y)) {
                    p[i].y = y;
                    p[i].v = v;
                    p[i].u = u;
                }
            }
    for (i = 0; i < 1024; ++i)
        mp_set_zero_yuv(p + i * 32);
}

static void motionpixels_tableinit(void)
{
    if (!mp_rgb_yuv_table[0].u)
        mp_build_rgb_yuv_table(mp_rgb_yuv_table);
}
#endif 

#endif 
