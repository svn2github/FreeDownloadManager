/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_MPEG4VIDEO_H
#define AVCODEC_MPEG4VIDEO_H

#include <stdint.h>
#include "get_bits.h"
#include "mpegvideo.h"
#include "rl.h"


#define RECT_SHAPE       0
#define BIN_SHAPE        1
#define BIN_ONLY_SHAPE   2
#define GRAY_SHAPE       3

#define SIMPLE_VO_TYPE             1
#define CORE_VO_TYPE               3
#define MAIN_VO_TYPE               4
#define NBIT_VO_TYPE               5
#define ARTS_VO_TYPE               10
#define ACE_VO_TYPE                12
#define ADV_SIMPLE_VO_TYPE         17


#define EXTENDED_PAR 15


#define STATIC_SPRITE 1
#define GMC_SPRITE 2

#define MOTION_MARKER 0x1F001
#define DC_MARKER     0x6B001

#define VOS_STARTCODE        0x1B0
#define USER_DATA_STARTCODE  0x1B2
#define GOP_STARTCODE        0x1B3
#define VISUAL_OBJ_STARTCODE 0x1B5
#define VOP_STARTCODE        0x1B6


extern const uint8_t ff_mpeg4_DCtab_lum[13][2];
extern const uint8_t ff_mpeg4_DCtab_chrom[13][2];

extern const uint16_t ff_mpeg4_intra_vlc[103][2];
extern RLTable ff_mpeg4_rl_intra;


extern const uint16_t inter_rvlc[170][2];
extern RLTable rvlc_rl_inter;

extern const uint16_t intra_rvlc[170][2];
extern RLTable rvlc_rl_intra;

extern const uint16_t sprite_trajectory_tab[15][2];
extern const uint8_t mb_type_b_tab[4][2];


extern const int16_t ff_mpeg4_default_intra_matrix[64];
extern const int16_t ff_mpeg4_default_non_intra_matrix[64];

extern const uint8_t ff_mpeg4_y_dc_scale_table[32];
extern const uint8_t ff_mpeg4_c_dc_scale_table[32];
extern const uint16_t ff_mpeg4_resync_prefix[8];

extern const uint8_t mpeg4_dc_threshold[8];

void mpeg4_encode_mb(MpegEncContext *s,
                    DCTELEM block[6][64],
                    int motion_x, int motion_y);
void mpeg4_pred_ac(MpegEncContext * s, DCTELEM *block, int n,
                   int dir);
void ff_set_mpeg4_time(MpegEncContext * s);
void mpeg4_encode_picture_header(MpegEncContext *s, int picture_number);

int ff_mpeg4_decode_picture_header(MpegEncContext * s, GetBitContext *gb);
void ff_mpeg4_encode_video_packet_header(MpegEncContext *s);
void ff_mpeg4_clean_buffers(MpegEncContext *s);
void ff_mpeg4_stuffing(PutBitContext * pbc);
void ff_mpeg4_init_partitions(MpegEncContext *s);
void ff_mpeg4_merge_partitions(MpegEncContext *s);
void ff_clean_mpeg4_qscales(MpegEncContext *s);
int ff_mpeg4_decode_partitions(MpegEncContext *s);
int ff_mpeg4_get_video_packet_prefix_length(MpegEncContext *s);
int mpeg4_decode_video_packet_header(MpegEncContext *s);
void ff_mpeg4_init_direct_mv(MpegEncContext *s);


int ff_mpeg4_set_direct_mv(MpegEncContext *s, int mx, int my);

extern uint8_t ff_mpeg4_static_rl_table_store[3][2][2*MAX_RUN + MAX_LEVEL + 3];


#if 0 
#define IS_3IV1 s->codec_tag == AV_RL32("3IV1")
#else
#define IS_3IV1 0
#endif



static inline int ff_mpeg4_pred_dc(MpegEncContext * s, int n, int level, int *dir_ptr, int encoding)
{
    int a, b, c, wrap, pred, scale, ret;
    int16_t *dc_val;

    
    if (n < 4) {
        scale = s->y_dc_scale;
    } else {
        scale = s->c_dc_scale;
    }
    if(IS_3IV1)
        scale= 8;

    wrap= s->block_wrap[n];
    dc_val = s->dc_val[0] + s->block_index[n];

    
    a = dc_val[ - 1];
    b = dc_val[ - 1 - wrap];
    c = dc_val[ - wrap];

    
    if(s->first_slice_line && n!=3){
        if(n!=2) b=c= 1024;
        if(n!=1 && s->mb_x == s->resync_mb_x) b=a= 1024;
    }
    if(s->mb_x == s->resync_mb_x && s->mb_y == s->resync_mb_y+1){
        if(n==0 || n==4 || n==5)
            b=1024;
    }

    if (abs(a - b) < abs(b - c)) {
        pred = c;
        *dir_ptr = 1; 
    } else {
        pred = a;
        *dir_ptr = 0; 
    }
    
    pred = FASTDIV((pred + (scale >> 1)), scale);

    if(encoding){
        ret = level - pred;
    }else{
        level += pred;
        ret= level;
        if(s->error_recognition>=3){
            if(level<0){
                av_log(s->avctx, AV_LOG_ERROR, "dc<0 at %dx%d\n", s->mb_x, s->mb_y);
                return -1;
            }
            if(level*scale > 2048 + scale){
                av_log(s->avctx, AV_LOG_ERROR, "dc overflow at %dx%d\n", s->mb_x, s->mb_y);
                return -1;
            }
        }
    }
    level *=scale;
    if(level&(~2047)){
        if(level<0)
            level=0;
        else if(!(s->workaround_bugs&FF_BUG_DC_CLIP))
            level=2047;
    }
    dc_val[0]= level;

    return ret;
}
#endif
