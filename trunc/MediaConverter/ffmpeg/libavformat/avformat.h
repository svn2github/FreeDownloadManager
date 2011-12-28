/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_AVFORMAT_H
#define AVFORMAT_AVFORMAT_H

#define LIBAVFORMAT_VERSION_MAJOR 52
#define LIBAVFORMAT_VERSION_MINOR 61
#define LIBAVFORMAT_VERSION_MICRO  0

#define LIBAVFORMAT_VERSION_INT AV_VERSION_INT(LIBAVFORMAT_VERSION_MAJOR, \
                                               LIBAVFORMAT_VERSION_MINOR, \
                                               LIBAVFORMAT_VERSION_MICRO)
#define LIBAVFORMAT_VERSION     AV_VERSION(LIBAVFORMAT_VERSION_MAJOR,   \
                                           LIBAVFORMAT_VERSION_MINOR,   \
                                           LIBAVFORMAT_VERSION_MICRO)
#define LIBAVFORMAT_BUILD       LIBAVFORMAT_VERSION_INT

#define LIBAVFORMAT_IDENT       "Lavf" AV_STRINGIFY(LIBAVFORMAT_VERSION)


unsigned avformat_version(void);


const char *avformat_configuration(void);


const char *avformat_license(void);

#include <time.h>
#include <stdio.h>  
#include "libavcodec/avcodec.h"

#include "avio.h"

struct AVFormatContext;




#define AV_METADATA_MATCH_CASE      1
#define AV_METADATA_IGNORE_SUFFIX   2
#define AV_METADATA_DONT_STRDUP_KEY 4
#define AV_METADATA_DONT_STRDUP_VAL 8
#define AV_METADATA_DONT_OVERWRITE 16   

typedef struct {
    char *key;
    char *value;
}AVMetadataTag;

typedef struct AVMetadata AVMetadata;
typedef struct AVMetadataConv AVMetadataConv;


AVMetadataTag *
av_metadata_get(AVMetadata *m, const char *key, const AVMetadataTag *prev, int flags);

#if LIBAVFORMAT_VERSION_MAJOR == 52

int av_metadata_set(AVMetadata **pm, const char *key, const char *value);
#endif


int av_metadata_set2(AVMetadata **pm, const char *key, const char *value, int flags);


void av_metadata_conv(struct AVFormatContext *ctx,const AVMetadataConv *d_conv,
                                                  const AVMetadataConv *s_conv);


void av_metadata_free(AVMetadata **m);






int av_get_packet(ByteIOContext *s, AVPacket *pkt, int size);






typedef struct AVFrac {
    int64_t val, num, den;
} AVFrac;




struct AVCodecTag;


typedef struct AVProbeData {
    const char *filename;
    unsigned char *buf; 
    int buf_size;       
} AVProbeData;

#define AVPROBE_SCORE_MAX 100               
#define AVPROBE_PADDING_SIZE 32             

typedef struct AVFormatParameters {
    AVRational time_base;
    int sample_rate;
    int channels;
    int width;
    int height;
    enum PixelFormat pix_fmt;
    int channel; 
    const char *standard; 
    unsigned int mpeg2ts_raw:1;  
    unsigned int mpeg2ts_compute_pcr:1; 
    unsigned int initial_pause:1;       
    unsigned int prealloced_context:1;
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    enum CodecID video_codec_id;
    enum CodecID audio_codec_id;
#endif
} AVFormatParameters;


#define AVFMT_NOFILE        0x0001
#define AVFMT_NEEDNUMBER    0x0002 
#define AVFMT_SHOW_IDS      0x0008 
#define AVFMT_RAWPICTURE    0x0020 
#define AVFMT_GLOBALHEADER  0x0040 
#define AVFMT_NOTIMESTAMPS  0x0080 
#define AVFMT_GENERIC_INDEX 0x0100 
#define AVFMT_TS_DISCONT    0x0200 
#define AVFMT_VARIABLE_FPS  0x0400 
#define AVFMT_NODIMENSIONS  0x0800 

typedef struct AVOutputFormat {
    const char *name;
    
    const char *long_name;
    const char *mime_type;
    const char *extensions; 
    
    int priv_data_size;
    
    enum CodecID audio_codec; 
    enum CodecID video_codec; 
    int (*write_header)(struct AVFormatContext *);
    int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);
    int (*write_trailer)(struct AVFormatContext *);
    
    int flags;
    
    int (*set_parameters)(struct AVFormatContext *, AVFormatParameters *);
    int (*interleave_packet)(struct AVFormatContext *, AVPacket *out,
                             AVPacket *in, int flush);

    
    const struct AVCodecTag * const *codec_tag;

    enum CodecID subtitle_codec; 

    const AVMetadataConv *metadata_conv;

    
    struct AVOutputFormat *next;
} AVOutputFormat;

typedef struct AVInputFormat {
    const char *name;
    
    const char *long_name;
    
    int priv_data_size;
    
    int (*read_probe)(AVProbeData *);
    
    int (*read_header)(struct AVFormatContext *,
                       AVFormatParameters *ap);
    
    int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);
    
    int (*read_close)(struct AVFormatContext *);

#if LIBAVFORMAT_VERSION_MAJOR < 53
    
    int (*read_seek)(struct AVFormatContext *,
                     int stream_index, int64_t timestamp, int flags);
#endif
    
    int64_t (*read_timestamp)(struct AVFormatContext *s, int stream_index,
                              int64_t *pos, int64_t pos_limit);
    
    int flags;
    
    const char *extensions;
    
    int value;

    
    int (*read_play)(struct AVFormatContext *);

    
    int (*read_pause)(struct AVFormatContext *);

    const struct AVCodecTag * const *codec_tag;

    
    int (*read_seek2)(struct AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);

    const AVMetadataConv *metadata_conv;

    
    struct AVInputFormat *next;
} AVInputFormat;

enum AVStreamParseType {
    AVSTREAM_PARSE_NONE,
    AVSTREAM_PARSE_FULL,       
    AVSTREAM_PARSE_HEADERS,    
    AVSTREAM_PARSE_TIMESTAMPS, 
};

typedef struct AVIndexEntry {
    int64_t pos;
    int64_t timestamp;
#define AVINDEX_KEYFRAME 0x0001
    int flags:2;
    int size:30; 
    int min_distance;         
} AVIndexEntry;

#define AV_DISPOSITION_DEFAULT   0x0001
#define AV_DISPOSITION_DUB       0x0002
#define AV_DISPOSITION_ORIGINAL  0x0004
#define AV_DISPOSITION_COMMENT   0x0008
#define AV_DISPOSITION_LYRICS    0x0010
#define AV_DISPOSITION_KARAOKE   0x0020


typedef struct AVStream {
    int index;    
    int id;       
    AVCodecContext *codec; 
    
    AVRational r_frame_rate;
    void *priv_data;

    
    int64_t first_dts;
    
    struct AVFrac pts;

    
    AVRational time_base;
    int pts_wrap_bits; 
    
    int stream_copy; 
    enum AVDiscard discard; 
    
    
    float quality;
    
    int64_t start_time;
    
    int64_t duration;

#if LIBAVFORMAT_VERSION_INT < (53<<16)
    char language[4]; 
#endif

    
    enum AVStreamParseType need_parsing;
    struct AVCodecParserContext *parser;

    int64_t cur_dts;
    int last_IP_duration;
    int64_t last_IP_pts;
    
    AVIndexEntry *index_entries; 
    int nb_index_entries;
    unsigned int index_entries_allocated_size;

    int64_t nb_frames;                 

#if LIBAVFORMAT_VERSION_INT < (53<<16)
    int64_t unused[4+1];

    char *filename; 
#endif

    int disposition; 

    AVProbeData probe_data;
#define MAX_REORDER_DELAY 16
    int64_t pts_buffer[MAX_REORDER_DELAY+1];

    
    AVRational sample_aspect_ratio;

    AVMetadata *metadata;

    
    const uint8_t *cur_ptr;
    int cur_len;
    AVPacket cur_pkt;

    
    
    int64_t reference_dts;

    
#define MAX_PROBE_PACKETS 2500
    int probe_packets;

    
    struct AVPacketList *last_in_packet_buffer;

    
    AVRational avg_frame_rate;

    
    int codec_info_nb_frames;
} AVStream;

#define AV_PROGRAM_RUNNING 1


typedef struct AVProgram {
    int            id;
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    char           *provider_name; 
    char           *name;          
#endif
    int            flags;
    enum AVDiscard discard;        
    unsigned int   *stream_index;
    unsigned int   nb_stream_indexes;
    AVMetadata *metadata;
} AVProgram;

#define AVFMTCTX_NOHEADER      0x0001 

typedef struct AVChapter {
    int id;                 
    AVRational time_base;   
    int64_t start, end;     
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    char *title;            
#endif
    AVMetadata *metadata;
} AVChapter;

#if LIBAVFORMAT_VERSION_MAJOR < 53
#define MAX_STREAMS 20
#else
#define MAX_STREAMS 100
#endif


typedef struct AVFormatContext {
    const AVClass *av_class; 
    
    struct AVInputFormat *iformat;
    struct AVOutputFormat *oformat;
    void *priv_data;
    ByteIOContext *pb;
    unsigned int nb_streams;
    AVStream *streams[MAX_STREAMS];
    char filename[1024]; 
    
    int64_t timestamp;
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    char title[512];
    char author[512];
    char copyright[512];
    char comment[512];
    char album[512];
    int year;  
    int track; 
    char genre[32]; 
#endif

    int ctx_flags; 
    
    
    struct AVPacketList *packet_buffer;

    
    int64_t start_time;
    
    int64_t duration;
    
    int64_t file_size;
    
    int bit_rate;

    
    AVStream *cur_st;
#if LIBAVFORMAT_VERSION_INT < (53<<16)
    const uint8_t *cur_ptr_deprecated;
    int cur_len_deprecated;
    AVPacket cur_pkt_deprecated;
#endif

    
    int64_t data_offset; 
    int index_built;

    int mux_rate;
    unsigned int packet_size;
    int preload;
    int max_delay;

#define AVFMT_NOOUTPUTLOOP -1
#define AVFMT_INFINITEOUTPUTLOOP 0
    
    int loop_output;

    int flags;
#define AVFMT_FLAG_GENPTS       0x0001 
#define AVFMT_FLAG_IGNIDX       0x0002 
#define AVFMT_FLAG_NONBLOCK     0x0004 
#define AVFMT_FLAG_IGNDTS       0x0008 
#define AVFMT_FLAG_NOFILLIN     0x0010 
#define AVFMT_FLAG_NOPARSE      0x0020 

    int loop_input;
    
    unsigned int probesize;

    
    int max_analyze_duration;

    const uint8_t *key;
    int keylen;

    unsigned int nb_programs;
    AVProgram **programs;

    
    enum CodecID video_codec_id;
    
    enum CodecID audio_codec_id;
    
    enum CodecID subtitle_codec_id;

    
    unsigned int max_index_size;

    
    unsigned int max_picture_buffer;

    unsigned int nb_chapters;
    AVChapter **chapters;

    
    int debug;
#define FF_FDEBUG_TS        0x0001

    
    struct AVPacketList *raw_packet_buffer;
    struct AVPacketList *raw_packet_buffer_end;

    struct AVPacketList *packet_buffer_end;

    AVMetadata *metadata;

    
#define RAW_PACKET_BUFFER_SIZE 2500000
    int raw_packet_buffer_remaining_size;

    
    int64_t start_time_realtime;
} AVFormatContext;

typedef struct AVPacketList {
    AVPacket pkt;
    struct AVPacketList *next;
} AVPacketList;

#if LIBAVFORMAT_VERSION_INT < (53<<16)
extern AVInputFormat *first_iformat;
extern AVOutputFormat *first_oformat;
#endif


AVInputFormat  *av_iformat_next(AVInputFormat  *f);


AVOutputFormat *av_oformat_next(AVOutputFormat *f);

enum CodecID av_guess_image2_codec(const char *filename);





void av_register_input_format(AVInputFormat *format);
void av_register_output_format(AVOutputFormat *format);
#if LIBAVFORMAT_VERSION_MAJOR < 53
attribute_deprecated AVOutputFormat *guess_stream_format(const char *short_name,
                                    const char *filename,
                                    const char *mime_type);


attribute_deprecated AVOutputFormat *guess_format(const char *short_name,
                                                  const char *filename,
                                                  const char *mime_type);
#endif


AVOutputFormat *av_guess_format(const char *short_name,
                                const char *filename,
                                const char *mime_type);


enum CodecID av_guess_codec(AVOutputFormat *fmt, const char *short_name,
                            const char *filename, const char *mime_type,
                            enum AVMediaType type);


void av_hex_dump(FILE *f, uint8_t *buf, int size);


void av_hex_dump_log(void *avcl, int level, uint8_t *buf, int size);


void av_pkt_dump(FILE *f, AVPacket *pkt, int dump_payload);


void av_pkt_dump_log(void *avcl, int level, AVPacket *pkt, int dump_payload);


void av_register_all(void);


enum CodecID av_codec_get_id(const struct AVCodecTag * const *tags, unsigned int tag);
unsigned int av_codec_get_tag(const struct AVCodecTag * const *tags, enum CodecID id);




AVInputFormat *av_find_input_format(const char *short_name);


AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened);


int av_open_input_stream(AVFormatContext **ic_ptr,
                         ByteIOContext *pb, const char *filename,
                         AVInputFormat *fmt, AVFormatParameters *ap);


int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
                       AVInputFormat *fmt,
                       int buf_size,
                       AVFormatParameters *ap);

#if LIBAVFORMAT_VERSION_MAJOR < 53

attribute_deprecated AVFormatContext *av_alloc_format_context(void);
#endif


AVFormatContext *avformat_alloc_context(void);


int av_find_stream_info(AVFormatContext *ic);


int av_read_packet(AVFormatContext *s, AVPacket *pkt);


int av_read_frame(AVFormatContext *s, AVPacket *pkt);


int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp,
                  int flags);


int avformat_seek_file(AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);


int av_read_play(AVFormatContext *s);


int av_read_pause(AVFormatContext *s);


void av_close_input_stream(AVFormatContext *s);


void av_close_input_file(AVFormatContext *s);


AVStream *av_new_stream(AVFormatContext *s, int id);
AVProgram *av_new_program(AVFormatContext *s, int id);


AVChapter *ff_new_chapter(AVFormatContext *s, int id, AVRational time_base,
                          int64_t start, int64_t end, const char *title);


void av_set_pts_info(AVStream *s, int pts_wrap_bits,
                     unsigned int pts_num, unsigned int pts_den);

#define AVSEEK_FLAG_BACKWARD 1 
#define AVSEEK_FLAG_BYTE     2 
#define AVSEEK_FLAG_ANY      4 
#define AVSEEK_FLAG_FRAME    8 

int av_find_default_stream_index(AVFormatContext *s);


int av_index_search_timestamp(AVStream *st, int64_t timestamp, int flags);


void ff_reduce_index(AVFormatContext *s, int stream_index);


int av_add_index_entry(AVStream *st, int64_t pos, int64_t timestamp,
                       int size, int distance, int flags);


int av_seek_frame_binary(AVFormatContext *s, int stream_index,
                         int64_t target_ts, int flags);


void av_update_cur_dts(AVFormatContext *s, AVStream *ref_st, int64_t timestamp);


int64_t av_gen_search(AVFormatContext *s, int stream_index,
                      int64_t target_ts, int64_t pos_min,
                      int64_t pos_max, int64_t pos_limit,
                      int64_t ts_min, int64_t ts_max,
                      int flags, int64_t *ts_ret,
                      int64_t (*read_timestamp)(struct AVFormatContext *, int , int64_t *, int64_t ));


int av_set_parameters(AVFormatContext *s, AVFormatParameters *ap);


int av_write_header(AVFormatContext *s);


int av_write_frame(AVFormatContext *s, AVPacket *pkt);


int av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt);


int av_interleave_packet_per_dts(AVFormatContext *s, AVPacket *out,
                                 AVPacket *pkt, int flush);


int av_write_trailer(AVFormatContext *s);

void dump_format(AVFormatContext *ic,
                 int index,
                 const char *url,
                 int is_output);

#if LIBAVFORMAT_VERSION_MAJOR < 53

attribute_deprecated int parse_image_size(int *width_ptr, int *height_ptr,
                                          const char *str);


attribute_deprecated int parse_frame_rate(int *frame_rate, int *frame_rate_base,
                                          const char *arg);
#endif


int64_t parse_date(const char *datestr, int duration);


int64_t av_gettime(void);


#define FFM_PACKET_SIZE 4096
int64_t ffm_read_write_index(int fd);
int ffm_write_write_index(int fd, int64_t pos);
void ffm_set_write_index(AVFormatContext *s, int64_t pos, int64_t file_size);


int find_info_tag(char *arg, int arg_size, const char *tag1, const char *info);


int av_get_frame_filename(char *buf, int buf_size,
                          const char *path, int number);


int av_filename_number_test(const char *filename);


int avf_sdp_create(AVFormatContext *ac[], int n_files, char *buff, int size);


int av_match_ext(const char *filename, const char *extensions);

#endif 
