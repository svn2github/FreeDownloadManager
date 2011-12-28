/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_RM_H
#define AVFORMAT_RM_H

#include "avformat.h"

extern const char * const ff_rm_metadata[4];
extern const unsigned char ff_sipr_subpk_size[4];

typedef struct RMStream RMStream;

RMStream *ff_rm_alloc_rmstream (void);
void      ff_rm_free_rmstream  (RMStream *rms);


extern AVInputFormat rdt_demuxer;


int ff_rm_read_mdpr_codecdata (AVFormatContext *s, ByteIOContext *pb,
                               AVStream *st, RMStream *rst,
                               int codec_data_size);


int ff_rm_parse_packet (AVFormatContext *s, ByteIOContext *pb,
                        AVStream *st, RMStream *rst, int len,
                        AVPacket *pkt, int *seq, int flags, int64_t ts);


int ff_rm_retrieve_cache (AVFormatContext *s, ByteIOContext *pb,
                          AVStream *st, RMStream *rst, AVPacket *pkt);


void ff_rm_reorder_sipr_data(uint8_t *buf, int sub_packet_h, int framesize);

#endif 
