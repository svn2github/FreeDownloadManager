/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_RIPEMD_H
#define HEADER_RIPEMD_H

#include <openssl/e_os2.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_NO_RIPEMD
#error RIPEMD is disabled.
#endif

#if defined(OPENSSL_SYS_WIN16) || defined(__LP32__)
#define RIPEMD160_LONG unsigned long
#elif defined(OPENSSL_SYS_CRAY) || defined(__ILP64__)
#define RIPEMD160_LONG unsigned long
#define RIPEMD160_LONG_LOG2 3
#else
#define RIPEMD160_LONG unsigned int
#endif

#define RIPEMD160_CBLOCK	64
#define RIPEMD160_LBLOCK	(RIPEMD160_CBLOCK/4)
#define RIPEMD160_DIGEST_LENGTH	20

typedef struct RIPEMD160state_st
	{
	RIPEMD160_LONG A,B,C,D,E;
	RIPEMD160_LONG Nl,Nh;
	RIPEMD160_LONG data[RIPEMD160_LBLOCK];
	unsigned int   num;
	} RIPEMD160_CTX;

int RIPEMD160_Init(RIPEMD160_CTX *c);
int RIPEMD160_Update(RIPEMD160_CTX *c, const void *data, size_t len);
int RIPEMD160_Final(unsigned char *md, RIPEMD160_CTX *c);
unsigned char *RIPEMD160(const unsigned char *d, size_t n,
	unsigned char *md);
void RIPEMD160_Transform(RIPEMD160_CTX *c, const unsigned char *b);
#ifdef  __cplusplus
}
#endif

#endif
