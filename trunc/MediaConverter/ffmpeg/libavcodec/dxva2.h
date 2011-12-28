/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DXVA_H
#define AVCODEC_DXVA_H

#include <stdint.h>

#include <dxva2api.h>


struct dxva_context {
    
    IDirectXVideoDecoder *decoder;

    
    const DXVA2_ConfigPictureDecode *cfg;

    
    unsigned surface_count;

    
    LPDIRECT3DSURFACE9 *surface;

    
    uint64_t workaround;

    
    unsigned report_id;
};

#endif 
