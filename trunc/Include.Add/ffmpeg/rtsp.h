/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef RTSP_H
#define RTSP_H

enum RTSPStatusCode {
#define DEF(n, c, s) c = n,
#include "rtspcodes.h"
#undef DEF
};

enum RTSPProtocol {
    RTSP_PROTOCOL_RTP_UDP = 0,
    RTSP_PROTOCOL_RTP_TCP = 1,
    RTSP_PROTOCOL_RTP_UDP_MULTICAST = 2,
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
    enum RTSPProtocol protocol;
} RTSPTransportField;

typedef struct RTSPHeader {
    int content_length;
    enum RTSPStatusCode status_code; 
    int nb_transports;
    
    int64_t range_start, range_end;
    RTSPTransportField transports[RTSP_MAX_TRANSPORTS];
    int seq; 
    char session_id[512];
} RTSPHeader;

enum RTSPCallbackAction {
    RTSP_ACTION_SERVER_SETUP,
    RTSP_ACTION_SERVER_TEARDOWN,
    RTSP_ACTION_CLIENT_SETUP,
    RTSP_ACTION_CLIENT_TEARDOWN,
};

typedef struct RTSPActionServerSetup {
    uint32_t ipaddr;
    char transport_option[512];
} RTSPActionServerSetup;

typedef int FFRTSPCallback(enum RTSPCallbackAction action,
                           const char *session_id,
                           char *buf, int buf_size,
                           void *arg);

void rtsp_set_callback(FFRTSPCallback *rtsp_cb);

int rtsp_init(void);
void rtsp_parse_line(RTSPHeader *reply, const char *buf);

extern int rtsp_default_protocols;
extern int rtsp_rtp_port_min;
extern int rtsp_rtp_port_max;
extern FFRTSPCallback *ff_rtsp_callback;
extern AVInputFormat rtsp_demuxer;

int rtsp_pause(AVFormatContext *s);
int rtsp_resume(AVFormatContext *s);

#endif 
