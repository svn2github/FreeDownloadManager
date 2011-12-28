/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_PSYMODEL_H
#define AVCODEC_PSYMODEL_H

#include "avcodec.h"


#define PSY_MAX_BANDS 128


typedef struct FFPsyBand {
    int   bits;
    float energy;
    float threshold;
    float distortion;
    float perceptual_weight;
} FFPsyBand;


typedef struct FFPsyWindowInfo {
    int window_type[3];               
    int window_shape;                 
    int num_windows;                  
    int grouping[8];                  
    int *window_sizes;                
} FFPsyWindowInfo;


typedef struct FFPsyContext {
    AVCodecContext *avctx;            
    const struct FFPsyModel *model;   

    FFPsyBand *psy_bands;             

    uint8_t **bands;                  
    int     *num_bands;               
    int num_lens;                     

    void* model_priv_data;            
} FFPsyContext;


typedef struct FFPsyModel {
    const char *name;
    int  (*init)   (FFPsyContext *apc);
    FFPsyWindowInfo (*window)(FFPsyContext *ctx, const int16_t *audio, const int16_t *la, int channel, int prev_type);
    void (*analyze)(FFPsyContext *ctx, int channel, const float *coeffs, FFPsyWindowInfo *wi);
    void (*end)    (FFPsyContext *apc);
} FFPsyModel;


av_cold int ff_psy_init(FFPsyContext *ctx, AVCodecContext *avctx,
                        int num_lens,
                        const uint8_t **bands, const int* num_bands);


FFPsyWindowInfo ff_psy_suggest_window(FFPsyContext *ctx,
                                      const int16_t *audio, const int16_t *la,
                                      int channel, int prev_type);



void ff_psy_set_band_info(FFPsyContext *ctx, int channel, const float *coeffs,
                          FFPsyWindowInfo *wi);


av_cold void ff_psy_end(FFPsyContext *ctx);



struct FFPsyPreprocessContext;


av_cold struct FFPsyPreprocessContext* ff_psy_preprocess_init(AVCodecContext *avctx);


void ff_psy_preprocess(struct FFPsyPreprocessContext *ctx,
                       const int16_t *audio, int16_t *dest,
                       int tag, int channels);


av_cold void ff_psy_preprocess_end(struct FFPsyPreprocessContext *ctx);

#endif 
