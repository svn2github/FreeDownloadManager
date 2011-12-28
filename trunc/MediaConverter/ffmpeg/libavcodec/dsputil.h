/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_DSPUTIL_H
#define AVCODEC_DSPUTIL_H

#include "libavutil/intreadwrite.h"
#include "avcodec.h"




typedef short DCTELEM;

void fdct_ifast (DCTELEM *data);
void fdct_ifast248 (DCTELEM *data);
void ff_jpeg_fdct_islow (DCTELEM *data);
void ff_fdct248_islow (DCTELEM *data);

void j_rev_dct (DCTELEM *data);
void j_rev_dct4 (DCTELEM *data);
void j_rev_dct2 (DCTELEM *data);
void j_rev_dct1 (DCTELEM *data);
void ff_wmv2_idct_c(DCTELEM *data);

void ff_fdct_mmx(DCTELEM *block);
void ff_fdct_mmx2(DCTELEM *block);
void ff_fdct_sse2(DCTELEM *block);

void ff_h264_idct8_add_c(uint8_t *dst, DCTELEM *block, int stride);
void ff_h264_idct_add_c(uint8_t *dst, DCTELEM *block, int stride);
void ff_h264_idct8_dc_add_c(uint8_t *dst, DCTELEM *block, int stride);
void ff_h264_idct_dc_add_c(uint8_t *dst, DCTELEM *block, int stride);
void ff_h264_lowres_idct_add_c(uint8_t *dst, int stride, DCTELEM *block);
void ff_h264_lowres_idct_put_c(uint8_t *dst, int stride, DCTELEM *block);
void ff_h264_idct_add16_c(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
void ff_h264_idct_add16intra_c(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
void ff_h264_idct8_add4_c(uint8_t *dst, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);
void ff_h264_idct_add8_c(uint8_t **dest, const int *blockoffset, DCTELEM *block, int stride, const uint8_t nnzc[6*8]);

void ff_vector_fmul_window_c(float *dst, const float *src0, const float *src1,
                             const float *win, float add_bias, int len);
void ff_float_to_int16_c(int16_t *dst, const float *src, long len);
void ff_float_to_int16_interleave_c(int16_t *dst, const float **src, long len, int channels);


extern const uint8_t ff_alternate_horizontal_scan[64];
extern const uint8_t ff_alternate_vertical_scan[64];
extern const uint8_t ff_zigzag_direct[64];
extern const uint8_t ff_zigzag248_direct[64];


#define MAX_NEG_CROP 1024


extern uint32_t ff_squareTbl[512];
extern uint8_t ff_cropTbl[256 + 2 * MAX_NEG_CROP];


void ff_vp3_idct_c(DCTELEM *block);
void ff_vp3_idct_put_c(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_add_c(uint8_t *dest, int line_size, DCTELEM *block);
void ff_vp3_idct_dc_add_c(uint8_t *dest, int line_size, const DCTELEM *block);

void ff_vp3_v_loop_filter_c(uint8_t *src, int stride, int *bounding_values);
void ff_vp3_h_loop_filter_c(uint8_t *src, int stride, int *bounding_values);


void ff_vp6_filter_diag4_c(uint8_t *dst, uint8_t *src, int stride,
                           const int16_t *h_weights, const int16_t *v_weights);


void ff_bink_idct_c    (DCTELEM *block);
void ff_bink_idct_add_c(uint8_t *dest, int linesize, DCTELEM *block);
void ff_bink_idct_put_c(uint8_t *dest, int linesize, DCTELEM *block);


void ff_put_cavs_qpel8_mc00_c(uint8_t *dst, uint8_t *src, int stride);
void ff_avg_cavs_qpel8_mc00_c(uint8_t *dst, uint8_t *src, int stride);
void ff_put_cavs_qpel16_mc00_c(uint8_t *dst, uint8_t *src, int stride);
void ff_avg_cavs_qpel16_mc00_c(uint8_t *dst, uint8_t *src, int stride);


void ff_put_vc1_mspel_mc00_c(uint8_t *dst, const uint8_t *src, int stride, int rnd);
void ff_avg_vc1_mspel_mc00_c(uint8_t *dst, const uint8_t *src, int stride, int rnd);


void ff_ea_idct_put_c(uint8_t *dest, int linesize, DCTELEM *block);


void ff_img_copy_plane(uint8_t *dst, int dst_wrap, const uint8_t *src, int src_wrap, int width, int height);
void ff_shrink22(uint8_t *dst, int dst_wrap, const uint8_t *src, int src_wrap, int width, int height);
void ff_shrink44(uint8_t *dst, int dst_wrap, const uint8_t *src, int src_wrap, int width, int height);
void ff_shrink88(uint8_t *dst, int dst_wrap, const uint8_t *src, int src_wrap, int width, int height);

void ff_gmc_c(uint8_t *dst, uint8_t *src, int stride, int h, int ox, int oy,
              int dxx, int dxy, int dyx, int dyy, int shift, int r, int width, int height);








typedef void (*op_pixels_func)(uint8_t *block, const uint8_t *pixels, int line_size, int h);
typedef void (*tpel_mc_func)(uint8_t *block, const uint8_t *pixels, int line_size, int w, int h);
typedef void (*qpel_mc_func)(uint8_t *dst, uint8_t *src, int stride);
typedef void (*h264_chroma_mc_func)(uint8_t *dst, uint8_t *src, int srcStride, int h, int x, int y);

typedef void (*op_fill_func)(uint8_t *block, uint8_t value, int line_size, int h);

#define DEF_OLD_QPEL(name)\
void ff_put_        ## name (uint8_t *dst, uint8_t *src, int stride);\
void ff_put_no_rnd_ ## name (uint8_t *dst, uint8_t *src, int stride);\
void ff_avg_        ## name (uint8_t *dst, uint8_t *src, int stride);

DEF_OLD_QPEL(qpel16_mc11_old_c)
DEF_OLD_QPEL(qpel16_mc31_old_c)
DEF_OLD_QPEL(qpel16_mc12_old_c)
DEF_OLD_QPEL(qpel16_mc32_old_c)
DEF_OLD_QPEL(qpel16_mc13_old_c)
DEF_OLD_QPEL(qpel16_mc33_old_c)
DEF_OLD_QPEL(qpel8_mc11_old_c)
DEF_OLD_QPEL(qpel8_mc31_old_c)
DEF_OLD_QPEL(qpel8_mc12_old_c)
DEF_OLD_QPEL(qpel8_mc32_old_c)
DEF_OLD_QPEL(qpel8_mc13_old_c)
DEF_OLD_QPEL(qpel8_mc33_old_c)

#define CALL_2X_PIXELS(a, b, n)\
static void a(uint8_t *block, const uint8_t *pixels, int line_size, int h){\
    b(block  , pixels  , line_size, h);\
    b(block+n, pixels+n, line_size, h);\
}




typedef int (*me_cmp_func)(void  *s, uint8_t *blk1, uint8_t *blk2, int line_size, int h);


typedef struct ScanTable{
    const uint8_t *scantable;
    uint8_t permutated[64];
    uint8_t raster_end[64];
#if ARCH_PPC
                
    DECLARE_ALIGNED(16, uint8_t, inverse)[64];
#endif
} ScanTable;

void ff_init_scantable(uint8_t *, ScanTable *st, const uint8_t *src_scantable);

void ff_emulated_edge_mc(uint8_t *buf, uint8_t *src, int linesize,
                         int block_w, int block_h,
                         int src_x, int src_y, int w, int h);


typedef struct DSPContext {
    
    void (*get_pixels)(DCTELEM *block, const uint8_t *pixels, int line_size);
    void (*diff_pixels)(DCTELEM *block, const uint8_t *s1, const uint8_t *s2, int stride);
    void (*put_pixels_clamped)(const DCTELEM *block, uint8_t *pixels, int line_size);
    void (*put_signed_pixels_clamped)(const DCTELEM *block, uint8_t *pixels, int line_size);
    void (*put_pixels_nonclamped)(const DCTELEM *block, uint8_t *pixels, int line_size);
    void (*add_pixels_clamped)(const DCTELEM *block, uint8_t *pixels, int line_size);
    void (*add_pixels8)(uint8_t *pixels, DCTELEM *block, int line_size);
    void (*add_pixels4)(uint8_t *pixels, DCTELEM *block, int line_size);
    int (*sum_abs_dctelem)(DCTELEM *block);
    
    void (*gmc1)(uint8_t *dst, uint8_t *src, int srcStride, int h, int x16, int y16, int rounder);
    
    void (*gmc )(uint8_t *dst, uint8_t *src, int stride, int h, int ox, int oy,
                    int dxx, int dxy, int dyx, int dyy, int shift, int r, int width, int height);
    void (*clear_block)(DCTELEM *block);
    void (*clear_blocks)(DCTELEM *blocks);
    int (*pix_sum)(uint8_t * pix, int line_size);
    int (*pix_norm1)(uint8_t * pix, int line_size);


    me_cmp_func sad[6]; 
    me_cmp_func sse[6];
    me_cmp_func hadamard8_diff[6];
    me_cmp_func dct_sad[6];
    me_cmp_func quant_psnr[6];
    me_cmp_func bit[6];
    me_cmp_func rd[6];
    me_cmp_func vsad[6];
    me_cmp_func vsse[6];
    me_cmp_func nsse[6];
    me_cmp_func w53[6];
    me_cmp_func w97[6];
    me_cmp_func dct_max[6];
    me_cmp_func dct264_sad[6];

    me_cmp_func me_pre_cmp[6];
    me_cmp_func me_cmp[6];
    me_cmp_func me_sub_cmp[6];
    me_cmp_func mb_cmp[6];
    me_cmp_func ildct_cmp[6]; 
    me_cmp_func frame_skip_cmp[6]; 

    int (*ssd_int8_vs_int16)(const int8_t *pix1, const int16_t *pix2,
                             int size);

    
    op_pixels_func put_pixels_tab[4][4];

    
    op_pixels_func avg_pixels_tab[4][4];

    
    op_pixels_func put_no_rnd_pixels_tab[4][4];

    
    op_pixels_func avg_no_rnd_pixels_tab[4][4];

    void (*put_no_rnd_pixels_l2[2])(uint8_t *block, const uint8_t *a, const uint8_t *b, int line_size, int h);

    
    tpel_mc_func put_tpel_pixels_tab[11]; 
    tpel_mc_func avg_tpel_pixels_tab[11]; 

    qpel_mc_func put_qpel_pixels_tab[2][16];
    qpel_mc_func avg_qpel_pixels_tab[2][16];
    qpel_mc_func put_no_rnd_qpel_pixels_tab[2][16];
    qpel_mc_func avg_no_rnd_qpel_pixels_tab[2][16];
    qpel_mc_func put_mspel_pixels_tab[8];

    
    h264_chroma_mc_func put_h264_chroma_pixels_tab[3];
    h264_chroma_mc_func avg_h264_chroma_pixels_tab[3];
    
    h264_chroma_mc_func put_no_rnd_vc1_chroma_pixels_tab[3];
    h264_chroma_mc_func avg_no_rnd_vc1_chroma_pixels_tab[3];

    qpel_mc_func put_h264_qpel_pixels_tab[4][16];
    qpel_mc_func avg_h264_qpel_pixels_tab[4][16];

    qpel_mc_func put_2tap_qpel_pixels_tab[4][16];
    qpel_mc_func avg_2tap_qpel_pixels_tab[4][16];

    
    qpel_mc_func put_cavs_qpel_pixels_tab[2][16];
    qpel_mc_func avg_cavs_qpel_pixels_tab[2][16];
    void (*cavs_filter_lv)(uint8_t *pix, int stride, int alpha, int beta, int tc, int bs1, int bs2);
    void (*cavs_filter_lh)(uint8_t *pix, int stride, int alpha, int beta, int tc, int bs1, int bs2);
    void (*cavs_filter_cv)(uint8_t *pix, int stride, int alpha, int beta, int tc, int bs1, int bs2);
    void (*cavs_filter_ch)(uint8_t *pix, int stride, int alpha, int beta, int tc, int bs1, int bs2);
    void (*cavs_idct8_add)(uint8_t *dst, DCTELEM *block, int stride);

    me_cmp_func pix_abs[2][4];

    
    void (*add_bytes)(uint8_t *dst, uint8_t *src, int w);
    void (*add_bytes_l2)(uint8_t *dst, uint8_t *src1, uint8_t *src2, int w);
    void (*diff_bytes)(uint8_t *dst, uint8_t *src1, uint8_t *src2,int w);
    
    void (*sub_hfyu_median_prediction)(uint8_t *dst, const uint8_t *src1, const uint8_t *src2, int w, int *left, int *left_top);
    void (*add_hfyu_median_prediction)(uint8_t *dst, const uint8_t *top, const uint8_t *diff, int w, int *left, int *left_top);
    int  (*add_hfyu_left_prediction)(uint8_t *dst, const uint8_t *src, int w, int left);
    void (*add_hfyu_left_prediction_bgr32)(uint8_t *dst, const uint8_t *src, int w, int *red, int *green, int *blue, int *alpha);
    
    void (*add_png_paeth_prediction)(uint8_t *dst, uint8_t *src, uint8_t *top, int w, int bpp);
    void (*bswap_buf)(uint32_t *dst, const uint32_t *src, int w);

    void (*h263_v_loop_filter)(uint8_t *src, int stride, int qscale);
    void (*h263_h_loop_filter)(uint8_t *src, int stride, int qscale);

    void (*h261_loop_filter)(uint8_t *src, int stride);

    void (*x8_v_loop_filter)(uint8_t *src, int stride, int qscale);
    void (*x8_h_loop_filter)(uint8_t *src, int stride, int qscale);

    void (*vp3_idct_dc_add)(uint8_t *dest, int line_size, const DCTELEM *block);
    void (*vp3_v_loop_filter)(uint8_t *src, int stride, int *bounding_values);
    void (*vp3_h_loop_filter)(uint8_t *src, int stride, int *bounding_values);

    void (*vp6_filter_diag4)(uint8_t *dst, uint8_t *src, int stride,
                             const int16_t *h_weights,const int16_t *v_weights);

    
    void (*vorbis_inverse_coupling)(float *mag, float *ang, int blocksize);
    void (*ac3_downmix)(float (*samples)[256], float (*matrix)[2], int out_ch, int in_ch, int len);
    
    void (*lpc_compute_autocorr)(const int32_t *data, int len, int lag, double *autoc);
    
    void (*vector_fmul)(float *dst, const float *src, int len);
    void (*vector_fmul_reverse)(float *dst, const float *src0, const float *src1, int len);
    
    void (*vector_fmul_add)(float *dst, const float *src0, const float *src1, const float *src2, int len);
    
    void (*vector_fmul_window)(float *dst, const float *src0, const float *src1, const float *win, float add_bias, int len);
    
    void (*int32_to_float_fmul_scalar)(float *dst, const int *src, float mul, int len);
    void (*vector_clipf)(float *dst , const float *src , float min, float max, int len );
    
    void (*vector_fmul_scalar)(float *dst, const float *src, float mul,
                               int len);
    
    void (*vector_fmul_sv_scalar[2])(float *dst, const float *src,
                                     const float **sv, float mul, int len);
    
    void (*sv_fmul_scalar[2])(float *dst, const float **sv,
                              float mul, int len);
    
    float (*scalarproduct_float)(const float *v1, const float *v2, int len);
    
    void (*butterflies_float)(float *restrict v1, float *restrict v2, int len);

    
    void (*float_to_int16)(int16_t *dst, const float *src, long len);
    void (*float_to_int16_interleave)(int16_t *dst, const float **src, long len, int channels);

    
    void (*fdct)(DCTELEM *block);
    void (*fdct248)(DCTELEM *block);

    
    void (*idct)(DCTELEM *block);

    
    void (*idct_put)(uint8_t *dest, int line_size, DCTELEM *block);

    
    void (*idct_add)(uint8_t *dest, int line_size, DCTELEM *block);

    
    uint8_t idct_permutation[64];
    int idct_permutation_type;
#define FF_NO_IDCT_PERM 1
#define FF_LIBMPEG2_IDCT_PERM 2
#define FF_SIMPLE_IDCT_PERM 3
#define FF_TRANSPOSE_IDCT_PERM 4
#define FF_PARTTRANS_IDCT_PERM 5
#define FF_SSE2_IDCT_PERM 6

    int (*try_8x8basis)(int16_t rem[64], int16_t weight[64], int16_t basis[64], int scale);
    void (*add_8x8basis)(int16_t rem[64], int16_t basis[64], int scale);
#define BASIS_SHIFT 16
#define RECON_SHIFT 6

    void (*draw_edges)(uint8_t *buf, int wrap, int width, int height, int w);
#define EDGE_WIDTH 16

    void (*prefetch)(void *mem, int stride, int h);

    void (*shrink[4])(uint8_t *dst, int dst_wrap, const uint8_t *src, int src_wrap, int width, int height);

    
    void (*mlp_filter_channel)(int32_t *state, const int32_t *coeff,
                               int firorder, int iirorder,
                               unsigned int filter_shift, int32_t mask, int blocksize,
                               int32_t *sample_buffer);

    
    void (*vc1_inv_trans_8x8)(DCTELEM *b);
    void (*vc1_inv_trans_8x4)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_4x8)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_4x4)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_8x8_dc)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_8x4_dc)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_4x8_dc)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_inv_trans_4x4_dc)(uint8_t *dest, int line_size, DCTELEM *block);
    void (*vc1_v_overlap)(uint8_t* src, int stride);
    void (*vc1_h_overlap)(uint8_t* src, int stride);
    void (*vc1_v_loop_filter4)(uint8_t *src, int stride, int pq);
    void (*vc1_h_loop_filter4)(uint8_t *src, int stride, int pq);
    void (*vc1_v_loop_filter8)(uint8_t *src, int stride, int pq);
    void (*vc1_h_loop_filter8)(uint8_t *src, int stride, int pq);
    void (*vc1_v_loop_filter16)(uint8_t *src, int stride, int pq);
    void (*vc1_h_loop_filter16)(uint8_t *src, int stride, int pq);
    
    op_pixels_func put_vc1_mspel_pixels_tab[16];
    op_pixels_func avg_vc1_mspel_pixels_tab[16];

    
    void (*x8_spatial_compensation[12])(uint8_t *src , uint8_t *dst, int linesize);
    void (*x8_setup_spatial_compensation)(uint8_t *src, uint8_t *dst, int linesize,
           int * range, int * sum,  int edges);

    
    int32_t (*scalarproduct_int16)(int16_t *v1, int16_t *v2, int len, int shift);
    
    
    int32_t (*scalarproduct_and_madd_int16)(int16_t *v1, int16_t *v2, int16_t *v3, int len, int mul);

    
    qpel_mc_func put_rv30_tpel_pixels_tab[4][16];
    qpel_mc_func avg_rv30_tpel_pixels_tab[4][16];

    
    qpel_mc_func put_rv40_qpel_pixels_tab[4][16];
    qpel_mc_func avg_rv40_qpel_pixels_tab[4][16];
    h264_chroma_mc_func put_rv40_chroma_pixels_tab[3];
    h264_chroma_mc_func avg_rv40_chroma_pixels_tab[3];

    
    op_fill_func fill_block_tab[2];
    void (*scale_block)(const uint8_t src[64], uint8_t *dst, int linesize);
} DSPContext;

void dsputil_static_init(void);
void dsputil_init(DSPContext* p, AVCodecContext *avctx);

int ff_check_alignment(void);


void ff_block_permute(DCTELEM *block, uint8_t *permutation, const uint8_t *scantable, int last);

void ff_set_cmp(DSPContext* c, me_cmp_func *cmp, int type);

#define         BYTE_VEC32(c)   ((c)*0x01010101UL)

static inline uint32_t rnd_avg32(uint32_t a, uint32_t b)
{
    return (a | b) - (((a ^ b) & ~BYTE_VEC32(0x01)) >> 1);
}

static inline uint32_t no_rnd_avg32(uint32_t a, uint32_t b)
{
    return (a & b) + (((a ^ b) & ~BYTE_VEC32(0x01)) >> 1);
}

static inline int get_penalty_factor(int lambda, int lambda2, int type){
    switch(type&0xFF){
    default:
    case FF_CMP_SAD:
        return lambda>>FF_LAMBDA_SHIFT;
    case FF_CMP_DCT:
        return (3*lambda)>>(FF_LAMBDA_SHIFT+1);
    case FF_CMP_W53:
        return (4*lambda)>>(FF_LAMBDA_SHIFT);
    case FF_CMP_W97:
        return (2*lambda)>>(FF_LAMBDA_SHIFT);
    case FF_CMP_SATD:
    case FF_CMP_DCT264:
        return (2*lambda)>>FF_LAMBDA_SHIFT;
    case FF_CMP_RD:
    case FF_CMP_PSNR:
    case FF_CMP_SSE:
    case FF_CMP_NSSE:
        return lambda2>>FF_LAMBDA_SHIFT;
    case FF_CMP_BIT:
        return 1;
    }
}


#define emms_c()


int mm_support(void);
extern int mm_flags;

void dsputil_init_alpha(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_arm(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_bfin(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_mlib(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_mmi(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_mmx(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_ppc(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_sh4(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_vis(DSPContext* c, AVCodecContext *avctx);

void ff_dsputil_init_dwt(DSPContext *c);
void ff_cavsdsp_init(DSPContext* c, AVCodecContext *avctx);
void ff_rv30dsp_init(DSPContext* c, AVCodecContext *avctx);
void ff_rv40dsp_init(DSPContext* c, AVCodecContext *avctx);
void ff_vc1dsp_init(DSPContext* c, AVCodecContext *avctx);
void ff_intrax8dsp_init(DSPContext* c, AVCodecContext *avctx);
void ff_mlp_init(DSPContext* c, AVCodecContext *avctx);
void ff_mlp_init_x86(DSPContext* c, AVCodecContext *avctx);

#if HAVE_MMX

#undef emms_c

static inline void emms(void)
{
    __asm__ volatile ("emms;":::"memory");
}


#define emms_c() \
{\
    if (mm_flags & FF_MM_MMX)\
        emms();\
}

#elif ARCH_ARM

#if HAVE_NEON
#   define STRIDE_ALIGN 16
#endif

#elif ARCH_PPC

#define STRIDE_ALIGN 16

#elif HAVE_MMI

#define STRIDE_ALIGN 16

#else

#define mm_flags 0
#define mm_support() 0

#endif

#ifndef STRIDE_ALIGN
#   define STRIDE_ALIGN 8
#endif

#define LOCAL_ALIGNED(a, t, v, s, ...)                          \
    uint8_t la_##v[sizeof(t s __VA_ARGS__) + (a)];              \
    t (*v) __VA_ARGS__ = (void *)FFALIGN((uintptr_t)la_##v, a)

#if HAVE_LOCAL_ALIGNED_8
#   define LOCAL_ALIGNED_8(t, v, s, ...) DECLARE_ALIGNED(8, t, v) s __VA_ARGS__
#else
#   define LOCAL_ALIGNED_8(t, v, s, ...) LOCAL_ALIGNED(8, t, v, s, __VA_ARGS__)
#endif

#if HAVE_LOCAL_ALIGNED_16
#   define LOCAL_ALIGNED_16(t, v, s, ...) DECLARE_ALIGNED(16, t, v) s __VA_ARGS__
#else
#   define LOCAL_ALIGNED_16(t, v, s, ...) LOCAL_ALIGNED(16, t, v, s, __VA_ARGS__)
#endif


void get_psnr(uint8_t *orig_image[3], uint8_t *coded_image[3],
              int orig_linesize[3], int coded_linesize,
              AVCodecContext *avctx);

#define WRAPPER8_16(name8, name16)\
static int name16(void  *s, uint8_t *dst, uint8_t *src, int stride, int h){\
    return name8(s, dst           , src           , stride, h)\
          +name8(s, dst+8         , src+8         , stride, h);\
}

#define WRAPPER8_16_SQ(name8, name16)\
static int name16(void  *s, uint8_t *dst, uint8_t *src, int stride, int h){\
    int score=0;\
    score +=name8(s, dst           , src           , stride, 8);\
    score +=name8(s, dst+8         , src+8         , stride, 8);\
    if(h==16){\
        dst += 8*stride;\
        src += 8*stride;\
        score +=name8(s, dst           , src           , stride, 8);\
        score +=name8(s, dst+8         , src+8         , stride, 8);\
    }\
    return score;\
}


static inline void copy_block2(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN16(dst   , AV_RN16(src   ));
        dst+=dstStride;
        src+=srcStride;
    }
}

static inline void copy_block4(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN32(dst   , AV_RN32(src   ));
        dst+=dstStride;
        src+=srcStride;
    }
}

static inline void copy_block8(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN32(dst   , AV_RN32(src   ));
        AV_WN32(dst+4 , AV_RN32(src+4 ));
        dst+=dstStride;
        src+=srcStride;
    }
}

static inline void copy_block9(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN32(dst   , AV_RN32(src   ));
        AV_WN32(dst+4 , AV_RN32(src+4 ));
        dst[8]= src[8];
        dst+=dstStride;
        src+=srcStride;
    }
}

static inline void copy_block16(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN32(dst   , AV_RN32(src   ));
        AV_WN32(dst+4 , AV_RN32(src+4 ));
        AV_WN32(dst+8 , AV_RN32(src+8 ));
        AV_WN32(dst+12, AV_RN32(src+12));
        dst+=dstStride;
        src+=srcStride;
    }
}

static inline void copy_block17(uint8_t *dst, const uint8_t *src, int dstStride, int srcStride, int h)
{
    int i;
    for(i=0; i<h; i++)
    {
        AV_WN32(dst   , AV_RN32(src   ));
        AV_WN32(dst+4 , AV_RN32(src+4 ));
        AV_WN32(dst+8 , AV_RN32(src+8 ));
        AV_WN32(dst+12, AV_RN32(src+12));
        dst[16]= src[16];
        dst+=dstStride;
        src+=srcStride;
    }
}

#endif 
