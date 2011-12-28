/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_ERROR_H
#define AVUTIL_ERROR_H

#include <errno.h>
#include "avutil.h"


#if EDOM > 0
#define AVERROR(e) (-(e))   
#define AVUNERROR(e) (-(e)) 
#else

#define AVERROR(e) (e)
#define AVUNERROR(e) (e)
#endif

#if LIBAVUTIL_VERSION_MAJOR < 51
#define AVERROR_INVALIDDATA AVERROR(EINVAL)  
#define AVERROR_IO          AVERROR(EIO)     
#define AVERROR_NOENT       AVERROR(ENOENT)  
#define AVERROR_NOFMT       AVERROR(EILSEQ)  
#define AVERROR_NOMEM       AVERROR(ENOMEM)  
#define AVERROR_NUMEXPECTED AVERROR(EDOM)    
#define AVERROR_UNKNOWN     AVERROR(EINVAL)  
#endif

#define AVERROR_EOF         AVERROR(EPIPE)   
#define AVERROR_NOTSUPP     AVERROR(ENOSYS)  

#define AVERROR_PATCHWELCOME    (-MKTAG('P','A','W','E')) 

#if LIBAVUTIL_VERSION_MAJOR > 50
#define AVERROR_INVALIDDATA     (-MKTAG('I','N','D','A')) 
#define AVERROR_NUMEXPECTED     (-MKTAG('N','U','E','X')) 
#endif


int av_strerror(int errnum, char *errbuf, size_t errbuf_size);

#endif 
