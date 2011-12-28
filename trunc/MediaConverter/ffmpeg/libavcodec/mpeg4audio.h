/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MPEG4AUDIO_H
#define AVCODEC_MPEG4AUDIO_H

#include <stdint.h>
#include "get_bits.h"
#include "put_bits.h"

typedef struct {
    int object_type;
    int sampling_index;
    int sample_rate;
    int chan_config;
    int sbr; 
    int ext_object_type;
    int ext_sampling_index;
    int ext_sample_rate;
    int ext_chan_config;
    int channels;
    int ps;  
} MPEG4AudioConfig;

extern const int ff_mpeg4audio_sample_rates[16];
extern const uint8_t ff_mpeg4audio_channels[8];

int ff_mpeg4audio_get_config(MPEG4AudioConfig *c, const uint8_t *buf, int buf_size);

enum AudioObjectType {
    AOT_NULL,
                               
    AOT_AAC_MAIN,              
    AOT_AAC_LC,                
    AOT_AAC_SSR,               
    AOT_AAC_LTP,               
    AOT_SBR,                   
    AOT_AAC_SCALABLE,          
    AOT_TWINVQ,                
    AOT_CELP,                  
    AOT_HVXC,                  
    AOT_TTSI             = 12, 
    AOT_MAINSYNTH,             
    AOT_WAVESYNTH,             
    AOT_MIDI,                  
    AOT_SAFX,                  
    AOT_ER_AAC_LC,             
    AOT_ER_AAC_LTP       = 19, 
    AOT_ER_AAC_SCALABLE,       
    AOT_ER_TWINVQ,             
    AOT_ER_BSAC,               
    AOT_ER_AAC_LD,             
    AOT_ER_CELP,               
    AOT_ER_HVXC,               
    AOT_ER_HILN,               
    AOT_ER_PARAM,              
    AOT_SSC,                   
    AOT_PS,                    
    AOT_SURROUND,              
    AOT_ESCAPE,                
    AOT_L1,                    
    AOT_L2,                    
    AOT_L3,                    
    AOT_DST,                   
    AOT_ALS,                   
    AOT_SLS,                   
    AOT_SLS_NON_CORE,          
    AOT_ER_AAC_ELD,            
    AOT_SMR_SIMPLE,            
    AOT_SMR_MAIN,              
    AOT_USAC_NOSBR,            
    AOT_SAOC,                  
    AOT_LD_SURROUND,           
    AOT_USAC,                  
};

#define MAX_PCE_SIZE 304 
                         

int ff_copy_pce_data(PutBitContext *pb, GetBitContext *gb);

#endif 
