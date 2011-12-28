/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_NUT_H
#define AVFORMAT_NUT_H




#include "avformat.h"
#include "riff.h"
#include "metadata.h"

#define      MAIN_STARTCODE (0x7A561F5F04ADULL + (((uint64_t)('N'<<8) + 'M')<<48))
#define    STREAM_STARTCODE (0x11405BF2F9DBULL + (((uint64_t)('N'<<8) + 'S')<<48))
#define SYNCPOINT_STARTCODE (0xE4ADEECA4569ULL + (((uint64_t)('N'<<8) + 'K')<<48))
#define     INDEX_STARTCODE (0xDD672F23E64EULL + (((uint64_t)('N'<<8) + 'X')<<48))
#define      INFO_STARTCODE (0xAB68B596BA78ULL + (((uint64_t)('N'<<8) + 'I')<<48))

#define ID_STRING "nut/multimedia container\0"

#define MAX_DISTANCE (1024*32-1)

typedef enum{
    FLAG_KEY        =   1, 
    FLAG_EOR        =   2, 
    FLAG_CODED_PTS  =   8, 
    FLAG_STREAM_ID  =  16, 
    FLAG_SIZE_MSB   =  32, 
    FLAG_CHECKSUM   =  64, 
    FLAG_RESERVED   = 128, 
    FLAG_HEADER_IDX =1024, 
    FLAG_MATCH_TIME =2048, 
    FLAG_CODED      =4096, 
    FLAG_INVALID    =8192, 
} Flag;

typedef struct {
    uint64_t pos;
    uint64_t back_ptr;

    int64_t ts;
} Syncpoint;

typedef struct {
    uint16_t flags;
    uint8_t  stream_id;
    uint16_t size_mul;
    uint16_t size_lsb;
    int16_t  pts_delta;
    uint8_t  reserved_count;
    uint8_t  header_idx;
} FrameCode;

typedef struct {
    int last_flags;
    int skip_until_key_frame;
    int64_t last_pts;
    int time_base_id;
    AVRational *time_base;
    int msb_pts_shift;
    int max_pts_distance;
    int decode_delay; 
} StreamContext;

typedef struct {
    AVFormatContext *avf;


    FrameCode frame_code[256];
    uint8_t header_len[128];
    const uint8_t *header[128];
    uint64_t next_startcode;     
    StreamContext *stream;
    unsigned int max_distance;
    unsigned int time_base_count;
    int64_t last_syncpoint_pos;
    int header_count;
    AVRational *time_base;
    struct AVTreeNode *syncpoints;
} NUTContext;

extern const AVCodecTag ff_nut_subtitle_tags[];

typedef struct {
    char str[9];
    int flag;
} Dispositions;

void ff_nut_reset_ts(NUTContext *nut, AVRational time_base, int64_t val);
int64_t ff_lsb2full(StreamContext *stream, int64_t lsb);
int ff_nut_sp_pos_cmp(const Syncpoint *a, const Syncpoint *b);
int ff_nut_sp_pts_cmp(const Syncpoint *a, const Syncpoint *b);
void ff_nut_add_sp(NUTContext *nut, int64_t pos, int64_t back_ptr, int64_t ts);
void ff_nut_free_sp(NUTContext *nut);

extern const Dispositions ff_nut_dispositions[];

extern const AVMetadataConv ff_nut_metadata_conv[];

#endif 
