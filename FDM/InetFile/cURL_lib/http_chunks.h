/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HTTP_CHUNKS_H
#define __HTTP_CHUNKS_H 

#define MAXNUM_SIZE 16

typedef enum {
  CHUNK_FIRST, 

  
  CHUNK_HEX,

  
  CHUNK_POSTHEX,

  
  CHUNK_CR,

  
  CHUNK_DATA,

  
  CHUNK_POSTCR,

  
  CHUNK_POSTLF,

  
  CHUNK_STOP,

  
  CHUNK_TRAILER,

  
  CHUNK_TRAILER_CR,

  
  CHUNK_TRAILER_POSTCR,

  CHUNK_LAST 

} ChunkyState;

typedef enum {
  CHUNKE_STOP = -1,
  CHUNKE_OK = 0,
  CHUNKE_TOO_LONG_HEX = 1,
  CHUNKE_ILLEGAL_HEX,
  CHUNKE_BAD_CHUNK,
  CHUNKE_WRITE_ERROR,
  CHUNKE_STATE_ERROR,
  CHUNKE_BAD_ENCODING,
  CHUNKE_OUT_OF_MEMORY,
  CHUNKE_LAST
} CHUNKcode;

struct Curl_chunker {
  char hexbuffer[ MAXNUM_SIZE + 1];
  int hexindex;
  ChunkyState state;
  size_t datasize;
  size_t dataleft; 
};

#endif
