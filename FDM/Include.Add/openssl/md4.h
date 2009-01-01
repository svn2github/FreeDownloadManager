/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_MD4_H
#define HEADER_MD4_H

#include <openssl/e_os2.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_NO_MD4
#error MD4 is disabled.
#endif  

#if defined(OPENSSL_SYS_WIN16) || defined(__LP32__)
#define MD4_LONG unsigned long
#elif defined(OPENSSL_SYS_CRAY) || defined(__ILP64__)
#define MD4_LONG unsigned long
#define MD4_LONG_LOG2 3

#else
#define MD4_LONG unsigned int
#endif

#define MD4_CBLOCK	64
#define MD4_LBLOCK	(MD4_CBLOCK/4)
#define MD4_DIGEST_LENGTH 16

typedef struct MD4state_st
	{
	MD4_LONG A,B,C,D;
	MD4_LONG Nl,Nh;
	MD4_LONG data[MD4_LBLOCK];
	unsigned int num;
	} MD4_CTX;

int MD4_Init(MD4_CTX *c);
int MD4_Update(MD4_CTX *c, const void *data, size_t len);
int MD4_Final(unsigned char *md, MD4_CTX *c);
unsigned char *MD4(const unsigned char *d, size_t n, unsigned char *md);
void MD4_Transform(MD4_CTX *c, const unsigned char *b);
#ifdef  __cplusplus
}
#endif

#endif
