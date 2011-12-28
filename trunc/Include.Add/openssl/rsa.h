/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef HEADER_RSA_H
#define HEADER_RSA_H

#include <openssl/asn1.h>

#ifndef OPENSSL_NO_BIO
#include <openssl/bio.h>
#endif
#include <openssl/crypto.h>
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef OPENSSL_NO_RSA
#error RSA is disabled.
#endif

#ifdef  __cplusplus
extern "C" {
#endif





struct rsa_meth_st
	{
	const char *name;
	int (*rsa_pub_enc)(int flen,const unsigned char *from,
			   unsigned char *to,
			   RSA *rsa,int padding);
	int (*rsa_pub_dec)(int flen,const unsigned char *from,
			   unsigned char *to,
			   RSA *rsa,int padding);
	int (*rsa_priv_enc)(int flen,const unsigned char *from,
			    unsigned char *to,
			    RSA *rsa,int padding);
	int (*rsa_priv_dec)(int flen,const unsigned char *from,
			    unsigned char *to,
			    RSA *rsa,int padding);
	int (*rsa_mod_exp)(BIGNUM *r0,const BIGNUM *I,RSA *rsa,BN_CTX *ctx); 
	int (*bn_mod_exp)(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
			  const BIGNUM *m, BN_CTX *ctx,
			  BN_MONT_CTX *m_ctx); 
	int (*init)(RSA *rsa);		
	int (*finish)(RSA *rsa);	
	int flags;			
	char *app_data;			

	int (*rsa_sign)(int type,
		const unsigned char *m, unsigned int m_length,
		unsigned char *sigret, unsigned int *siglen, const RSA *rsa);
	int (*rsa_verify)(int dtype,
		const unsigned char *m, unsigned int m_length,
		unsigned char *sigbuf, unsigned int siglen, const RSA *rsa);

	int (*rsa_keygen)(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);
	};

struct rsa_st
	{
	
	int pad;
	long version;
	const RSA_METHOD *meth;
	
	ENGINE *engine;
	BIGNUM *n;
	BIGNUM *e;
	BIGNUM *d;
	BIGNUM *p;
	BIGNUM *q;
	BIGNUM *dmp1;
	BIGNUM *dmq1;
	BIGNUM *iqmp;
	
	CRYPTO_EX_DATA ex_data;
	int references;
	int flags;

	
	BN_MONT_CTX *_method_mod_n;
	BN_MONT_CTX *_method_mod_p;
	BN_MONT_CTX *_method_mod_q;

	
	char *bignum_data;
	BN_BLINDING *blinding;
	BN_BLINDING *mt_blinding;
	};

#ifndef OPENSSL_RSA_MAX_MODULUS_BITS
# define OPENSSL_RSA_MAX_MODULUS_BITS	16384
#endif

#ifndef OPENSSL_RSA_SMALL_MODULUS_BITS
# define OPENSSL_RSA_SMALL_MODULUS_BITS	3072
#endif
#ifndef OPENSSL_RSA_MAX_PUBEXP_BITS
# define OPENSSL_RSA_MAX_PUBEXP_BITS	64 
#endif

#define RSA_3	0x3L
#define RSA_F4	0x10001L

#define RSA_METHOD_FLAG_NO_CHECK	0x0001 

#define RSA_FLAG_CACHE_PUBLIC		0x0002
#define RSA_FLAG_CACHE_PRIVATE		0x0004
#define RSA_FLAG_BLINDING		0x0008
#define RSA_FLAG_THREAD_SAFE		0x0010

#define RSA_FLAG_EXT_PKEY		0x0020


#define RSA_FLAG_SIGN_VER		0x0040

#define RSA_FLAG_NO_BLINDING		0x0080 
#define RSA_FLAG_NO_CONSTTIME		0x0100  
#ifndef OPENSSL_NO_DEPRECATED
#define RSA_FLAG_NO_EXP_CONSTTIME RSA_FLAG_NO_CONSTTIME 
                                                
#endif


#define RSA_PKCS1_PADDING	1
#define RSA_SSLV23_PADDING	2
#define RSA_NO_PADDING		3
#define RSA_PKCS1_OAEP_PADDING	4
#define RSA_X931_PADDING	5

#define RSA_PKCS1_PADDING_SIZE	11

#define RSA_set_app_data(s,arg)         RSA_set_ex_data(s,0,arg)
#define RSA_get_app_data(s)             RSA_get_ex_data(s,0)

RSA *	RSA_new(void);
RSA *	RSA_new_method(ENGINE *engine);
int	RSA_size(const RSA *);


#ifndef OPENSSL_NO_DEPRECATED
RSA *	RSA_generate_key(int bits, unsigned long e,void
		(*callback)(int,int,void *),void *cb_arg);
#endif 


int	RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);

int	RSA_check_key(const RSA *);
	
int	RSA_public_encrypt(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding);
int	RSA_private_encrypt(int flen, const unsigned char *from,
		unsigned char *to, RSA *rsa,int padding);
int	RSA_public_decrypt(int flen, const unsigned char *from, 
		unsigned char *to, RSA *rsa,int padding);
int	RSA_private_decrypt(int flen, const unsigned char *from, 
		unsigned char *to, RSA *rsa,int padding);
void	RSA_free (RSA *r);

int	RSA_up_ref(RSA *r);

int	RSA_flags(const RSA *r);

void RSA_set_default_method(const RSA_METHOD *meth);
const RSA_METHOD *RSA_get_default_method(void);
const RSA_METHOD *RSA_get_method(const RSA *rsa);
int RSA_set_method(RSA *rsa, const RSA_METHOD *meth);


int RSA_memory_lock(RSA *r);


const RSA_METHOD *RSA_PKCS1_SSLeay(void);

const RSA_METHOD *RSA_null_method(void);

DECLARE_ASN1_ENCODE_FUNCTIONS_const(RSA, RSAPublicKey)
DECLARE_ASN1_ENCODE_FUNCTIONS_const(RSA, RSAPrivateKey)

#ifndef OPENSSL_NO_FP_API
int	RSA_print_fp(FILE *fp, const RSA *r,int offset);
#endif

#ifndef OPENSSL_NO_BIO
int	RSA_print(BIO *bp, const RSA *r,int offset);
#endif

int i2d_RSA_NET(const RSA *a, unsigned char **pp,
		int (*cb)(char *buf, int len, const char *prompt, int verify),
		int sgckey);
RSA *d2i_RSA_NET(RSA **a, const unsigned char **pp, long length,
		 int (*cb)(char *buf, int len, const char *prompt, int verify),
		 int sgckey);

int i2d_Netscape_RSA(const RSA *a, unsigned char **pp,
		     int (*cb)(char *buf, int len, const char *prompt,
			       int verify));
RSA *d2i_Netscape_RSA(RSA **a, const unsigned char **pp, long length,
		      int (*cb)(char *buf, int len, const char *prompt,
				int verify));


int RSA_sign(int type, const unsigned char *m, unsigned int m_length,
	unsigned char *sigret, unsigned int *siglen, RSA *rsa);
int RSA_verify(int type, const unsigned char *m, unsigned int m_length,
	unsigned char *sigbuf, unsigned int siglen, RSA *rsa);


int RSA_sign_ASN1_OCTET_STRING(int type,
	const unsigned char *m, unsigned int m_length,
	unsigned char *sigret, unsigned int *siglen, RSA *rsa);
int RSA_verify_ASN1_OCTET_STRING(int type,
	const unsigned char *m, unsigned int m_length,
	unsigned char *sigbuf, unsigned int siglen, RSA *rsa);

int RSA_blinding_on(RSA *rsa, BN_CTX *ctx);
void RSA_blinding_off(RSA *rsa);
BN_BLINDING *RSA_setup_blinding(RSA *rsa, BN_CTX *ctx);

int RSA_padding_add_PKCS1_type_1(unsigned char *to,int tlen,
	const unsigned char *f,int fl);
int RSA_padding_check_PKCS1_type_1(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_PKCS1_type_2(unsigned char *to,int tlen,
	const unsigned char *f,int fl);
int RSA_padding_check_PKCS1_type_2(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len);
int PKCS1_MGF1(unsigned char *mask, long len,
	const unsigned char *seed, long seedlen, const EVP_MD *dgst);
int RSA_padding_add_PKCS1_OAEP(unsigned char *to,int tlen,
	const unsigned char *f,int fl,
	const unsigned char *p,int pl);
int RSA_padding_check_PKCS1_OAEP(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len,
	const unsigned char *p,int pl);
int RSA_padding_add_SSLv23(unsigned char *to,int tlen,
	const unsigned char *f,int fl);
int RSA_padding_check_SSLv23(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_none(unsigned char *to,int tlen,
	const unsigned char *f,int fl);
int RSA_padding_check_none(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_X931(unsigned char *to,int tlen,
	const unsigned char *f,int fl);
int RSA_padding_check_X931(unsigned char *to,int tlen,
	const unsigned char *f,int fl,int rsa_len);
int RSA_X931_hash_id(int nid);

int RSA_verify_PKCS1_PSS(RSA *rsa, const unsigned char *mHash,
			const EVP_MD *Hash, const unsigned char *EM, int sLen);
int RSA_padding_add_PKCS1_PSS(RSA *rsa, unsigned char *EM,
			const unsigned char *mHash,
			const EVP_MD *Hash, int sLen);

int RSA_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int RSA_set_ex_data(RSA *r,int idx,void *arg);
void *RSA_get_ex_data(const RSA *r, int idx);

RSA *RSAPublicKey_dup(RSA *rsa);
RSA *RSAPrivateKey_dup(RSA *rsa);



void ERR_load_RSA_strings(void);




#define RSA_F_MEMORY_LOCK				 100
#define RSA_F_RSA_BUILTIN_KEYGEN			 129
#define RSA_F_RSA_CHECK_KEY				 123
#define RSA_F_RSA_EAY_PRIVATE_DECRYPT			 101
#define RSA_F_RSA_EAY_PRIVATE_ENCRYPT			 102
#define RSA_F_RSA_EAY_PUBLIC_DECRYPT			 103
#define RSA_F_RSA_EAY_PUBLIC_ENCRYPT			 104
#define RSA_F_RSA_GENERATE_KEY				 105
#define RSA_F_RSA_MEMORY_LOCK				 130
#define RSA_F_RSA_NEW_METHOD				 106
#define RSA_F_RSA_NULL					 124
#define RSA_F_RSA_NULL_MOD_EXP				 131
#define RSA_F_RSA_NULL_PRIVATE_DECRYPT			 132
#define RSA_F_RSA_NULL_PRIVATE_ENCRYPT			 133
#define RSA_F_RSA_NULL_PUBLIC_DECRYPT			 134
#define RSA_F_RSA_NULL_PUBLIC_ENCRYPT			 135
#define RSA_F_RSA_PADDING_ADD_NONE			 107
#define RSA_F_RSA_PADDING_ADD_PKCS1_OAEP		 121
#define RSA_F_RSA_PADDING_ADD_PKCS1_PSS			 125
#define RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_1		 108
#define RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_2		 109
#define RSA_F_RSA_PADDING_ADD_SSLV23			 110
#define RSA_F_RSA_PADDING_ADD_X931			 127
#define RSA_F_RSA_PADDING_CHECK_NONE			 111
#define RSA_F_RSA_PADDING_CHECK_PKCS1_OAEP		 122
#define RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_1		 112
#define RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_2		 113
#define RSA_F_RSA_PADDING_CHECK_SSLV23			 114
#define RSA_F_RSA_PADDING_CHECK_X931			 128
#define RSA_F_RSA_PRINT					 115
#define RSA_F_RSA_PRINT_FP				 116
#define RSA_F_RSA_SETUP_BLINDING			 136
#define RSA_F_RSA_SIGN					 117
#define RSA_F_RSA_SIGN_ASN1_OCTET_STRING		 118
#define RSA_F_RSA_VERIFY				 119
#define RSA_F_RSA_VERIFY_ASN1_OCTET_STRING		 120
#define RSA_F_RSA_VERIFY_PKCS1_PSS			 126


#define RSA_R_ALGORITHM_MISMATCH			 100
#define RSA_R_BAD_E_VALUE				 101
#define RSA_R_BAD_FIXED_HEADER_DECRYPT			 102
#define RSA_R_BAD_PAD_BYTE_COUNT			 103
#define RSA_R_BAD_SIGNATURE				 104
#define RSA_R_BLOCK_TYPE_IS_NOT_01			 106
#define RSA_R_BLOCK_TYPE_IS_NOT_02			 107
#define RSA_R_DATA_GREATER_THAN_MOD_LEN			 108
#define RSA_R_DATA_TOO_LARGE				 109
#define RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE		 110
#define RSA_R_DATA_TOO_LARGE_FOR_MODULUS		 132
#define RSA_R_DATA_TOO_SMALL				 111
#define RSA_R_DATA_TOO_SMALL_FOR_KEY_SIZE		 122
#define RSA_R_DIGEST_TOO_BIG_FOR_RSA_KEY		 112
#define RSA_R_DMP1_NOT_CONGRUENT_TO_D			 124
#define RSA_R_DMQ1_NOT_CONGRUENT_TO_D			 125
#define RSA_R_D_E_NOT_CONGRUENT_TO_1			 123
#define RSA_R_FIRST_OCTET_INVALID			 133
#define RSA_R_INVALID_HEADER				 137
#define RSA_R_INVALID_MESSAGE_LENGTH			 131
#define RSA_R_INVALID_PADDING				 138
#define RSA_R_INVALID_TRAILER				 139
#define RSA_R_IQMP_NOT_INVERSE_OF_Q			 126
#define RSA_R_KEY_SIZE_TOO_SMALL			 120
#define RSA_R_LAST_OCTET_INVALID			 134
#define RSA_R_MODULUS_TOO_LARGE				 105
#define RSA_R_NO_PUBLIC_EXPONENT			 140
#define RSA_R_NULL_BEFORE_BLOCK_MISSING			 113
#define RSA_R_N_DOES_NOT_EQUAL_P_Q			 127
#define RSA_R_OAEP_DECODING_ERROR			 121
#define RSA_R_PADDING_CHECK_FAILED			 114
#define RSA_R_P_NOT_PRIME				 128
#define RSA_R_Q_NOT_PRIME				 129
#define RSA_R_RSA_OPERATIONS_NOT_SUPPORTED		 130
#define RSA_R_SLEN_CHECK_FAILED				 136
#define RSA_R_SLEN_RECOVERY_FAILED			 135
#define RSA_R_SSLV3_ROLLBACK_ATTACK			 115
#define RSA_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD 116
#define RSA_R_UNKNOWN_ALGORITHM_TYPE			 117
#define RSA_R_UNKNOWN_PADDING_TYPE			 118
#define RSA_R_WRONG_SIGNATURE_LENGTH			 119

#ifdef  __cplusplus
}
#endif
#endif
