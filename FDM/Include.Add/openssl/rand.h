/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#ifndef HEADER_RAND_H
#define HEADER_RAND_H

#include <stdlib.h>
#include <openssl/ossl_typ.h>
#include <openssl/e_os2.h>

#if defined(OPENSSL_SYS_WINDOWS)
#include <windows.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#if defined(OPENSSL_FIPS)
#define FIPS_RAND_SIZE_T size_t
#endif   

struct rand_meth_st
	{
	void (*seed)(const void *buf, int num);
	int (*bytes)(unsigned char *buf, int num);
	void (*cleanup)(void);
	void (*add)(const void *buf, int num, double entropy);
	int (*pseudorand)(unsigned char *buf, int num);
	int (*status)(void);
	};

#ifdef BN_DEBUG
extern int rand_predictable;
#endif

int RAND_set_rand_method(const RAND_METHOD *meth);
const RAND_METHOD *RAND_get_rand_method(void);
#ifndef OPENSSL_NO_ENGINE
int RAND_set_rand_engine(ENGINE *engine);
#endif
RAND_METHOD *RAND_SSLeay(void);
void RAND_cleanup(void );
int  RAND_bytes(unsigned char *buf,int num);
int  RAND_pseudo_bytes(unsigned char *buf,int num);
void RAND_seed(const void *buf,int num);
void RAND_add(const void *buf,int num,double entropy);
int  RAND_load_file(const char *file,long max_bytes);
int  RAND_write_file(const char *file);
const char *RAND_file_name(char *file,size_t num);
int RAND_status(void);
int RAND_query_egd_bytes(const char *path, unsigned char *buf, int bytes);
int RAND_egd(const char *path);
int RAND_egd_bytes(const char *path,int bytes);
int RAND_poll(void);

#if defined(OPENSSL_SYS_WINDOWS) || defined(OPENSSL_SYS_WIN32)

void RAND_screen(void);
int RAND_event(UINT, WPARAM, LPARAM);

#endif  

void ERR_load_RAND_strings(void);   

#define RAND_F_RAND_GET_RAND_METHOD			 101
#define RAND_F_SSLEAY_RAND_BYTES			 100 

#define RAND_R_PRNG_NOT_SEEDED				 100

#ifdef  __cplusplus
}
#endif
#endif
