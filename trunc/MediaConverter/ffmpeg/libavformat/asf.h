/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ASF_H
#define AVFORMAT_ASF_H

#include <stdint.h>
#include "avformat.h"
#include "metadata.h"

#define PACKET_SIZE 3200

typedef struct {
    int num;
    unsigned char seq;
    
    AVPacket pkt;
    int frag_offset;
    int timestamp;
    int64_t duration;

    int ds_span;                
    int ds_packet_size;
    int ds_chunk_size;

    int64_t packet_pos;

    uint16_t stream_language_index;

} ASFStream;

typedef uint8_t ff_asf_guid[16];

typedef struct {
    ff_asf_guid guid;                  
    uint64_t file_size;         
    uint64_t create_time;       
    uint64_t play_time;         
    uint64_t send_time;         
    uint32_t preroll;           
    uint32_t ignore;            
    uint32_t flags;             
    uint32_t min_pktsize;       
    uint32_t max_pktsize;       
    uint32_t max_bitrate;       
} ASFMainHeader;


typedef struct {
    uint32_t packet_number;
    uint16_t packet_count;
} ASFIndex;


typedef struct {
    uint32_t seqno;
    int is_streamed;
    int asfid2avid[128];                 
    ASFStream streams[128];              
    uint32_t stream_bitrates[128];       
    char stream_languages[128][6];       
    
    uint64_t nb_packets;                 
    int64_t duration;                    
    
    unsigned char multi_payloads_present;
    int packet_size_left;
    int packet_timestamp_start;
    int packet_timestamp_end;
    unsigned int packet_nb_payloads;
    int packet_nb_frames;
    uint8_t packet_buf[PACKET_SIZE];
    ByteIOContext pb;
    
    uint64_t data_offset;                
    uint64_t data_object_offset;         
    uint64_t data_object_size;           
    int index_read;

    ASFMainHeader hdr;

    int packet_flags;
    int packet_property;
    int packet_timestamp;
    int packet_segsizetype;
    int packet_segments;
    int packet_seq;
    int packet_replic_size;
    int packet_key_frame;
    int packet_padsize;
    unsigned int packet_frag_offset;
    unsigned int packet_frag_size;
    int64_t packet_frag_timestamp;
    int packet_multi_size;
    int packet_obj_size;
    int packet_time_delta;
    int packet_time_start;
    int64_t packet_pos;

    int stream_index;


    int64_t last_indexed_pts;
    ASFIndex* index_ptr;
    uint32_t nb_index_count;
    uint32_t nb_index_memory_alloc;
    uint16_t maximum_packet;

    ASFStream* asf_st;                   
} ASFContext;

extern const ff_asf_guid ff_asf_header;
extern const ff_asf_guid ff_asf_file_header;
extern const ff_asf_guid ff_asf_stream_header;
extern const ff_asf_guid ff_asf_ext_stream_header;
extern const ff_asf_guid ff_asf_audio_stream;
extern const ff_asf_guid ff_asf_audio_conceal_none;
extern const ff_asf_guid ff_asf_audio_conceal_spread;
extern const ff_asf_guid ff_asf_video_stream;
extern const ff_asf_guid ff_asf_video_conceal_none;
extern const ff_asf_guid ff_asf_command_stream;
extern const ff_asf_guid ff_asf_comment_header;
extern const ff_asf_guid ff_asf_codec_comment_header;
extern const ff_asf_guid ff_asf_codec_comment1_header;
extern const ff_asf_guid ff_asf_data_header;
extern const ff_asf_guid ff_asf_head1_guid;
extern const ff_asf_guid ff_asf_head2_guid;
extern const ff_asf_guid ff_asf_extended_content_header;
extern const ff_asf_guid ff_asf_simple_index_header;
extern const ff_asf_guid ff_asf_ext_stream_embed_stream_header;
extern const ff_asf_guid ff_asf_ext_stream_audio_stream;
extern const ff_asf_guid ff_asf_metadata_header;
extern const ff_asf_guid ff_asf_marker_header;
extern const ff_asf_guid ff_asf_my_guid;
extern const ff_asf_guid ff_asf_language_guid;
extern const ff_asf_guid ff_asf_content_encryption;
extern const ff_asf_guid ff_asf_ext_content_encryption;
extern const ff_asf_guid ff_asf_digital_signature;

extern const AVMetadataConv ff_asf_metadata_conv[];

#define ASF_PACKET_FLAG_ERROR_CORRECTION_PRESENT 0x80 


















#define ASF_PPI_FLAG_MULTIPLE_PAYLOADS_PRESENT 1

#define ASF_PPI_FLAG_SEQUENCE_FIELD_IS_BYTE  0x02 
#define ASF_PPI_FLAG_SEQUENCE_FIELD_IS_WORD  0x04 
#define ASF_PPI_FLAG_SEQUENCE_FIELD_IS_DWORD 0x06 
#define ASF_PPI_MASK_SEQUENCE_FIELD_SIZE     0x06 

#define ASF_PPI_FLAG_PADDING_LENGTH_FIELD_IS_BYTE  0x08 
#define ASF_PPI_FLAG_PADDING_LENGTH_FIELD_IS_WORD  0x10 
#define ASF_PPI_FLAG_PADDING_LENGTH_FIELD_IS_DWORD 0x18 
#define ASF_PPI_MASK_PADDING_LENGTH_FIELD_SIZE     0x18 

#define ASF_PPI_FLAG_PACKET_LENGTH_FIELD_IS_BYTE  0x20 
#define ASF_PPI_FLAG_PACKET_LENGTH_FIELD_IS_WORD  0x40 
#define ASF_PPI_FLAG_PACKET_LENGTH_FIELD_IS_DWORD 0x60 
#define ASF_PPI_MASK_PACKET_LENGTH_FIELD_SIZE     0x60 


#define ASF_PL_FLAG_REPLICATED_DATA_LENGTH_FIELD_IS_BYTE   0x01 
#define ASF_PL_FLAG_REPLICATED_DATA_LENGTH_FIELD_IS_WORD   0x02 
#define ASF_PL_FLAG_REPLICATED_DATA_LENGTH_FIELD_IS_DWORD  0x03 
#define ASF_PL_MASK_REPLICATED_DATA_LENGTH_FIELD_SIZE      0x03 

#define ASF_PL_FLAG_OFFSET_INTO_MEDIA_OBJECT_LENGTH_FIELD_IS_BYTE  0x04 
#define ASF_PL_FLAG_OFFSET_INTO_MEDIA_OBJECT_LENGTH_FIELD_IS_WORD  0x08 
#define ASF_PL_FLAG_OFFSET_INTO_MEDIA_OBJECT_LENGTH_FIELD_IS_DWORD 0x0c 
#define ASF_PL_MASK_OFFSET_INTO_MEDIA_OBJECT_LENGTH_FIELD_SIZE     0x0c 

#define ASF_PL_FLAG_MEDIA_OBJECT_NUMBER_LENGTH_FIELD_IS_BYTE  0x10 
#define ASF_PL_FLAG_MEDIA_OBJECT_NUMBER_LENGTH_FIELD_IS_WORD  0x20 
#define ASF_PL_FLAG_MEDIA_OBJECT_NUMBER_LENGTH_FIELD_IS_DWORD 0x30 
#define ASF_PL_MASK_MEDIA_OBJECT_NUMBER_LENGTH_FIELD_SIZE     0x30 

#define ASF_PL_FLAG_STREAM_NUMBER_LENGTH_FIELD_IS_BYTE  0x40 
#define ASF_PL_MASK_STREAM_NUMBER_LENGTH_FIELD_SIZE     0xc0 

#define ASF_PL_FLAG_PAYLOAD_LENGTH_FIELD_IS_BYTE  0x40 
#define ASF_PL_FLAG_PAYLOAD_LENGTH_FIELD_IS_WORD  0x80 
#define ASF_PL_MASK_PAYLOAD_LENGTH_FIELD_SIZE     0xc0 

#define ASF_PL_FLAG_KEY_FRAME 0x80 

extern AVInputFormat asf_demuxer;
int ff_put_str16_nolen(ByteIOContext *s, const char *tag);

#endif 
