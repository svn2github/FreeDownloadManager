/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef ADLER32_H
#define ADLER32_H

unsigned long av_adler32_update(unsigned long adler, const uint8_t *buf,
                                unsigned int len);

#endif
