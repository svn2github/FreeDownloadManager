/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_IVI_DSP_H
#define AVCODEC_IVI_DSP_H

#include "avcodec.h"
#include "ivi_common.h"


void ff_ivi_recompose53(const IVIPlaneDesc *plane, uint8_t *dst,
                        const int dst_pitch, const int num_bands);


void ff_ivi_inverse_slant_8x8(const int32_t *in, int16_t *out, uint32_t pitch,
                              const uint8_t *flags);


void ff_ivi_inverse_slant_4x4(const int32_t *in, int16_t *out, uint32_t pitch,
                              const uint8_t *flags);


void ff_ivi_dc_slant_2d(const int32_t *in, int16_t *out, uint32_t pitch, int blk_size);


void ff_ivi_row_slant8(const int32_t *in, int16_t *out, uint32_t pitch,
                       const uint8_t *flags);


void ff_ivi_col_slant8(const int32_t *in, int16_t *out, uint32_t pitch,
                       const uint8_t *flags);


void ff_ivi_dc_row_slant(const int32_t *in, int16_t *out, uint32_t pitch, int blk_size);


void ff_ivi_dc_col_slant(const int32_t *in, int16_t *out, uint32_t pitch, int blk_size);


void ff_ivi_put_pixels_8x8(const int32_t *in, int16_t *out, uint32_t pitch, const uint8_t *flags);


void ff_ivi_put_dc_pixel_8x8(const int32_t *in, int16_t *out, uint32_t pitch, int blk_size);


void ff_ivi_mc_8x8_delta(int16_t *buf, const int16_t *ref_buf, uint32_t pitch, int mc_type);


void ff_ivi_mc_4x4_delta(int16_t *buf, const int16_t *ref_buf, uint32_t pitch, int mc_type);


void ff_ivi_mc_8x8_no_delta(int16_t *buf, const int16_t *ref_buf, uint32_t pitch, int mc_type);


void ff_ivi_mc_4x4_no_delta(int16_t *buf, const int16_t *ref_buf, uint32_t pitch, int mc_type);

#endif 
