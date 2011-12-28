/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_IVI_COMMON_H
#define AVCODEC_IVI_COMMON_H

#include "avcodec.h"
#include "get_bits.h"
#include <stdint.h>

#define IVI_DEBUG 0

#define IVI_VLC_BITS 13 


typedef struct {
    int32_t     num_rows;
    uint8_t     xbits[16];
} IVIHuffDesc;


typedef struct {
    int32_t     tab_sel;    
                            
    VLC         *tab;       

    
    IVIHuffDesc cust_desc;  
    VLC         cust_tab;   
} IVIHuffTab;

enum {
    IVI_MB_HUFF   = 0,      
    IVI_BLK_HUFF  = 1       
};

extern VLC ff_ivi_mb_vlc_tabs [8]; 
extern VLC ff_ivi_blk_vlc_tabs[8]; 



typedef struct {
    uint8_t     eob_sym; 
    uint8_t     esc_sym; 
    uint8_t     runtab[256];
    int8_t      valtab[256];
} RVMapDesc;

extern const RVMapDesc ff_ivi_rvmap_tabs[9];



typedef struct {
    int16_t     xpos;
    int16_t     ypos;
    uint32_t    buf_offs; 
    uint8_t     type;     
    uint8_t     cbp;      
    uint8_t     q_delta;  
    int8_t      mv_x;     
    int8_t      mv_y;     
} IVIMbInfo;



typedef struct {
    int         xpos;
    int         ypos;
    int         width;
    int         height;
    int         is_empty;  
    int         data_size; 
    int         num_MBs;   
    IVIMbInfo   *mbs;      
    IVIMbInfo   *ref_mbs;  
} IVITile;



typedef struct {
    int             plane;          
    int             band_num;       
    int             width;
    int             height;
    const uint8_t   *data_ptr;      
    int             data_size;      
    int16_t         *buf;           
    int16_t         *ref_buf;       
    int16_t         *bufs[3];       
    int             pitch;          
    int             is_empty;       
    int             mb_size;        
    int             blk_size;       
    int             is_halfpel;     
    int             inherit_mv;     
    int             inherit_qdelta; 
    int             qdelta_present; 
    int             quant_mat;      
    int             glob_quant;     
    const uint8_t   *scan;          

    IVIHuffTab      blk_vlc;        

    uint16_t        *dequant_intra; 
    uint16_t        *dequant_inter; 
    int             num_corr;       
    uint8_t         corr[61*2];     
    int             rvmap_sel;      
    RVMapDesc       *rv_map;        
    int             num_tiles;      
    IVITile         *tiles;         
    void (*inv_transform)(const int32_t *in, int16_t *out, uint32_t pitch, const uint8_t *flags); 
    void (*dc_transform) (const int32_t *in, int16_t *out, uint32_t pitch, int blk_size);   
    int             is_2d_trans;    
    int32_t         checksum;       
    int             checksum_present;
    int             bufsize;        
    const uint8_t   *intra_base;    
    const uint8_t   *inter_base;    
    const uint8_t   *intra_scale;   
    const uint8_t   *inter_scale;   
} IVIBandDesc;



typedef struct {
    uint16_t    width;
    uint16_t    height;
    uint8_t     num_bands;  
    IVIBandDesc *bands;     
} IVIPlaneDesc;


typedef struct {
    uint16_t    pic_width;
    uint16_t    pic_height;
    uint16_t    chroma_width;
    uint16_t    chroma_height;
    uint16_t    tile_width;
    uint16_t    tile_height;
    uint8_t     luma_bands;
    uint8_t     chroma_bands;
} IVIPicConfig;


static inline int ivi_pic_config_cmp(IVIPicConfig *str1, IVIPicConfig *str2)
{
    return (str1->pic_width    != str2->pic_width    || str1->pic_height    != str2->pic_height    ||
            str1->chroma_width != str2->chroma_width || str1->chroma_height != str2->chroma_height ||
            str1->tile_width   != str2->tile_width   || str1->tile_height   != str2->tile_height   ||
            str1->luma_bands   != str2->luma_bands   || str1->chroma_bands  != str2->chroma_bands);
}


#define IVI_NUM_TILES(stride, tile_size) (((stride) + (tile_size) - 1) / (tile_size))


#define IVI_MBs_PER_TILE(tile_width, tile_height, mb_size) \
    ((((tile_width) + (mb_size) - 1) / (mb_size)) * (((tile_height) + (mb_size) - 1) / (mb_size)))


#define IVI_TOSIGNED(val) (-(((val) >> 1) ^ -((val) & 1)))


static inline int ivi_scale_mv(int mv, int mv_scale)
{
    return (mv + (mv > 0) + (mv_scale - 1)) >> mv_scale;
}


int  ff_ivi_create_huff_from_desc(const IVIHuffDesc *cb, VLC *vlc, int flag);


void ff_ivi_init_static_vlc(void);


int  ff_ivi_dec_huff_desc(GetBitContext *gb, int desc_coded, int which_tab,
                          IVIHuffTab *huff_tab, AVCodecContext *avctx);


int  ff_ivi_huff_desc_cmp(const IVIHuffDesc *desc1, const IVIHuffDesc *desc2);


void ff_ivi_huff_desc_copy(IVIHuffDesc *dst, const IVIHuffDesc *src);


int  ff_ivi_init_planes(IVIPlaneDesc *planes, const IVIPicConfig *cfg);


void ff_ivi_free_buffers(IVIPlaneDesc *planes);


int  ff_ivi_init_tiles(IVIPlaneDesc *planes, int tile_width, int tile_height);


int  ff_ivi_dec_tile_data_size(GetBitContext *gb);


int  ff_ivi_decode_blocks(GetBitContext *gb, IVIBandDesc *band, IVITile *tile);


void ff_ivi_process_empty_tile(AVCodecContext *avctx, IVIBandDesc *band,
                               IVITile *tile, int32_t mv_scale);


void ff_ivi_output_plane(IVIPlaneDesc *plane, uint8_t *dst, int dst_pitch);

#if IVI_DEBUG

uint16_t ivi_calc_band_checksum (IVIBandDesc *band);


int ivi_check_band (IVIBandDesc *band, const uint8_t *ref, int pitch);
#endif

#endif 
