/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
  

#ifndef HEADER_NEW_DES_H
#define HEADER_NEW_DES_H

#include <openssl/e_os2.h>	

#ifdef OPENSSL_NO_DES
#error DES is disabled.
#endif

#ifdef OPENSSL_BUILD_SHLIBCRYPTO
# undef OPENSSL_EXTERN
# define OPENSSL_EXTERN OPENSSL_EXPORT
#endif

#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned char DES_cblock[8];
typedef  unsigned char const_DES_cblock[8]; 

typedef struct DES_ks
    {
    union
	{
	DES_cblock cblock;
	
	DES_LONG deslong[2];
	} ks[16];
    } DES_key_schedule;

#ifndef OPENSSL_DISABLE_OLD_DES_SUPPORT
# ifndef OPENSSL_ENABLE_OLD_DES_SUPPORT
#  define OPENSSL_ENABLE_OLD_DES_SUPPORT
# endif
#endif

#ifdef OPENSSL_ENABLE_OLD_DES_SUPPORT
# include <openssl/des_old.h>
#endif

#define DES_KEY_SZ 	(sizeof(DES_cblock))
#define DES_SCHEDULE_SZ (sizeof(DES_key_schedule))

#define DES_ENCRYPT	1
#define DES_DECRYPT	0

#define DES_CBC_MODE	0
#define DES_PCBC_MODE	1

#define DES_ecb2_encrypt(i,o,k1,k2,e) \
	DES_ecb3_encrypt((i),(o),(k1),(k2),(k1),(e))

#define DES_ede2_cbc_encrypt(i,o,l,k1,k2,iv,e) \
	DES_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(e))

#define DES_ede2_cfb64_encrypt(i,o,l,k1,k2,iv,n,e) \
	DES_ede3_cfb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n),(e))

#define DES_ede2_ofb64_encrypt(i,o,l,k1,k2,iv,n) \
	DES_ede3_ofb64_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(n))

OPENSSL_DECLARE_GLOBAL(int,DES_check_key);	
#define DES_check_key OPENSSL_GLOBAL_REF(DES_check_key)
OPENSSL_DECLARE_GLOBAL(int,DES_rw_mode);	
#define DES_rw_mode OPENSSL_GLOBAL_REF(DES_rw_mode)

const char *DES_options(void);
void DES_ecb3_encrypt(const_DES_cblock *input, DES_cblock *output,
		      DES_key_schedule *ks1,DES_key_schedule *ks2,
		      DES_key_schedule *ks3, int enc);
DES_LONG DES_cbc_cksum(const unsigned char *input,DES_cblock *output,
		       long length,DES_key_schedule *schedule,
		       const_DES_cblock *ivec);

void DES_cbc_encrypt(const unsigned char *input,unsigned char *output,
		     long length,DES_key_schedule *schedule,DES_cblock *ivec,
		     int enc);
void DES_ncbc_encrypt(const unsigned char *input,unsigned char *output,
		      long length,DES_key_schedule *schedule,DES_cblock *ivec,
		      int enc);
void DES_xcbc_encrypt(const unsigned char *input,unsigned char *output,
		      long length,DES_key_schedule *schedule,DES_cblock *ivec,
		      const_DES_cblock *inw,const_DES_cblock *outw,int enc);
void DES_cfb_encrypt(const unsigned char *in,unsigned char *out,int numbits,
		     long length,DES_key_schedule *schedule,DES_cblock *ivec,
		     int enc);
void DES_ecb_encrypt(const_DES_cblock *input,DES_cblock *output,
		     DES_key_schedule *ks,int enc); 

void DES_encrypt1(DES_LONG *data,DES_key_schedule *ks, int enc); 

void DES_encrypt2(DES_LONG *data,DES_key_schedule *ks, int enc);

void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1,
		  DES_key_schedule *ks2, DES_key_schedule *ks3);
void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1,
		  DES_key_schedule *ks2, DES_key_schedule *ks3);
void DES_ede3_cbc_encrypt(const unsigned char *input,unsigned char *output, 
			  long length,
			  DES_key_schedule *ks1,DES_key_schedule *ks2,
			  DES_key_schedule *ks3,DES_cblock *ivec,int enc);
void DES_ede3_cbcm_encrypt(const unsigned char *in,unsigned char *out,
			   long length,
			   DES_key_schedule *ks1,DES_key_schedule *ks2,
			   DES_key_schedule *ks3,
			   DES_cblock *ivec1,DES_cblock *ivec2,
			   int enc);
void DES_ede3_cfb64_encrypt(const unsigned char *in,unsigned char *out,
			    long length,DES_key_schedule *ks1,
			    DES_key_schedule *ks2,DES_key_schedule *ks3,
			    DES_cblock *ivec,int *num,int enc);
void DES_ede3_cfb_encrypt(const unsigned char *in,unsigned char *out,
			  int numbits,long length,DES_key_schedule *ks1,
			  DES_key_schedule *ks2,DES_key_schedule *ks3,
			  DES_cblock *ivec,int enc);
void DES_ede3_ofb64_encrypt(const unsigned char *in,unsigned char *out,
			    long length,DES_key_schedule *ks1,
			    DES_key_schedule *ks2,DES_key_schedule *ks3,
			    DES_cblock *ivec,int *num);

void DES_xwhite_in2out(const_DES_cblock *DES_key,const_DES_cblock *in_white,
		       DES_cblock *out_white);

int DES_enc_read(int fd,void *buf,int len,DES_key_schedule *sched,
		 DES_cblock *iv);
int DES_enc_write(int fd,const void *buf,int len,DES_key_schedule *sched,
		  DES_cblock *iv);
char *DES_fcrypt(const char *buf,const char *salt, char *ret);
char *DES_crypt(const char *buf,const char *salt);
void DES_ofb_encrypt(const unsigned char *in,unsigned char *out,int numbits,
		     long length,DES_key_schedule *schedule,DES_cblock *ivec);
void DES_pcbc_encrypt(const unsigned char *input,unsigned char *output,
		      long length,DES_key_schedule *schedule,DES_cblock *ivec,
		      int enc);
DES_LONG DES_quad_cksum(const unsigned char *input,DES_cblock output[],
			long length,int out_count,DES_cblock *seed);
int DES_random_key(DES_cblock *ret);
void DES_set_odd_parity(DES_cblock *key);
int DES_check_key_parity(const_DES_cblock *key);
int DES_is_weak_key(const_DES_cblock *key);

int DES_set_key(const_DES_cblock *key,DES_key_schedule *schedule);
int DES_key_sched(const_DES_cblock *key,DES_key_schedule *schedule);
int DES_set_key_checked(const_DES_cblock *key,DES_key_schedule *schedule);
void DES_set_key_unchecked(const_DES_cblock *key,DES_key_schedule *schedule);
void DES_string_to_key(const char *str,DES_cblock *key);
void DES_string_to_2keys(const char *str,DES_cblock *key1,DES_cblock *key2);
void DES_cfb64_encrypt(const unsigned char *in,unsigned char *out,long length,
		       DES_key_schedule *schedule,DES_cblock *ivec,int *num,
		       int enc);
void DES_ofb64_encrypt(const unsigned char *in,unsigned char *out,long length,
		       DES_key_schedule *schedule,DES_cblock *ivec,int *num);

int DES_read_password(DES_cblock *key, const char *prompt, int verify);
int DES_read_2passwords(DES_cblock *key1, DES_cblock *key2, const char *prompt,
	int verify);

#define DES_fixup_key_parity DES_set_odd_parity

#ifdef  __cplusplus
}
#endif

#endif
