/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef MPEGAUDIO_TABLEGEN_H
#define MPEGAUDIO_TABLEGEN_H

#include <stdint.h>


#include <math.h>

#define TABLE_4_3_SIZE (8191 + 16)*4
#if CONFIG_HARDCODED_TABLES
#define mpegaudio_tableinit()
#include "libavcodec/mpegaudio_tables.h"
#else
static int8_t   table_4_3_exp[TABLE_4_3_SIZE];
static uint32_t table_4_3_value[TABLE_4_3_SIZE];
static uint32_t exp_table[512];
static uint32_t expval_table[512][16];

static void mpegaudio_tableinit(void)
{
    int i, value, exponent;
    for (i = 1; i < TABLE_4_3_SIZE; i++) {
        double value = i / 4;
        double f, fm;
        int e, m;
        f  = value * cbrtf(value) * pow(2, (i & 3) * 0.25);
        fm = frexp(f, &e);
        m  = (uint32_t)(fm * (1LL << 31) + 0.5);
        e += FRAC_BITS - 31 + 5 - 100;

        
        table_4_3_value[i] =  m;
        table_4_3_exp[i]   = -e;
    }
    for (exponent = 0; exponent < 512; exponent++) {
        for (value = 0; value < 16; value++) {
            double f = (double)value * cbrtf(value) * pow(2, (exponent - 400) * 0.25 + FRAC_BITS + 5);
            expval_table[exponent][value] = llrint(f);
        }
        exp_table[exponent] = expval_table[exponent][1];
    }
}
#endif 

#endif 
