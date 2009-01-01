/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

    

#ifndef AVOPT_H
#define AVOPT_H    

enum AVOptionType{
    FF_OPT_TYPE_FLAGS,
    FF_OPT_TYPE_INT,
    FF_OPT_TYPE_INT64,
    FF_OPT_TYPE_DOUBLE,
    FF_OPT_TYPE_FLOAT,
    FF_OPT_TYPE_STRING,
    FF_OPT_TYPE_RATIONAL,
    FF_OPT_TYPE_CONST=128,
};  

typedef struct AVOption {
    const char *name;

    
    const char *help;
    int offset;             
    enum AVOptionType type;

    double default_val;
    double min;
    double max;

    int flags;
#define AV_OPT_FLAG_ENCODING_PARAM  1   
#define AV_OPT_FLAG_DECODING_PARAM  2   
#define AV_OPT_FLAG_METADATA        4   
#define AV_OPT_FLAG_AUDIO_PARAM     8
#define AV_OPT_FLAG_VIDEO_PARAM     16
#define AV_OPT_FLAG_SUBTITLE_PARAM  32

    const char *unit;
} AVOption;  

const AVOption *av_set_string(void *obj, const char *name, const char *val);
const AVOption *av_set_double(void *obj, const char *name, double n);
const AVOption *av_set_q(void *obj, const char *name, AVRational n);
const AVOption *av_set_int(void *obj, const char *name, int64_t n);
double av_get_double(void *obj, const char *name, const AVOption **o_out);
AVRational av_get_q(void *obj, const char *name, const AVOption **o_out);
int64_t av_get_int(void *obj, const char *name, const AVOption **o_out);
const char *av_get_string(void *obj, const char *name, const AVOption **o_out, char *buf, int buf_len);
const AVOption *av_next_option(void *obj, const AVOption *last);
int av_opt_show(void *obj, void *av_log_obj);
void av_opt_set_defaults(void *s);

#endif
