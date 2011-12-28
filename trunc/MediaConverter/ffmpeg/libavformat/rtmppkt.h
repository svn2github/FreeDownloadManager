/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_RTMPPKT_H
#define AVFORMAT_RTMPPKT_H

#include "avformat.h"


#define RTMP_CHANNELS 65599


enum RTMPChannel {
    RTMP_NETWORK_CHANNEL = 2,   
    RTMP_SYSTEM_CHANNEL,        
    RTMP_SOURCE_CHANNEL,        
    RTMP_VIDEO_CHANNEL = 8,     
    RTMP_AUDIO_CHANNEL,         
};


typedef enum RTMPPacketType {
    RTMP_PT_CHUNK_SIZE   =  1,  
    RTMP_PT_BYTES_READ   =  3,  
    RTMP_PT_PING,               
    RTMP_PT_SERVER_BW,          
    RTMP_PT_CLIENT_BW,          
    RTMP_PT_AUDIO        =  8,  
    RTMP_PT_VIDEO,              
    RTMP_PT_FLEX_STREAM  = 15,  
    RTMP_PT_FLEX_OBJECT,        
    RTMP_PT_FLEX_MESSAGE,       
    RTMP_PT_NOTIFY,             
    RTMP_PT_SHARED_OBJ,         
    RTMP_PT_INVOKE,             
    RTMP_PT_METADATA     = 22,  
} RTMPPacketType;


enum RTMPPacketSize {
    RTMP_PS_TWELVEBYTES = 0, 
    RTMP_PS_EIGHTBYTES,      
    RTMP_PS_FOURBYTES,       
    RTMP_PS_ONEBYTE          
};


typedef struct RTMPPacket {
    int            channel_id; 
    RTMPPacketType type;       
    uint32_t       timestamp;  
    uint32_t       ts_delta;   
    uint32_t       extra;      
    uint8_t        *data;      
    int            data_size;  
} RTMPPacket;


int ff_rtmp_packet_create(RTMPPacket *pkt, int channel_id, RTMPPacketType type,
                          int timestamp, int size);


void ff_rtmp_packet_destroy(RTMPPacket *pkt);


int ff_rtmp_packet_read(URLContext *h, RTMPPacket *p,
                        int chunk_size, RTMPPacket *prev_pkt);


int ff_rtmp_packet_write(URLContext *h, RTMPPacket *p,
                         int chunk_size, RTMPPacket *prev_pkt);


void ff_rtmp_packet_dump(void *ctx, RTMPPacket *p);




int ff_amf_tag_size(const uint8_t *data, const uint8_t *data_end);


int ff_amf_get_field_value(const uint8_t *data, const uint8_t *data_end,
                           const uint8_t *name, uint8_t *dst, int dst_size);


void ff_amf_write_bool(uint8_t **dst, int val);


void ff_amf_write_number(uint8_t **dst, double num);


void ff_amf_write_string(uint8_t **dst, const char *str);


void ff_amf_write_null(uint8_t **dst);


void ff_amf_write_object_start(uint8_t **dst);


void ff_amf_write_field_name(uint8_t **dst, const char *str);


void ff_amf_write_object_end(uint8_t **dst);

 

#endif 
