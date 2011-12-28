/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_SBR_H
#define AVCODEC_SBR_H

#include <stdint.h>
#include "fft.h"


typedef struct {
    uint8_t bs_start_freq;
    uint8_t bs_stop_freq;
    uint8_t bs_xover_band;

    
    uint8_t bs_freq_scale;
    uint8_t bs_alter_scale;
    uint8_t bs_noise_bands;
    
} SpectrumParameters;

#define SBR_SYNTHESIS_BUF_SIZE ((1280-128)*2)


typedef struct {
    
    unsigned           bs_frame_class;
    unsigned           bs_add_harmonic_flag;
    unsigned           bs_num_env;
    uint8_t            bs_freq_res[7];
    unsigned           bs_num_noise;
    uint8_t            bs_df_env[5];
    uint8_t            bs_df_noise[2];
    uint8_t            bs_invf_mode[2][5];
    uint8_t            bs_add_harmonic[48];
    unsigned           bs_amp_res;
    

    
    DECLARE_ALIGNED(16, float, synthesis_filterbank_samples)[SBR_SYNTHESIS_BUF_SIZE];
    DECLARE_ALIGNED(16, float, analysis_filterbank_samples) [1312];
    int                synthesis_filterbank_samples_offset;
    
    int                e_a[2];
    
    float              bw_array[5];
    
    float              W[2][32][32][2];
    
    float              Y[2][38][64][2];
    float              g_temp[42][48];
    float              q_temp[42][48];
    uint8_t            s_indexmapped[8][48];
    
    float              env_facs[6][48];
    
    float              noise_facs[3][5];
    
    uint8_t            t_env[8];
    
    uint8_t            t_env_num_env_old;
    
    uint8_t            t_q[3];
    unsigned           f_indexnoise;
    unsigned           f_indexsine;
    
} SBRData;


typedef struct {
    int                sample_rate;
    int                start;
    int                reset;
    SpectrumParameters spectrum_params;
    int                bs_amp_res_header;
    
    unsigned           bs_limiter_bands;
    unsigned           bs_limiter_gains;
    unsigned           bs_interpol_freq;
    unsigned           bs_smoothing_mode;
    
    unsigned           bs_coupling;
    unsigned           k[5]; 
    
    
    unsigned           kx[2];
    
    unsigned           m[2];
    
    unsigned           n_master;
    SBRData            data[2];
    
    unsigned           n[2];
    
    unsigned           n_q;
    
    unsigned           n_lim;
    
    uint16_t           f_master[49];
    
    uint16_t           f_tablelow[25];
    
    uint16_t           f_tablehigh[49];
    
    uint16_t           f_tablenoise[6];
    
    uint16_t           f_tablelim[29];
    unsigned           num_patches;
    uint8_t            patch_num_subbands[6];
    uint8_t            patch_start_subband[6];
    
    float              X_low[32][40][2];
    
    float              X_high[64][40][2];
    
    DECLARE_ALIGNED(16, float, X)[2][32][64];
    
    float              alpha0[64][2];
    
    float              alpha1[64][2];
    
    float              e_origmapped[7][48];
    
    float              q_mapped[7][48];
    
    uint8_t            s_mapped[7][48];
    
    float              e_curr[7][48];
    
    float              q_m[7][48];
    
    float              s_m[7][48];
    float              gain[7][48];
    DECLARE_ALIGNED(16, float, qmf_filter_scratch)[5][64];
    RDFTContext        rdft;
    FFTContext         mdct;
} SpectralBandReplication;

#endif 
