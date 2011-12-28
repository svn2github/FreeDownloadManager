/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_RTPDEC_ASF_H
#define AVFORMAT_RTPDEC_ASF_H

#include "avformat.h"
#include "rtpdec.h"


void ff_wms_parse_sdp_a_line(AVFormatContext *s, const char *p);


extern RTPDynamicProtocolHandler ff_ms_rtp_asf_pfv_handler,
                                 ff_ms_rtp_asf_pfa_handler;

#endif 
