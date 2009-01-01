/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#ifndef AVCODEC_H
#define AVCODEC_H      

#ifdef __cplusplus
extern "C" {
#endif

#include "avutil.h"
#include <sys/types.h> 

#define AV_STRINGIFY(s)         AV_TOSTRING(s)
#define AV_TOSTRING(s) #s

#define LIBAVCODEC_VERSION_INT  ((51<<16)+(29<<8)+0)
#define LIBAVCODEC_VERSION      51.29.0
#define LIBAVCODEC_BUILD        LIBAVCODEC_VERSION_INT

#define LIBAVCODEC_IDENT        "Lavc" AV_STRINGIFY(LIBAVCODEC_VERSION)

#define AV_NOPTS_VALUE          INT64_C(0x8000000000000000)
#define AV_TIME_BASE            1000000
#define AV_TIME_BASE_Q          (AVRational){1, AV_TIME_BASE}

enum CodecID {
    CODEC_ID_NONE,
    CODEC_ID_MPEG1VIDEO,
    CODEC_ID_MPEG2VIDEO, 
    CODEC_ID_MPEG2VIDEO_XVMC,
    CODEC_ID_H261,
    CODEC_ID_H263,
    CODEC_ID_RV10,
    CODEC_ID_RV20,
    CODEC_ID_MJPEG,
    CODEC_ID_MJPEGB,
    CODEC_ID_LJPEG,
    CODEC_ID_SP5X,
    CODEC_ID_JPEGLS,
    CODEC_ID_MPEG4,
    CODEC_ID_RAWVIDEO,
    CODEC_ID_MSMPEG4V1,
    CODEC_ID_MSMPEG4V2,
    CODEC_ID_MSMPEG4V3,
    CODEC_ID_WMV1,
    CODEC_ID_WMV2,
    CODEC_ID_H263P,
    CODEC_ID_H263I,
    CODEC_ID_FLV1,
    CODEC_ID_SVQ1,
    CODEC_ID_SVQ3,
    CODEC_ID_DVVIDEO,
    CODEC_ID_HUFFYUV,
    CODEC_ID_CYUV,
    CODEC_ID_H264,
    CODEC_ID_INDEO3,
    CODEC_ID_VP3,
    CODEC_ID_THEORA,
    CODEC_ID_ASV1,
    CODEC_ID_ASV2,
    CODEC_ID_FFV1,
    CODEC_ID_4XM,
    CODEC_ID_VCR1,
    CODEC_ID_CLJR,
    CODEC_ID_MDEC,
    CODEC_ID_ROQ,
    CODEC_ID_INTERPLAY_VIDEO,
    CODEC_ID_XAN_WC3,
    CODEC_ID_XAN_WC4,
    CODEC_ID_RPZA,
    CODEC_ID_CINEPAK,
    CODEC_ID_WS_VQA,
    CODEC_ID_MSRLE,
    CODEC_ID_MSVIDEO1,
    CODEC_ID_IDCIN,
    CODEC_ID_8BPS,
    CODEC_ID_SMC,
    CODEC_ID_FLIC,
    CODEC_ID_TRUEMOTION1,
    CODEC_ID_VMDVIDEO,
    CODEC_ID_MSZH,
    CODEC_ID_ZLIB,
    CODEC_ID_QTRLE,
    CODEC_ID_SNOW,
    CODEC_ID_TSCC,
    CODEC_ID_ULTI,
    CODEC_ID_QDRAW,
    CODEC_ID_VIXL,
    CODEC_ID_QPEG,
    CODEC_ID_XVID,
    CODEC_ID_PNG,
    CODEC_ID_PPM,
    CODEC_ID_PBM,
    CODEC_ID_PGM,
    CODEC_ID_PGMYUV,
    CODEC_ID_PAM,
    CODEC_ID_FFVHUFF,
    CODEC_ID_RV30,
    CODEC_ID_RV40,
    CODEC_ID_VC1,
    CODEC_ID_WMV3,
    CODEC_ID_LOCO,
    CODEC_ID_WNV1,
    CODEC_ID_AASC,
    CODEC_ID_INDEO2,
    CODEC_ID_FRAPS,
    CODEC_ID_TRUEMOTION2,
    CODEC_ID_BMP,
    CODEC_ID_CSCD,
    CODEC_ID_MMVIDEO,
    CODEC_ID_ZMBV,
    CODEC_ID_AVS,
    CODEC_ID_SMACKVIDEO,
    CODEC_ID_NUV,
    CODEC_ID_KMVC,
    CODEC_ID_FLASHSV,
    CODEC_ID_CAVS,
    CODEC_ID_JPEG2000,
    CODEC_ID_VMNC,
    CODEC_ID_VP5,
    CODEC_ID_VP6,
    CODEC_ID_VP6F,
    CODEC_ID_TARGA,
    CODEC_ID_DSICINVIDEO,
    CODEC_ID_TIERTEXSEQVIDEO,
    CODEC_ID_TIFF,
    CODEC_ID_GIF,
    CODEC_ID_FFH264,

    
    CODEC_ID_PCM_S16LE= 0x10000,
    CODEC_ID_PCM_S16BE,
    CODEC_ID_PCM_U16LE,
    CODEC_ID_PCM_U16BE,
    CODEC_ID_PCM_S8,
    CODEC_ID_PCM_U8,
    CODEC_ID_PCM_MULAW,
    CODEC_ID_PCM_ALAW,
    CODEC_ID_PCM_S32LE,
    CODEC_ID_PCM_S32BE,
    CODEC_ID_PCM_U32LE,
    CODEC_ID_PCM_U32BE,
    CODEC_ID_PCM_S24LE,
    CODEC_ID_PCM_S24BE,
    CODEC_ID_PCM_U24LE,
    CODEC_ID_PCM_U24BE,
    CODEC_ID_PCM_S24DAUD,

    
    CODEC_ID_ADPCM_IMA_QT= 0x11000,
    CODEC_ID_ADPCM_IMA_WAV,
    CODEC_ID_ADPCM_IMA_DK3,
    CODEC_ID_ADPCM_IMA_DK4,
    CODEC_ID_ADPCM_IMA_WS,
    CODEC_ID_ADPCM_IMA_SMJPEG,
    CODEC_ID_ADPCM_MS,
    CODEC_ID_ADPCM_4XM,
    CODEC_ID_ADPCM_XA,
    CODEC_ID_ADPCM_ADX,
    CODEC_ID_ADPCM_EA,
    CODEC_ID_ADPCM_G726,
    CODEC_ID_ADPCM_CT,
    CODEC_ID_ADPCM_SWF,
    CODEC_ID_ADPCM_YAMAHA,
    CODEC_ID_ADPCM_SBPRO_4,
    CODEC_ID_ADPCM_SBPRO_3,
    CODEC_ID_ADPCM_SBPRO_2,

    
    CODEC_ID_AMR_NB= 0x12000,
    CODEC_ID_AMR_WB,

    
    CODEC_ID_RA_144= 0x13000,
    CODEC_ID_RA_288,

    
    CODEC_ID_ROQ_DPCM= 0x14000,
    CODEC_ID_INTERPLAY_DPCM,
    CODEC_ID_XAN_DPCM,
    CODEC_ID_SOL_DPCM,

    CODEC_ID_MP2= 0x15000,
    CODEC_ID_MP3, 
    CODEC_ID_AAC,
#if LIBAVCODEC_VERSION_INT < ((52<<16)+(0<<8)+0)
    CODEC_ID_MPEG4AAC,
#endif
    CODEC_ID_AC3,
    CODEC_ID_DTS,
    CODEC_ID_VORBIS,
    CODEC_ID_DVAUDIO,
    CODEC_ID_WMAV1,
    CODEC_ID_WMAV2,
    CODEC_ID_MACE3,
    CODEC_ID_MACE6,
    CODEC_ID_VMDAUDIO,
    CODEC_ID_SONIC,
    CODEC_ID_SONIC_LS,
    CODEC_ID_FLAC,
    CODEC_ID_MP3ADU,
    CODEC_ID_MP3ON4,
    CODEC_ID_SHORTEN,
    CODEC_ID_ALAC,
    CODEC_ID_WESTWOOD_SND1,
    CODEC_ID_GSM,
    CODEC_ID_QDM2,
    CODEC_ID_COOK,
    CODEC_ID_TRUESPEECH,
    CODEC_ID_TTA,
    CODEC_ID_SMACKAUDIO,
    CODEC_ID_QCELP,
    CODEC_ID_WAVPACK,
    CODEC_ID_DSICINAUDIO,
    CODEC_ID_IMC,
    CODEC_ID_MUSEPACK7,

    
    CODEC_ID_DVD_SUBTITLE= 0x17000,
    CODEC_ID_DVB_SUBTITLE,

    CODEC_ID_MPEG2TS= 0x20000, 
};  

#define CODEC_ID_MP3LAME CODEC_ID_MP3

enum CodecType {
    CODEC_TYPE_UNKNOWN = -1,
    CODEC_TYPE_VIDEO,
    CODEC_TYPE_AUDIO,
    CODEC_TYPE_DATA,
    CODEC_TYPE_SUBTITLE,
};    

enum SampleFormat {
    SAMPLE_FMT_NONE = -1,
    SAMPLE_FMT_U8,              
    SAMPLE_FMT_S16,             
    SAMPLE_FMT_S24,             
    SAMPLE_FMT_S32,             
    SAMPLE_FMT_FLT,             
};  

#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000   

#define FF_INPUT_BUFFER_PADDING_SIZE 8  

#define FF_MIN_BUFFER_SIZE 16384  

enum Motion_Est_ID {
    ME_ZERO = 1,
    ME_FULL,
    ME_LOG,
    ME_PHODS,
    ME_EPZS,
    ME_X1,
    ME_HEX,
    ME_UMH,
    ME_ITER,
};

enum AVDiscard{

    AVDISCARD_NONE   =-16, 
    AVDISCARD_DEFAULT=  0, 
    AVDISCARD_NONREF =  8, 
    AVDISCARD_BIDIR  = 16, 
    AVDISCARD_NONKEY = 32, 
    AVDISCARD_ALL    = 48, 
};

typedef struct RcOverride{
    int start_frame;
    int end_frame;
    int qscale; 
    float quality_factor;
} RcOverride;

#define FF_MAX_B_FRAMES 16    

#define CODEC_FLAG_QSCALE 0x0002  
#define CODEC_FLAG_4MV    0x0004  
#define CODEC_FLAG_QPEL   0x0010  
#define CODEC_FLAG_GMC    0x0020  
#define CODEC_FLAG_MV0    0x0040  
#define CODEC_FLAG_PART   0x0080  

#define CODEC_FLAG_INPUT_PRESERVED 0x0100
#define CODEC_FLAG_PASS1 0x0200   
#define CODEC_FLAG_PASS2 0x0400   
#define CODEC_FLAG_EXTERN_HUFF 0x1000 
#define CODEC_FLAG_GRAY  0x2000   
#define CODEC_FLAG_EMU_EDGE 0x4000
#define CODEC_FLAG_PSNR           0x8000 
#define CODEC_FLAG_TRUNCATED  0x00010000 
#define CODEC_FLAG_NORMALIZE_AQP  0x00020000 
#define CODEC_FLAG_INTERLACED_DCT 0x00040000 
#define CODEC_FLAG_LOW_DELAY      0x00080000 
#define CODEC_FLAG_ALT_SCAN       0x00100000 
#define CODEC_FLAG_TRELLIS_QUANT  0x00200000 
#define CODEC_FLAG_GLOBAL_HEADER  0x00400000 
#define CODEC_FLAG_BITEXACT       0x00800000 

#define CODEC_FLAG_H263P_AIC      0x01000000 
#define CODEC_FLAG_AC_PRED        0x01000000 
#define CODEC_FLAG_H263P_UMV      0x02000000 
#define CODEC_FLAG_CBP_RD         0x04000000 
#define CODEC_FLAG_QP_RD          0x08000000 
#define CODEC_FLAG_H263P_AIV      0x00000008 
#define CODEC_FLAG_OBMC           0x00000001 
#define CODEC_FLAG_LOOP_FILTER    0x00000800 
#define CODEC_FLAG_H263P_SLICE_STRUCT 0x10000000
#define CODEC_FLAG_INTERLACED_ME  0x20000000 
#define CODEC_FLAG_SVCD_SCAN_OFFSET 0x40000000 
#define CODEC_FLAG_CLOSED_GOP     ((int)0x80000000)
#define CODEC_FLAG2_FAST          0x00000001 
#define CODEC_FLAG2_STRICT_GOP    0x00000002 
#define CODEC_FLAG2_NO_OUTPUT     0x00000004 
#define CODEC_FLAG2_LOCAL_HEADER  0x00000008 
#define CODEC_FLAG2_BPYRAMID      0x00000010 
#define CODEC_FLAG2_WPRED         0x00000020 
#define CODEC_FLAG2_MIXED_REFS    0x00000040 
#define CODEC_FLAG2_8X8DCT        0x00000080 
#define CODEC_FLAG2_FASTPSKIP     0x00000100 
#define CODEC_FLAG2_AUD           0x00000200 
#define CODEC_FLAG2_BRDO          0x00000400 
#define CODEC_FLAG2_INTRA_VLC     0x00000800 
#define CODEC_FLAG2_MEMC_ONLY     0x00001000 
#define CODEC_FLAG2_DROP_FRAME_TIMECODE 0x00002000 
#define CODEC_FLAG2_SKIP_RD       0x00004000         

#define CODEC_CAP_DRAW_HORIZ_BAND 0x0001 

#define CODEC_CAP_DR1             0x0002

#define CODEC_CAP_PARSE_ONLY      0x0004
#define CODEC_CAP_TRUNCATED       0x0008

#define CODEC_CAP_HWACCEL         0x0010

#define CODEC_CAP_DELAY           0x0020

#define CODEC_CAP_SMALL_LAST_FRAME 0x0040  

#define MB_TYPE_INTRA4x4   0x0001
#define MB_TYPE_INTRA16x16 0x0002 
#define MB_TYPE_INTRA_PCM  0x0004 
#define MB_TYPE_16x16      0x0008
#define MB_TYPE_16x8       0x0010
#define MB_TYPE_8x16       0x0020
#define MB_TYPE_8x8        0x0040
#define MB_TYPE_INTERLACED 0x0080
#define MB_TYPE_DIRECT2     0x0100 
#define MB_TYPE_ACPRED     0x0200
#define MB_TYPE_GMC        0x0400
#define MB_TYPE_SKIP       0x0800
#define MB_TYPE_P0L0       0x1000
#define MB_TYPE_P1L0       0x2000
#define MB_TYPE_P0L1       0x4000
#define MB_TYPE_P1L1       0x8000
#define MB_TYPE_L0         (MB_TYPE_P0L0 | MB_TYPE_P1L0)
#define MB_TYPE_L1         (MB_TYPE_P0L1 | MB_TYPE_P1L1)
#define MB_TYPE_L0L1       (MB_TYPE_L0   | MB_TYPE_L1)
#define MB_TYPE_QUANT      0x00010000
#define MB_TYPE_CBP        0x00020000    

typedef struct AVPanScan{
    
    int id;

    
    int width;
    int height;

    
    int16_t position[3][2];
}AVPanScan;

#define FF_COMMON_FRAME \
    \
    uint8_t *data[4];\
    int linesize[4];\
    \
    uint8_t *base[4];\
    \
    int key_frame;\
\
    \
    int pict_type;\
\
    \
    int64_t pts;\
\
    \
    int coded_picture_number;\
    \
    int display_picture_number;\
\
    \
    int quality; \
\
    \
    int age;\
\
    \
    int reference;\
\
    \
    int8_t *qscale_table;\
    \
    int qstride;\
\
    \
    uint8_t *mbskip_table;\
\
    \
    int16_t (*motion_val[2])[2];\
\
    \
    uint32_t *mb_type;\
\
    \
    uint8_t motion_subsample_log2;\
\
    \
    void *opaque;\
\
    \
    uint64_t error[4];\
\
    \
    int type;\
    \
    \
    int repeat_pict;\
    \
    \
    int qscale_type;\
    \
    \
    int interlaced_frame;\
    \
    \
    int top_field_first;\
    \
    \
    AVPanScan *pan_scan;\
    \
    \
    int palette_has_changed;\
    \
    \
    int buffer_hints;\
\
    \
    short *dct_coeff;\
\
    \
    int8_t *ref_index[2];

#define FF_QSCALE_TYPE_MPEG1 0
#define FF_QSCALE_TYPE_MPEG2 1
#define FF_QSCALE_TYPE_H264  2

#define FF_BUFFER_TYPE_INTERNAL 1
#define FF_BUFFER_TYPE_USER     2 
#define FF_BUFFER_TYPE_SHARED   4 
#define FF_BUFFER_TYPE_COPY     8   

#define FF_I_TYPE 1 
#define FF_P_TYPE 2 
#define FF_B_TYPE 3 
#define FF_S_TYPE 4 
#define FF_SI_TYPE 5
#define FF_SP_TYPE 6

#define FF_BUFFER_HINTS_VALID    0x01 
#define FF_BUFFER_HINTS_READABLE 0x02 
#define FF_BUFFER_HINTS_PRESERVE 0x04 
#define FF_BUFFER_HINTS_REUSABLE 0x08   

typedef struct AVFrame {
    FF_COMMON_FRAME
} AVFrame;

#define DEFAULT_FRAME_RATE_BASE 1001000  

typedef struct AVCodecContext {
    
    AVClass *av_class;
    
    int bit_rate;

    
    int bit_rate_tolerance;

    
    int flags;

    
    int sub_id;

    
    int me_method;

    
    uint8_t *extradata;
    int extradata_size;

    
    AVRational time_base;

    
    
    int width, height;

#define FF_ASPECT_EXTENDED 15

    
    int gop_size;

    
    enum PixelFormat pix_fmt;

    
    int rate_emu;

    
    void (*draw_horiz_band)(struct AVCodecContext *s,
                            const AVFrame *src, int offset[4],
                            int y, int type, int height);

    
    int sample_rate; 
    int channels;

    
    enum SampleFormat sample_fmt;  

    
    
    int frame_size;
    int frame_number;   
    int real_pict_num;  

    
    int delay;

    
    float qcompress;  
    float qblur;      

    
    int qmin;

    
    int qmax;

    
    int max_qdiff;

    
    int max_b_frames;

    
    float b_quant_factor;

    
    int rc_strategy;
#define FF_RC_STRATEGY_XVID 1

    int b_frame_strategy;

    
    int hurry_up;

    struct AVCodec *codec;

    void *priv_data;

    
    int rtp_mode;

    int rtp_payload_size;   
                            
                            
                            
                            
                              

    
    
    
    
    
    
    void (*rtp_callback)(struct AVCodecContext *avctx, void *data, int size, int mb_nb);

    
    int mv_bits;
    int header_bits;
    int i_tex_bits;
    int p_tex_bits;
    int i_count;
    int p_count;
    int skip_count;
    int misc_bits;

    
    int frame_bits;

    
    void *opaque;

    char codec_name[32];
    enum CodecType codec_type; 
    enum CodecID codec_id; 

    
    unsigned int codec_tag;

    
    int workaround_bugs;
#define FF_BUG_AUTODETECT       1  
#define FF_BUG_OLD_MSMPEG4      2
#define FF_BUG_XVID_ILACE       4
#define FF_BUG_UMP4             8
#define FF_BUG_NO_PADDING       16
#define FF_BUG_AMV              32
#define FF_BUG_AC_VLC           0  
#define FF_BUG_QPEL_CHROMA      64
#define FF_BUG_STD_QPEL         128
#define FF_BUG_QPEL_CHROMA2     256
#define FF_BUG_DIRECT_BLOCKSIZE 512
#define FF_BUG_EDGE             1024
#define FF_BUG_HPEL_CHROMA      2048
#define FF_BUG_DC_CLIP          4096
#define FF_BUG_MS               8192   

    
    int luma_elim_threshold;

    
    int chroma_elim_threshold;

    
    int strict_std_compliance;
#define FF_COMPLIANCE_VERY_STRICT   2 
#define FF_COMPLIANCE_STRICT        1 
#define FF_COMPLIANCE_NORMAL        0
#define FF_COMPLIANCE_INOFFICIAL   -1 
#define FF_COMPLIANCE_EXPERIMENTAL -2 

    
    float b_quant_offset;

    
    int error_resilience;
#define FF_ER_CAREFUL         1
#define FF_ER_COMPLIANT       2
#define FF_ER_AGGRESSIVE      3
#define FF_ER_VERY_AGGRESSIVE 4

    
    int (*get_buffer)(struct AVCodecContext *c, AVFrame *pic);

    
    void (*release_buffer)(struct AVCodecContext *c, AVFrame *pic);

    
    int has_b_frames;

    
    int block_align;

    int parse_only; 

    
    int mpeg_quant;

    
    char *stats_out;

    
    char *stats_in;

    
    float rc_qsquish;

    float rc_qmod_amp;
    int rc_qmod_freq;

    
    RcOverride *rc_override;
    int rc_override_count;

    
    char *rc_eq;

    
    int rc_max_rate;

    
    int rc_min_rate;

    
    int rc_buffer_size;
    float rc_buffer_aggressivity;

    
    float i_quant_factor;

    
    float i_quant_offset;

    
    float rc_initial_cplx;

    
    int dct_algo;
#define FF_DCT_AUTO    0
#define FF_DCT_FASTINT 1
#define FF_DCT_INT     2
#define FF_DCT_MMX     3
#define FF_DCT_MLIB    4
#define FF_DCT_ALTIVEC 5
#define FF_DCT_FAAN    6

    
    float lumi_masking;

    
    float temporal_cplx_masking;

    
    float spatial_cplx_masking;

    
    float p_masking;

    
    float dark_masking;  

    
    int unused;

    
    int idct_algo;
#define FF_IDCT_AUTO         0
#define FF_IDCT_INT          1
#define FF_IDCT_SIMPLE       2
#define FF_IDCT_SIMPLEMMX    3
#define FF_IDCT_LIBMPEG2MMX  4
#define FF_IDCT_PS2          5
#define FF_IDCT_MLIB         6
#define FF_IDCT_ARM          7
#define FF_IDCT_ALTIVEC      8
#define FF_IDCT_SH4          9
#define FF_IDCT_SIMPLEARM    10
#define FF_IDCT_H264         11
#define FF_IDCT_VP3          12
#define FF_IDCT_IPP          13
#define FF_IDCT_XVIDMMX      14
#define FF_IDCT_CAVS         15
#define FF_IDCT_SIMPLEARMV5TE 16
#define FF_IDCT_SIMPLEARMV6  17

    
    int slice_count;
    
    int *slice_offset;

    
    int error_concealment;
#define FF_EC_GUESS_MVS   1
#define FF_EC_DEBLOCK     2

    
    unsigned dsp_mask;
#define FF_MM_FORCE    0x80000000 
    
#ifdef HAVE_MMX
#define FF_MM_MMX      0x0001 
#define FF_MM_3DNOW    0x0004 
#define FF_MM_MMXEXT   0x0002 
#define FF_MM_SSE      0x0008 
#define FF_MM_SSE2     0x0010 
#define FF_MM_3DNOWEXT 0x0020 
#endif 
#ifdef HAVE_IWMMXT
#define FF_MM_IWMMXT   0x0100 
#endif 

    
     int bits_per_sample;

    
     int prediction_method;
#define FF_PRED_LEFT   0
#define FF_PRED_PLANE  1
#define FF_PRED_MEDIAN 2

    
    AVRational sample_aspect_ratio;

    
    AVFrame *coded_frame;

    
    int debug;
#define FF_DEBUG_PICT_INFO 1
#define FF_DEBUG_RC        2
#define FF_DEBUG_BITSTREAM 4
#define FF_DEBUG_MB_TYPE   8
#define FF_DEBUG_QP        16
#define FF_DEBUG_MV        32
#define FF_DEBUG_DCT_COEFF 0x00000040
#define FF_DEBUG_SKIP      0x00000080
#define FF_DEBUG_STARTCODE 0x00000100
#define FF_DEBUG_PTS       0x00000200
#define FF_DEBUG_ER        0x00000400
#define FF_DEBUG_MMCO      0x00000800
#define FF_DEBUG_BUGS      0x00001000
#define FF_DEBUG_VIS_QP    0x00002000
#define FF_DEBUG_VIS_MB_TYPE 0x00004000

    
    int debug_mv;
#define FF_DEBUG_VIS_MV_P_FOR  0x00000001 
#define FF_DEBUG_VIS_MV_B_FOR  0x00000002 
#define FF_DEBUG_VIS_MV_B_BACK 0x00000004 

    
    uint64_t error[4];

    
    int mb_qmin;

    
    int mb_qmax;

    
    int me_cmp;
    
    int me_sub_cmp;
    
    int mb_cmp;
    
    int ildct_cmp;
#define FF_CMP_SAD  0
#define FF_CMP_SSE  1
#define FF_CMP_SATD 2
#define FF_CMP_DCT  3
#define FF_CMP_PSNR 4
#define FF_CMP_BIT  5
#define FF_CMP_RD   6
#define FF_CMP_ZERO 7
#define FF_CMP_VSAD 8
#define FF_CMP_VSSE 9
#define FF_CMP_NSSE 10
#define FF_CMP_W53  11
#define FF_CMP_W97  12
#define FF_CMP_DCTMAX 13
#define FF_CMP_DCT264 14
#define FF_CMP_CHROMA 256

    
    int dia_size;

    
    int last_predictor_count;

    
    int pre_me;

    
    int me_pre_cmp;

    
    int pre_dia_size;

    
    int me_subpel_quality;

    
    enum PixelFormat (*get_format)(struct AVCodecContext *s, const enum PixelFormat * fmt);

    
    int dtg_active_format;
#define FF_DTG_AFD_SAME         8
#define FF_DTG_AFD_4_3          9
#define FF_DTG_AFD_16_9         10
#define FF_DTG_AFD_14_9         11
#define FF_DTG_AFD_4_3_SP_14_9  13
#define FF_DTG_AFD_16_9_SP_14_9 14
#define FF_DTG_AFD_SP_4_3       15

    
    int me_range;

    
    int intra_quant_bias;
#define FF_DEFAULT_QUANT_BIAS 999999

    
    int inter_quant_bias;

    
    int color_table_id;

    
    int internal_buffer_count;

    
    void *internal_buffer;

#define FF_LAMBDA_SHIFT 7
#define FF_LAMBDA_SCALE (1<<FF_LAMBDA_SHIFT)
#define FF_QP2LAMBDA 118 
#define FF_LAMBDA_MAX (256*128-1)

#define FF_QUALITY_SCALE FF_LAMBDA_SCALE 
    
    int global_quality;

#define FF_CODER_TYPE_VLC   0
#define FF_CODER_TYPE_AC    1
    
    int coder_type;

    
    int context_model;
#if 0
    
    uint8_t * (*realloc)(struct AVCodecContext *s, uint8_t *buf, int buf_size);
#endif

    
    int slice_flags;
#define SLICE_FLAG_CODED_ORDER    0x0001 
#define SLICE_FLAG_ALLOW_FIELD    0x0002 
#define SLICE_FLAG_ALLOW_PLANE    0x0004 

    
    int xvmc_acceleration;

    
    int mb_decision;
#define FF_MB_DECISION_SIMPLE 0        
#define FF_MB_DECISION_BITS   1        
#define FF_MB_DECISION_RD     2        

    
    uint16_t *intra_matrix;

    
    uint16_t *inter_matrix;

    
    unsigned int stream_codec_tag;

    
    int scenechange_threshold;

    
    int lmin;

    
    int lmax;

    
    struct AVPaletteControl *palctrl;

    
    int noise_reduction;

    
    int (*reget_buffer)(struct AVCodecContext *c, AVFrame *pic);

    
    int rc_initial_buffer_occupancy;

    
    int inter_threshold;

    
    int flags2;

    
    int error_rate;

    
    int antialias_algo;
#define FF_AA_AUTO    0
#define FF_AA_FASTINT 1 
#define FF_AA_INT     2
#define FF_AA_FLOAT   3
    
    int quantizer_noise_shaping;

    
    int thread_count;

    
    int (*execute)(struct AVCodecContext *c, int (*func)(struct AVCodecContext *c2, void *arg), void **arg2, int *ret, int count);

    
    void *thread_opaque;

    
     int me_threshold;

    
     int mb_threshold;

    
     int intra_dc_precision;

    
     int nsse_weight;

    
     int skip_top;

    
     int skip_bottom;

    
     int profile;
#define FF_PROFILE_UNKNOWN -99

    
     int level;
#define FF_LEVEL_UNKNOWN -99

    
     int lowres;

    
    int coded_width, coded_height;

    
    int frame_skip_threshold;

    
    int frame_skip_factor;

    
    int frame_skip_exp;

    
    int frame_skip_cmp;

    
    float border_masking;

    
    int mb_lmin;

    
    int mb_lmax;

    
    int me_penalty_compensation;

    
    enum AVDiscard skip_loop_filter;

    
    enum AVDiscard skip_idct;

    
    enum AVDiscard skip_frame;

    
    int bidir_refine;

    
    int brd_scale;

    
    float crf;

    
    int cqp;

    
    int keyint_min;

    
    int refs;

    
    int chromaoffset;

    
    int bframebias;

    
    int trellis;

    
    float complexityblur;

    
    int deblockalpha;

    
    int deblockbeta;

    
    int partitions;
#define X264_PART_I4X4 0x001  
#define X264_PART_I8X8 0x002  
#define X264_PART_P8X8 0x010  
#define X264_PART_P4X4 0x020  
#define X264_PART_B8X8 0x100  

    
    int directpred;

    
    int cutoff;

    
    int scenechange_factor;

    
    int mv0_threshold;

    
    int b_sensitivity;

    
    int compression_level;
#define FF_COMPRESSION_DEFAULT -1

    
    int use_lpc;

    
    int lpc_coeff_precision;

    
    int min_prediction_order;

    
    int max_prediction_order;

    
    int prediction_order_method;

    
    int min_partition_order;

    
    int max_partition_order;

    
    int64_t timecode_frame_start;
} AVCodecContext;  

typedef struct AVCodec {
    const char *name;
    enum CodecType type;
    enum CodecID id;
    int priv_data_size;
    int (*init)(AVCodecContext *);
    int (*encode)(AVCodecContext *, uint8_t *buf, int buf_size, void *data);
    int (*close)(AVCodecContext *);
    int (*decode)(AVCodecContext *, void *outdata, int *outdata_size,
                  uint8_t *buf, int buf_size);
    int capabilities;
    struct AVCodec *next;
    void (*flush)(AVCodecContext *);
    const AVRational *supported_framerates; 
    const enum PixelFormat *pix_fmts;       
} AVCodec;  

typedef struct AVPicture {
    uint8_t *data[4];
    int linesize[4];       
} AVPicture;  

#define AVPALETTE_SIZE 1024
#define AVPALETTE_COUNT 256
typedef struct AVPaletteControl {

    
    int palette_changed;

    
    unsigned int palette[AVPALETTE_COUNT];

} AVPaletteControl attribute_deprecated;

typedef struct AVSubtitleRect {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint16_t nb_colors;
    int linesize;
    uint32_t *rgba_palette;
    uint8_t *bitmap;
} AVSubtitleRect;

typedef struct AVSubtitle {
    uint16_t format; 
    uint32_t start_display_time; 
    uint32_t end_display_time; 
    uint32_t num_rects;
    AVSubtitleRect *rects;
} AVSubtitle;

extern AVCodec ac3_encoder;
extern AVCodec amr_nb_encoder;
extern AVCodec amr_wb_encoder;
extern AVCodec asv1_encoder;
extern AVCodec asv2_encoder;
extern AVCodec dvvideo_encoder;
extern AVCodec faac_encoder;
extern AVCodec ffv1_encoder;
extern AVCodec ffvhuff_encoder;
extern AVCodec flac_encoder;
extern AVCodec flashsv_encoder;
extern AVCodec flv_encoder;
extern AVCodec gif_encoder;
extern AVCodec h261_encoder;
extern AVCodec h263_encoder;
extern AVCodec h263p_encoder;
extern AVCodec h264_encoder;
extern AVCodec huffyuv_encoder;
extern AVCodec jpegls_encoder;
extern AVCodec libgsm_encoder;
extern AVCodec libtheora_encoder;
extern AVCodec ljpeg_encoder;
extern AVCodec mdec_encoder;
extern AVCodec mjpeg_encoder;
extern AVCodec mp2_encoder;
extern AVCodec mp3lame_encoder;
extern AVCodec mpeg1video_encoder;
extern AVCodec mpeg2video_encoder;
extern AVCodec mpeg4_encoder;
extern AVCodec msmpeg4v1_encoder;
extern AVCodec msmpeg4v2_encoder;
extern AVCodec msmpeg4v3_encoder;
extern AVCodec oggvorbis_encoder;
extern AVCodec pam_encoder;
extern AVCodec pbm_encoder;
extern AVCodec pgm_encoder;
extern AVCodec pgmyuv_encoder;
extern AVCodec png_encoder;
extern AVCodec ppm_encoder;
extern AVCodec rv10_encoder;
extern AVCodec rv20_encoder;
extern AVCodec snow_encoder;
extern AVCodec sonic_encoder;
extern AVCodec sonic_ls_encoder;
extern AVCodec svq1_encoder;
extern AVCodec vcr1_encoder;
extern AVCodec vorbis_encoder;
extern AVCodec wmv1_encoder;
extern AVCodec wmv2_encoder;
extern AVCodec x264_encoder;
extern AVCodec xvid_encoder;
extern AVCodec zlib_encoder;
extern AVCodec zmbv_encoder;

extern AVCodec aac_decoder;
extern AVCodec aasc_decoder;
extern AVCodec alac_decoder;
extern AVCodec amr_nb_decoder;
extern AVCodec amr_wb_decoder;
extern AVCodec asv1_decoder;
extern AVCodec asv2_decoder;
extern AVCodec avs_decoder;
extern AVCodec bmp_decoder;
extern AVCodec cavs_decoder;
extern AVCodec cinepak_decoder;
extern AVCodec cljr_decoder;
extern AVCodec cook_decoder;
extern AVCodec cscd_decoder;
extern AVCodec cyuv_decoder;
extern AVCodec dsicinaudio_decoder;
extern AVCodec dsicinvideo_decoder;
extern AVCodec dvvideo_decoder;
extern AVCodec eightbps_decoder;
extern AVCodec ffv1_decoder;
extern AVCodec ffvhuff_decoder;
extern AVCodec flac_decoder;
extern AVCodec flashsv_decoder;
extern AVCodec flic_decoder;
extern AVCodec flv_decoder;
extern AVCodec fourxm_decoder;
extern AVCodec fraps_decoder;
extern AVCodec gif_decoder;
extern AVCodec h261_decoder;
extern AVCodec h263_decoder;
extern AVCodec h263i_decoder;
extern AVCodec h264_decoder;
extern AVCodec huffyuv_decoder;
extern AVCodec idcin_decoder;
extern AVCodec imc_decoder;
extern AVCodec indeo2_decoder;
extern AVCodec indeo3_decoder;
extern AVCodec interplay_dpcm_decoder;
extern AVCodec interplay_video_decoder;
extern AVCodec kmvc_decoder;
extern AVCodec libgsm_decoder;
extern AVCodec loco_decoder;
extern AVCodec mace3_decoder;
extern AVCodec mace6_decoder;
extern AVCodec mdec_decoder;
extern AVCodec mjpeg_decoder;
extern AVCodec mjpegb_decoder;
extern AVCodec mmvideo_decoder;
extern AVCodec mp2_decoder;
extern AVCodec mp3_decoder;
extern AVCodec mp3adu_decoder;
extern AVCodec mp3on4_decoder;
extern AVCodec mpc7_decoder;
extern AVCodec mpeg1video_decoder;
extern AVCodec mpeg2video_decoder;
extern AVCodec mpeg4_decoder;
extern AVCodec mpeg4aac_decoder;
extern AVCodec mpeg_xvmc_decoder;
extern AVCodec mpegvideo_decoder;
extern AVCodec msmpeg4v1_decoder;
extern AVCodec msmpeg4v2_decoder;
extern AVCodec msmpeg4v3_decoder;
extern AVCodec msrle_decoder;
extern AVCodec msvideo1_decoder;
extern AVCodec mszh_decoder;
extern AVCodec nuv_decoder;
extern AVCodec oggvorbis_decoder;
extern AVCodec png_decoder;
extern AVCodec qdm2_decoder;
extern AVCodec qdraw_decoder;
extern AVCodec qpeg_decoder;
extern AVCodec qtrle_decoder;
extern AVCodec ra_144_decoder;
extern AVCodec ra_288_decoder;
extern AVCodec roq_decoder;
extern AVCodec roq_dpcm_decoder;
extern AVCodec rpza_decoder;
extern AVCodec rv10_decoder;
extern AVCodec rv20_decoder;
extern AVCodec rv30_decoder;
extern AVCodec rv40_decoder;
extern AVCodec shorten_decoder;
extern AVCodec smackaud_decoder;
extern AVCodec smacker_decoder;
extern AVCodec smc_decoder;
extern AVCodec snow_decoder;
extern AVCodec sol_dpcm_decoder;
extern AVCodec sonic_decoder;
extern AVCodec sp5x_decoder;
extern AVCodec svq1_decoder;
extern AVCodec svq3_decoder;
extern AVCodec targa_decoder;
extern AVCodec theora_decoder;
extern AVCodec tiertexseqvideo_decoder;
extern AVCodec tiff_decoder;
extern AVCodec truemotion1_decoder;
extern AVCodec truemotion2_decoder;
extern AVCodec truespeech_decoder;
extern AVCodec tscc_decoder;
extern AVCodec tta_decoder;
extern AVCodec ulti_decoder;
extern AVCodec vc1_decoder;
extern AVCodec vcr1_decoder;
extern AVCodec vmdaudio_decoder;
extern AVCodec vmdvideo_decoder;
extern AVCodec vmnc_decoder;
extern AVCodec vorbis_decoder;
extern AVCodec vp3_decoder;
extern AVCodec vp5_decoder;
extern AVCodec vp6_decoder;
extern AVCodec vp6f_decoder;
extern AVCodec vqa_decoder;
extern AVCodec wavpack_decoder;
extern AVCodec wmav1_decoder;
extern AVCodec wmav2_decoder;
extern AVCodec wmv1_decoder;
extern AVCodec wmv2_decoder;
extern AVCodec wmv3_decoder;
extern AVCodec wnv1_decoder;
extern AVCodec ws_snd1_decoder;
extern AVCodec xan_dpcm_decoder;
extern AVCodec xan_wc3_decoder;
extern AVCodec xl_decoder;
extern AVCodec zlib_decoder;
extern AVCodec zmbv_decoder;  

#define PCM_CODEC(id, name) \
extern AVCodec name ## _decoder; \
extern AVCodec name ## _encoder

PCM_CODEC(CODEC_ID_PCM_ALAW,    pcm_alaw);
PCM_CODEC(CODEC_ID_PCM_MULAW,   pcm_mulaw);
PCM_CODEC(CODEC_ID_PCM_S8,      pcm_s8);
PCM_CODEC(CODEC_ID_PCM_S16BE,   pcm_s16be);
PCM_CODEC(CODEC_ID_PCM_S16LE,   pcm_s16le);
PCM_CODEC(CODEC_ID_PCM_S24BE,   pcm_s24be);
PCM_CODEC(CODEC_ID_PCM_S24DAUD, pcm_s24daud);
PCM_CODEC(CODEC_ID_PCM_S24LE,   pcm_s24le);
PCM_CODEC(CODEC_ID_PCM_S32BE,   pcm_s32be);
PCM_CODEC(CODEC_ID_PCM_S32LE,   pcm_s32le);
PCM_CODEC(CODEC_ID_PCM_U8,      pcm_u8);
PCM_CODEC(CODEC_ID_PCM_U16BE,   pcm_u16be);
PCM_CODEC(CODEC_ID_PCM_U16LE,   pcm_u16le);
PCM_CODEC(CODEC_ID_PCM_U24BE,   pcm_u24be);
PCM_CODEC(CODEC_ID_PCM_U24LE,   pcm_u24le);
PCM_CODEC(CODEC_ID_PCM_U32BE,   pcm_u32be);
PCM_CODEC(CODEC_ID_PCM_U32LE,   pcm_u32le);    

PCM_CODEC(CODEC_ID_ADPCM_4XM,     adpcm_4xm);
PCM_CODEC(CODEC_ID_ADPCM_ADX,     adpcm_adx);
PCM_CODEC(CODEC_ID_ADPCM_CT,      adpcm_ct);
PCM_CODEC(CODEC_ID_ADPCM_EA,      adpcm_ea);
PCM_CODEC(CODEC_ID_ADPCM_G726,    adpcm_g726);
PCM_CODEC(CODEC_ID_ADPCM_IMA_DK3, adpcm_ima_dk3);
PCM_CODEC(CODEC_ID_ADPCM_IMA_DK4, adpcm_ima_dk4);
PCM_CODEC(CODEC_ID_ADPCM_IMA_QT,  adpcm_ima_qt);
PCM_CODEC(CODEC_ID_ADPCM_IMA_WAV, adpcm_ima_wav);
PCM_CODEC(CODEC_ID_ADPCM_IMA_WS,  adpcm_ima_ws);
PCM_CODEC(CODEC_ID_ADPCM_MS,      adpcm_ms);
PCM_CODEC(CODEC_ID_ADPCM_SBPRO_2, adpcm_sbpro_2);
PCM_CODEC(CODEC_ID_ADPCM_SBPRO_3, adpcm_sbpro_3);
PCM_CODEC(CODEC_ID_ADPCM_SBPRO_4, adpcm_sbpro_4);
PCM_CODEC(CODEC_ID_ADPCM_SMJPEG,  adpcm_ima_smjpeg);
PCM_CODEC(CODEC_ID_ADPCM_SWF,     adpcm_swf);
PCM_CODEC(CODEC_ID_ADPCM_XA,      adpcm_xa);
PCM_CODEC(CODEC_ID_ADPCM_YAMAHA,  adpcm_yamaha);

#undef PCM_CODEC  

extern AVCodec rawvideo_decoder;
extern AVCodec rawvideo_encoder;  

extern AVCodec ac3_decoder;
extern AVCodec dts_decoder;  

extern AVCodec dvbsub_decoder;
extern AVCodec dvbsub_encoder;
extern AVCodec dvdsub_decoder;
extern AVCodec dvdsub_encoder;    

struct ReSampleContext;
struct AVResampleContext;

typedef struct ReSampleContext ReSampleContext;

ReSampleContext *audio_resample_init(int output_channels, int input_channels,
                                     int output_rate, int input_rate);
int audio_resample(ReSampleContext *s, short *output, short *input, int nb_samples);
void audio_resample_close(ReSampleContext *s);

struct AVResampleContext *av_resample_init(int out_rate, int in_rate, int filter_length, int log2_phase_count, int linear, double cutoff);
int av_resample(struct AVResampleContext *c, short *dst, short *src, int *consumed, int src_size, int dst_size, int update_ctx);
void av_resample_compensate(struct AVResampleContext *c, int sample_delta, int compensation_distance);
void av_resample_close(struct AVResampleContext *c);    

struct ImgReSampleContext;

typedef struct ImgReSampleContext ImgReSampleContext;

ImgReSampleContext *img_resample_init(int output_width, int output_height,
                                      int input_width, int input_height);

ImgReSampleContext *img_resample_full_init(int owidth, int oheight,
                                      int iwidth, int iheight,
                                      int topBand, int bottomBand,
                                      int leftBand, int rightBand,
                                      int padtop, int padbottom,
                                      int padleft, int padright);  

void img_resample(ImgReSampleContext *s,
                  AVPicture *output, const AVPicture *input);

void img_resample_close(ImgReSampleContext *s);  

int avpicture_alloc(AVPicture *picture, int pix_fmt, int width, int height);  

void avpicture_free(AVPicture *picture);

int avpicture_fill(AVPicture *picture, uint8_t *ptr,
                   int pix_fmt, int width, int height);
int avpicture_layout(const AVPicture* src, int pix_fmt, int width, int height,
                     unsigned char *dest, int dest_size);
int avpicture_get_size(int pix_fmt, int width, int height);
void avcodec_get_chroma_sub_sample(int pix_fmt, int *h_shift, int *v_shift);
const char *avcodec_get_pix_fmt_name(int pix_fmt);
void avcodec_set_dimensions(AVCodecContext *s, int width, int height);
enum PixelFormat avcodec_get_pix_fmt(const char* name);
unsigned int avcodec_pix_fmt_to_codec_tag(enum PixelFormat p);

#define FF_LOSS_RESOLUTION  0x0001 
#define FF_LOSS_DEPTH       0x0002 
#define FF_LOSS_COLORSPACE  0x0004 
#define FF_LOSS_ALPHA       0x0008 
#define FF_LOSS_COLORQUANT  0x0010 
#define FF_LOSS_CHROMA      0x0020 

int avcodec_get_pix_fmt_loss(int dst_pix_fmt, int src_pix_fmt,
                             int has_alpha);
int avcodec_find_best_pix_fmt(int pix_fmt_mask, int src_pix_fmt,
                              int has_alpha, int *loss_ptr);

#define FF_ALPHA_TRANSP       0x0001 
#define FF_ALPHA_SEMI_TRANSP  0x0002 
int img_get_alpha_info(const AVPicture *src,
                       int pix_fmt, int width, int height);  

int img_convert(AVPicture *dst, int dst_pix_fmt,
                const AVPicture *src, int pix_fmt,
                int width, int height);  

int avpicture_deinterlace(AVPicture *dst, const AVPicture *src,
                          int pix_fmt, int width, int height);    

extern AVCodec *first_avcodec;  

unsigned avcodec_version(void);

unsigned avcodec_build(void);
void avcodec_init(void);

void register_avcodec(AVCodec *format);
AVCodec *avcodec_find_encoder(enum CodecID id);
AVCodec *avcodec_find_encoder_by_name(const char *name);
AVCodec *avcodec_find_decoder(enum CodecID id);
AVCodec *avcodec_find_decoder_by_name(const char *name);
void avcodec_string(char *buf, int buf_size, AVCodecContext *enc, int encode);

void avcodec_get_context_defaults(AVCodecContext *s);
AVCodecContext *avcodec_alloc_context(void);
void avcodec_get_frame_defaults(AVFrame *pic);
AVFrame *avcodec_alloc_frame(void);

int avcodec_default_get_buffer(AVCodecContext *s, AVFrame *pic);
void avcodec_default_release_buffer(AVCodecContext *s, AVFrame *pic);
int avcodec_default_reget_buffer(AVCodecContext *s, AVFrame *pic);
void avcodec_align_dimensions(AVCodecContext *s, int *width, int *height);
int avcodec_check_dimensions(void *av_log_ctx, unsigned int w, unsigned int h);
enum PixelFormat avcodec_default_get_format(struct AVCodecContext *s, const enum PixelFormat * fmt);

int avcodec_thread_init(AVCodecContext *s, int thread_count);
void avcodec_thread_free(AVCodecContext *s);
int avcodec_thread_execute(AVCodecContext *s, int (*func)(AVCodecContext *c2, void *arg2),void **arg, int *ret, int count);
int avcodec_default_execute(AVCodecContext *c, int (*func)(AVCodecContext *c2, void *arg2),void **arg, int *ret, int count);    

int avcodec_open(AVCodecContext *avctx, AVCodec *codec);  

attribute_deprecated int avcodec_decode_audio(AVCodecContext *avctx, int16_t *samples,
                         int *frame_size_ptr,
                         uint8_t *buf, int buf_size);

int avcodec_decode_audio2(AVCodecContext *avctx, int16_t *samples,
                         int *frame_size_ptr,
                         uint8_t *buf, int buf_size);
int avcodec_decode_video(AVCodecContext *avctx, AVFrame *picture,
                         int *got_picture_ptr,
                         uint8_t *buf, int buf_size);
int avcodec_decode_subtitle(AVCodecContext *avctx, AVSubtitle *sub,
                            int *got_sub_ptr,
                            const uint8_t *buf, int buf_size);
int avcodec_parse_frame(AVCodecContext *avctx, uint8_t **pdata,
                        int *data_size_ptr,
                        uint8_t *buf, int buf_size);
int avcodec_encode_audio(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                         const short *samples);
int avcodec_encode_video(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                         const AVFrame *pict);
int avcodec_encode_subtitle(AVCodecContext *avctx, uint8_t *buf, int buf_size,
                            const AVSubtitle *sub);

int avcodec_close(AVCodecContext *avctx);

void avcodec_register_all(void);

void avcodec_flush_buffers(AVCodecContext *avctx);

void avcodec_default_free_buffers(AVCodecContext *s);      

char av_get_pict_type_char(int pict_type);  

int av_get_bits_per_sample(enum CodecID codec_id);  

typedef struct AVCodecParserContext {
    void *priv_data;
    struct AVCodecParser *parser;
    int64_t frame_offset; 
    int64_t cur_offset; 
    int64_t last_frame_offset; 
    
    int pict_type; 
    int repeat_pict; 
    int64_t pts;     
    int64_t dts;     

    
    int64_t last_pts;
    int64_t last_dts;
    int fetch_timestamp;

#define AV_PARSER_PTS_NB 4
    int cur_frame_start_index;
    int64_t cur_frame_offset[AV_PARSER_PTS_NB];
    int64_t cur_frame_pts[AV_PARSER_PTS_NB];
    int64_t cur_frame_dts[AV_PARSER_PTS_NB];

    int flags;
#define PARSER_FLAG_COMPLETE_FRAMES           0x0001
} AVCodecParserContext;

typedef struct AVCodecParser {
    int codec_ids[5]; 
    int priv_data_size;
    int (*parser_init)(AVCodecParserContext *s);
    int (*parser_parse)(AVCodecParserContext *s,
                        AVCodecContext *avctx,
                        uint8_t **poutbuf, int *poutbuf_size,
                        const uint8_t *buf, int buf_size);
    void (*parser_close)(AVCodecParserContext *s);
    int (*split)(AVCodecContext *avctx, const uint8_t *buf, int buf_size);
    struct AVCodecParser *next;
} AVCodecParser;

extern AVCodecParser *av_first_parser;

void av_register_codec_parser(AVCodecParser *parser);
AVCodecParserContext *av_parser_init(int codec_id);
int av_parser_parse(AVCodecParserContext *s,
                    AVCodecContext *avctx,
                    uint8_t **poutbuf, int *poutbuf_size,
                    const uint8_t *buf, int buf_size,
                    int64_t pts, int64_t dts);
int av_parser_change(AVCodecParserContext *s,
                     AVCodecContext *avctx,
                     uint8_t **poutbuf, int *poutbuf_size,
                     const uint8_t *buf, int buf_size, int keyframe);
void av_parser_close(AVCodecParserContext *s);

extern AVCodecParser aac_parser;
extern AVCodecParser ac3_parser;
extern AVCodecParser cavsvideo_parser;
extern AVCodecParser dvbsub_parser;
extern AVCodecParser dvdsub_parser;
extern AVCodecParser h261_parser;
extern AVCodecParser h263_parser;
extern AVCodecParser h264_parser;
extern AVCodecParser mjpeg_parser;
extern AVCodecParser mpeg4video_parser;
extern AVCodecParser mpegaudio_parser;
extern AVCodecParser mpegvideo_parser;
extern AVCodecParser pnm_parser;  

typedef struct AVBitStreamFilterContext {
    void *priv_data;
    struct AVBitStreamFilter *filter;
    AVCodecParserContext *parser;
    struct AVBitStreamFilterContext *next;
} AVBitStreamFilterContext;  

typedef struct AVBitStreamFilter {
    const char *name;
    int priv_data_size;
    int (*filter)(AVBitStreamFilterContext *bsfc,
                  AVCodecContext *avctx, const char *args,
                  uint8_t **poutbuf, int *poutbuf_size,
                  const uint8_t *buf, int buf_size, int keyframe);
    struct AVBitStreamFilter *next;
} AVBitStreamFilter;

extern AVBitStreamFilter *av_first_bitstream_filter;

void av_register_bitstream_filter(AVBitStreamFilter *bsf);
AVBitStreamFilterContext *av_bitstream_filter_init(const char *name);
int av_bitstream_filter_filter(AVBitStreamFilterContext *bsfc,
                               AVCodecContext *avctx, const char *args,
                               uint8_t **poutbuf, int *poutbuf_size,
                               const uint8_t *buf, int buf_size, int keyframe);
void av_bitstream_filter_close(AVBitStreamFilterContext *bsf);

extern AVBitStreamFilter dump_extradata_bsf;
extern AVBitStreamFilter remove_extradata_bsf;
extern AVBitStreamFilter noise_bsf;
extern AVBitStreamFilter mp3_header_compress_bsf;
extern AVBitStreamFilter mp3_header_decompress_bsf;
extern AVBitStreamFilter mjpega_dump_header_bsf;    

void *av_fast_realloc(void *ptr, unsigned int *size, unsigned int min_size);  

void av_free_static(void);
void *av_mallocz_static(unsigned int size);
void *av_realloc_static(void *ptr, unsigned int size);

void img_copy(AVPicture *dst, const AVPicture *src,
              int pix_fmt, int width, int height);

int img_crop(AVPicture *dst, const AVPicture *src,
             int pix_fmt, int top_band, int left_band);

int img_pad(AVPicture *dst, const AVPicture *src, int height, int width, int pix_fmt,
            int padtop, int padbottom, int padleft, int padright, int *color);

extern unsigned int av_xiphlacing(unsigned char *s, unsigned int v);

#ifdef __cplusplus
}
#endif

#endif 
