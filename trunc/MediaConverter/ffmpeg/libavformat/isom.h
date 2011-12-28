/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ISOM_H
#define AVFORMAT_ISOM_H

#include "avio.h"
#include "riff.h"
#include "dv.h"


extern const AVCodecTag ff_mp4_obj_type[];
extern const AVCodecTag codec_movvideo_tags[];
extern const AVCodecTag codec_movaudio_tags[];
extern const AVCodecTag ff_codec_movsubtitle_tags[];

int ff_mov_iso639_to_lang(const char lang[4], int mp4);
int ff_mov_lang_to_iso639(unsigned code, char to[4]);



typedef struct {
    int count;
    int duration;
} MOVStts;

typedef struct {
    int first;
    int count;
    int id;
} MOVStsc;

typedef struct {
    uint32_t type;
    char *path;
    char *dir;
    char volume[28];
    char filename[64];
    int16_t nlvl_to, nlvl_from;
} MOVDref;

typedef struct {
    uint32_t type;
    int64_t size; 
} MOVAtom;

struct MOVParseTableEntry;

typedef struct {
    unsigned track_id;
    uint64_t base_data_offset;
    uint64_t moof_offset;
    unsigned stsd_id;
    unsigned duration;
    unsigned size;
    unsigned flags;
} MOVFragment;

typedef struct {
    unsigned track_id;
    unsigned stsd_id;
    unsigned duration;
    unsigned size;
    unsigned flags;
} MOVTrackExt;

typedef struct MOVStreamContext {
    ByteIOContext *pb;
    int ffindex;          
    int next_chunk;
    unsigned int chunk_count;
    int64_t *chunk_offsets;
    unsigned int stts_count;
    MOVStts *stts_data;
    unsigned int ctts_count;
    MOVStts *ctts_data;
    unsigned int stsc_count;
    MOVStsc *stsc_data;
    unsigned int stps_count;
    unsigned *stps_data;  
    int ctts_index;
    int ctts_sample;
    unsigned int sample_size;
    unsigned int sample_count;
    int *sample_sizes;
    unsigned int keyframe_count;
    int *keyframes;
    int time_scale;
    int time_offset;      
    int current_sample;
    unsigned int bytes_per_frame;
    unsigned int samples_per_frame;
    int dv_audio_container;
    int pseudo_stream_id; 
    int16_t audio_cid;    
    unsigned drefs_count;
    MOVDref *drefs;
    int dref_id;
    int wrong_dts;        
    int width;            
    int height;           
    int dts_shift;        
} MOVStreamContext;

typedef struct MOVContext {
    AVFormatContext *fc;
    int time_scale;
    int64_t duration;     
    int found_moov;       
    int found_mdat;       
    DVDemuxContext *dv_demux;
    AVFormatContext *dv_fctx;
    int isom;             
    MOVFragment fragment; 
    MOVTrackExt *trex_data;
    unsigned trex_count;
    int itunes_metadata;  
    int chapter_track;
} MOVContext;

int ff_mp4_read_descr_len(ByteIOContext *pb);
int ff_mov_read_esds(AVFormatContext *fc, ByteIOContext *pb, MOVAtom atom);
enum CodecID ff_mov_get_lpcm_codec_id(int bps, int flags);

#endif 
