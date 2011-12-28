/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_RTPDEC_H
#define AVFORMAT_RTPDEC_H

#include "libavcodec/avcodec.h"
#include "avformat.h"
#include "rtp.h"


typedef struct rtp_payload_data
{
    int sizelength;
    int indexlength;
    int indexdeltalength;
    int profile_level_id;
    int streamtype;
    int objecttype;
    char *mode;

    
    struct AUHeaders {
        int size;
        int index;
        int cts_flag;
        int cts;
        int dts_flag;
        int dts;
        int rap_flag;
        int streamstate;
    } *au_headers;
    int au_headers_allocated;
    int nb_au_headers;
    int au_headers_length_bytes;
    int cur_au_index;
} RTPPayloadData;

typedef struct PayloadContext PayloadContext;
typedef struct RTPDynamicProtocolHandler_s RTPDynamicProtocolHandler;

#define RTP_MIN_PACKET_LENGTH 12
#define RTP_MAX_PACKET_LENGTH 1500 

typedef struct RTPDemuxContext RTPDemuxContext;
RTPDemuxContext *rtp_parse_open(AVFormatContext *s1, AVStream *st, URLContext *rtpc, int payload_type, RTPPayloadData *rtp_payload_data);
void rtp_parse_set_dynamic_protocol(RTPDemuxContext *s, PayloadContext *ctx,
                                    RTPDynamicProtocolHandler *handler);
int rtp_parse_packet(RTPDemuxContext *s, AVPacket *pkt,
                     const uint8_t *buf, int len);
void rtp_parse_close(RTPDemuxContext *s);
#if (LIBAVFORMAT_VERSION_MAJOR <= 53)
int rtp_get_local_port(URLContext *h);
#endif
int rtp_get_local_rtp_port(URLContext *h);
int rtp_get_local_rtcp_port(URLContext *h);

int rtp_set_remote_url(URLContext *h, const char *uri);
#if (LIBAVFORMAT_VERSION_MAJOR <= 52)
void rtp_get_file_handles(URLContext *h, int *prtp_fd, int *prtcp_fd);
#endif


void rtp_send_punch_packets(URLContext* rtp_handle);


int rtp_check_and_send_back_rr(RTPDemuxContext *s, int count);


typedef struct {
    uint16_t max_seq;           
    uint32_t cycles;            
    uint32_t base_seq;          
    uint32_t bad_seq;           
    int probation;              
    int received;               
    int expected_prior;         
    int received_prior;         
    uint32_t transit;           
    uint32_t jitter;            
} RTPStatistics;

#define RTP_FLAG_KEY    0x1 
#define RTP_FLAG_MARKER 0x2 

typedef int (*DynamicPayloadPacketHandlerProc) (AVFormatContext *ctx,
                                                PayloadContext *s,
                                                AVStream *st,
                                                AVPacket * pkt,
                                                uint32_t *timestamp,
                                                const uint8_t * buf,
                                                int len, int flags);

struct RTPDynamicProtocolHandler_s {
    
    const char enc_name[50];    
    enum AVMediaType codec_type;
    enum CodecID codec_id;

    
    int (*parse_sdp_a_line) (AVFormatContext *s,
                             int st_index,
                             PayloadContext *priv_data,
                             const char *line); 
    PayloadContext *(*open) (void); 
    void (*close)(PayloadContext *protocol_data); 
    DynamicPayloadPacketHandlerProc parse_packet; 

    struct RTPDynamicProtocolHandler_s *next;
};


struct RTPDemuxContext {
    AVFormatContext *ic;
    AVStream *st;
    int payload_type;
    uint32_t ssrc;
    uint16_t seq;
    uint32_t timestamp;
    uint32_t base_timestamp;
    uint32_t cur_timestamp;
    int64_t  range_start_offset;
    int max_payload_size;
    struct MpegTSContext *ts;   
    int read_buf_index;
    int read_buf_size;
    
    URLContext *rtp_ctx;
    char hostname[256];

    RTPStatistics statistics; 

    
    int64_t last_rtcp_ntp_time;    
    int64_t first_rtcp_ntp_time;   
    uint32_t last_rtcp_timestamp;  

    
    unsigned int packet_count;     
    unsigned int octet_count;      
    unsigned int last_octet_count; 
    int first_packet;
    
    uint8_t buf[RTP_MAX_PACKET_LENGTH];
    uint8_t *buf_ptr;

    
    RTPPayloadData *rtp_payload_data; 

    
    DynamicPayloadPacketHandlerProc parse_packet;     
    PayloadContext *dynamic_protocol_context;        
    int max_frames_per_packet;
};

extern RTPDynamicProtocolHandler *RTPFirstDynamicPayloadHandler;
void ff_register_dynamic_payload_handler(RTPDynamicProtocolHandler *handler);

int ff_rtsp_next_attr_and_value(const char **p, char *attr, int attr_size, char *value, int value_size); 

void av_register_rtp_dynamic_payload_handlers(void);

#endif 
