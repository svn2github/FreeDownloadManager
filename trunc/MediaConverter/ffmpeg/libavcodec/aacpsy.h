/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AACPSY_H
#define AVCODEC_AACPSY_H

#include "avcodec.h"
#include "aac.h"


enum AACPsyModelType{
    AAC_PSY_TEST,              
    AAC_PSY_3GPP,              

    AAC_NB_PSY_MODELS          
};


typedef struct AACPsyContext {
    AVCodecContext *avctx;            
}AACPsyContext;


void ff_aac_psy_end(AACPsyContext *ctx);

#endif 
