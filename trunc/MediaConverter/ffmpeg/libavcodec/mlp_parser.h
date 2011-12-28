/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MLP_PARSER_H
#define AVCODEC_MLP_PARSER_H

#include "get_bits.h"

typedef struct MLPHeaderInfo
{
    int stream_type;            

    int group1_bits;            
    int group2_bits;            

    int group1_samplerate;      
    int group2_samplerate;      

    int channels_mlp;           
    int channels_thd_stream1;   
    int channels_thd_stream2;   

    int access_unit_size;       
    int access_unit_size_pow2;  

    int is_vbr;                 
    int peak_bitrate;           

    int num_substreams;         
} MLPHeaderInfo;


int ff_mlp_read_major_sync(void *log, MLPHeaderInfo *mh, GetBitContext *gb);

#endif 

