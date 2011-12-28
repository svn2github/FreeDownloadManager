/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_ID3V1_H
#define AVFORMAT_ID3V1_H

#include "avformat.h"

#define ID3v1_TAG_SIZE 128

#define ID3v1_GENRE_MAX 147


extern const char * const ff_id3v1_genre_str[ID3v1_GENRE_MAX + 1];


void ff_id3v1_read(AVFormatContext *s);

#endif 

