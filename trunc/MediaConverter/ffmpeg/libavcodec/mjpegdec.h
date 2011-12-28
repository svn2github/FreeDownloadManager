/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_MJPEGDEC_H
#define AVCODEC_MJPEGDEC_H

#include "avcodec.h"
#include "get_bits.h"
#include "dsputil.h"

#define MAX_COMPONENTS 4

typedef struct MJpegDecodeContext {
    AVCodecContext *avctx;
    GetBitContext gb;

    int start_code; 
    int buffer_size;
    uint8_t *buffer;

    int16_t quant_matrixes[4][64];
    VLC vlcs[2][4];
    int qscale[4];      

    int org_height;  
    int first_picture;    
    int interlaced;     
    int bottom_field;   
    int lossless;
    int ls;
    int progressive;
    int rgb;
    int rct;            
    int pegasus_rct;    
    int bits;           

    int maxval;
    int near;         
    int t1,t2,t3;
    int reset;        

    int width, height;
    int mb_width, mb_height;
    int nb_components;
    int block_stride[MAX_COMPONENTS];
    int component_id[MAX_COMPONENTS];
    int h_count[MAX_COMPONENTS]; 
    int v_count[MAX_COMPONENTS];
    int comp_index[MAX_COMPONENTS];
    int dc_index[MAX_COMPONENTS];
    int ac_index[MAX_COMPONENTS];
    int nb_blocks[MAX_COMPONENTS];
    int h_scount[MAX_COMPONENTS];
    int v_scount[MAX_COMPONENTS];
    int h_max, v_max; 
    int quant_index[4];   
    int last_dc[MAX_COMPONENTS]; 
    AVFrame picture; 
    int got_picture;                                
    int linesize[MAX_COMPONENTS];                   
    int8_t *qscale_table;
    DECLARE_ALIGNED(16, DCTELEM, block)[64];
    DCTELEM (*blocks[MAX_COMPONENTS])[64]; 
    uint8_t *last_nnz[MAX_COMPONENTS];
    uint64_t coefs_finished[MAX_COMPONENTS]; 
    ScanTable scantable;
    DSPContext dsp;

    int restart_interval;
    int restart_count;

    int buggy_avid;
    int cs_itu601;
    int interlace_polarity;

    int mjpb_skiptosod;

    int cur_scan; 
    int flipped; 

    uint16_t (*ljpeg_buffer)[4];
    unsigned int ljpeg_buffer_size;
} MJpegDecodeContext;

int ff_mjpeg_decode_init(AVCodecContext *avctx);
int ff_mjpeg_decode_end(AVCodecContext *avctx);
int ff_mjpeg_decode_frame(AVCodecContext *avctx,
                          void *data, int *data_size,
                          AVPacket *avpkt);
int ff_mjpeg_decode_dqt(MJpegDecodeContext *s);
int ff_mjpeg_decode_dht(MJpegDecodeContext *s);
int ff_mjpeg_decode_sof(MJpegDecodeContext *s);
int ff_mjpeg_decode_sos(MJpegDecodeContext *s);

#endif 
