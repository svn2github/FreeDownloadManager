/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef CBRT_TABLEGEN_H
#define CBRT_TABLEGEN_H

#include <stdint.h>
#include <math.h>

#if CONFIG_HARDCODED_TABLES
#define cbrt_tableinit()
#include "libavcodec/cbrt_tables.h"
#else
static uint32_t cbrt_tab[1 << 13];

static void cbrt_tableinit(void)
{
    if (!cbrt_tab[(1<<13) - 1]) {
        int i;
        for (i = 0; i < 1<<13; i++) {
            union {
                float f;
                uint32_t i;
            } f;
            f.f = cbrtf(i) * i;
            cbrt_tab[i] = f.i;
        }
    }
}
#endif 

#endif 
