/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_XVMC_H
#define AVCODEC_XVMC_H

#include <X11/extensions/XvMC.h>

#include "avcodec.h"

#if LIBAVCODEC_VERSION_MAJOR < 53
#define AV_XVMC_STATE_DISPLAY_PENDING          1  
#define AV_XVMC_STATE_PREDICTION               2  
#define AV_XVMC_STATE_OSD_SOURCE               4  
#endif
#define AV_XVMC_ID                    0x1DC711C0  

struct xvmc_pix_fmt {
    
    int             xvmc_id;

    
    short*          data_blocks;

    
    XvMCMacroBlock* mv_blocks;

    
    int             allocated_mv_blocks;

    
    int             allocated_data_blocks;

    
    int             idct;

    
    int             unsigned_intra;

    
    XvMCSurface*    p_surface;



    
    XvMCSurface*    p_past_surface;

    
    XvMCSurface*    p_future_surface;

    
    unsigned int    picture_structure;

    
    unsigned int    flags;


    
    int             start_mv_blocks_num;

    
    int             filled_mv_blocks_num;

    
    int             next_free_data_block_num;


#if LIBAVCODEC_VERSION_MAJOR < 53

    
    int             state;

    
    void*           p_osd_target_surface_render;

#endif
};

#endif 
