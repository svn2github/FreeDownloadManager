/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VP56_H
#define AVCODEC_VP56_H

#include "vp56data.h"
#include "dsputil.h"
#include "get_bits.h"
#include "bytestream.h"


typedef struct vp56_context VP56Context;
typedef struct vp56_mv VP56mv;

typedef void (*VP56ParseVectorAdjustment)(VP56Context *s,
                                          VP56mv *vect);
typedef int  (*VP56Adjust)(int v, int t);
typedef void (*VP56Filter)(VP56Context *s, uint8_t *dst, uint8_t *src,
                           int offset1, int offset2, int stride,
                           VP56mv mv, int mask, int select, int luma);
typedef void (*VP56ParseCoeff)(VP56Context *s);
typedef void (*VP56DefaultModelsInit)(VP56Context *s);
typedef void (*VP56ParseVectorModels)(VP56Context *s);
typedef void (*VP56ParseCoeffModels)(VP56Context *s);
typedef int  (*VP56ParseHeader)(VP56Context *s, const uint8_t *buf,
                                int buf_size, int *golden_frame);

typedef struct {
    int high;
    int bits;
    const uint8_t *buffer;
    const uint8_t *end;
    unsigned long code_word;
} VP56RangeCoder;

typedef struct {
    uint8_t not_null_dc;
    VP56Frame ref_frame;
    DCTELEM dc_coeff;
} VP56RefDc;

struct vp56_mv {
    int x;
    int y;
};

typedef struct {
    uint8_t type;
    VP56mv mv;
} VP56Macroblock;

typedef struct {
    uint8_t coeff_reorder[64];       
    uint8_t coeff_index_to_pos[64];  
    uint8_t vector_sig[2];           
    uint8_t vector_dct[2];           
    uint8_t vector_pdi[2][2];        
    uint8_t vector_pdv[2][7];        
    uint8_t vector_fdv[2][8];        
    uint8_t coeff_dccv[2][11];       
    uint8_t coeff_ract[2][3][6][11]; 
    uint8_t coeff_acct[2][3][3][6][5];
    uint8_t coeff_dcct[2][36][5];    
    uint8_t coeff_runv[2][14];       
    uint8_t mb_type[3][10][10];      
    uint8_t mb_types_stats[3][10][2];
} VP56Model;

struct vp56_context {
    AVCodecContext *avctx;
    DSPContext dsp;
    ScanTable scantable;
    AVFrame frames[4];
    AVFrame *framep[6];
    uint8_t *edge_emu_buffer_alloc;
    uint8_t *edge_emu_buffer;
    VP56RangeCoder c;
    VP56RangeCoder cc;
    VP56RangeCoder *ccp;
    int sub_version;

    
    int plane_width[4];
    int plane_height[4];
    int mb_width;   
    int mb_height;  
    int block_offset[6];

    int quantizer;
    uint16_t dequant_dc;
    uint16_t dequant_ac;
    int8_t *qscale_table;

    
    VP56RefDc *above_blocks;
    VP56RefDc left_block[4];
    int above_block_idx[6];
    DCTELEM prev_dc[3][3];    

    
    VP56mb mb_type;
    VP56Macroblock *macroblocks;
    DECLARE_ALIGNED(16, DCTELEM, block_coeff)[6][64];

    
    VP56mv mv[6];  
    VP56mv vector_candidate[2];
    int vector_candidate_pos;

    
    int filter_header;               
    int deblock_filtering;
    int filter_selection;
    int filter_mode;
    int max_vector_length;
    int sample_variance_threshold;

    uint8_t coeff_ctx[4][64];              
    uint8_t coeff_ctx_last[4];             

    int has_alpha;

    
    int flip;  
    int frbi;  
    int srbi;  
    int stride[4];  

    const uint8_t *vp56_coord_div;
    VP56ParseVectorAdjustment parse_vector_adjustment;
    VP56Adjust adjust;
    VP56Filter filter;
    VP56ParseCoeff parse_coeff;
    VP56DefaultModelsInit default_models_init;
    VP56ParseVectorModels parse_vector_models;
    VP56ParseCoeffModels parse_coeff_models;
    VP56ParseHeader parse_header;

    VP56Model *modelp;
    VP56Model models[2];

    
    int use_huffman;
    GetBitContext gb;
    VLC dccv_vlc[2];
    VLC runv_vlc[2];
    VLC ract_vlc[2][3][6];
    unsigned int nb_null[2][2];       
};


void vp56_init(AVCodecContext *avctx, int flip, int has_alpha);
int vp56_free(AVCodecContext *avctx);
void vp56_init_dequant(VP56Context *s, int quantizer);
int vp56_decode_frame(AVCodecContext *avctx, void *data, int *data_size,
                      AVPacket *avpkt);




static inline void vp56_init_range_decoder(VP56RangeCoder *c,
                                           const uint8_t *buf, int buf_size)
{
    c->high = 255;
    c->bits = 8;
    c->buffer = buf;
    c->end = buf + buf_size;
    c->code_word = bytestream_get_be16(&c->buffer);
}

static inline int vp56_rac_get_prob(VP56RangeCoder *c, uint8_t prob)
{
    unsigned int low = 1 + (((c->high - 1) * prob) / 256);
    unsigned int low_shift = low << 8;
    int bit = c->code_word >= low_shift;

    if (bit) {
        c->high -= low;
        c->code_word -= low_shift;
    } else {
        c->high = low;
    }

    
    while (c->high < 128) {
        c->high <<= 1;
        c->code_word <<= 1;
        if (--c->bits == 0 && c->buffer < c->end) {
            c->bits = 8;
            c->code_word |= *c->buffer++;
        }
    }
    return bit;
}

static inline int vp56_rac_get(VP56RangeCoder *c)
{
    
    int low = (c->high + 1) >> 1;
    unsigned int low_shift = low << 8;
    int bit = c->code_word >= low_shift;
    if (bit) {
        c->high = (c->high - low) << 1;
        c->code_word -= low_shift;
    } else {
        c->high = low << 1;
    }

    
    c->code_word <<= 1;
    if (--c->bits == 0 && c->buffer < c->end) {
        c->bits = 8;
        c->code_word |= *c->buffer++;
    }
    return bit;
}

static inline int vp56_rac_gets(VP56RangeCoder *c, int bits)
{
    int value = 0;

    while (bits--) {
        value = (value << 1) | vp56_rac_get(c);
    }

    return value;
}

static inline int vp56_rac_gets_nn(VP56RangeCoder *c, int bits)
{
    int v = vp56_rac_gets(c, 7) << 1;
    return v + !v;
}

static inline int vp56_rac_get_tree(VP56RangeCoder *c,
                                    const VP56Tree *tree,
                                    const uint8_t *probs)
{
    while (tree->val > 0) {
        if (vp56_rac_get_prob(c, probs[tree->prob_idx]))
            tree += tree->val;
        else
            tree++;
    }
    return -tree->val;
}

#endif 
