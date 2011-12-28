/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef PCM_TABLEGEN_H
#define PCM_TABLEGEN_H

#include <stdint.h>
#include "../libavutil/attributes.h"



#define         SIGN_BIT        (0x80)      
#define         QUANT_MASK      (0xf)       
#define         NSEGS           (8)         
#define         SEG_SHIFT       (4)         
#define         SEG_MASK        (0x70)      

#define         BIAS            (0x84)      


static av_cold int alaw2linear(unsigned char a_val)
{
        int t;
        int seg;

        a_val ^= 0x55;

        t = a_val & QUANT_MASK;
        seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
        if(seg) t= (t + t + 1 + 32) << (seg + 2);
        else    t= (t + t + 1     ) << 3;

        return (a_val & SIGN_BIT) ? t : -t;
}

static av_cold int ulaw2linear(unsigned char u_val)
{
        int t;

        
        u_val = ~u_val;

        
        t = ((u_val & QUANT_MASK) << 3) + BIAS;
        t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

        return (u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS);
}

#if CONFIG_HARDCODED_TABLES
#define pcm_alaw_tableinit()
#define pcm_ulaw_tableinit()
#include "libavcodec/pcm_tables.h"
#else

static uint8_t linear_to_alaw[16384];
static uint8_t linear_to_ulaw[16384];

static av_cold void build_xlaw_table(uint8_t *linear_to_xlaw,
                             int (*xlaw2linear)(unsigned char),
                             int mask)
{
    int i, j, v, v1, v2;

    j = 0;
    for(i=0;i<128;i++) {
        if (i != 127) {
            v1 = xlaw2linear(i ^ mask);
            v2 = xlaw2linear((i + 1) ^ mask);
            v = (v1 + v2 + 4) >> 3;
        } else {
            v = 8192;
        }
        for(;j<v;j++) {
            linear_to_xlaw[8192 + j] = (i ^ mask);
            if (j > 0)
                linear_to_xlaw[8192 - j] = (i ^ (mask ^ 0x80));
        }
    }
    linear_to_xlaw[0] = linear_to_xlaw[1];
}

static void pcm_alaw_tableinit(void)
{
    build_xlaw_table(linear_to_alaw, alaw2linear, 0xd5);
}

static void pcm_ulaw_tableinit(void)
{
    build_xlaw_table(linear_to_ulaw, ulaw2linear, 0xff);
}
#endif 

#endif 
