/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_RTPENC_H
#define AVFORMAT_RTPENC_H

#include "avformat.h"
#include "rtp.h"

struct RTPMuxContext {
    AVFormatContext *ic;
    AVStream *st;
    int payload_type;
    uint32_t ssrc;
    uint16_t seq;
    uint32_t timestamp;
    uint32_t base_timestamp;
    uint32_t cur_timestamp;
    int max_payload_size;
    int num_frames;

    
    int64_t last_rtcp_ntp_time;    
    int64_t first_rtcp_ntp_time;   

    
    unsigned int packet_count;     
    unsigned int octet_count;      
    unsigned int last_octet_count; 
    int first_packet;
    
    uint8_t *buf;
    uint8_t *buf_ptr;

    int max_frames_per_packet;
};

typedef struct RTPMuxContext RTPMuxContext;

void ff_rtp_send_data(AVFormatContext *s1, const uint8_t *buf1, int len, int m);

void ff_rtp_send_h264(AVFormatContext *s1, const uint8_t *buf1, int size);
void ff_rtp_send_h263(AVFormatContext *s1, const uint8_t *buf1, int size);
void ff_rtp_send_aac(AVFormatContext *s1, const uint8_t *buff, int size);
void ff_rtp_send_amr(AVFormatContext *s1, const uint8_t *buff, int size);
void ff_rtp_send_mpegvideo(AVFormatContext *s1, const uint8_t *buf1, int size);

#endif 
