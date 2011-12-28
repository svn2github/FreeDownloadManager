/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_VORBISCOMMENT_H
#define AVFORMAT_VORBISCOMMENT_H

#include "avformat.h"
#include "metadata.h"


int ff_vorbiscomment_length(AVMetadata *m, const char *vendor_string,
                            unsigned *count);


int ff_vorbiscomment_write(uint8_t **p, AVMetadata *m,
                           const char *vendor_string, const unsigned count);

extern const AVMetadataConv ff_vorbiscomment_metadata_conv[];

#endif 
