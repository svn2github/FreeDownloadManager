/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_FFM_H
#define AVFORMAT_FFM_H

#include <stdint.h>
#include "avformat.h"
#include "avio.h"


#define FFM_HEADER_SIZE 14
#define PACKET_ID       0x666d


#define FRAME_HEADER_SIZE    16
#define FLAG_KEY_FRAME       0x01
#define FLAG_DTS             0x02

enum {
    READ_HEADER,
    READ_DATA,
};

typedef struct FFMContext {
    
    int64_t write_index, file_size;
    int read_state;
    uint8_t header[FRAME_HEADER_SIZE+4];

    
    int first_packet; 
    int packet_size;
    int frame_offset;
    int64_t dts;
    uint8_t *packet_ptr, *packet_end;
    uint8_t packet[FFM_PACKET_SIZE];
} FFMContext;

#endif 
