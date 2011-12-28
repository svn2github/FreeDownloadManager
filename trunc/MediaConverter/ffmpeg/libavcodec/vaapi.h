/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VAAPI_H
#define AVCODEC_VAAPI_H

#include <stdint.h>




struct vaapi_context {
    
    void *display;

    
    uint32_t config_id;

    
    uint32_t context_id;

    
    uint32_t pic_param_buf_id;

    
    uint32_t iq_matrix_buf_id;

    
    uint32_t bitplane_buf_id;

    
    uint32_t *slice_buf_ids;

    
    unsigned int n_slice_buf_ids;

    
    unsigned int slice_buf_ids_alloc;

    
    void *slice_params;

    
    unsigned int slice_param_size;

    
    unsigned int slice_params_alloc;

    
    unsigned int slice_count;

    
    const uint8_t *slice_data;

    
    uint32_t slice_data_size;
};



#endif 
