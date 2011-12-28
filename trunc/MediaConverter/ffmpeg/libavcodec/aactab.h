/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_AACTAB_H
#define AVCODEC_AACTAB_H

#include "libavutil/mem.h"
#include "aac.h"

#include <stdint.h>




DECLARE_ALIGNED(16, extern float,  ff_aac_kbd_long_1024)[1024];
DECLARE_ALIGNED(16, extern float,  ff_aac_kbd_short_128)[128];



extern const uint8_t ff_aac_num_swb_1024[];
extern const uint8_t ff_aac_num_swb_128 [];


extern const uint8_t ff_aac_pred_sfb_max [];

extern const uint32_t ff_aac_scalefactor_code[121];
extern const uint8_t  ff_aac_scalefactor_bits[121];

extern const uint16_t * const ff_aac_spectral_codes[11];
extern const uint8_t  * const ff_aac_spectral_bits [11];
extern const uint16_t  ff_aac_spectral_sizes[11];

extern const float *ff_aac_codebook_vectors[];
extern const float *ff_aac_codebook_vector_vals[];
extern const uint16_t *ff_aac_codebook_vector_idx[];

extern const uint16_t * const ff_swb_offset_1024[13];
extern const uint16_t * const ff_swb_offset_128 [13];

extern const uint8_t ff_tns_max_bands_1024[13];
extern const uint8_t ff_tns_max_bands_128 [13];

#if CONFIG_HARDCODED_TABLES
extern const float ff_aac_pow2sf_tab[428];
#else
extern       float ff_aac_pow2sf_tab[428];
#endif 

#endif 
