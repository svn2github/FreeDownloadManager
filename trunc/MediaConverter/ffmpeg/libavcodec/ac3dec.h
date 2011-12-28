/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_AC3DEC_H
#define AVCODEC_AC3DEC_H

#include "libavutil/lfg.h"
#include "ac3.h"
#include "get_bits.h"
#include "dsputil.h"
#include "fft.h"


#undef AC3_MAX_CHANNELS
#define AC3_MAX_CHANNELS 7
#define CPL_CH 0

#define AC3_OUTPUT_LFEON  8

#define AC3_MAX_COEFS   256
#define AC3_BLOCK_SIZE  256
#define MAX_BLOCKS        6
#define SPX_MAX_BANDS    17

typedef struct {
    AVCodecContext *avctx;                  
    GetBitContext gbc;                      
    uint8_t *input_buffer;                  



    int frame_type;                         
    int substreamid;                        
    int frame_size;                         
    int bit_rate;                           
    int sample_rate;                        
    int num_blocks;                         
    int channel_mode;                       
    int channel_layout;                     
    int lfe_on;                             
    int channel_map;                        
    int center_mix_level;                   
    int surround_mix_level;                 
    int eac3;                               



    int snr_offset_strategy;                
    int block_switch_syntax;                
    int dither_flag_syntax;                 
    int bit_allocation_syntax;              
    int fast_gain_syntax;                   
    int dba_syntax;                         
    int skip_syntax;                        
 


    int cpl_in_use[MAX_BLOCKS];             
    int cpl_strategy_exists[MAX_BLOCKS];    
    int channel_in_cpl[AC3_MAX_CHANNELS];   
    int phase_flags_in_use;                 
    int phase_flags[18];                    
    int num_cpl_bands;                      
    uint8_t cpl_band_sizes[18];             
    int firstchincpl;                       
    int first_cpl_coords[AC3_MAX_CHANNELS]; 
    int cpl_coords[AC3_MAX_CHANNELS][18];   




    int spx_in_use;                             
    uint8_t channel_uses_spx[AC3_MAX_CHANNELS]; 
    int8_t spx_atten_code[AC3_MAX_CHANNELS];    
    int spx_src_start_freq;                     
    int spx_dst_end_freq;                       
    int spx_dst_start_freq;                     
                                                
    int num_spx_bands;                          
    uint8_t spx_band_sizes[SPX_MAX_BANDS];      
    uint8_t first_spx_coords[AC3_MAX_CHANNELS]; 
    float spx_noise_blend[AC3_MAX_CHANNELS][SPX_MAX_BANDS]; 
    float spx_signal_blend[AC3_MAX_CHANNELS][SPX_MAX_BANDS];



    int channel_uses_aht[AC3_MAX_CHANNELS];                         
    int pre_mantissa[AC3_MAX_CHANNELS][AC3_MAX_COEFS][MAX_BLOCKS];  



    int fbw_channels;                           
    int channels;                               
    int lfe_ch;                                 
    float downmix_coeffs[AC3_MAX_CHANNELS][2];  
    int downmixed;                              
    int output_mode;                            
    int out_channels;                           



    float dynamic_range[2];                 



    int start_freq[AC3_MAX_CHANNELS];       
    int end_freq[AC3_MAX_CHANNELS];         



    int num_rematrixing_bands;              
    int rematrixing_flags[4];               



    int num_exp_groups[AC3_MAX_CHANNELS];           
    int8_t dexps[AC3_MAX_CHANNELS][AC3_MAX_COEFS];  
    int exp_strategy[MAX_BLOCKS][AC3_MAX_CHANNELS]; 



    AC3BitAllocParameters bit_alloc_params;         
    int first_cpl_leak;                             
    int snr_offset[AC3_MAX_CHANNELS];               
    int fast_gain[AC3_MAX_CHANNELS];                
    uint8_t bap[AC3_MAX_CHANNELS][AC3_MAX_COEFS];   
    int16_t psd[AC3_MAX_CHANNELS][AC3_MAX_COEFS];   
    int16_t band_psd[AC3_MAX_CHANNELS][50];         
    int16_t mask[AC3_MAX_CHANNELS][50];             
    int dba_mode[AC3_MAX_CHANNELS];                 
    int dba_nsegs[AC3_MAX_CHANNELS];                
    uint8_t dba_offsets[AC3_MAX_CHANNELS][8];       
    uint8_t dba_lengths[AC3_MAX_CHANNELS][8];       
    uint8_t dba_values[AC3_MAX_CHANNELS][8];        



    int dither_flag[AC3_MAX_CHANNELS];      
    AVLFG dith_state;                       



    int block_switch[AC3_MAX_CHANNELS];     
    FFTContext imdct_512;                   
    FFTContext imdct_256;                   



    DSPContext dsp;                         
    float add_bias;                         
    float mul_bias;                         



    DECLARE_ALIGNED(16, int,   fixed_coeffs)[AC3_MAX_CHANNELS][AC3_MAX_COEFS];       
    DECLARE_ALIGNED(16, float, transform_coeffs)[AC3_MAX_CHANNELS][AC3_MAX_COEFS];   
    DECLARE_ALIGNED(16, float, delay)[AC3_MAX_CHANNELS][AC3_BLOCK_SIZE];             
    DECLARE_ALIGNED(16, float, window)[AC3_BLOCK_SIZE];                              
    DECLARE_ALIGNED(16, float, tmp_output)[AC3_BLOCK_SIZE];                          
    DECLARE_ALIGNED(16, float, output)[AC3_MAX_CHANNELS][AC3_BLOCK_SIZE];            

} AC3DecodeContext;


int ff_eac3_parse_header(AC3DecodeContext *s);


void ff_eac3_decode_transform_coeffs_aht_ch(AC3DecodeContext *s, int ch);

void ff_ac3_downmix_c(float (*samples)[256], float (*matrix)[2],
                      int out_ch, int in_ch, int len);


void ff_eac3_apply_spectral_extension(AC3DecodeContext *s);

#endif 
