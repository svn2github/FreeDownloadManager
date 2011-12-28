/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_AAC_H
#define AVCODEC_AAC_H

#include "avcodec.h"
#include "dsputil.h"
#include "fft.h"
#include "mpeg4audio.h"
#include "sbr.h"

#include <stdint.h>

#define AAC_INIT_VLC_STATIC(num, size) \
    INIT_VLC_STATIC(&vlc_spectral[num], 8, ff_aac_spectral_sizes[num], \
         ff_aac_spectral_bits[num], sizeof( ff_aac_spectral_bits[num][0]), sizeof( ff_aac_spectral_bits[num][0]), \
        ff_aac_spectral_codes[num], sizeof(ff_aac_spectral_codes[num][0]), sizeof(ff_aac_spectral_codes[num][0]), \
        size);

#define MAX_CHANNELS 64
#define MAX_ELEM_ID 16

#define TNS_MAX_ORDER 20

enum RawDataBlockType {
    TYPE_SCE,
    TYPE_CPE,
    TYPE_CCE,
    TYPE_LFE,
    TYPE_DSE,
    TYPE_PCE,
    TYPE_FIL,
    TYPE_END,
};

enum ExtensionPayloadID {
    EXT_FILL,
    EXT_FILL_DATA,
    EXT_DATA_ELEMENT,
    EXT_DYNAMIC_RANGE = 0xb,
    EXT_SBR_DATA      = 0xd,
    EXT_SBR_DATA_CRC  = 0xe,
};

enum WindowSequence {
    ONLY_LONG_SEQUENCE,
    LONG_START_SEQUENCE,
    EIGHT_SHORT_SEQUENCE,
    LONG_STOP_SEQUENCE,
};

enum BandType {
    ZERO_BT        = 0,     
    FIRST_PAIR_BT  = 5,     
    ESC_BT         = 11,    
    NOISE_BT       = 13,    
    INTENSITY_BT2  = 14,    
    INTENSITY_BT   = 15,    
};

#define IS_CODEBOOK_UNSIGNED(x) ((x - 1) & 10)

enum ChannelPosition {
    AAC_CHANNEL_FRONT = 1,
    AAC_CHANNEL_SIDE  = 2,
    AAC_CHANNEL_BACK  = 3,
    AAC_CHANNEL_LFE   = 4,
    AAC_CHANNEL_CC    = 5,
};


enum CouplingPoint {
    BEFORE_TNS,
    BETWEEN_TNS_AND_IMDCT,
    AFTER_IMDCT = 3,
};


enum OCStatus {
    OC_NONE,        
    OC_TRIAL_PCE,   
    OC_TRIAL_FRAME, 
    OC_GLOBAL_HDR,  
    OC_LOCKED,      
};


typedef struct {
    float cor0;
    float cor1;
    float var0;
    float var1;
    float r0;
    float r1;
} PredictorState;

#define MAX_PREDICTORS 672

#define SCALE_DIV_512    36    
#define SCALE_ONE_POS   140    
#define SCALE_MAX_POS   255    
#define SCALE_MAX_DIFF   60    
#define SCALE_DIFF_ZERO  60    


typedef struct {
    uint8_t max_sfb;            
    enum WindowSequence window_sequence[2];
    uint8_t use_kb_window[2];   
    int num_window_groups;
    uint8_t group_len[8];
    const uint16_t *swb_offset; 
    const uint8_t *swb_sizes;   
    int num_swb;                
    int num_windows;
    int tns_max_bands;
    int predictor_present;
    int predictor_initialized;
    int predictor_reset_group;
    uint8_t prediction_used[41];
} IndividualChannelStream;


typedef struct {
    int present;
    int n_filt[8];
    int length[8][4];
    int direction[8][4];
    int order[8][4];
    float coef[8][4][TNS_MAX_ORDER];
} TemporalNoiseShaping;


typedef struct {
    int pce_instance_tag;                           
    int dyn_rng_sgn[17];                            
    int dyn_rng_ctl[17];                            
    int exclude_mask[MAX_CHANNELS];                 
    int band_incr;                                  
    int interpolation_scheme;                       
    int band_top[17];                               
    int prog_ref_level;                             
} DynamicRangeControl;

typedef struct {
    int num_pulse;
    int start;
    int pos[4];
    int amp[4];
} Pulse;


typedef struct {
    enum CouplingPoint coupling_point;  
    int num_coupled;       
    enum RawDataBlockType type[8];   
    int id_select[8];      
    int ch_select[8];      
    float gain[16][120];
} ChannelCoupling;


typedef struct {
    IndividualChannelStream ics;
    TemporalNoiseShaping tns;
    Pulse pulse;
    enum BandType band_type[128];             
    int band_type_run_end[120];               
    float sf[120];                            
    int sf_idx[128];                          
    uint8_t zeroes[128];                      
    DECLARE_ALIGNED(16, float, coeffs)[1024]; 
    DECLARE_ALIGNED(16, float, saved)[1024];  
    DECLARE_ALIGNED(16, float, ret)[2048];    
    PredictorState predictor_state[MAX_PREDICTORS];
} SingleChannelElement;


typedef struct {
    
    int common_window;        
    int     ms_mode;          
    uint8_t ms_mask[128];     
    
    SingleChannelElement ch[2];
    
    ChannelCoupling coup;
    SpectralBandReplication sbr;
} ChannelElement;


typedef struct {
    AVCodecContext * avccontext;

    MPEG4AudioConfig m4ac;

    int is_saved;                 
    DynamicRangeControl che_drc;

    
    enum ChannelPosition che_pos[4][MAX_ELEM_ID]; 
    ChannelElement * che[4][MAX_ELEM_ID];
    ChannelElement * tag_che_map[4][MAX_ELEM_ID];
    int tags_mapped;
    

    
    DECLARE_ALIGNED(16, float, buf_mdct)[1024];
    

    
    FFTContext mdct;
    FFTContext mdct_small;
    DSPContext dsp;
    int random_state;
    

    
    float *output_data[MAX_CHANNELS];                 
    float add_bias;                                   
    float sf_scale;                                   
    int sf_offset;                                    
    

    DECLARE_ALIGNED(16, float, temp)[128];

    enum OCStatus output_configured;
} AACContext;

#endif 
