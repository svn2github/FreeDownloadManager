/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef POSTPROC_POSTPROCESS_INTERNAL_H
#define POSTPROC_POSTPROCESS_INTERNAL_H

#include <string.h>
#include "libavutil/avutil.h"
#include "postprocess.h"

#define V_DEBLOCK       0x01
#define H_DEBLOCK       0x02
#define DERING          0x04
#define LEVEL_FIX       0x08 

#define LUM_V_DEBLOCK   V_DEBLOCK               
#define LUM_H_DEBLOCK   H_DEBLOCK               
#define CHROM_V_DEBLOCK (V_DEBLOCK<<4)          
#define CHROM_H_DEBLOCK (H_DEBLOCK<<4)          
#define LUM_DERING      DERING                  
#define CHROM_DERING    (DERING<<4)             
#define LUM_LEVEL_FIX   LEVEL_FIX               
#define CHROM_LEVEL_FIX (LEVEL_FIX<<4)          


#define V_X1_FILTER     0x0200                  
#define V_A_DEBLOCK     0x0400


#define H_X1_FILTER     0x2000                  
#define H_A_DEBLOCK     0x4000


#define FULL_Y_RANGE    0x8000                  


#define LINEAR_IPOL_DEINT_FILTER        0x10000 
#define LINEAR_BLEND_DEINT_FILTER       0x20000 
#define CUBIC_BLEND_DEINT_FILTER        0x8000  
#define CUBIC_IPOL_DEINT_FILTER         0x40000 
#define MEDIAN_DEINT_FILTER             0x80000 
#define FFMPEG_DEINT_FILTER             0x400000
#define LOWPASS5_DEINT_FILTER           0x800000

#define TEMP_NOISE_FILTER               0x100000
#define FORCE_QUANT                     0x200000







static inline int CLIP(int a){
    if(a&256) return ((a)>>31)^(-1);
    else      return a;
}

struct PPFilter{
    const char *shortName;
    const char *longName;
    int chromDefault;       
    int minLumQuality;      
    int minChromQuality;    
    int mask;               
};


typedef struct PPMode{
    int lumMode;                    
    int chromMode;                  
    int error;                      

    int minAllowedY;                
    int maxAllowedY;                
    float maxClippedThreshold;      

    int maxTmpNoise[3];             

    int baseDcDiff;
    int flatnessThreshold;

    int forcedQuant;                
} PPMode;


typedef struct PPContext{
    
    const AVClass *av_class;

    uint8_t *tempBlocks; 

    
    uint64_t *yHistogram;

    DECLARE_ALIGNED(8, uint64_t, packedYOffset);
    DECLARE_ALIGNED(8, uint64_t, packedYScale);

    
    uint8_t *tempBlurred[3];
    int32_t *tempBlurredPast[3];

    
    uint8_t *tempDst;
    uint8_t *tempSrc;

    uint8_t *deintTemp;

    DECLARE_ALIGNED(8, uint64_t, pQPb);
    DECLARE_ALIGNED(8, uint64_t, pQPb2);

    DECLARE_ALIGNED(8, uint64_t, mmxDcOffset)[64];
    DECLARE_ALIGNED(8, uint64_t, mmxDcThreshold)[64];

    QP_STORE_T *stdQPTable;       
    QP_STORE_T *nonBQPTable;
    QP_STORE_T *forcedQPTable;

    int QP;
    int nonBQP;

    int frameNum;

    int cpuCaps;

    int qpStride; 
    int stride;   

    int hChromaSubSample;
    int vChromaSubSample;

    PPMode ppMode;
} PPContext;


static inline void linecpy(void *dest, const void *src, int lines, int stride) {
    if (stride > 0) {
        memcpy(dest, src, lines*stride);
    } else {
        memcpy((uint8_t*)dest+(lines-1)*stride, (const uint8_t*)src+(lines-1)*stride, -lines*stride);
    }
}

#endif 
