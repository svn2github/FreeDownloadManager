/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_BLOWFISH_H
#define HEADER_BLOWFISH_H

#include <openssl/e_os2.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_NO_BF
#error BF is disabled.
#endif

#define BF_ENCRYPT	1
#define BF_DECRYPT	0  

#if defined(OPENSSL_SYS_WIN16) || defined(__LP32__)
#define BF_LONG unsigned long
#elif defined(OPENSSL_SYS_CRAY) || defined(__ILP64__)
#define BF_LONG unsigned long
#define BF_LONG_LOG2 3

#else
#define BF_LONG unsigned int
#endif

#define BF_ROUNDS	16
#define BF_BLOCK	8

typedef struct bf_key_st
	{
	BF_LONG P[BF_ROUNDS+2];
	BF_LONG S[4*256];
	} BF_KEY;

 
void BF_set_key(BF_KEY *key, int len, const unsigned char *data);

void BF_encrypt(BF_LONG *data,const BF_KEY *key);
void BF_decrypt(BF_LONG *data,const BF_KEY *key);

void BF_ecb_encrypt(const unsigned char *in, unsigned char *out,
	const BF_KEY *key, int enc);
void BF_cbc_encrypt(const unsigned char *in, unsigned char *out, long length,
	const BF_KEY *schedule, unsigned char *ivec, int enc);
void BF_cfb64_encrypt(const unsigned char *in, unsigned char *out, long length,
	const BF_KEY *schedule, unsigned char *ivec, int *num, int enc);
void BF_ofb64_encrypt(const unsigned char *in, unsigned char *out, long length,
	const BF_KEY *schedule, unsigned char *ivec, int *num);
const char *BF_options(void);

#ifdef  __cplusplus
}
#endif

#endif
