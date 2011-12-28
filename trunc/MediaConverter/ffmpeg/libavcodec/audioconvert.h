/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AUDIOCONVERT_H
#define AVCODEC_AUDIOCONVERT_H




#include "avcodec.h"



void avcodec_sample_fmt_string(char *buf, int buf_size, int sample_fmt);


const char *avcodec_get_sample_fmt_name(int sample_fmt);


enum SampleFormat avcodec_get_sample_fmt(const char* name);


const char *avcodec_get_channel_name(int channel_id);


void avcodec_get_channel_layout_string(char *buf, int buf_size, int nb_channels, int64_t channel_layout);


int64_t avcodec_guess_channel_layout(int nb_channels, enum CodecID codec_id, const char *fmt_name);


int avcodec_channel_layout_num_channels(int64_t channel_layout);

struct AVAudioConvert;
typedef struct AVAudioConvert AVAudioConvert;


AVAudioConvert *av_audio_convert_alloc(enum SampleFormat out_fmt, int out_channels,
                                       enum SampleFormat in_fmt, int in_channels,
                                       const float *matrix, int flags);


void av_audio_convert_free(AVAudioConvert *ctx);


int av_audio_convert(AVAudioConvert *ctx,
                           void * const out[6], const int out_stride[6],
                     const void * const  in[6], const int  in_stride[6], int len);

#endif 
