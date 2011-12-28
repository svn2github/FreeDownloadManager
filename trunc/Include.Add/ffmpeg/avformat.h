/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef AVFORMAT_H
#define AVFORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#define LIBAVFORMAT_VERSION_INT ((51<<16)+(8<<8)+0)
#define LIBAVFORMAT_VERSION     51.8.0
#define LIBAVFORMAT_BUILD       LIBAVFORMAT_VERSION_INT

#define LIBAVFORMAT_IDENT       "Lavf" AV_STRINGIFY(LIBAVFORMAT_VERSION)

#include <time.h>
#include <stdio.h>  
#include "avcodec.h"

#include "avio.h"

typedef struct AVPacket {
    int64_t pts;                            
    int64_t dts;                            
    uint8_t *data;
    int   size;
    int   stream_index;
    int   flags;
    int   duration;                         
    void  (*destruct)(struct AVPacket *);
    void  *priv;
    int64_t pos;                            
} AVPacket;
#define PKT_FLAG_KEY   0x0001

void av_destruct_packet_nofree(AVPacket *pkt);
void av_destruct_packet(AVPacket *pkt);

static inline void av_init_packet(AVPacket *pkt)
{
    pkt->pts   = AV_NOPTS_VALUE;
    pkt->dts   = AV_NOPTS_VALUE;
    pkt->pos   = -1;
    pkt->duration = 0;
    pkt->flags = 0;
    pkt->stream_index = 0;
    pkt->destruct= av_destruct_packet_nofree;
}

int av_new_packet(AVPacket *pkt, int size);
int av_get_packet(ByteIOContext *s, AVPacket *pkt, int size);
int av_dup_packet(AVPacket *pkt);

static inline void av_free_packet(AVPacket *pkt)
{
    if (pkt && pkt->destruct) {
        pkt->destruct(pkt);
    }
}

typedef struct AVFrac {
    int64_t val, num, den;
} AVFrac attribute_deprecated;

struct AVCodecTag;

struct AVFormatContext;

typedef struct AVProbeData {
    const char *filename;
    unsigned char *buf;
    int buf_size;
} AVProbeData;

#define AVPROBE_SCORE_MAX 100               

typedef struct AVFormatParameters {
    AVRational time_base;
    int sample_rate;
    int channels;
    int width;
    int height;
    enum PixelFormat pix_fmt;
    int channel; 
    const char *device; 
    const char *standard; 
    int mpeg2ts_raw:1;  
    int mpeg2ts_compute_pcr:1; 
    int initial_pause:1;       
    int prealloced_context:1;
    enum CodecID video_codec_id;
    enum CodecID audio_codec_id;
} AVFormatParameters;

#define AVFMT_NOFILE        0x0001
#define AVFMT_NEEDNUMBER    0x0002 
#define AVFMT_SHOW_IDS      0x0008 
#define AVFMT_RAWPICTURE    0x0020 
#define AVFMT_GLOBALHEADER  0x0040 
#define AVFMT_NOTIMESTAMPS  0x0080 

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
    int (*interleave_packet)(struct AVFormatContext *, AVPacket *out, AVPacket *in, int flush);

    
    const struct AVCodecTag **codec_tag;

    
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
    
    int (*read_seek)(struct AVFormatContext *,
                     int stream_index, int64_t timestamp, int flags);
    
    int64_t (*read_timestamp)(struct AVFormatContext *s, int stream_index,
                              int64_t *pos, int64_t pos_limit);
    
    int flags;
    
    const char *extensions;
    
    int value;

    
    int (*read_play)(struct AVFormatContext *);

    
    int (*read_pause)(struct AVFormatContext *);

    const struct AVCodecTag **codec_tag;

    
    struct AVInputFormat *next;
} AVInputFormat;

typedef struct AVIndexEntry {
    int64_t pos;
    int64_t timestamp;
#define AVINDEX_KEYFRAME 0x0001
    int flags:2;
    int size:30; 
    int min_distance;         
} AVIndexEntry;

typedef struct AVStream {
    int index;    
    int id;       
    AVCodecContext *codec; 
    
    AVRational r_frame_rate;
    void *priv_data;
    
    int64_t codec_info_duration;
    int codec_info_nb_frames;
    
    AVFrac pts;

    
    AVRational time_base;
    int pts_wrap_bits; 
    
    int stream_copy; 
    enum AVDiscard discard; 
    
    
    float quality;
    
    int64_t start_time;
    
    int64_t duration;

    char language[4]; 

    
    int need_parsing;                  
    struct AVCodecParserContext *parser;

    int64_t cur_dts;
    int last_IP_duration;
    int64_t last_IP_pts;
    
    AVIndexEntry *index_entries; 
    int nb_index_entries;
    unsigned int index_entries_allocated_size;

    int64_t nb_frames;                 

#define MAX_REORDER_DELAY 4
    int64_t pts_buffer[MAX_REORDER_DELAY+1];
} AVStream;

#define AVFMTCTX_NOHEADER      0x0001 

#define MAX_STREAMS 20

typedef struct AVFormatContext {
    const AVClass *av_class; 
    
    struct AVInputFormat *iformat;
    struct AVOutputFormat *oformat;
    void *priv_data;
    ByteIOContext pb;
    unsigned int nb_streams;
    AVStream *streams[MAX_STREAMS];
    char filename[1024]; 
    
    int64_t timestamp;
    char title[512];
    char author[512];
    char copyright[512];
    char comment[512];
    char album[512];
    int year;  
    int track; 
    char genre[32]; 

    int ctx_flags; 
    
    
    struct AVPacketList *packet_buffer;

    
    int64_t start_time;
    
    int64_t duration;
    
    int64_t file_size;
    
    int bit_rate;

    
    AVStream *cur_st;
    const uint8_t *cur_ptr;
    int cur_len;
    AVPacket cur_pkt;

    
    int64_t data_offset; 
    int index_built;

    int mux_rate;
    int packet_size;
    int preload;
    int max_delay;

#define AVFMT_NOOUTPUTLOOP -1
#define AVFMT_INFINITEOUTPUTLOOP 0
    
    int loop_output;

    int flags;
#define AVFMT_FLAG_GENPTS       0x0001 
#define AVFMT_FLAG_IGNIDX       0x0002 

    int loop_input;
    
    unsigned int probesize;

    
    int max_analyze_duration;
} AVFormatContext;

typedef struct AVPacketList {
    AVPacket pkt;
    struct AVPacketList *next;
} AVPacketList;

extern AVInputFormat *first_iformat;
extern AVOutputFormat *first_oformat;

enum CodecID av_guess_image2_codec(const char *filename);

#include "rtp.h"

#include "rtsp.h"

void av_register_input_format(AVInputFormat *format);
void av_register_output_format(AVOutputFormat *format);
AVOutputFormat *guess_stream_format(const char *short_name,
                                    const char *filename, const char *mime_type);
AVOutputFormat *guess_format(const char *short_name,
                             const char *filename, const char *mime_type);
enum CodecID av_guess_codec(AVOutputFormat *fmt, const char *short_name,
                            const char *filename, const char *mime_type, enum CodecType type);

void av_hex_dump(FILE *f, uint8_t *buf, int size);
void av_pkt_dump(FILE *f, AVPacket *pkt, int dump_payload);

void av_register_all(void);

enum CodecID av_codec_get_id(const struct AVCodecTag **tags, unsigned int tag);
unsigned int av_codec_get_tag(const struct AVCodecTag **tags, enum CodecID id);

AVInputFormat *av_find_input_format(const char *short_name);
AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened);
int av_open_input_stream(AVFormatContext **ic_ptr,
                         ByteIOContext *pb, const char *filename,
                         AVInputFormat *fmt, AVFormatParameters *ap);
int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
                       AVInputFormat *fmt,
                       int buf_size,
                       AVFormatParameters *ap);

AVFormatContext *av_alloc_format_context(void);

#define AVERROR_UNKNOWN     (-1)  
#define AVERROR_IO          (-2)  
#define AVERROR_NUMEXPECTED (-3)  
#define AVERROR_INVALIDDATA (-4)  
#define AVERROR_NOMEM       (-5)  
#define AVERROR_NOFMT       (-6)  
#define AVERROR_NOTSUPP     (-7)  

int av_find_stream_info(AVFormatContext *ic);
int av_read_packet(AVFormatContext *s, AVPacket *pkt);
int av_read_frame(AVFormatContext *s, AVPacket *pkt);
int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags);
int av_read_play(AVFormatContext *s);
int av_read_pause(AVFormatContext *s);
void av_close_input_file(AVFormatContext *s);
AVStream *av_new_stream(AVFormatContext *s, int id);
void av_set_pts_info(AVStream *s, int pts_wrap_bits,
                     int pts_num, int pts_den);

#define AVSEEK_FLAG_BACKWARD 1 
#define AVSEEK_FLAG_BYTE     2 
#define AVSEEK_FLAG_ANY      4 

int av_find_default_stream_index(AVFormatContext *s);
int av_index_search_timestamp(AVStream *st, int64_t timestamp, int flags);
int av_add_index_entry(AVStream *st,
                       int64_t pos, int64_t timestamp, int size, int distance, int flags);
int av_seek_frame_binary(AVFormatContext *s, int stream_index, int64_t target_ts, int flags);
void av_update_cur_dts(AVFormatContext *s, AVStream *ref_st, int64_t timestamp);
int64_t av_gen_search(AVFormatContext *s, int stream_index, int64_t target_ts, int64_t pos_min, int64_t pos_max, int64_t pos_limit, int64_t ts_min, int64_t ts_max, int flags, int64_t *ts_ret, int64_t (*read_timestamp)(struct AVFormatContext *, int , int64_t *, int64_t ));

int av_set_parameters(AVFormatContext *s, AVFormatParameters *ap);
int av_write_header(AVFormatContext *s);
int av_write_frame(AVFormatContext *s, AVPacket *pkt);
int av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt);
int av_interleave_packet_per_dts(AVFormatContext *s, AVPacket *out, AVPacket *pkt, int flush);

int av_write_trailer(AVFormatContext *s);

void dump_format(AVFormatContext *ic,
                 int index,
                 const char *url,
                 int is_output);
int parse_image_size(int *width_ptr, int *height_ptr, const char *str);
int parse_frame_rate(int *frame_rate, int *frame_rate_base, const char *arg);
int64_t parse_date(const char *datestr, int duration);

int64_t av_gettime(void);

#define FFM_PACKET_SIZE 4096
offset_t ffm_read_write_index(int fd);
void ffm_write_write_index(int fd, offset_t pos);
void ffm_set_write_index(AVFormatContext *s, offset_t pos, offset_t file_size);

int find_info_tag(char *arg, int arg_size, const char *tag1, const char *info);

int av_get_frame_filename(char *buf, int buf_size,
                          const char *path, int number);
int av_filename_number_test(const char *filename);

int video_grab_init(void);
int audio_init(void);

int dv1394_init(void);
int dc1394_init(void);

#ifdef HAVE_AV_CONFIG_H

#include "os_support.h"

int strstart(const char *str, const char *val, const char **ptr);
int stristart(const char *str, const char *val, const char **ptr);
void pstrcpy(char *buf, int buf_size, const char *str);
char *pstrcat(char *buf, int buf_size, const char *s);

void __dynarray_add(unsigned long **tab_ptr, int *nb_ptr, unsigned long elem);

#ifdef __GNUC__
#define dynarray_add(tab, nb_ptr, elem)\
do {\
    typeof(tab) _tab = (tab);\
    typeof(elem) _elem = (elem);\
    (void)sizeof(**_tab == _elem); \
    __dynarray_add((unsigned long **)_tab, nb_ptr, (unsigned long)_elem);\
} while(0)
#else
#define dynarray_add(tab, nb_ptr, elem)\
do {\
    __dynarray_add((unsigned long **)(tab), nb_ptr, (unsigned long)(elem));\
} while(0)
#endif

time_t mktimegm(struct tm *tm);
struct tm *brktimegm(time_t secs, struct tm *tm);
const char *small_strptime(const char *p, const char *fmt,
                           struct tm *dt);

struct in_addr;
int resolve_host(struct in_addr *sin_addr, const char *hostname);

void url_split(char *proto, int proto_size,
               char *authorization, int authorization_size,
               char *hostname, int hostname_size,
               int *port_ptr,
               char *path, int path_size,
               const char *url);

int match_ext(const char *filename, const char *extensions);

#endif 

#ifdef __cplusplus
}
#endif

#endif 

