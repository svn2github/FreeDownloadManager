/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/    

#ifndef _SHA2_H
#define _SHA2_H

#include <limits.h>    

#define sfx_lo(x,y) x##y
#define sfx_hi(x,y) sfx_lo(x,y)
#define n_u32(p)    sfx_hi(0x##p,s_u32)
#define n_u64(p)    sfx_hi(0x##p,s_u64)    

#if UINT_MAX == 0xffffffff
  typedef   unsigned int     sha2_32t;
  #define s_u32    u
#elif ULONG_MAX == 0xffffffff
  typedef   unsigned long    sha2_32t;
  #define s_u32   ul
#else
#error Please define sha2_32t as an unsigned 32 bit type in sha2.h
#endif    

#if defined( _MSC_VER )
  typedef unsigned __int64   sha2_64t;
  #define s_u64 ui64
#elif ULONG_MAX == 0xffffffffffffffff
  typedef unsigned long      sha2_64t;
  #define s_u64   ul
#elif ULONG_MAX == 0xffffffff
  typedef unsigned long long sha2_64t;   
  #define s_u64  ull
#else
#error Please define sha2_64t as an unsigned 64 bit type in sha2.h
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

#define SHA256_DIGEST_SIZE  32
#define SHA384_DIGEST_SIZE  48
#define SHA512_DIGEST_SIZE  64

#define SHA256_BLOCK_SIZE   64
#define SHA384_BLOCK_SIZE  128
#define SHA512_BLOCK_SIZE  128

#define SHA2_DIGEST_SIZE        SHA256_DIGEST_SIZE
#define SHA2_MAX_DIGEST_SIZE    SHA512_DIGEST_SIZE

#define SHA2_GOOD   0
#define SHA2_BAD    1    

typedef struct
{   sha2_32t count[2];
    sha2_32t hash[8];
    sha2_32t wbuf[16];
} sha256_ctx;    

typedef struct
{   sha2_64t count[2];
    sha2_64t hash[8];
    sha2_64t wbuf[16];
} sha512_ctx;

typedef sha512_ctx  sha384_ctx;    

typedef struct
{   union
    {   sha256_ctx  ctx256[1];
        sha512_ctx  ctx512[1];
    } uu[1];
    sha2_32t    sha2_len;
} sha2_ctx;

void sha256_compile(sha256_ctx ctx[1]);
void sha512_compile(sha512_ctx ctx[1]);

void sha256_begin(sha256_ctx ctx[1]);
void sha256_hash(const unsigned char data[], unsigned long len, sha256_ctx ctx[1]);
void sha256_end(unsigned char hval[], sha256_ctx ctx[1]);
void sha256(unsigned char hval[], const unsigned char data[], unsigned long len); 

void sha384_begin(sha384_ctx ctx[1]);
#define sha384_hash sha512_hash
void sha384_end(unsigned char hval[], sha384_ctx ctx[1]);
void sha384(unsigned char hval[], const unsigned char data[], unsigned long len); 

void sha512_begin(sha512_ctx ctx[1]);
void sha512_hash(const unsigned char data[], unsigned long len, sha512_ctx ctx[1]);
void sha512_end(unsigned char hval[], sha512_ctx ctx[1]);
void sha512(unsigned char hval[], const unsigned char data[], unsigned long len); 

int sha2_begin(unsigned long size, sha2_ctx ctx[1]);
void sha2_hash(const unsigned char data[], unsigned long len, sha2_ctx ctx[1]);
void sha2_end(unsigned char hval[], sha2_ctx ctx[1]);
int sha2(unsigned char hval[], unsigned long size, const unsigned char data[], unsigned long len); 

#if defined(__cplusplus)
}
#endif

#include "sha2.cpp"

#endif
