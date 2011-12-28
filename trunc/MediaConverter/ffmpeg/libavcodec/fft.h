/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_FFT_H
#define AVCODEC_FFT_H

#include <stdint.h>
#include "config.h"
#include "libavutil/mem.h"
#include "avfft.h"



struct FFTContext {
    int nbits;
    int inverse;
    uint16_t *revtab;
    FFTComplex *exptab;
    FFTComplex *exptab1; 
    FFTComplex *tmp_buf;
    int mdct_size; 
    int mdct_bits; 
    
    FFTSample *tcos;
    FFTSample *tsin;
    void (*fft_permute)(struct FFTContext *s, FFTComplex *z);
    void (*fft_calc)(struct FFTContext *s, FFTComplex *z);
    void (*imdct_calc)(struct FFTContext *s, FFTSample *output, const FFTSample *input);
    void (*imdct_half)(struct FFTContext *s, FFTSample *output, const FFTSample *input);
    void (*mdct_calc)(struct FFTContext *s, FFTSample *output, const FFTSample *input);
    int split_radix;
    int permutation;
#define FF_MDCT_PERM_NONE       0
#define FF_MDCT_PERM_INTERLEAVE 1
};

#if CONFIG_HARDCODED_TABLES
#define COSTABLE_CONST const
#define SINTABLE_CONST const
#define SINETABLE_CONST const
#else
#define COSTABLE_CONST
#define SINTABLE_CONST
#define SINETABLE_CONST
#endif

#define COSTABLE(size) \
    COSTABLE_CONST DECLARE_ALIGNED(16, FFTSample, ff_cos_##size)[size/2]
#define SINTABLE(size) \
    SINTABLE_CONST DECLARE_ALIGNED(16, FFTSample, ff_sin_##size)[size/2]
#define SINETABLE(size) \
    SINETABLE_CONST DECLARE_ALIGNED(16, float, ff_sine_##size)[size]
extern COSTABLE(16);
extern COSTABLE(32);
extern COSTABLE(64);
extern COSTABLE(128);
extern COSTABLE(256);
extern COSTABLE(512);
extern COSTABLE(1024);
extern COSTABLE(2048);
extern COSTABLE(4096);
extern COSTABLE(8192);
extern COSTABLE(16384);
extern COSTABLE(32768);
extern COSTABLE(65536);
extern COSTABLE_CONST FFTSample* const ff_cos_tabs[17];


void ff_init_ff_cos_tabs(int index);

extern SINTABLE(16);
extern SINTABLE(32);
extern SINTABLE(64);
extern SINTABLE(128);
extern SINTABLE(256);
extern SINTABLE(512);
extern SINTABLE(1024);
extern SINTABLE(2048);
extern SINTABLE(4096);
extern SINTABLE(8192);
extern SINTABLE(16384);
extern SINTABLE(32768);
extern SINTABLE(65536);


int ff_fft_init(FFTContext *s, int nbits, int inverse);
void ff_fft_permute_c(FFTContext *s, FFTComplex *z);
void ff_fft_calc_c(FFTContext *s, FFTComplex *z);

void ff_fft_init_altivec(FFTContext *s);
void ff_fft_init_mmx(FFTContext *s);
void ff_fft_init_arm(FFTContext *s);


static inline void ff_fft_permute(FFTContext *s, FFTComplex *z)
{
    s->fft_permute(s, z);
}

static inline void ff_fft_calc(FFTContext *s, FFTComplex *z)
{
    s->fft_calc(s, z);
}
void ff_fft_end(FFTContext *s);



static inline void ff_imdct_calc(FFTContext *s, FFTSample *output, const FFTSample *input)
{
    s->imdct_calc(s, output, input);
}
static inline void ff_imdct_half(FFTContext *s, FFTSample *output, const FFTSample *input)
{
    s->imdct_half(s, output, input);
}

static inline void ff_mdct_calc(FFTContext *s, FFTSample *output,
                                const FFTSample *input)
{
    s->mdct_calc(s, output, input);
}


void ff_kbd_window_init(float *window, float alpha, int n);


void ff_sine_window_init(float *window, int n);


void ff_init_ff_sine_windows(int index);
extern SINETABLE(  32);
extern SINETABLE(  64);
extern SINETABLE( 128);
extern SINETABLE( 256);
extern SINETABLE( 512);
extern SINETABLE(1024);
extern SINETABLE(2048);
extern SINETABLE(4096);
extern SINETABLE_CONST float * const ff_sine_windows[13];

int ff_mdct_init(FFTContext *s, int nbits, int inverse, double scale);
void ff_imdct_calc_c(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_imdct_half_c(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_mdct_calc_c(FFTContext *s, FFTSample *output, const FFTSample *input);
void ff_mdct_end(FFTContext *s);



struct RDFTContext {
    int nbits;
    int inverse;
    int sign_convention;

    
    const FFTSample *tcos;
    SINTABLE_CONST FFTSample *tsin;
    FFTContext fft;
    void (*rdft_calc)(struct RDFTContext *s, FFTSample *z);
};


int ff_rdft_init(RDFTContext *s, int nbits, enum RDFTransformType trans);
void ff_rdft_end(RDFTContext *s);

void ff_rdft_init_arm(RDFTContext *s);

static av_always_inline void ff_rdft_calc(RDFTContext *s, FFTSample *data)
{
    s->rdft_calc(s, data);
}



struct DCTContext {
    int nbits;
    int inverse;
    RDFTContext rdft;
    const float *costab;
    FFTSample *csc2;
    void (*dct_calc)(struct DCTContext *s, FFTSample *data);
};


int  ff_dct_init(DCTContext *s, int nbits, enum DCTTransformType type);
void ff_dct_calc(DCTContext *s, FFTSample *data);
void ff_dct_end (DCTContext *s);

#endif 
