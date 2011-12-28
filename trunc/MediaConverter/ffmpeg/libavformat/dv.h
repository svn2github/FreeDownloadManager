/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_DV_H
#define AVFORMAT_DV_H

#include "avformat.h"

typedef struct DVDemuxContext DVDemuxContext;
DVDemuxContext* dv_init_demux(AVFormatContext* s);
int dv_get_packet(DVDemuxContext*, AVPacket *);
int dv_produce_packet(DVDemuxContext*, AVPacket*, uint8_t*, int);
void dv_offset_reset(DVDemuxContext *c, int64_t frame_offset);

typedef struct DVMuxContext DVMuxContext;
DVMuxContext* dv_init_mux(AVFormatContext* s);
int dv_assemble_frame(DVMuxContext *c, AVStream*, uint8_t*, int, uint8_t**);
void dv_delete_mux(DVMuxContext*);

#endif 
