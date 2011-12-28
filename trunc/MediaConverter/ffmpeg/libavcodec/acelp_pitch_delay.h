/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_ACELP_PITCH_DELAY_H
#define AVCODEC_ACELP_PITCH_DELAY_H

#include <stdint.h>
#include "dsputil.h"

#define PITCH_DELAY_MIN             20
#define PITCH_DELAY_MAX             143


int ff_acelp_decode_8bit_to_1st_delay3(int ac_index);


int ff_acelp_decode_5_6_bit_to_2nd_delay3(
        int ac_index,
        int pitch_delay_min);


int ff_acelp_decode_4bit_to_2nd_delay3(
        int ac_index,
        int pitch_delay_min);


int ff_acelp_decode_9bit_to_1st_delay6(int ac_index);


int ff_acelp_decode_6bit_to_2nd_delay6(
        int ac_index,
        int pitch_delay_min);


void ff_acelp_update_past_gain(
        int16_t* quant_energy,
        int gain_corr_factor,
        int log2_ma_pred_order,
        int erasure);


int16_t ff_acelp_decode_gain_code(
    DSPContext *dsp,
    int gain_corr_factor,
    const int16_t* fc_v,
    int mr_energy,
    const int16_t* quant_energy,
    const int16_t* ma_prediction_coeff,
    int subframe_size,
    int max_pred_order);


float ff_amr_set_fixed_gain(float fixed_gain_factor, float fixed_mean_energy,
                            float *prediction_error, float energy_mean,
                            const float *pred_table);



void ff_decode_pitch_lag(int *lag_int, int *lag_frac, int pitch_index,
                         const int prev_lag_int, const int subframe,
                         int third_as_first, int resolution);

#endif 
