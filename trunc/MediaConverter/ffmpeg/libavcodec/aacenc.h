/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AACENC_H
#define AVCODEC_AACENC_H

#include "avcodec.h"
#include "put_bits.h"
#include "dsputil.h"

#include "aac.h"

#include "psymodel.h"

struct AACEncContext;

typedef struct AACCoefficientsEncoder {
    void (*search_for_quantizers)(AVCodecContext *avctx, struct AACEncContext *s,
                                  SingleChannelElement *sce, const float lambda);
    void (*encode_window_bands_info)(struct AACEncContext *s, SingleChannelElement *sce,
                                     int win, int group_len, const float lambda);
    void (*quantize_and_encode_band)(struct AACEncContext *s, PutBitContext *pb, const float *in, int size,
                                     int scale_idx, int cb, const float lambda);
    void (*search_for_ms)(struct AACEncContext *s, ChannelElement *cpe, const float lambda);
} AACCoefficientsEncoder;

extern AACCoefficientsEncoder ff_aac_coders[];


typedef struct AACEncContext {
    PutBitContext pb;
    FFTContext mdct1024;                         
    FFTContext mdct128;                          
    DSPContext  dsp;
    DECLARE_ALIGNED(16, FFTSample, output)[2048]; 
    int16_t* samples;                            

    int samplerate_index;                        

    ChannelElement *cpe;                         
    FFPsyContext psy;
    struct FFPsyPreprocessContext* psypp;
    AACCoefficientsEncoder *coder;
    int cur_channel;
    int last_frame;
    float lambda;
    DECLARE_ALIGNED(16, int,   qcoefs)[96][2];   
    DECLARE_ALIGNED(16, float, scoefs)[1024];    
} AACEncContext;

#endif 
