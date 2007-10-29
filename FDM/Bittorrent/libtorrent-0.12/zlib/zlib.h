/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

 

#ifndef ZLIB_H
#define ZLIB_H

#include "zconf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ZLIB_VERSION "1.2.3"
#define ZLIB_VERNUM 0x1230  

typedef voidpf (*alloc_func) OF((voidpf opaque, uInt items, uInt size));
typedef void   (*free_func)  OF((voidpf opaque, voidpf address));

struct internal_state;

typedef struct z_stream_s {
    Bytef    *next_in;  
    uInt     avail_in;  
    uLong    total_in;  

    Bytef    *next_out; 
    uInt     avail_out; 
    uLong    total_out; 

    char     *msg;      
    struct internal_state FAR *state; 

    alloc_func zalloc;  
    free_func  zfree;   
    voidpf     opaque;  

    int     data_type;  
    uLong   adler;      
    uLong   reserved;   
} z_stream;

typedef z_stream FAR *z_streamp; 

typedef struct gz_header_s {
    int     text;       
    uLong   time;       
    int     xflags;     
    int     os;         
    Bytef   *extra;     
    uInt    extra_len;  
    uInt    extra_max;  
    Bytef   *name;      
    uInt    name_max;   
    Bytef   *comment;   
    uInt    comm_max;   
    int     hcrc;       
    int     done;       
} gz_header;

typedef gz_header FAR *gz_headerp;  

                        

#define Z_NO_FLUSH      0
#define Z_PARTIAL_FLUSH 1 
#define Z_SYNC_FLUSH    2
#define Z_FULL_FLUSH    3
#define Z_FINISH        4
#define Z_BLOCK         5 

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_NEED_DICT     2
#define Z_ERRNO        (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR   (-3)
#define Z_MEM_ERROR    (-4)
#define Z_BUF_ERROR    (-5)
#define Z_VERSION_ERROR (-6) 

#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1) 

#define Z_FILTERED            1
#define Z_HUFFMAN_ONLY        2
#define Z_RLE                 3
#define Z_FIXED               4
#define Z_DEFAULT_STRATEGY    0 

#define Z_BINARY   0
#define Z_TEXT     1
#define Z_ASCII    Z_TEXT   
#define Z_UNKNOWN  2 

#define Z_DEFLATED   8 

#define Z_NULL  0  

#define zlib_version zlibVersion() 

                        

ZEXTERN const char * ZEXPORT zlibVersion OF((void));    

ZEXTERN int ZEXPORT deflate OF((z_streamp strm, int flush));  

ZEXTERN int ZEXPORT deflateEnd OF((z_streamp strm));     

ZEXTERN int ZEXPORT inflate OF((z_streamp strm, int flush));  

ZEXTERN int ZEXPORT inflateEnd OF((z_streamp strm)); 

                            

ZEXTERN int ZEXPORT deflateSetDictionary OF((z_streamp strm,
                                             const Bytef *dictionary,
                                             uInt  dictLength)); 

ZEXTERN int ZEXPORT deflateCopy OF((z_streamp dest,
                                    z_streamp source)); 

ZEXTERN int ZEXPORT deflateReset OF((z_streamp strm)); 

ZEXTERN int ZEXPORT deflateParams OF((z_streamp strm,
                                      int level,
                                      int strategy)); 

ZEXTERN int ZEXPORT deflateTune OF((z_streamp strm,
                                    int good_length,
                                    int max_lazy,
                                    int nice_length,
                                    int max_chain)); 

ZEXTERN uLong ZEXPORT deflateBound OF((z_streamp strm,
                                       uLong sourceLen)); 

ZEXTERN int ZEXPORT deflatePrime OF((z_streamp strm,
                                     int bits,
                                     int value)); 

ZEXTERN int ZEXPORT deflateSetHeader OF((z_streamp strm,
                                         gz_headerp head));   

ZEXTERN int ZEXPORT inflateSetDictionary OF((z_streamp strm,
                                             const Bytef *dictionary,
                                             uInt  dictLength)); 

ZEXTERN int ZEXPORT inflateSync OF((z_streamp strm)); 

ZEXTERN int ZEXPORT inflateCopy OF((z_streamp dest,
                                    z_streamp source)); 

ZEXTERN int ZEXPORT inflateReset OF((z_streamp strm)); 

ZEXTERN int ZEXPORT inflatePrime OF((z_streamp strm,
                                     int bits,
                                     int value)); 

ZEXTERN int ZEXPORT inflateGetHeader OF((z_streamp strm,
                                         gz_headerp head));   

typedef unsigned (*in_func) OF((void FAR *, unsigned char FAR * FAR *));
typedef int (*out_func) OF((void FAR *, unsigned char FAR *, unsigned));

ZEXTERN int ZEXPORT inflateBack OF((z_streamp strm,
                                    in_func in, void FAR *in_desc,
                                    out_func out, void FAR *out_desc)); 

ZEXTERN int ZEXPORT inflateBackEnd OF((z_streamp strm)); 

ZEXTERN uLong ZEXPORT zlibCompileFlags OF((void));  

                          

ZEXTERN int ZEXPORT compress OF((Bytef *dest,   uLongf *destLen,
                                 const Bytef *source, uLong sourceLen)); 

ZEXTERN int ZEXPORT compress2 OF((Bytef *dest,   uLongf *destLen,
                                  const Bytef *source, uLong sourceLen,
                                  int level)); 

ZEXTERN uLong ZEXPORT compressBound OF((uLong sourceLen)); 

ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
                                   const Bytef *source, uLong sourceLen));  

typedef voidp gzFile;

ZEXTERN gzFile ZEXPORT gzopen  OF((const char *path, const char *mode)); 

ZEXTERN gzFile ZEXPORT gzdopen  OF((int fd, const char *mode)); 

ZEXTERN int ZEXPORT gzsetparams OF((gzFile file, int level, int strategy)); 

ZEXTERN int ZEXPORT    gzread  OF((gzFile file, voidp buf, unsigned len)); 

ZEXTERN int ZEXPORT    gzwrite OF((gzFile file,
                                   voidpc buf, unsigned len)); 

ZEXTERN int ZEXPORTVA   gzprintf OF((gzFile file, const char *format, ...)); 

ZEXTERN int ZEXPORT gzputs OF((gzFile file, const char *s)); 

ZEXTERN char * ZEXPORT gzgets OF((gzFile file, char *buf, int len)); 

ZEXTERN int ZEXPORT    gzputc OF((gzFile file, int c)); 

ZEXTERN int ZEXPORT    gzgetc OF((gzFile file)); 

ZEXTERN int ZEXPORT    gzungetc OF((int c, gzFile file)); 

ZEXTERN int ZEXPORT    gzflush OF((gzFile file, int flush)); 

ZEXTERN z_off_t ZEXPORT    gzseek OF((gzFile file,
                                      z_off_t offset, int whence)); 

ZEXTERN int ZEXPORT    gzrewind OF((gzFile file)); 

ZEXTERN z_off_t ZEXPORT    gztell OF((gzFile file)); 

ZEXTERN int ZEXPORT gzeof OF((gzFile file)); 

ZEXTERN int ZEXPORT gzdirect OF((gzFile file)); 

ZEXTERN int ZEXPORT    gzclose OF((gzFile file)); 

ZEXTERN const char * ZEXPORT gzerror OF((gzFile file, int *errnum)); 

ZEXTERN void ZEXPORT gzclearerr OF((gzFile file)); 

                          

ZEXTERN uLong ZEXPORT adler32 OF((uLong adler, const Bytef *buf, uInt len)); 

ZEXTERN uLong ZEXPORT adler32_combine OF((uLong adler1, uLong adler2,
                                          z_off_t len2)); 

ZEXTERN uLong ZEXPORT crc32   OF((uLong crc, const Bytef *buf, uInt len)); 

ZEXTERN uLong ZEXPORT crc32_combine OF((uLong crc1, uLong crc2, z_off_t len2));   

                         

ZEXTERN int ZEXPORT deflateInit_ OF((z_streamp strm, int level,
                                     const char *version, int stream_size));
ZEXTERN int ZEXPORT inflateInit_ OF((z_streamp strm,
                                     const char *version, int stream_size));
ZEXTERN int ZEXPORT deflateInit2_ OF((z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size));
ZEXTERN int ZEXPORT inflateInit2_ OF((z_streamp strm, int  windowBits,
                                      const char *version, int stream_size));
ZEXTERN int ZEXPORT inflateBackInit_ OF((z_streamp strm, int windowBits,
                                         unsigned char FAR *window,
                                         const char *version,
                                         int stream_size));
#define deflateInit(strm, level) \
        deflateInit_((strm), (level),       ZLIB_VERSION, sizeof(z_stream))
#define inflateInit(strm) \
        inflateInit_((strm),                ZLIB_VERSION, sizeof(z_stream))
#define deflateInit2(strm, level, method, windowBits, memLevel, strategy) \
        deflateInit2_((strm),(level),(method),(windowBits),(memLevel),\
                      (strategy),           ZLIB_VERSION, sizeof(z_stream))
#define inflateInit2(strm, windowBits) \
        inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))
#define inflateBackInit(strm, windowBits, window) \
        inflateBackInit_((strm), (windowBits), (window), \
        ZLIB_VERSION, sizeof(z_stream)) 

#if !defined(ZUTIL_H) && !defined(NO_DUMMY_DECL)
    struct internal_state {int dummy;}; 
#endif

ZEXTERN const char   * ZEXPORT zError           OF((int));
ZEXTERN int            ZEXPORT inflateSyncPoint OF((z_streamp z));
ZEXTERN const uLongf * ZEXPORT get_crc_table    OF((void));

#ifdef __cplusplus
}
#endif

#endif 
