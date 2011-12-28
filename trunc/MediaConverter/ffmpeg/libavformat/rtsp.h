/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_RTSP_H
#define AVFORMAT_RTSP_H

#include <stdint.h>
#include "avformat.h"
#include "rtspcodes.h"
#include "rtpdec.h"
#include "network.h"
#include "httpauth.h"


enum RTSPLowerTransport {
    RTSP_LOWER_TRANSPORT_UDP = 0,           
    RTSP_LOWER_TRANSPORT_TCP = 1,           
    RTSP_LOWER_TRANSPORT_UDP_MULTICAST = 2, 
    RTSP_LOWER_TRANSPORT_NB
};


enum RTSPTransport {
    RTSP_TRANSPORT_RTP, 
    RTSP_TRANSPORT_RDT, 
    RTSP_TRANSPORT_NB
};

#define RTSP_DEFAULT_PORT   554
#define RTSP_MAX_TRANSPORTS 8
#define RTSP_TCP_MAX_PACKET_SIZE 1472
#define RTSP_DEFAULT_NB_AUDIO_CHANNELS 2
#define RTSP_DEFAULT_AUDIO_SAMPLERATE 44100
#define RTSP_RTP_PORT_MIN 5000
#define RTSP_RTP_PORT_MAX 10000


typedef struct RTSPTransportField {
    
    int interleaved_min, interleaved_max;

    
    int port_min, port_max;

    
    int client_port_min, client_port_max;

    
    int server_port_min, server_port_max;

    
    int ttl;

    uint32_t destination; 

    
    enum RTSPTransport transport;

    
    enum RTSPLowerTransport lower_transport;
} RTSPTransportField;


typedef struct RTSPMessageHeader {
    
    int content_length;

    enum RTSPStatusCode status_code; 

    
    int nb_transports;

    
    int64_t range_start, range_end;

    
    RTSPTransportField transports[RTSP_MAX_TRANSPORTS];

    int seq;                         

    
    char session_id[512];

    
    char location[4096];

    
    char real_challenge[64];

    
    char server[64];

    
    int timeout;

    
    int notice;
} RTSPMessageHeader;


enum RTSPClientState {
    RTSP_STATE_IDLE,    
    RTSP_STATE_STREAMING, 
    RTSP_STATE_PAUSED,  
    RTSP_STATE_SEEKING, 
};


enum RTSPServerType {
    RTSP_SERVER_RTP,  
    RTSP_SERVER_REAL, 
    RTSP_SERVER_WMS,  
    RTSP_SERVER_NB
};


typedef struct RTSPState {
    URLContext *rtsp_hd; 

    
    int nb_rtsp_streams;

    struct RTSPStream **rtsp_streams; 

    
    enum RTSPClientState state;

    
    int64_t seek_timestamp;

    
    

    int seq;                          

    
    char session_id[512];

    
    int timeout;

    
    int64_t last_cmd_time;

    
    enum RTSPTransport transport;

    
    enum RTSPLowerTransport lower_transport;

    
    enum RTSPServerType server_type;

    
    char auth[128];

    
    HTTPAuthState auth_state;

    
    char last_reply[2048]; 

    
    void *cur_transport_priv;

    
    
    
    int need_subscription;

    
    enum AVDiscard real_setup_cache[MAX_STREAMS];

    
    char last_subscription[1024];
    

    
    
    
    AVFormatContext *asf_ctx;

    
    uint64_t asf_pb_pos;
    

    
    char control_uri[1024];

    
    int64_t start_time;
} RTSPState;


typedef struct RTSPStream {
    URLContext *rtp_handle;   
    void *transport_priv; 

    
    int stream_index;

    
    int interleaved_min, interleaved_max;

    char control_url[1024];   

    
    
    int sdp_port;             
    struct in_addr sdp_ip;    
    int sdp_ttl;              
    int sdp_payload_type;     
    

    
    RTPPayloadData rtp_payload_data;

    
    
    
    RTPDynamicProtocolHandler *dynamic_handler;

    
    PayloadContext *dynamic_protocol_context;
    
} RTSPStream;

void ff_rtsp_parse_line(RTSPMessageHeader *reply, const char *buf,
                        HTTPAuthState *auth_state);

#if LIBAVFORMAT_VERSION_INT < (53 << 16)
extern int rtsp_default_protocols;
#endif
extern int rtsp_rtp_port_min;
extern int rtsp_rtp_port_max;


void ff_rtsp_send_cmd_with_content_async(AVFormatContext *s,
                                         const char *method, const char *url,
                                         const char *headers,
                                         const unsigned char *send_content,
                                         int send_content_length);

void ff_rtsp_send_cmd_async(AVFormatContext *s, const char *method,
                            const char *url, const char *headers);


void ff_rtsp_send_cmd_with_content(AVFormatContext *s,
                                   const char *method, const char *url,
                                   const char *headers,
                                   RTSPMessageHeader *reply,
                                   unsigned char **content_ptr,
                                   const unsigned char *send_content,
                                   int send_content_length);


void ff_rtsp_send_cmd(AVFormatContext *s, const char *method,
                      const char *url, const char *headers,
                      RTSPMessageHeader *reply, unsigned char **content_ptr);


int ff_rtsp_read_reply(AVFormatContext *s, RTSPMessageHeader *reply,
                       unsigned char **content_ptr,
                       int return_on_interleaved_data);


void ff_rtsp_skip_packet(AVFormatContext *s);


int ff_rtsp_connect(AVFormatContext *s);


void ff_rtsp_close_streams(AVFormatContext *s);

#endif 
