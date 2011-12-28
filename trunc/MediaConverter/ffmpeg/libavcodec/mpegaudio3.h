/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




typedef struct GranuleDef {
    uint8_t scfsi;
    int part2_3_length;
    int big_values;
    int global_gain;
    int scalefac_compress;
    uint8_t block_type;
    uint8_t switch_point;
    int table_select[3];
    int subblock_gain[3];
    uint8_t scalefac_scale;
    uint8_t count1table_select;
    int region_size[3]; 
    int preflag;
    int short_start, long_end; 
    uint8_t scale_factors[40];
    int32_t sb_hybrid[SBLIMIT * 18]; 
} GranuleDef;

void ff_mp3_init(void);


void ff_init_short_region(MPADecodeContext *s, GranuleDef *g);


void ff_init_long_region(MPADecodeContext *s, GranuleDef *g, int ra1, int ra2);

void ff_compute_band_indexes(MPADecodeContext *s, GranuleDef *g);
