/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVCODEC_AC3_PARSER_H
#define AVCODEC_AC3_PARSER_H

#include "ac3.h"
#include "get_bits.h"


int ff_ac3_parse_header(GetBitContext *gbc, AC3HeaderInfo *hdr);


int ff_ac3_parse_header_full(GetBitContext *gbc, AC3HeaderInfo *hdr);

#endif 
