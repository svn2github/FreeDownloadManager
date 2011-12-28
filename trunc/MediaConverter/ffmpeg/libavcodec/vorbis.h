/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VORBIS_H
#define AVCODEC_VORBIS_H

#include "avcodec.h"

extern const float ff_vorbis_floor1_inverse_db_table[256];
extern const float * const ff_vorbis_vwin[8];
extern const uint8_t ff_vorbis_channel_layout_offsets[8][8];
extern const int64_t ff_vorbis_channel_layouts[9];

typedef struct {
    uint_fast16_t x;
    uint_fast16_t sort;
    uint_fast16_t low;
    uint_fast16_t high;
} vorbis_floor1_entry;

void ff_vorbis_ready_floor1_list(vorbis_floor1_entry * list, int values);
unsigned int ff_vorbis_nth_root(unsigned int x, unsigned int n); 
int ff_vorbis_len2vlc(uint8_t *bits, uint32_t *codes, uint_fast32_t num);
void ff_vorbis_floor1_render_list(vorbis_floor1_entry * list, int values,
                                  uint_fast16_t * y_list, int * flag,
                                  int multiplier, float * out, int samples);
void vorbis_inverse_coupling(float *mag, float *ang, int blocksize);

#define ilog(i) av_log2(2*(i))

#endif 
