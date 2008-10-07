/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
  

#ifndef HEADER_CAST_H
#define HEADER_CAST_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_CAST
#error CAST is disabled.
#endif

#define CAST_ENCRYPT	1
#define CAST_DECRYPT	0

#define CAST_LONG unsigned long

#define CAST_BLOCK	8
#define CAST_KEY_LENGTH	16

typedef struct cast_key_st
	{
	CAST_LONG data[32];
	int short_key;	
	} CAST_KEY;

 
void CAST_set_key(CAST_KEY *key, int len, const unsigned char *data);
void CAST_ecb_encrypt(const unsigned char *in,unsigned char *out,CAST_KEY *key,
		      int enc);
void CAST_encrypt(CAST_LONG *data,CAST_KEY *key);
void CAST_decrypt(CAST_LONG *data,CAST_KEY *key);
void CAST_cbc_encrypt(const unsigned char *in, unsigned char *out, long length,
		      CAST_KEY *ks, unsigned char *iv, int enc);
void CAST_cfb64_encrypt(const unsigned char *in, unsigned char *out,
			long length, CAST_KEY *schedule, unsigned char *ivec,
			int *num, int enc);
void CAST_ofb64_encrypt(const unsigned char *in, unsigned char *out, 
			long length, CAST_KEY *schedule, unsigned char *ivec,
			int *num);

#ifdef  __cplusplus
}
#endif

#endif
