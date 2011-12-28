/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef HEADER_IDEA_H
#define HEADER_IDEA_H

#include <openssl/opensslconf.h> 

#ifdef OPENSSL_NO_IDEA
#error IDEA is disabled.
#endif

#define IDEA_ENCRYPT	1
#define IDEA_DECRYPT	0

#define IDEA_BLOCK	8
#define IDEA_KEY_LENGTH	16

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct idea_key_st
	{
	IDEA_INT data[9][6];
	} IDEA_KEY_SCHEDULE;

const char *idea_options(void);
void idea_ecb_encrypt(const unsigned char *in, unsigned char *out,
	IDEA_KEY_SCHEDULE *ks);
void idea_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks);
void idea_set_decrypt_key(const IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk);
void idea_cbc_encrypt(const unsigned char *in, unsigned char *out,
	long length, IDEA_KEY_SCHEDULE *ks, unsigned char *iv,int enc);
void idea_cfb64_encrypt(const unsigned char *in, unsigned char *out,
	long length, IDEA_KEY_SCHEDULE *ks, unsigned char *iv,
	int *num,int enc);
void idea_ofb64_encrypt(const unsigned char *in, unsigned char *out,
	long length, IDEA_KEY_SCHEDULE *ks, unsigned char *iv, int *num);
void idea_encrypt(unsigned long *in, IDEA_KEY_SCHEDULE *ks);
#ifdef  __cplusplus
}
#endif

#endif
