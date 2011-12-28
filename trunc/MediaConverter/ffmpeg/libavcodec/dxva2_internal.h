/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_DXVA_INTERNAL_H
#define AVCODEC_DXVA_INTERNAL_H

#include "dxva2.h"
#include "avcodec.h"
#include "mpegvideo.h"

void *ff_dxva2_get_surface(const Picture *picture);

unsigned ff_dxva2_get_surface_index(const struct dxva_context *,
                                    const Picture *picture);

int ff_dxva2_commit_buffer(AVCodecContext *, struct dxva_context *,
                           DXVA2_DecodeBufferDesc *,
                           unsigned type, const void *data, unsigned size,
                           unsigned mb_count);


int ff_dxva2_common_end_frame(AVCodecContext *, MpegEncContext *,
                              const void *pp, unsigned pp_size,
                              const void *qm, unsigned qm_size,
                              int (*commit_bs_si)(AVCodecContext *,
                                                  DXVA2_DecodeBufferDesc *bs,
                                                  DXVA2_DecodeBufferDesc *slice));

#endif 
