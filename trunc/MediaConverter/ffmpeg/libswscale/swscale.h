/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef SWSCALE_SWSCALE_H
#define SWSCALE_SWSCALE_H



#include "libavutil/avutil.h"

#define LIBSWSCALE_VERSION_MAJOR 0
#define LIBSWSCALE_VERSION_MINOR 10
#define LIBSWSCALE_VERSION_MICRO 0

#define LIBSWSCALE_VERSION_INT  AV_VERSION_INT(LIBSWSCALE_VERSION_MAJOR, \
                                               LIBSWSCALE_VERSION_MINOR, \
                                               LIBSWSCALE_VERSION_MICRO)
#define LIBSWSCALE_VERSION      AV_VERSION(LIBSWSCALE_VERSION_MAJOR, \
                                           LIBSWSCALE_VERSION_MINOR, \
                                           LIBSWSCALE_VERSION_MICRO)
#define LIBSWSCALE_BUILD        LIBSWSCALE_VERSION_INT

#define LIBSWSCALE_IDENT        "SwS" AV_STRINGIFY(LIBSWSCALE_VERSION)


unsigned swscale_version(void);


const char *swscale_configuration(void);


const char *swscale_license(void);


#define SWS_FAST_BILINEAR     1
#define SWS_BILINEAR          2
#define SWS_BICUBIC           4
#define SWS_X                 8
#define SWS_POINT          0x10
#define SWS_AREA           0x20
#define SWS_BICUBLIN       0x40
#define SWS_GAUSS          0x80
#define SWS_SINC          0x100
#define SWS_LANCZOS       0x200
#define SWS_SPLINE        0x400

#define SWS_SRC_V_CHR_DROP_MASK     0x30000
#define SWS_SRC_V_CHR_DROP_SHIFT    16

#define SWS_PARAM_DEFAULT           123456

#define SWS_PRINT_INFO              0x1000



#define SWS_FULL_CHR_H_INT    0x2000

#define SWS_FULL_CHR_H_INP    0x4000
#define SWS_DIRECT_BGR        0x8000
#define SWS_ACCURATE_RND      0x40000
#define SWS_BITEXACT          0x80000

#define SWS_CPU_CAPS_MMX      0x80000000
#define SWS_CPU_CAPS_MMX2     0x20000000
#define SWS_CPU_CAPS_3DNOW    0x40000000
#define SWS_CPU_CAPS_ALTIVEC  0x10000000
#define SWS_CPU_CAPS_BFIN     0x01000000

#define SWS_MAX_REDUCE_CUTOFF 0.002

#define SWS_CS_ITU709         1
#define SWS_CS_FCC            4
#define SWS_CS_ITU601         5
#define SWS_CS_ITU624         5
#define SWS_CS_SMPTE170M      5
#define SWS_CS_SMPTE240M      7
#define SWS_CS_DEFAULT        5


const int *sws_getCoefficients(int colorspace);




typedef struct {
    double *coeff;              
    int length;                 
} SwsVector;


typedef struct {
    SwsVector *lumH;
    SwsVector *lumV;
    SwsVector *chrH;
    SwsVector *chrV;
} SwsFilter;

struct SwsContext;


int sws_isSupportedInput(enum PixelFormat pix_fmt);


int sws_isSupportedOutput(enum PixelFormat pix_fmt);


void sws_freeContext(struct SwsContext *swsContext);


struct SwsContext *sws_getContext(int srcW, int srcH, enum PixelFormat srcFormat,
                                  int dstW, int dstH, enum PixelFormat dstFormat,
                                  int flags, SwsFilter *srcFilter,
                                  SwsFilter *dstFilter, const double *param);


int sws_scale(struct SwsContext *context, const uint8_t* const srcSlice[], const int srcStride[],
              int srcSliceY, int srcSliceH, uint8_t* const dst[], const int dstStride[]);
#if LIBSWSCALE_VERSION_MAJOR < 1

int sws_scale_ordered(struct SwsContext *context, const uint8_t* const src[],
                      int srcStride[], int srcSliceY, int srcSliceH,
                      uint8_t* dst[], int dstStride[]) attribute_deprecated;
#endif


int sws_setColorspaceDetails(struct SwsContext *c, const int inv_table[4],
                             int srcRange, const int table[4], int dstRange,
                             int brightness, int contrast, int saturation);


int sws_getColorspaceDetails(struct SwsContext *c, int **inv_table,
                             int *srcRange, int **table, int *dstRange,
                             int *brightness, int *contrast, int *saturation);


SwsVector *sws_allocVec(int length);


SwsVector *sws_getGaussianVec(double variance, double quality);


SwsVector *sws_getConstVec(double c, int length);


SwsVector *sws_getIdentityVec(void);


void sws_scaleVec(SwsVector *a, double scalar);


void sws_normalizeVec(SwsVector *a, double height);
void sws_convVec(SwsVector *a, SwsVector *b);
void sws_addVec(SwsVector *a, SwsVector *b);
void sws_subVec(SwsVector *a, SwsVector *b);
void sws_shiftVec(SwsVector *a, int shift);


SwsVector *sws_cloneVec(SwsVector *a);

#if LIBSWSCALE_VERSION_MAJOR < 1

attribute_deprecated void sws_printVec(SwsVector *a);
#endif


void sws_printVec2(SwsVector *a, AVClass *log_ctx, int log_level);

void sws_freeVec(SwsVector *a);

SwsFilter *sws_getDefaultFilter(float lumaGBlur, float chromaGBlur,
                                float lumaSharpen, float chromaSharpen,
                                float chromaHShift, float chromaVShift,
                                int verbose);
void sws_freeFilter(SwsFilter *filter);


struct SwsContext *sws_getCachedContext(struct SwsContext *context,
                                        int srcW, int srcH, enum PixelFormat srcFormat,
                                        int dstW, int dstH, enum PixelFormat dstFormat,
                                        int flags, SwsFilter *srcFilter,
                                        SwsFilter *dstFilter, const double *param);

#endif 
