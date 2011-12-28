/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/


#ifndef AVFORMAT_RTP_H
#define AVFORMAT_RTP_H

#include "libavcodec/avcodec.h"


int ff_rtp_get_payload_type(AVCodecContext *codec);


int ff_rtp_get_codec_info(AVCodecContext *codec, int payload_type);


const char *ff_rtp_enc_name(int payload_type);


enum CodecID ff_rtp_codec_id(const char *buf, enum AVMediaType codec_type);

#define RTP_PT_PRIVATE 96
#define RTP_VERSION 2
#define RTP_MAX_SDES 256   


#define RTCP_TX_RATIO_NUM 5
#define RTCP_TX_RATIO_DEN 1000

#endif 
