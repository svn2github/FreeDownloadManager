/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#include <assert.h>


#include <math.h>
#include "../libavutil/attributes.h"

#if !CONFIG_HARDCODED_TABLES
SINETABLE(  32);
SINETABLE(  64);
SINETABLE( 128);
SINETABLE( 256);
SINETABLE( 512);
SINETABLE(1024);
SINETABLE(2048);
SINETABLE(4096);
#else
#include "libavcodec/mdct_tables.h"
#endif

SINETABLE_CONST float * const ff_sine_windows[] = {
    NULL, NULL, NULL, NULL, NULL, 
    ff_sine_32 , ff_sine_64 ,
    ff_sine_128, ff_sine_256, ff_sine_512, ff_sine_1024, ff_sine_2048, ff_sine_4096
};


av_cold void ff_sine_window_init(float *window, int n) {
    int i;
    for(i = 0; i < n; i++)
        window[i] = sinf((i + 0.5) * (M_PI / (2.0 * n)));
}

av_cold void ff_init_ff_sine_windows(int index) {
    assert(index >= 0 && index < FF_ARRAY_ELEMS(ff_sine_windows));
#if !CONFIG_HARDCODED_TABLES
    ff_sine_window_init(ff_sine_windows[index], 1 << index);
#endif
}
