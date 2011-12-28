/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_SEEK_H
#define AVFORMAT_SEEK_H

#include "avformat.h"


typedef struct AVParserStreamState {
    
    AVCodecParserContext   *parser;
    AVPacket                cur_pkt;
    int64_t                 last_IP_pts;
    int64_t                 cur_dts;
    int64_t                 reference_dts;
    const uint8_t          *cur_ptr;
    int                     cur_len;
    int                     probe_packets;
} AVParserStreamState;


typedef struct AVParserState {
    int64_t         fpos;                   

    
    AVStream       *cur_st;                 
    AVPacketList   *packet_buffer;          
    AVPacketList   *raw_packet_buffer;      
    int raw_packet_buffer_remaining_size;   

    
    int                  nb_streams;        
    AVParserStreamState *stream_states;     
} AVParserState;


int64_t ff_gen_syncpoint_search(AVFormatContext *s,
                                int stream_index,
                                int64_t pos,
                                int64_t min_ts,
                                int64_t ts,
                                int64_t max_ts,
                                int flags);


AVParserState *ff_store_parser_state(AVFormatContext *s);


void ff_restore_parser_state(AVFormatContext *s, AVParserState *state);


void ff_free_parser_state(AVFormatContext *s, AVParserState *state);

#endif 
