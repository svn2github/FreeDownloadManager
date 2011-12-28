/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_X86_DSPUTIL_MMX_H
#define AVCODEC_X86_DSPUTIL_MMX_H

#include <stdint.h>
#include "libavcodec/dsputil.h"

typedef struct { uint64_t a, b; } xmm_reg;

extern const uint64_t ff_bone;
extern const uint64_t ff_wtwo;

extern const uint64_t ff_pdw_80000000[2];

extern const uint64_t ff_pw_3;
extern const uint64_t ff_pw_4;
extern const xmm_reg  ff_pw_5;
extern const xmm_reg  ff_pw_8;
extern const uint64_t ff_pw_15;
extern const xmm_reg  ff_pw_16;
extern const uint64_t ff_pw_20;
extern const xmm_reg  ff_pw_28;
extern const xmm_reg  ff_pw_32;
extern const uint64_t ff_pw_42;
extern const xmm_reg  ff_pw_64;
extern const uint64_t ff_pw_96;
extern const uint64_t ff_pw_128;
extern const uint64_t ff_pw_255;

extern const uint64_t ff_pb_1;
extern const uint64_t ff_pb_3;
extern const uint64_t ff_pb_7;
extern const uint64_t ff_pb_1F;
extern const uint64_t ff_pb_3F;
extern const uint64_t ff_pb_81;
extern const uint64_t ff_pb_A1;
extern const uint64_t ff_pb_FC;

extern const double ff_pd_1[2];
extern const double ff_pd_2[2];

#define LOAD4(stride,in,a,b,c,d)\
    "movq 0*"#stride"+"#in", "#a"\n\t"\
    "movq 1*"#stride"+"#in", "#b"\n\t"\
    "movq 2*"#stride"+"#in", "#c"\n\t"\
    "movq 3*"#stride"+"#in", "#d"\n\t"

#define STORE4(stride,out,a,b,c,d)\
    "movq "#a", 0*"#stride"+"#out"\n\t"\
    "movq "#b", 1*"#stride"+"#out"\n\t"\
    "movq "#c", 2*"#stride"+"#out"\n\t"\
    "movq "#d", 3*"#stride"+"#out"\n\t"


#define SUMSUB_BA( a, b ) \
    "paddw "#b", "#a" \n\t"\
    "paddw "#b", "#b" \n\t"\
    "psubw "#a", "#b" \n\t"

#define SBUTTERFLY(a,b,t,n,m)\
    "mov" #m " " #a ", " #t "         \n\t" \
    "punpckl" #n " " #b ", " #a "     \n\t" \
    "punpckh" #n " " #b ", " #t "     \n\t" \

#define TRANSPOSE4(a,b,c,d,t)\
    SBUTTERFLY(a,b,t,wd,q) \
    SBUTTERFLY(c,d,b,wd,q) \
    SBUTTERFLY(a,c,d,dq,q) \
    SBUTTERFLY(t,b,c,dq,q) 



#define TRANSPOSE8x4(a,b,c,d,e,f,g,h,t)\
    "punpcklbw " #e ", " #a " \n\t" \
    "punpcklbw " #f ", " #b " \n\t" \
    "punpcklbw " #g ", " #c " \n\t" \
    "punpcklbw " #h ", " #d " \n\t" \
    SBUTTERFLY(a, b, t, bw, q)   \
                                 \
    SBUTTERFLY(c, d, b, bw, q)   \
                                 \
    SBUTTERFLY(a, c, d, wd, q)   \
                                 \
    SBUTTERFLY(t, b, c, wd, q)   \
                                 

#if ARCH_X86_64

#define TRANSPOSE8(a,b,c,d,e,f,g,h,t)\
    SBUTTERFLY(a,b,%%xmm8,wd,dqa)\
    SBUTTERFLY(c,d,b,wd,dqa)\
    SBUTTERFLY(e,f,d,wd,dqa)\
    SBUTTERFLY(g,h,f,wd,dqa)\
    SBUTTERFLY(a,c,h,dq,dqa)\
    SBUTTERFLY(%%xmm8,b,c,dq,dqa)\
    SBUTTERFLY(e,g,b,dq,dqa)\
    SBUTTERFLY(d,f,g,dq,dqa)\
    SBUTTERFLY(a,e,f,qdq,dqa)\
    SBUTTERFLY(%%xmm8,d,e,qdq,dqa)\
    SBUTTERFLY(h,b,d,qdq,dqa)\
    SBUTTERFLY(c,g,b,qdq,dqa)\
    "movdqa %%xmm8, "#g"              \n\t"
#else
#define TRANSPOSE8(a,b,c,d,e,f,g,h,t)\
    "movdqa "#h", "#t"                \n\t"\
    SBUTTERFLY(a,b,h,wd,dqa)\
    "movdqa "#h", 16"#t"              \n\t"\
    "movdqa "#t", "#h"                \n\t"\
    SBUTTERFLY(c,d,b,wd,dqa)\
    SBUTTERFLY(e,f,d,wd,dqa)\
    SBUTTERFLY(g,h,f,wd,dqa)\
    SBUTTERFLY(a,c,h,dq,dqa)\
    "movdqa "#h", "#t"                \n\t"\
    "movdqa 16"#t", "#h"              \n\t"\
    SBUTTERFLY(h,b,c,dq,dqa)\
    SBUTTERFLY(e,g,b,dq,dqa)\
    SBUTTERFLY(d,f,g,dq,dqa)\
    SBUTTERFLY(a,e,f,qdq,dqa)\
    SBUTTERFLY(h,d,e,qdq,dqa)\
    "movdqa "#h", 16"#t"              \n\t"\
    "movdqa "#t", "#h"                \n\t"\
    SBUTTERFLY(h,b,d,qdq,dqa)\
    SBUTTERFLY(c,g,b,qdq,dqa)\
    "movdqa 16"#t", "#g"              \n\t"
#endif

#define MOVQ_WONE(regd) \
    __asm__ volatile ( \
    "pcmpeqd %%" #regd ", %%" #regd " \n\t" \
    "psrlw $15, %%" #regd ::)

void dsputilenc_init_mmx(DSPContext* c, AVCodecContext *avctx);
void dsputil_init_pix_mmx(DSPContext* c, AVCodecContext *avctx);

void add_pixels_clamped_mmx(const DCTELEM *block, uint8_t *pixels, int line_size);
void put_pixels_clamped_mmx(const DCTELEM *block, uint8_t *pixels, int line_size);
void put_signed_pixels_clamped_mmx(const DCTELEM *block, uint8_t *pixels, int line_size);

void ff_cavsdsp_init_mmx2(DSPContext* c, AVCodecContext *avctx);
void ff_cavsdsp_init_3dnow(DSPContext* c, AVCodecContext *avctx);
void ff_put_cavs_qpel8_mc00_mmx2(uint8_t *dst, uint8_t *src, int stride);
void ff_avg_cavs_qpel8_mc00_mmx2(uint8_t *dst, uint8_t *src, int stride);
void ff_put_cavs_qpel16_mc00_mmx2(uint8_t *dst, uint8_t *src, int stride);
void ff_avg_cavs_qpel16_mc00_mmx2(uint8_t *dst, uint8_t *src, int stride);

void ff_vc1dsp_init_mmx(DSPContext* dsp, AVCodecContext *avctx);
void ff_put_vc1_mspel_mc00_mmx(uint8_t *dst, const uint8_t *src, int stride, int rnd);
void ff_avg_vc1_mspel_mc00_mmx2(uint8_t *dst, const uint8_t *src, int stride, int rnd);

void ff_lpc_compute_autocorr_sse2(const int32_t *data, int len, int lag,
                                   double *autoc);

void ff_mmx_idct(DCTELEM *block);
void ff_mmxext_idct(DCTELEM *block);

#endif 
