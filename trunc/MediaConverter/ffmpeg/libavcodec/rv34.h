/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_RV34_H
#define AVCODEC_RV34_H

#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"

#include "h264pred.h"

#define MB_TYPE_SEPARATE_DC 0x01000000
#define IS_SEPARATE_DC(a)   ((a) & MB_TYPE_SEPARATE_DC)


enum RV40BlockTypes{
    RV34_MB_TYPE_INTRA,      
    RV34_MB_TYPE_INTRA16x16, 
    RV34_MB_P_16x16,         
    RV34_MB_P_8x8,           
    RV34_MB_B_FORWARD,       
    RV34_MB_B_BACKWARD,      
    RV34_MB_SKIP,            
    RV34_MB_B_DIRECT,        
    RV34_MB_P_16x8,          
    RV34_MB_P_8x16,          
    RV34_MB_B_BIDIR,         
    RV34_MB_P_MIX16x16,      
    RV34_MB_TYPES
};


typedef struct RV34VLC{
    VLC cbppattern[2];     
    VLC cbp[2][4];         
    VLC first_pattern[4];  
    VLC second_pattern[2]; 
    VLC third_pattern[2];  
    VLC coefficient;       
}RV34VLC;


typedef struct SliceInfo{
    int type;              
    int quant;             
    int vlc_set;           
    int start, end;        
    int width;             
    int height;            
    int pts;               
}SliceInfo;


typedef struct RV34DecContext{
    MpegEncContext s;
    int8_t *intra_types_hist;
    int8_t *intra_types;     
    int    intra_types_stride;
    const uint8_t *luma_dc_quant_i;
    const uint8_t *luma_dc_quant_p;

    RV34VLC *cur_vlcs;       
    int bits;                
    H264PredContext h;       
    SliceInfo si;            

    int *mb_type;            
    int block_type;          
    int luma_vlc;            
    int chroma_vlc;          
    int is16;                
    int dmv[4][2];           

    int rv30;                
    int rpr;                 

    int cur_pts, last_pts, next_pts;

    uint16_t *cbp_luma;      
    uint8_t  *cbp_chroma;    
    int      *deblock_coefs; 

    
    DECLARE_ALIGNED(8, uint32_t, avail_cache)[3*4];

    int (*parse_slice_header)(struct RV34DecContext *r, GetBitContext *gb, SliceInfo *si);
    int (*decode_mb_info)(struct RV34DecContext *r);
    int (*decode_intra_types)(struct RV34DecContext *r, GetBitContext *gb, int8_t *dst);
    void (*loop_filter)(struct RV34DecContext *r, int row);
}RV34DecContext;


int ff_rv34_get_start_offset(GetBitContext *gb, int blocks);
int ff_rv34_decode_init(AVCodecContext *avctx);
int ff_rv34_decode_frame(AVCodecContext *avctx, void *data, int *data_size, AVPacket *avpkt);
int ff_rv34_decode_end(AVCodecContext *avctx);

#endif 
