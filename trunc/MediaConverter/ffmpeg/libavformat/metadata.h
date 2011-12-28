/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_METADATA_H
#define AVFORMAT_METADATA_H




#include "avformat.h"

struct AVMetadata{
    int count;
    AVMetadataTag *elems;
};

struct AVMetadataConv{
    const char *native;
    const char *generic;
};

#if LIBAVFORMAT_VERSION_MAJOR < 53
void ff_metadata_demux_compat(AVFormatContext *s);
void ff_metadata_mux_compat(AVFormatContext *s);
#endif

void metadata_conv(AVMetadata **pm, const AVMetadataConv *d_conv,
                                    const AVMetadataConv *s_conv);

#endif 
