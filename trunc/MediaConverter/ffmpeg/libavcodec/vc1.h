/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VC1_H
#define AVCODEC_VC1_H

#include "avcodec.h"
#include "mpegvideo.h"
#include "intrax8.h"



enum VC1Code{
    VC1_CODE_RES0       = 0x00000100,
    VC1_CODE_ENDOFSEQ   = 0x0000010A,
    VC1_CODE_SLICE,
    VC1_CODE_FIELD,
    VC1_CODE_FRAME,
    VC1_CODE_ENTRYPOINT,
    VC1_CODE_SEQHDR,
};


#define IS_MARKER(x) (((x) & ~0xFF) == VC1_CODE_RES0)



enum Profile {
    PROFILE_SIMPLE,
    PROFILE_MAIN,
    PROFILE_COMPLEX, 
    PROFILE_ADVANCED
};




enum QuantMode {
    QUANT_FRAME_IMPLICIT,    
    QUANT_FRAME_EXPLICIT,    
    QUANT_NON_UNIFORM,       
    QUANT_UNIFORM            
};




enum DQProfile {
    DQPROFILE_FOUR_EDGES,
    DQPROFILE_DOUBLE_EDGES,
    DQPROFILE_SINGLE_EDGE,
    DQPROFILE_ALL_MBS
};




enum DQSingleEdge {
    DQSINGLE_BEDGE_LEFT,
    DQSINGLE_BEDGE_TOP,
    DQSINGLE_BEDGE_RIGHT,
    DQSINGLE_BEDGE_BOTTOM
};




enum DQDoubleEdge {
    DQDOUBLE_BEDGE_TOPLEFT,
    DQDOUBLE_BEDGE_TOPRIGHT,
    DQDOUBLE_BEDGE_BOTTOMRIGHT,
    DQDOUBLE_BEDGE_BOTTOMLEFT
};




enum MVModes {
    MV_PMODE_1MV_HPEL_BILIN,
    MV_PMODE_1MV,
    MV_PMODE_1MV_HPEL,
    MV_PMODE_MIXED_MV,
    MV_PMODE_INTENSITY_COMP
};




enum BMVTypes {
    BMV_TYPE_BACKWARD,
    BMV_TYPE_FORWARD,
    BMV_TYPE_INTERPOLATED
};




enum TransformTypes {
    TT_8X8,
    TT_8X4_BOTTOM,
    TT_8X4_TOP,
    TT_8X4, 
    TT_4X8_RIGHT,
    TT_4X8_LEFT,
    TT_4X8, 
    TT_4X4
};


enum CodingSet {
    CS_HIGH_MOT_INTRA = 0,
    CS_HIGH_MOT_INTER,
    CS_LOW_MOT_INTRA,
    CS_LOW_MOT_INTER,
    CS_MID_RATE_INTRA,
    CS_MID_RATE_INTER,
    CS_HIGH_RATE_INTRA,
    CS_HIGH_RATE_INTER
};



enum COTypes {
    CONDOVER_NONE = 0,
    CONDOVER_ALL,
    CONDOVER_SELECT
};




typedef struct VC1Context{
    MpegEncContext s;
    IntraX8Context x8;

    int bits;

    
    
    int res_sm;           
    int res_x8;           
    int multires;         
    int res_fasttx;       
    int res_transtab;     
    int rangered;         
                          
    int res_rtm_flag;     
    int reserved;         
    

    
    
    int level;            
    int chromaformat;     
    int postprocflag;     
    int broadcast;        
    int interlace;        
    int tfcntrflag;       
    int panscanflag;      
    int refdist_flag;     
    int extended_dmv;     
    int color_prim;       
    int transfer_char;    
    int matrix_coef;      
    int hrd_param_flag;   
                          
    int psf;              
    

    
    
    int profile;          
    int frmrtq_postproc;  
    int bitrtq_postproc;  
    int fastuvmc;         
    int extended_mv;      
    int dquant;           
    int vstransform;      
    int overlap;          
    int quantizer_mode;   
    int finterpflag;      
    

    
    
    uint8_t mv_mode;      
    uint8_t mv_mode2;     
    int k_x;              
    int k_y;              
    int range_x, range_y; 
    uint8_t pq, altpq;    
    const uint8_t* zz_8x4;
    const uint8_t* zz_4x8;
    
    
    uint8_t dquantfrm;
    uint8_t dqprofile;
    uint8_t dqsbedge;
    uint8_t dqbilevel;
    
    
    
    int c_ac_table_index; 
    int y_ac_table_index; 
    
    int ttfrm;            
    uint8_t ttmbf;        
    uint8_t ttblk4x4;     
    int codingset;        
    int codingset2;       
    int pqindex;          
    int a_avail, c_avail;
    uint8_t *mb_type_base, *mb_type[3];


    
    
    uint8_t lumscale;
    uint8_t lumshift;
    
    int16_t bfraction;    
    uint8_t halfpq;       
    uint8_t respic;       
    int buffer_fullness;  
    
    uint8_t mvrange;
    uint8_t pquantizer;           
    VLC *cbpcy_vlc;               
    int tt_index;                 
    uint8_t* mv_type_mb_plane;    
    uint8_t* direct_mb_plane;     
    int mv_type_is_raw;           
    int dmb_is_raw;               
    int skip_is_raw;              
    uint8_t luty[256], lutuv[256]; 
    int use_ic;                   
    int rnd;                      

    
    
    uint8_t rangeredfrm; 
    uint8_t interpfrm;
    

    
    
    uint8_t fcm; 
    uint8_t numpanscanwin;
    uint8_t tfcntr;
    uint8_t rptfrm, tff, rff;
    uint16_t topleftx;
    uint16_t toplefty;
    uint16_t bottomrightx;
    uint16_t bottomrighty;
    uint8_t uvsamp;
    uint8_t postproc;
    int hrd_num_leaky_buckets;
    uint8_t bit_rate_exponent;
    uint8_t buffer_size_exponent;
    uint8_t* acpred_plane;       
    int acpred_is_raw;
    uint8_t* over_flags_plane;   
    int overflg_is_raw;
    uint8_t condover;
    uint16_t *hrd_rate, *hrd_buffer;
    uint8_t *hrd_fullness;
    uint8_t range_mapy_flag;
    uint8_t range_mapuv_flag;
    uint8_t range_mapy;
    uint8_t range_mapuv;
    

    int p_frame_skipped;
    int bi_type;
    int x8_type;

    uint32_t *cbp_base, *cbp;
    uint8_t bfraction_lut_index;
    uint8_t broken_link;        
    uint8_t closed_entry;       

    int parse_only;             

    int warn_interlaced;
} VC1Context;


static av_always_inline const uint8_t* find_next_marker(const uint8_t *src, const uint8_t *end)
{
    uint32_t mrk = 0xFFFFFFFF;

    if(end-src < 4) return end;
    while(src < end){
        mrk = (mrk << 8) | *src++;
        if(IS_MARKER(mrk))
            return src-4;
    }
    return end;
}

static av_always_inline int vc1_unescape_buffer(const uint8_t *src, int size, uint8_t *dst)
{
    int dsize = 0, i;

    if(size < 4){
        for(dsize = 0; dsize < size; dsize++) *dst++ = *src++;
        return size;
    }
    for(i = 0; i < size; i++, src++) {
        if(src[0] == 3 && i >= 2 && !src[-1] && !src[-2] && i < size-1 && src[1] < 4) {
            dst[dsize++] = src[1];
            src++;
            i++;
        } else
            dst[dsize++] = *src;
    }
    return dsize;
}


int vc1_decode_sequence_header(AVCodecContext *avctx, VC1Context *v, GetBitContext *gb);

int vc1_decode_entry_point(AVCodecContext *avctx, VC1Context *v, GetBitContext *gb);

int vc1_parse_frame_header    (VC1Context *v, GetBitContext *gb);
int vc1_parse_frame_header_adv(VC1Context *v, GetBitContext *gb);

#endif 
