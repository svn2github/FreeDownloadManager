/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef FFMPEG_CMDUTILS_H
#define FFMPEG_CMDUTILS_H

#include <inttypes.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"


extern const char program_name[];


extern const int program_birth_year;

extern const int this_year;

extern const char **opt_names;
extern AVCodecContext *avcodec_opts[AVMEDIA_TYPE_NB];
extern AVFormatContext *avformat_opts;
extern struct SwsContext *sws_opts;


int opt_default(const char *opt, const char *arg);


int opt_loglevel(const char *opt, const char *arg);


int opt_timelimit(const char *opt, const char *arg);


double parse_number_or_die(const char *context, const char *numstr, int type, double min, double max);


int64_t parse_time_or_die(const char *context, const char *timestr, int is_duration);

typedef struct {
    const char *name;
    int flags;
#define HAS_ARG    0x0001
#define OPT_BOOL   0x0002
#define OPT_EXPERT 0x0004
#define OPT_STRING 0x0008
#define OPT_VIDEO  0x0010
#define OPT_AUDIO  0x0020
#define OPT_GRAB   0x0040
#define OPT_INT    0x0080
#define OPT_FLOAT  0x0100
#define OPT_SUBTITLE 0x0200
#define OPT_FUNC2  0x0400
#define OPT_INT64  0x0800
#define OPT_EXIT   0x1000
     union {
        void (*func_arg)(const char *); 
        int *int_arg;
        char **str_arg;
        float *float_arg;
        int (*func2_arg)(const char *, const char *);
        int64_t *int64_arg;
    } u;
    const char *help;
    const char *argname;
} OptionDef;

void show_help_options(const OptionDef *options, const char *msg, int mask, int value);


void parse_options(int argc, char **argv, const OptionDef *options,
                   void (* parse_arg_function)(const char*));

void set_context_opts(void *ctx, void *opts_ctx, int flags);

void print_error(const char *filename, int err);

void list_fmts(void (*get_fmt_string)(char *buf, int buf_size, int fmt), int nb_fmts);


void show_banner(void);


void show_version(void);


void show_license(void);


void show_formats(void);


void show_codecs(void);


void show_filters(void);


void show_bsfs(void);


void show_protocols(void);


void show_pix_fmts(void);


int read_yesno(void);


int read_file(const char *filename, char **bufptr, size_t *size);

#endif 
