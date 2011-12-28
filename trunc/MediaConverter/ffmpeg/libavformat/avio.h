/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_AVIO_H
#define AVFORMAT_AVIO_H



#include <stdint.h>

#include "libavutil/common.h"




typedef struct URLContext {
#if LIBAVFORMAT_VERSION_MAJOR >= 53
    const AVClass *av_class; 
#endif
    struct URLProtocol *prot;
    int flags;
    int is_streamed;  
    int max_packet_size;  
    void *priv_data;
    char *filename; 
} URLContext;

typedef struct URLPollEntry {
    URLContext *handle;
    int events;
    int revents;
} URLPollEntry;

#define URL_RDONLY 0
#define URL_WRONLY 1
#define URL_RDWR   2

typedef int URLInterruptCB(void);


int url_open_protocol (URLContext **puc, struct URLProtocol *up,
                       const char *url, int flags);


int url_open(URLContext **h, const char *url, int flags);


int url_read(URLContext *h, unsigned char *buf, int size);


int url_read_complete(URLContext *h, unsigned char *buf, int size);
int url_write(URLContext *h, unsigned char *buf, int size);


int64_t url_seek(URLContext *h, int64_t pos, int whence);


int url_close(URLContext *h);


int url_exist(const char *url);

int64_t url_filesize(URLContext *h);


int url_get_file_handle(URLContext *h);


int url_get_max_packet_size(URLContext *h);
void url_get_filename(URLContext *h, char *buf, int buf_size);


void url_set_interrupt_cb(URLInterruptCB *interrupt_cb);


int url_poll(URLPollEntry *poll_table, int n, int timeout);


int av_url_read_pause(URLContext *h, int pause);


int64_t av_url_read_seek(URLContext *h, int stream_index,
                         int64_t timestamp, int flags);


#define AVSEEK_SIZE 0x10000


#define AVSEEK_FORCE 0x20000

typedef struct URLProtocol {
    const char *name;
    int (*url_open)(URLContext *h, const char *url, int flags);
    int (*url_read)(URLContext *h, unsigned char *buf, int size);
    int (*url_write)(URLContext *h, unsigned char *buf, int size);
    int64_t (*url_seek)(URLContext *h, int64_t pos, int whence);
    int (*url_close)(URLContext *h);
    struct URLProtocol *next;
    int (*url_read_pause)(URLContext *h, int pause);
    int64_t (*url_read_seek)(URLContext *h, int stream_index,
                             int64_t timestamp, int flags);
    int (*url_get_file_handle)(URLContext *h);
} URLProtocol;

#if LIBAVFORMAT_VERSION_MAJOR < 53
extern URLProtocol *first_protocol;
#endif

extern URLInterruptCB *url_interrupt_cb;


URLProtocol *av_protocol_next(URLProtocol *p);

#if LIBAVFORMAT_VERSION_MAJOR < 53

attribute_deprecated int register_protocol(URLProtocol *protocol);
#endif


int av_register_protocol(URLProtocol *protocol);


typedef struct {
    unsigned char *buffer;
    int buffer_size;
    unsigned char *buf_ptr, *buf_end;
    void *opaque;
    int (*read_packet)(void *opaque, uint8_t *buf, int buf_size);
    int (*write_packet)(void *opaque, uint8_t *buf, int buf_size);
    int64_t (*seek)(void *opaque, int64_t offset, int whence);
    int64_t pos; 
    int must_flush; 
    int eof_reached; 
    int write_flag;  
    int is_streamed;
    int max_packet_size;
    unsigned long checksum;
    unsigned char *checksum_ptr;
    unsigned long (*update_checksum)(unsigned long checksum, const uint8_t *buf, unsigned int size);
    int error;         
    int (*read_pause)(void *opaque, int pause);
    int64_t (*read_seek)(void *opaque, int stream_index,
                         int64_t timestamp, int flags);
} ByteIOContext;

int init_put_byte(ByteIOContext *s,
                  unsigned char *buffer,
                  int buffer_size,
                  int write_flag,
                  void *opaque,
                  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int64_t (*seek)(void *opaque, int64_t offset, int whence));
ByteIOContext *av_alloc_put_byte(
                  unsigned char *buffer,
                  int buffer_size,
                  int write_flag,
                  void *opaque,
                  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int64_t (*seek)(void *opaque, int64_t offset, int whence));

void put_byte(ByteIOContext *s, int b);
void put_buffer(ByteIOContext *s, const unsigned char *buf, int size);
void put_le64(ByteIOContext *s, uint64_t val);
void put_be64(ByteIOContext *s, uint64_t val);
void put_le32(ByteIOContext *s, unsigned int val);
void put_be32(ByteIOContext *s, unsigned int val);
void put_le24(ByteIOContext *s, unsigned int val);
void put_be24(ByteIOContext *s, unsigned int val);
void put_le16(ByteIOContext *s, unsigned int val);
void put_be16(ByteIOContext *s, unsigned int val);
void put_tag(ByteIOContext *s, const char *tag);

void put_strz(ByteIOContext *s, const char *buf);


int64_t url_fseek(ByteIOContext *s, int64_t offset, int whence);


void url_fskip(ByteIOContext *s, int64_t offset);


int64_t url_ftell(ByteIOContext *s);


int64_t url_fsize(ByteIOContext *s);


int url_feof(ByteIOContext *s);

int url_ferror(ByteIOContext *s);

int av_url_read_fpause(ByteIOContext *h, int pause);
int64_t av_url_read_fseek(ByteIOContext *h, int stream_index,
                          int64_t timestamp, int flags);

#define URL_EOF (-1)

int url_fgetc(ByteIOContext *s);


#ifdef __GNUC__
int url_fprintf(ByteIOContext *s, const char *fmt, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
#else
int url_fprintf(ByteIOContext *s, const char *fmt, ...);
#endif


char *url_fgets(ByteIOContext *s, char *buf, int buf_size);

void put_flush_packet(ByteIOContext *s);



int get_buffer(ByteIOContext *s, unsigned char *buf, int size);


int get_partial_buffer(ByteIOContext *s, unsigned char *buf, int size);


int get_byte(ByteIOContext *s);
unsigned int get_le24(ByteIOContext *s);
unsigned int get_le32(ByteIOContext *s);
uint64_t get_le64(ByteIOContext *s);
unsigned int get_le16(ByteIOContext *s);

char *get_strz(ByteIOContext *s, char *buf, int maxlen);
unsigned int get_be16(ByteIOContext *s);
unsigned int get_be24(ByteIOContext *s);
unsigned int get_be32(ByteIOContext *s);
uint64_t get_be64(ByteIOContext *s);

uint64_t ff_get_v(ByteIOContext *bc);

static inline int url_is_streamed(ByteIOContext *s)
{
    return s->is_streamed;
}


int url_fdopen(ByteIOContext **s, URLContext *h);


int url_setbufsize(ByteIOContext *s, int buf_size);
#if LIBAVFORMAT_VERSION_MAJOR < 53

int url_resetbuf(ByteIOContext *s, int flags);
#endif


int ff_rewind_with_probe_data(ByteIOContext *s, unsigned char *buf, int buf_size);


int url_fopen(ByteIOContext **s, const char *url, int flags);

int url_fclose(ByteIOContext *s);
URLContext *url_fileno(ByteIOContext *s);


int url_fget_max_packet_size(ByteIOContext *s);

int url_open_buf(ByteIOContext **s, uint8_t *buf, int buf_size, int flags);


int url_close_buf(ByteIOContext *s);


int url_open_dyn_buf(ByteIOContext **s);


int url_open_dyn_packet_buf(ByteIOContext **s, int max_packet_size);


int url_close_dyn_buf(ByteIOContext *s, uint8_t **pbuffer);

unsigned long ff_crc04C11DB7_update(unsigned long checksum, const uint8_t *buf,
                                    unsigned int len);
unsigned long get_checksum(ByteIOContext *s);
void init_checksum(ByteIOContext *s,
                   unsigned long (*update_checksum)(unsigned long c, const uint8_t *p, unsigned int len),
                   unsigned long checksum);


int udp_set_remote_url(URLContext *h, const char *uri);
int udp_get_local_port(URLContext *h);
#if (LIBAVFORMAT_VERSION_MAJOR <= 52)
int udp_get_file_handle(URLContext *h);
#endif

#endif 
