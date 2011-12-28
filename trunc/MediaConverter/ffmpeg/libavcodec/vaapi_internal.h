/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_VAAPI_INTERNAL_H
#define AVCODEC_VAAPI_INTERNAL_H

#include <va/va.h>
#include "vaapi.h"
#include "avcodec.h"
#include "mpegvideo.h"




static inline VASurfaceID ff_vaapi_get_surface_id(Picture *pic)
{
    return (uintptr_t)pic->data[3];
}


int ff_vaapi_common_end_frame(MpegEncContext *s);


void *ff_vaapi_alloc_pic_param(struct vaapi_context *vactx, unsigned int size);


void *ff_vaapi_alloc_iq_matrix(struct vaapi_context *vactx, unsigned int size);


uint8_t *ff_vaapi_alloc_bitplane(struct vaapi_context *vactx, uint32_t size);


VASliceParameterBufferBase *ff_vaapi_alloc_slice(struct vaapi_context *vactx, const uint8_t *buffer, uint32_t size);



#endif 
