/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MPEGVIDEO_H
#define AVCODEC_MPEGVIDEO_H

#include "dsputil.h"
#include "get_bits.h"
#include "put_bits.h"
#include "ratecontrol.h"
#include "parser.h"
#include "mpeg12data.h"
#include "rl.h"

#define FRAME_SKIPPED 100 

enum OutputFormat {
    FMT_MPEG1,
    FMT_H261,
    FMT_H263,
    FMT_MJPEG,
    FMT_H264,
};

#define MPEG_BUF_SIZE (16 * 1024)

#define QMAT_SHIFT_MMX 16
#define QMAT_SHIFT 22

#define MAX_FCODE 7
#define MAX_MV 2048

#define MAX_THREADS 16

#define MAX_PICTURE_COUNT 32

#define ME_MAP_SIZE 64
#define ME_MAP_SHIFT 3
#define ME_MAP_MV_BITS 11

#define MAX_MB_BYTES (30*16*16*3/8 + 120)

#define INPLACE_OFFSET 16


#define SEQ_END_CODE            0x000001b7
#define SEQ_START_CODE          0x000001b3
#define GOP_START_CODE          0x000001b8
#define PICTURE_START_CODE      0x00000100
#define SLICE_MIN_START_CODE    0x00000101
#define SLICE_MAX_START_CODE    0x000001af
#define EXT_START_CODE          0x000001b5
#define USER_START_CODE         0x000001b2


typedef struct Picture{
    FF_COMMON_FRAME

    
    uint8_t *interpolated[3];
    int16_t (*motion_val_base[2])[2];
    uint32_t *mb_type_base;
#define MB_TYPE_INTRA MB_TYPE_INTRA4x4 
#define IS_INTRA4x4(a)   ((a)&MB_TYPE_INTRA4x4)
#define IS_INTRA16x16(a) ((a)&MB_TYPE_INTRA16x16)
#define IS_PCM(a)        ((a)&MB_TYPE_INTRA_PCM)
#define IS_INTRA(a)      ((a)&7)
#define IS_INTER(a)      ((a)&(MB_TYPE_16x16|MB_TYPE_16x8|MB_TYPE_8x16|MB_TYPE_8x8))
#define IS_SKIP(a)       ((a)&MB_TYPE_SKIP)
#define IS_INTRA_PCM(a)  ((a)&MB_TYPE_INTRA_PCM)
#define IS_INTERLACED(a) ((a)&MB_TYPE_INTERLACED)
#define IS_DIRECT(a)     ((a)&MB_TYPE_DIRECT2)
#define IS_GMC(a)        ((a)&MB_TYPE_GMC)
#define IS_16X16(a)      ((a)&MB_TYPE_16x16)
#define IS_16X8(a)       ((a)&MB_TYPE_16x8)
#define IS_8X16(a)       ((a)&MB_TYPE_8x16)
#define IS_8X8(a)        ((a)&MB_TYPE_8x8)
#define IS_SUB_8X8(a)    ((a)&MB_TYPE_16x16) 
#define IS_SUB_8X4(a)    ((a)&MB_TYPE_16x8)  
#define IS_SUB_4X8(a)    ((a)&MB_TYPE_8x16)  
#define IS_SUB_4X4(a)    ((a)&MB_TYPE_8x8)   
#define IS_ACPRED(a)     ((a)&MB_TYPE_ACPRED)
#define IS_QUANT(a)      ((a)&MB_TYPE_QUANT)
#define IS_DIR(a, part, list) ((a) & (MB_TYPE_P0L0<<((part)+2*(list))))
#define USES_LIST(a, list) ((a) & ((MB_TYPE_P0L0|MB_TYPE_P1L0)<<(2*(list)))) 
#define HAS_CBP(a)        ((a)&MB_TYPE_CBP)

    int field_poc[2];           
    int poc;                    
    int frame_num;              
    int mmco_reset;             
    int pic_id;                 
    int long_ref;               
    int ref_poc[2][2][16];      
    int ref_count[2][2];        
    int mbaff;                  

    int mb_var_sum;             
    int mc_mb_var_sum;          
    uint16_t *mb_var;           
    uint16_t *mc_mb_var;        
    uint8_t *mb_mean;           
    int32_t *mb_cmp_score;      
    int b_frame_score;          
} Picture;

struct MpegEncContext;


typedef struct MotionEstContext{
    AVCodecContext *avctx;
    int skip;                          
    int co_located_mv[4][2];           
    int direct_basis_mv[4][2];
    uint8_t *scratchpad;               
    uint8_t *best_mb;
    uint8_t *temp_mb[2];
    uint8_t *temp;
    int best_bits;
    uint32_t *map;                     
    uint32_t *score_map;               
    int map_generation;
    int pre_penalty_factor;
    int penalty_factor;                
    int sub_penalty_factor;
    int mb_penalty_factor;
    int flags;
    int sub_flags;
    int mb_flags;
    int pre_pass;                      
    int dia_size;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    int pred_x;
    int pred_y;
    uint8_t *src[4][4];
    uint8_t *ref[4][4];
    int stride;
    int uvstride;
    
    int mc_mb_var_sum_temp;
    int mb_var_sum_temp;
    int scene_change_score;

    op_pixels_func (*hpel_put)[4];
    op_pixels_func (*hpel_avg)[4];
    qpel_mc_func (*qpel_put)[16];
    qpel_mc_func (*qpel_avg)[16];
    uint8_t (*mv_penalty)[MAX_MV*2+1];  
    uint8_t *current_mv_penalty;
    int (*sub_motion_search)(struct MpegEncContext * s,
                                  int *mx_ptr, int *my_ptr, int dmin,
                                  int src_index, int ref_index,
                                  int size, int h);
}MotionEstContext;


typedef struct MpegEncContext {
    struct AVCodecContext *avctx;
    
    int width, height;
    int gop_size;
    int intra_only;   
    int bit_rate;     
    enum OutputFormat out_format; 
    int h263_pred;    
    int pb_frame;     


    int h263_plus;    
    int h263_msmpeg4; 
    int h263_flv;     

    enum CodecID codec_id;     
    int fixed_qscale; 
    int encoding;     
    int flags;        
    int flags2;       
    int max_b_frames; 
    int luma_elim_threshold;
    int chroma_elim_threshold;
    int strict_std_compliance; 
    int workaround_bugs;       
    int codec_tag;             
    int stream_codec_tag;      
    

    
    PutBitContext pb;

    
    int context_initialized;
    int input_picture_number;  
    int coded_picture_number;  
    int picture_number;       
    int picture_in_gop_number; 
    int b_frames_since_non_b;  
    int64_t user_specified_pts;
    int mb_width, mb_height;   
    int mb_stride;             
    int b8_stride;             
    int b4_stride;             
    int h_edge_pos, v_edge_pos;
    int mb_num;                
    int linesize;              
    int uvlinesize;            
    Picture *picture;          
    Picture **input_picture;   
    Picture **reordered_input_picture; 

    int start_mb_y;            
    int end_mb_y;              
    struct MpegEncContext *thread_context[MAX_THREADS];

    
    Picture last_picture;

    
    Picture next_picture;

    
    Picture new_picture;

    
    Picture current_picture;    

    Picture *last_picture_ptr;     
    Picture *next_picture_ptr;     
    Picture *current_picture_ptr;  
    uint8_t *visualization_buffer[3]; 
    int last_dc[3];                
    int16_t *dc_val_base;
    int16_t *dc_val[3];            
    int16_t dc_cache[4*5];
    int y_dc_scale, c_dc_scale;
    const uint8_t *y_dc_scale_table;     
    const uint8_t *c_dc_scale_table;     
    const uint8_t *chroma_qscale_table;  
    uint8_t *coded_block_base;
    uint8_t *coded_block;          
    int16_t (*ac_val_base)[16];
    int16_t (*ac_val[3])[16];      
    int ac_pred;
    uint8_t *prev_pict_types;     
#define PREV_PICT_TYPES_BUFFER_SIZE 256
    int mb_skipped;                
    uint8_t *mbskip_table;        
    uint8_t *mbintra_table;       
    uint8_t *cbp_table;           
    uint8_t *pred_dir_table;      
    uint8_t *allocated_edge_emu_buffer;
    uint8_t *edge_emu_buffer;     
    uint8_t *rd_scratchpad;       
    uint8_t *obmc_scratchpad;
    uint8_t *b_scratchpad;        

    int qscale;                 
    int chroma_qscale;          
    unsigned int lambda;        
    unsigned int lambda2;       
    int *lambda_table;
    int adaptive_quant;         
    int dquant;                 
    int closed_gop;             
    int pict_type;              
    int last_pict_type; 
    int last_non_b_pict_type;   
    int dropable;
    int frame_rate_index;
    int last_lambda_for[5];     
    int skipdct;                

    
    int unrestricted_mv;        
    int h263_long_vectors;      
    int decode;                 

    DSPContext dsp;             
    int f_code;                 
    int b_code;                 
    int16_t (*p_mv_table_base)[2];
    int16_t (*b_forw_mv_table_base)[2];
    int16_t (*b_back_mv_table_base)[2];
    int16_t (*b_bidir_forw_mv_table_base)[2];
    int16_t (*b_bidir_back_mv_table_base)[2];
    int16_t (*b_direct_mv_table_base)[2];
    int16_t (*p_field_mv_table_base[2][2])[2];
    int16_t (*b_field_mv_table_base[2][2][2])[2];
    int16_t (*p_mv_table)[2];            
    int16_t (*b_forw_mv_table)[2];       
    int16_t (*b_back_mv_table)[2];       
    int16_t (*b_bidir_forw_mv_table)[2]; 
    int16_t (*b_bidir_back_mv_table)[2]; 
    int16_t (*b_direct_mv_table)[2];     
    int16_t (*p_field_mv_table[2][2])[2];   
    int16_t (*b_field_mv_table[2][2][2])[2];
    uint8_t (*p_field_select_table[2]);
    uint8_t (*b_field_select_table[2][2]);
    int me_method;                       
    int mv_dir;
#define MV_DIR_FORWARD   1
#define MV_DIR_BACKWARD  2
#define MV_DIRECT        4 
    int mv_type;
#define MV_TYPE_16X16       0   
#define MV_TYPE_8X8         1   
#define MV_TYPE_16X8        2   
#define MV_TYPE_FIELD       3   
#define MV_TYPE_DMV         4   
    
    int mv[2][4][2];
    int field_select[2][2];
    int last_mv[2][2][2];             
    uint8_t *fcode_tab;               
    int16_t direct_scale_mv[2][64];   

    MotionEstContext me;

    int no_rounding;  

    int hurry_up;     

    
    int mb_x, mb_y;
    int mb_skip_run;
    int mb_intra;
    uint16_t *mb_type;           
#define CANDIDATE_MB_TYPE_INTRA    0x01
#define CANDIDATE_MB_TYPE_INTER    0x02
#define CANDIDATE_MB_TYPE_INTER4V  0x04
#define CANDIDATE_MB_TYPE_SKIPPED   0x08


#define CANDIDATE_MB_TYPE_DIRECT   0x10
#define CANDIDATE_MB_TYPE_FORWARD  0x20
#define CANDIDATE_MB_TYPE_BACKWARD 0x40
#define CANDIDATE_MB_TYPE_BIDIR    0x80

#define CANDIDATE_MB_TYPE_INTER_I    0x100
#define CANDIDATE_MB_TYPE_FORWARD_I  0x200
#define CANDIDATE_MB_TYPE_BACKWARD_I 0x400
#define CANDIDATE_MB_TYPE_BIDIR_I    0x800

#define CANDIDATE_MB_TYPE_DIRECT0    0x1000

    int block_index[6]; 
    int block_wrap[6];
    uint8_t *dest[3];

    int *mb_index2xy;        

    
    uint16_t intra_matrix[64];
    uint16_t chroma_intra_matrix[64];
    uint16_t inter_matrix[64];
    uint16_t chroma_inter_matrix[64];
#define QUANT_BIAS_SHIFT 8
    int intra_quant_bias;    
    int inter_quant_bias;    
    int min_qcoeff;          
    int max_qcoeff;          
    int ac_esc_length;       
    uint8_t *intra_ac_vlc_length;
    uint8_t *intra_ac_vlc_last_length;
    uint8_t *inter_ac_vlc_length;
    uint8_t *inter_ac_vlc_last_length;
    uint8_t *luma_dc_vlc_length;
    uint8_t *chroma_dc_vlc_length;
#define UNI_AC_ENC_INDEX(run,level) ((run)*128 + (level))

    int coded_score[8];

    
    int (*q_intra_matrix)[64];
    int (*q_inter_matrix)[64];
    
    uint16_t (*q_intra_matrix16)[2][64];
    uint16_t (*q_inter_matrix16)[2][64];
    int block_last_index[12];  
    
    ScanTable intra_scantable;
    ScanTable intra_h_scantable;
    ScanTable intra_v_scantable;
    ScanTable inter_scantable; 

    
    int (*dct_error_sum)[64];
    int dct_count[2];
    uint16_t (*dct_offset)[64];

    void *opaque;              

    
    int64_t wanted_bits;
    int64_t total_bits;
    int frame_bits;                
    int next_lambda;               
    RateControlContext rc_context; 

    
    int mv_bits;
    int header_bits;
    int i_tex_bits;
    int p_tex_bits;
    int i_count;
    int f_count;
    int b_count;
    int skip_count;
    int misc_bits; 
    int last_bits; 

    
    int error_count;
    uint8_t *error_status_table;       
#define VP_START            1          
#define AC_ERROR            2
#define DC_ERROR            4
#define MV_ERROR            8
#define AC_END              16
#define DC_END              32
#define MV_END              64


    int resync_mb_x;                 
    int resync_mb_y;                 
    GetBitContext last_resync_gb;    
    int mb_num_left;                 
    int next_p_frame_damaged;        
    int error_recognition;

    ParseContext parse_context;

    
    int gob_index;
    int obmc;                       
    int showed_packed_warning;      

    
    int umvplus;                    
    int h263_aic;                   
    int h263_aic_dir;               
    int h263_slice_structured;
    int alt_inter_vlc;              
    int modified_quant;
    int loop_filter;
    int custom_pcf;

    
    int time_increment_bits;        
    int last_time_base;
    int time_base;                  
    int64_t time;                   
    int64_t last_non_b_time;
    uint16_t pp_time;               
    uint16_t pb_time;               
    uint16_t pp_field_time;
    uint16_t pb_field_time;         
    int shape;
    int vol_sprite_usage;
    int sprite_width;
    int sprite_height;
    int sprite_left;
    int sprite_top;
    int sprite_brightness_change;
    int num_sprite_warping_points;
    int real_sprite_warping_points;
    uint16_t sprite_traj[4][2];      
    int sprite_offset[2][2];         
    int sprite_delta[2][2];          
    int sprite_shift[2];             
    int mcsel;
    int quant_precision;
    int quarter_sample;              
    int scalability;
    int hierachy_type;
    int enhancement_type;
    int new_pred;
    int reduced_res_vop;
    int aspect_ratio_info; 
    int sprite_warping_accuracy;
    int low_latency_sprite;
    int data_partitioning;           
    int partitioned_frame;           
    int rvlc;                        
    int resync_marker;               
    int low_delay;                   
    int vo_type;
    int vol_control_parameters;      
    int intra_dc_threshold;          
    int use_intra_dc_vlc;
    PutBitContext tex_pb;            
    PutBitContext pb2;               
    int mpeg_quant;
    int t_frame;                       
    int padding_bug_score;             
    int cplx_estimation_trash_i;
    int cplx_estimation_trash_p;
    int cplx_estimation_trash_b;

    
    int divx_version;
    int divx_build;
    int divx_packed;
    uint8_t *bitstream_buffer; 
    int bitstream_buffer_size;
    unsigned int allocated_bitstream_buffer_size;

    int xvid_build;

    
    int lavc_build;

    
    int rv10_version; 
    int rv10_first_dc_coded[3];
    int orig_width, orig_height;

    
    struct MJpegContext *mjpeg_ctx;
    int mjpeg_vsample[3];       
    int mjpeg_hsample[3];       

    
    int mv_table_index;
    int rl_table_index;
    int rl_chroma_table_index;
    int dc_table_index;
    int use_skip_mb_code;
    int slice_height;      
    int first_slice_line;  
    int flipflop_rounding;
    int msmpeg4_version;   
    int per_mb_rl_table;
    int esc3_level_length;
    int esc3_run_length;
    
    int (*ac_stats)[2][MAX_LEVEL+1][MAX_RUN+1][2];
    int inter_intra_pred;
    int mspel;

    
    GetBitContext gb;

    
    int gop_picture_number;  
    int last_mv_dir;         
    int broken_link;         
    uint8_t *vbv_delay_ptr;  

    
    int progressive_sequence;
    int mpeg_f_code[2][2];
    int picture_structure;

#define PICT_TOP_FIELD     1
#define PICT_BOTTOM_FIELD  2
#define PICT_FRAME         3

    int intra_dc_precision;
    int frame_pred_frame_dct;
    int top_field_first;
    int concealment_motion_vectors;
    int q_scale_type;
    int intra_vlc_format;
    int alternate_scan;
    int repeat_first_field;
    int chroma_420_type;
    int chroma_format;
#define CHROMA_420 1
#define CHROMA_422 2
#define CHROMA_444 3
    int chroma_x_shift;
    int chroma_y_shift;

    int progressive_frame;
    int full_pel[2];
    int interlaced_dct;
    int first_slice;
    int first_field;         

    
    int rtp_mode;

    uint8_t *ptr_lastgob;
    int swap_uv;             
    DCTELEM (*pblocks[12])[64];

    DCTELEM (*block)[64]; 
    DCTELEM (*blocks)[8][64]; 
    int (*decode_mb)(struct MpegEncContext *s, DCTELEM block[6][64]); 
#define SLICE_OK         0
#define SLICE_ERROR     -1
#define SLICE_END       -2 
#define SLICE_NOEND     -3 

    void (*dct_unquantize_mpeg1_intra)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_mpeg1_inter)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_mpeg2_intra)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_mpeg2_inter)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_h263_intra)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_h263_inter)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_h261_intra)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_h261_inter)(struct MpegEncContext *s,
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_intra)(struct MpegEncContext *s, 
                           DCTELEM *block, int n, int qscale);
    void (*dct_unquantize_inter)(struct MpegEncContext *s, 
                           DCTELEM *block, int n, int qscale);
    int (*dct_quantize)(struct MpegEncContext *s, DCTELEM *block, int n, int qscale, int *overflow);
    int (*fast_dct_quantize)(struct MpegEncContext *s, DCTELEM *block, int n, int qscale, int *overflow);
    void (*denoise_dct)(struct MpegEncContext *s, DCTELEM *block);
} MpegEncContext;


void MPV_decode_defaults(MpegEncContext *s);
int MPV_common_init(MpegEncContext *s);
void MPV_common_end(MpegEncContext *s);
void MPV_decode_mb(MpegEncContext *s, DCTELEM block[12][64]);
int MPV_frame_start(MpegEncContext *s, AVCodecContext *avctx);
void MPV_frame_end(MpegEncContext *s);
int MPV_encode_init(AVCodecContext *avctx);
int MPV_encode_end(AVCodecContext *avctx);
int MPV_encode_picture(AVCodecContext *avctx, unsigned char *buf, int buf_size, void *data);
void MPV_common_init_mmx(MpegEncContext *s);
void MPV_common_init_axp(MpegEncContext *s);
void MPV_common_init_mlib(MpegEncContext *s);
void MPV_common_init_mmi(MpegEncContext *s);
void MPV_common_init_arm(MpegEncContext *s);
void MPV_common_init_altivec(MpegEncContext *s);
void MPV_common_init_bfin(MpegEncContext *s);
void ff_clean_intra_table_entries(MpegEncContext *s);
void ff_draw_horiz_band(MpegEncContext *s, int y, int h);
void ff_mpeg_flush(AVCodecContext *avctx);
void ff_print_debug_info(MpegEncContext *s, AVFrame *pict);
void ff_write_quant_matrix(PutBitContext *pb, uint16_t *matrix);
int ff_find_unused_picture(MpegEncContext *s, int shared);
void ff_denoise_dct(MpegEncContext *s, DCTELEM *block);
void ff_update_duplicate_context(MpegEncContext *dst, MpegEncContext *src);
const uint8_t *ff_find_start_code(const uint8_t *p, const uint8_t *end, uint32_t *state);
void ff_set_qscale(MpegEncContext * s, int qscale);

void ff_er_frame_start(MpegEncContext *s);
void ff_er_frame_end(MpegEncContext *s);
void ff_er_add_slice(MpegEncContext *s, int startx, int starty, int endx, int endy, int status);

int ff_dct_common_init(MpegEncContext *s);
void ff_convert_matrix(DSPContext *dsp, int (*qmat)[64], uint16_t (*qmat16)[2][64],
                       const uint16_t *quant_matrix, int bias, int qmin, int qmax, int intra);

void ff_init_block_index(MpegEncContext *s);
void ff_copy_picture(Picture *dst, Picture *src);


int ff_alloc_picture(MpegEncContext *s, Picture *pic, int shared);

extern const enum PixelFormat ff_pixfmt_list_420[];
extern const enum PixelFormat ff_hwaccel_pixfmt_list_420[];

static inline void ff_update_block_index(MpegEncContext *s){
    const int block_size= 8>>s->avctx->lowres;

    s->block_index[0]+=2;
    s->block_index[1]+=2;
    s->block_index[2]+=2;
    s->block_index[3]+=2;
    s->block_index[4]++;
    s->block_index[5]++;
    s->dest[0]+= 2*block_size;
    s->dest[1]+= block_size;
    s->dest[2]+= block_size;
}

static inline int get_bits_diff(MpegEncContext *s){
    const int bits= put_bits_count(&s->pb);
    const int last= s->last_bits;

    s->last_bits = bits;

    return bits - last;
}

static inline int ff_h263_round_chroma(int x){
    static const uint8_t h263_chroma_roundtab[16] = {
    
        0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
    };
    return h263_chroma_roundtab[x & 0xf] + (x >> 3);
}


void ff_estimate_p_frame_motion(MpegEncContext * s,
                             int mb_x, int mb_y);
void ff_estimate_b_frame_motion(MpegEncContext * s,
                             int mb_x, int mb_y);
int ff_get_best_fcode(MpegEncContext * s, int16_t (*mv_table)[2], int type);
void ff_fix_long_p_mvs(MpegEncContext * s);
void ff_fix_long_mvs(MpegEncContext * s, uint8_t *field_select_table, int field_select,
                     int16_t (*mv_table)[2], int f_code, int type, int truncate);
int ff_init_me(MpegEncContext *s);
int ff_pre_estimate_p_frame_motion(MpegEncContext * s, int mb_x, int mb_y);
int ff_epzs_motion_search(MpegEncContext * s, int *mx_ptr, int *my_ptr,
                             int P[10][2], int src_index, int ref_index, int16_t (*last_mv)[2],
                             int ref_mv_scale, int size, int h);
int ff_get_mb_score(MpegEncContext * s, int mx, int my, int src_index,
                               int ref_index, int size, int h, int add_rate);


extern const uint8_t ff_mpeg1_dc_scale_table[128];
extern const uint8_t * const ff_mpeg2_dc_scale_table[4];

void mpeg1_encode_picture_header(MpegEncContext *s, int picture_number);
void mpeg1_encode_mb(MpegEncContext *s,
                     DCTELEM block[6][64],
                     int motion_x, int motion_y);
void ff_mpeg1_encode_init(MpegEncContext *s);
void ff_mpeg1_encode_slice_header(MpegEncContext *s);
void ff_mpeg1_clean_buffers(MpegEncContext *s);
int ff_mpeg1_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size, AVCodecParserContext *s);

extern const uint8_t ff_aic_dc_scale_table[32];
extern const uint8_t ff_h263_chroma_qscale_table[32];
extern const uint8_t ff_h263_loop_filter_strength[32];


void ff_h261_loop_filter(MpegEncContext *s);
void ff_h261_reorder_mb_index(MpegEncContext* s);
void ff_h261_encode_mb(MpegEncContext *s,
                    DCTELEM block[6][64],
                    int motion_x, int motion_y);
void ff_h261_encode_picture_header(MpegEncContext * s, int picture_number);
void ff_h261_encode_init(MpegEncContext *s);
int ff_h261_get_picture_format(int width, int height);



void rv10_encode_picture_header(MpegEncContext *s, int picture_number);
int rv_decode_dc(MpegEncContext *s, int n);
void rv20_encode_picture_header(MpegEncContext *s, int picture_number);



void msmpeg4_encode_picture_header(MpegEncContext * s, int picture_number);
void msmpeg4_encode_ext_header(MpegEncContext * s);
void msmpeg4_encode_mb(MpegEncContext * s,
                       DCTELEM block[6][64],
                       int motion_x, int motion_y);
int msmpeg4_decode_picture_header(MpegEncContext * s);
int msmpeg4_decode_ext_header(MpegEncContext * s, int buf_size);
int ff_msmpeg4_decode_init(AVCodecContext *avctx);
void ff_msmpeg4_encode_init(MpegEncContext *s);
int ff_wmv2_decode_picture_header(MpegEncContext * s);
int ff_wmv2_decode_secondary_picture_header(MpegEncContext * s);
void ff_wmv2_add_mb(MpegEncContext *s, DCTELEM block[6][64], uint8_t *dest_y, uint8_t *dest_cb, uint8_t *dest_cr);
void ff_mspel_motion(MpegEncContext *s,
                               uint8_t *dest_y, uint8_t *dest_cb, uint8_t *dest_cr,
                               uint8_t **ref_picture, op_pixels_func (*pix_op)[4],
                               int motion_x, int motion_y, int h);
int ff_wmv2_encode_picture_header(MpegEncContext * s, int picture_number);
void ff_wmv2_encode_mb(MpegEncContext * s,
                       DCTELEM block[6][64],
                       int motion_x, int motion_y);

#endif 

