/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
   

#ifndef HEADER_BN_H
#define HEADER_BN_H

#include <openssl/e_os2.h>
#ifndef OPENSSL_NO_FP_API
#include <stdio.h> 
#endif
#include <openssl/ossl_typ.h>

#ifdef  __cplusplus
extern "C" {
#endif    

#define BN_MUL_COMBA
#define BN_SQR_COMBA
#define BN_RECURSION 

#if defined(OPENSSL_SYS_MSDOS) || defined(OPENSSL_SYS_WINDOWS) || \
    defined(OPENSSL_SYS_WIN32) || defined(linux)
# ifndef BN_DIV2W
#  define BN_DIV2W
# endif
#endif 

#ifdef SIXTY_FOUR_BIT_LONG
#define BN_ULLONG	unsigned long long
#define BN_ULONG	unsigned long
#define BN_LONG		long
#define BN_BITS		128
#define BN_BYTES	8
#define BN_BITS2	64
#define BN_BITS4	32
#define BN_MASK		(0xffffffffffffffffffffffffffffffffLL)
#define BN_MASK2	(0xffffffffffffffffL)
#define BN_MASK2l	(0xffffffffL)
#define BN_MASK2h	(0xffffffff00000000L)
#define BN_MASK2h1	(0xffffffff80000000L)
#define BN_TBIT		(0x8000000000000000L)
#define BN_DEC_CONV	(10000000000000000000UL)
#define BN_DEC_FMT1	"%lu"
#define BN_DEC_FMT2	"%019lu"
#define BN_DEC_NUM	19
#endif 

#ifdef SIXTY_FOUR_BIT
#undef BN_LLONG
#undef BN_ULLONG
#define BN_ULONG	unsigned long long
#define BN_LONG		long long
#define BN_BITS		128
#define BN_BYTES	8
#define BN_BITS2	64
#define BN_BITS4	32
#define BN_MASK2	(0xffffffffffffffffLL)
#define BN_MASK2l	(0xffffffffL)
#define BN_MASK2h	(0xffffffff00000000LL)
#define BN_MASK2h1	(0xffffffff80000000LL)
#define BN_TBIT		(0x8000000000000000LL)
#define BN_DEC_CONV	(10000000000000000000ULL)
#define BN_DEC_FMT1	"%llu"
#define BN_DEC_FMT2	"%019llu"
#define BN_DEC_NUM	19
#endif

#ifdef THIRTY_TWO_BIT
#ifdef BN_LLONG
# if defined(OPENSSL_SYS_WIN32) && !defined(__GNUC__)
#  define BN_ULLONG	unsigned __int64
# else
#  define BN_ULLONG	unsigned long long
# endif
#endif
#define BN_ULONG	unsigned long
#define BN_LONG		long
#define BN_BITS		64
#define BN_BYTES	4
#define BN_BITS2	32
#define BN_BITS4	16
#ifdef OPENSSL_SYS_WIN32

#define BN_MASK		(0xffffffffffffffffL)
#else
#define BN_MASK		(0xffffffffffffffffLL)
#endif
#define BN_MASK2	(0xffffffffL)
#define BN_MASK2l	(0xffff)
#define BN_MASK2h1	(0xffff8000L)
#define BN_MASK2h	(0xffff0000L)
#define BN_TBIT		(0x80000000L)
#define BN_DEC_CONV	(1000000000L)
#define BN_DEC_FMT1	"%lu"
#define BN_DEC_FMT2	"%09lu"
#define BN_DEC_NUM	9
#endif

#ifdef SIXTEEN_BIT
#ifndef BN_DIV2W
#define BN_DIV2W
#endif
#define BN_ULLONG	unsigned long
#define BN_ULONG	unsigned short
#define BN_LONG		short
#define BN_BITS		32
#define BN_BYTES	2
#define BN_BITS2	16
#define BN_BITS4	8
#define BN_MASK		(0xffffffff)
#define BN_MASK2	(0xffff)
#define BN_MASK2l	(0xff)
#define BN_MASK2h1	(0xff80)
#define BN_MASK2h	(0xff00)
#define BN_TBIT		(0x8000)
#define BN_DEC_CONV	(100000)
#define BN_DEC_FMT1	"%u"
#define BN_DEC_FMT2	"%05u"
#define BN_DEC_NUM	5
#endif

#ifdef EIGHT_BIT
#ifndef BN_DIV2W
#define BN_DIV2W
#endif
#define BN_ULLONG	unsigned short
#define BN_ULONG	unsigned char
#define BN_LONG		char
#define BN_BITS		16
#define BN_BYTES	1
#define BN_BITS2	8
#define BN_BITS4	4
#define BN_MASK		(0xffff)
#define BN_MASK2	(0xff)
#define BN_MASK2l	(0xf)
#define BN_MASK2h1	(0xf8)
#define BN_MASK2h	(0xf0)
#define BN_TBIT		(0x80)
#define BN_DEC_CONV	(100)
#define BN_DEC_FMT1	"%u"
#define BN_DEC_FMT2	"%02u"
#define BN_DEC_NUM	2
#endif

#define BN_DEFAULT_BITS	1280

#define BN_FLG_MALLOCED		0x01
#define BN_FLG_STATIC_DATA	0x02
#define BN_FLG_CONSTTIME	0x04 

#ifndef OPENSSL_NO_DEPRECATED
#define BN_FLG_EXP_CONSTTIME BN_FLG_CONSTTIME 
                                      
#endif

#ifndef OPENSSL_NO_DEPRECATED
#define BN_FLG_FREE		0x8000	
#endif
#define BN_set_flags(b,n)	((b)->flags|=(n))
#define BN_get_flags(b,n)	((b)->flags&(n)) 

#define BN_with_flags(dest,b,n)  ((dest)->d=(b)->d, \
                                  (dest)->top=(b)->top, \
                                  (dest)->dmax=(b)->dmax, \
                                  (dest)->neg=(b)->neg, \
                                  (dest)->flags=(((dest)->flags & BN_FLG_MALLOCED) \
                                                 |  ((b)->flags & ~BN_FLG_MALLOCED) \
                                                 |  BN_FLG_STATIC_DATA \
                                                 |  (n))) 

#if 0
typedef struct bignum_st BIGNUM;

typedef struct bignum_ctx BN_CTX;
typedef struct bn_blinding_st BN_BLINDING;
typedef struct bn_mont_ctx_st BN_MONT_CTX;
typedef struct bn_recp_ctx_st BN_RECP_CTX;
typedef struct bn_gencb_st BN_GENCB;
#endif

struct bignum_st
	{
	BN_ULONG *d;	
	int top;	
	
	int dmax;	
	int neg;	
	int flags;
	}; 

struct bn_mont_ctx_st
	{
	int ri;        
	BIGNUM RR;     
	BIGNUM N;      
	BIGNUM Ni;     
	BN_ULONG n0;   
	int flags;
	}; 

struct bn_recp_ctx_st
	{
	BIGNUM N;	
	BIGNUM Nr;	
	int num_bits;
	int shift;
	int flags;
	}; 

struct bn_gencb_st
	{
	unsigned int ver;	
	void *arg;		
	union
		{
		
		void (*cb_1)(int, int, void *);
		
		int (*cb_2)(int, int, BN_GENCB *);
		} cb;
	};

int BN_GENCB_call(BN_GENCB *cb, int a, int b);

#define BN_GENCB_set_old(gencb, callback, cb_arg) { \
		BN_GENCB *tmp_gencb = (gencb); \
		tmp_gencb->ver = 1; \
		tmp_gencb->arg = (cb_arg); \
		tmp_gencb->cb.cb_1 = (callback); }

#define BN_GENCB_set(gencb, callback, cb_arg) { \
		BN_GENCB *tmp_gencb = (gencb); \
		tmp_gencb->ver = 2; \
		tmp_gencb->arg = (cb_arg); \
		tmp_gencb->cb.cb_2 = (callback); }

#define BN_prime_checks 0  

#define BN_prime_checks_for_size(b) ((b) >= 1300 ?  2 : \
                                (b) >=  850 ?  3 : \
                                (b) >=  650 ?  4 : \
                                (b) >=  550 ?  5 : \
                                (b) >=  450 ?  6 : \
                                (b) >=  400 ?  7 : \
                                (b) >=  350 ?  8 : \
                                (b) >=  300 ?  9 : \
                                (b) >=  250 ? 12 : \
                                (b) >=  200 ? 15 : \
                                (b) >=  150 ? 18 : \
                                 27)

#define BN_num_bytes(a)	((BN_num_bits(a)+7)/8) 

#define BN_abs_is_word(a,w) ((((a)->top == 1) && ((a)->d[0] == (BN_ULONG)(w))) || \
				(((w) == 0) && ((a)->top == 0)))
#define BN_is_zero(a)       ((a)->top == 0)
#define BN_is_one(a)        (BN_abs_is_word((a),1) && !(a)->neg)
#define BN_is_word(a,w)     (BN_abs_is_word((a),(w)) && (!(w) || !(a)->neg))
#define BN_is_odd(a)	    (((a)->top > 0) && ((a)->d[0] & 1))

#define BN_one(a)	(BN_set_word((a),1))
#define BN_zero_ex(a) \
	do { \
		BIGNUM *_tmp_bn = (a); \
		_tmp_bn->top = 0; \
		_tmp_bn->neg = 0; \
	} while(0)
#ifdef OPENSSL_NO_DEPRECATED
#define BN_zero(a)	BN_zero_ex(a)
#else
#define BN_zero(a)	(BN_set_word((a),0))
#endif

const BIGNUM *BN_value_one(void);
char *	BN_options(void);
BN_CTX *BN_CTX_new(void);
#ifndef OPENSSL_NO_DEPRECATED
void	BN_CTX_init(BN_CTX *c);
#endif
void	BN_CTX_free(BN_CTX *c);
void	BN_CTX_start(BN_CTX *ctx);
BIGNUM *BN_CTX_get(BN_CTX *ctx);
void	BN_CTX_end(BN_CTX *ctx);
int     BN_rand(BIGNUM *rnd, int bits, int top,int bottom);
int     BN_pseudo_rand(BIGNUM *rnd, int bits, int top,int bottom);
int	BN_rand_range(BIGNUM *rnd, BIGNUM *range);
int	BN_pseudo_rand_range(BIGNUM *rnd, BIGNUM *range);
int	BN_num_bits(const BIGNUM *a);
int	BN_num_bits_word(BN_ULONG);
BIGNUM *BN_new(void);
void	BN_init(BIGNUM *);
void	BN_clear_free(BIGNUM *a);
BIGNUM *BN_copy(BIGNUM *a, const BIGNUM *b);
void	BN_swap(BIGNUM *a, BIGNUM *b);
BIGNUM *BN_bin2bn(const unsigned char *s,int len,BIGNUM *ret);
int	BN_bn2bin(const BIGNUM *a, unsigned char *to);
BIGNUM *BN_mpi2bn(const unsigned char *s,int len,BIGNUM *ret);
int	BN_bn2mpi(const BIGNUM *a, unsigned char *to);
int	BN_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_usub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_uadd(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, BN_CTX *ctx);
int	BN_sqr(BIGNUM *r, const BIGNUM *a,BN_CTX *ctx);

void	BN_set_negative(BIGNUM *b, int n);

#define BN_is_negative(a) ((a)->neg != 0)

int	BN_div(BIGNUM *dv, BIGNUM *rem, const BIGNUM *m, const BIGNUM *d,
	BN_CTX *ctx);
#define BN_mod(rem,m,d,ctx) BN_div(NULL,(rem),(m),(d),(ctx))
int	BN_nnmod(BIGNUM *r, const BIGNUM *m, const BIGNUM *d, BN_CTX *ctx);
int	BN_mod_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_add_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m);
int	BN_mod_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_sub_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m);
int	BN_mod_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_sqr(BIGNUM *r, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_lshift1(BIGNUM *r, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_lshift1_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *m);
int	BN_mod_lshift(BIGNUM *r, const BIGNUM *a, int n, const BIGNUM *m, BN_CTX *ctx);
int	BN_mod_lshift_quick(BIGNUM *r, const BIGNUM *a, int n, const BIGNUM *m);

BN_ULONG BN_mod_word(const BIGNUM *a, BN_ULONG w);
BN_ULONG BN_div_word(BIGNUM *a, BN_ULONG w);
int	BN_mul_word(BIGNUM *a, BN_ULONG w);
int	BN_add_word(BIGNUM *a, BN_ULONG w);
int	BN_sub_word(BIGNUM *a, BN_ULONG w);
int	BN_set_word(BIGNUM *a, BN_ULONG w);
BN_ULONG BN_get_word(const BIGNUM *a);

int	BN_cmp(const BIGNUM *a, const BIGNUM *b);
void	BN_free(BIGNUM *a);
int	BN_is_bit_set(const BIGNUM *a, int n);
int	BN_lshift(BIGNUM *r, const BIGNUM *a, int n);
int	BN_lshift1(BIGNUM *r, const BIGNUM *a);
int	BN_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,BN_CTX *ctx);

int	BN_mod_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	const BIGNUM *m,BN_CTX *ctx);
int	BN_mod_exp_mont(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int BN_mod_exp_mont_consttime(BIGNUM *rr, const BIGNUM *a, const BIGNUM *p,
	const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *in_mont);
int	BN_mod_exp_mont_word(BIGNUM *r, BN_ULONG a, const BIGNUM *p,
	const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int	BN_mod_exp2_mont(BIGNUM *r, const BIGNUM *a1, const BIGNUM *p1,
	const BIGNUM *a2, const BIGNUM *p2,const BIGNUM *m,
	BN_CTX *ctx,BN_MONT_CTX *m_ctx);
int	BN_mod_exp_simple(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	const BIGNUM *m,BN_CTX *ctx);

int	BN_mask_bits(BIGNUM *a,int n);
#ifndef OPENSSL_NO_FP_API
int	BN_print_fp(FILE *fp, const BIGNUM *a);
#endif
#ifdef HEADER_BIO_H
int	BN_print(BIO *fp, const BIGNUM *a);
#else
int	BN_print(void *fp, const BIGNUM *a);
#endif
int	BN_reciprocal(BIGNUM *r, const BIGNUM *m, int len, BN_CTX *ctx);
int	BN_rshift(BIGNUM *r, const BIGNUM *a, int n);
int	BN_rshift1(BIGNUM *r, const BIGNUM *a);
void	BN_clear(BIGNUM *a);
BIGNUM *BN_dup(const BIGNUM *a);
int	BN_ucmp(const BIGNUM *a, const BIGNUM *b);
int	BN_set_bit(BIGNUM *a, int n);
int	BN_clear_bit(BIGNUM *a, int n);
char *	BN_bn2hex(const BIGNUM *a);
char *	BN_bn2dec(const BIGNUM *a);
int 	BN_hex2bn(BIGNUM **a, const char *str);
int 	BN_dec2bn(BIGNUM **a, const char *str);
int	BN_gcd(BIGNUM *r,const BIGNUM *a,const BIGNUM *b,BN_CTX *ctx);
int	BN_kronecker(const BIGNUM *a,const BIGNUM *b,BN_CTX *ctx); 
BIGNUM *BN_mod_inverse(BIGNUM *ret,
	const BIGNUM *a, const BIGNUM *n,BN_CTX *ctx);
BIGNUM *BN_mod_sqrt(BIGNUM *ret,
	const BIGNUM *a, const BIGNUM *n,BN_CTX *ctx); 

#ifndef OPENSSL_NO_DEPRECATED
BIGNUM *BN_generate_prime(BIGNUM *ret,int bits,int safe,
	const BIGNUM *add, const BIGNUM *rem,
	void (*callback)(int,int,void *),void *cb_arg);
int	BN_is_prime(const BIGNUM *p,int nchecks,
	void (*callback)(int,int,void *),
	BN_CTX *ctx,void *cb_arg);
int	BN_is_prime_fasttest(const BIGNUM *p,int nchecks,
	void (*callback)(int,int,void *),BN_CTX *ctx,void *cb_arg,
	int do_trial_division);
#endif  

int	BN_generate_prime_ex(BIGNUM *ret,int bits,int safe, const BIGNUM *add,
		const BIGNUM *rem, BN_GENCB *cb);
int	BN_is_prime_ex(const BIGNUM *p,int nchecks, BN_CTX *ctx, BN_GENCB *cb);
int	BN_is_prime_fasttest_ex(const BIGNUM *p,int nchecks, BN_CTX *ctx,
		int do_trial_division, BN_GENCB *cb);

BN_MONT_CTX *BN_MONT_CTX_new(void );
void BN_MONT_CTX_init(BN_MONT_CTX *ctx);
int BN_mod_mul_montgomery(BIGNUM *r,const BIGNUM *a,const BIGNUM *b,
	BN_MONT_CTX *mont, BN_CTX *ctx);
#define BN_to_montgomery(r,a,mont,ctx)	BN_mod_mul_montgomery(\
	(r),(a),&((mont)->RR),(mont),(ctx))
int BN_from_montgomery(BIGNUM *r,const BIGNUM *a,
	BN_MONT_CTX *mont, BN_CTX *ctx);
void BN_MONT_CTX_free(BN_MONT_CTX *mont);
int BN_MONT_CTX_set(BN_MONT_CTX *mont,const BIGNUM *mod,BN_CTX *ctx);
BN_MONT_CTX *BN_MONT_CTX_copy(BN_MONT_CTX *to,BN_MONT_CTX *from);
BN_MONT_CTX *BN_MONT_CTX_set_locked(BN_MONT_CTX **pmont, int lock,
					const BIGNUM *mod, BN_CTX *ctx); 

#define	BN_BLINDING_NO_UPDATE	0x00000001
#define	BN_BLINDING_NO_RECREATE	0x00000002

BN_BLINDING *BN_BLINDING_new(const BIGNUM *A, const BIGNUM *Ai,  BIGNUM *mod);
void BN_BLINDING_free(BN_BLINDING *b);
int BN_BLINDING_update(BN_BLINDING *b,BN_CTX *ctx);
int BN_BLINDING_convert(BIGNUM *n, BN_BLINDING *b, BN_CTX *ctx);
int BN_BLINDING_invert(BIGNUM *n, BN_BLINDING *b, BN_CTX *ctx);
int BN_BLINDING_convert_ex(BIGNUM *n, BIGNUM *r, BN_BLINDING *b, BN_CTX *);
int BN_BLINDING_invert_ex(BIGNUM *n, const BIGNUM *r, BN_BLINDING *b, BN_CTX *);
unsigned long BN_BLINDING_get_thread_id(const BN_BLINDING *);
void BN_BLINDING_set_thread_id(BN_BLINDING *, unsigned long);
unsigned long BN_BLINDING_get_flags(const BN_BLINDING *);
void BN_BLINDING_set_flags(BN_BLINDING *, unsigned long);
BN_BLINDING *BN_BLINDING_create_param(BN_BLINDING *b,
	const BIGNUM *e,  BIGNUM *m, BN_CTX *ctx,
	int (*bn_mod_exp)(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
			  const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx),
	BN_MONT_CTX *m_ctx);

#ifndef OPENSSL_NO_DEPRECATED
void BN_set_params(int mul,int high,int low,int mont);
int BN_get_params(int which); 
#endif

void	BN_RECP_CTX_init(BN_RECP_CTX *recp);
BN_RECP_CTX *BN_RECP_CTX_new(void);
void	BN_RECP_CTX_free(BN_RECP_CTX *recp);
int	BN_RECP_CTX_set(BN_RECP_CTX *recp,const BIGNUM *rdiv,BN_CTX *ctx);
int	BN_mod_mul_reciprocal(BIGNUM *r, const BIGNUM *x, const BIGNUM *y,
	BN_RECP_CTX *recp,BN_CTX *ctx);
int	BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	const BIGNUM *m, BN_CTX *ctx);
int	BN_div_recp(BIGNUM *dv, BIGNUM *rem, const BIGNUM *m,
	BN_RECP_CTX *recp, BN_CTX *ctx);  

int	BN_GF2m_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b); 
#define BN_GF2m_sub(r, a, b) BN_GF2m_add(r, a, b)
int	BN_GF2m_mod(BIGNUM *r, const BIGNUM *a, const BIGNUM *p); 
int	BN_GF2m_mod_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const BIGNUM *p, BN_CTX *ctx); 
int	BN_GF2m_mod_sqr(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	BN_CTX *ctx); 
int	BN_GF2m_mod_inv(BIGNUM *r, const BIGNUM *b, const BIGNUM *p,
	BN_CTX *ctx); 
int	BN_GF2m_mod_div(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const BIGNUM *p, BN_CTX *ctx); 
int	BN_GF2m_mod_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const BIGNUM *p, BN_CTX *ctx); 
int	BN_GF2m_mod_sqrt(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	BN_CTX *ctx); 
int	BN_GF2m_mod_solve_quad(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
	BN_CTX *ctx); 
#define BN_GF2m_cmp(a, b) BN_ucmp((a), (b))

int	BN_GF2m_mod_arr(BIGNUM *r, const BIGNUM *a, const unsigned int p[]);
	
int	BN_GF2m_mod_mul_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const unsigned int p[], BN_CTX *ctx); 
int	BN_GF2m_mod_sqr_arr(BIGNUM *r, const BIGNUM *a, const unsigned int p[],
	BN_CTX *ctx); 
int	BN_GF2m_mod_inv_arr(BIGNUM *r, const BIGNUM *b, const unsigned int p[],
	BN_CTX *ctx); 
int	BN_GF2m_mod_div_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const unsigned int p[], BN_CTX *ctx); 
int	BN_GF2m_mod_exp_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
	const unsigned int p[], BN_CTX *ctx); 
int	BN_GF2m_mod_sqrt_arr(BIGNUM *r, const BIGNUM *a,
	const unsigned int p[], BN_CTX *ctx); 
int	BN_GF2m_mod_solve_quad_arr(BIGNUM *r, const BIGNUM *a,
	const unsigned int p[], BN_CTX *ctx); 
int	BN_GF2m_poly2arr(const BIGNUM *a, unsigned int p[], int max);
int	BN_GF2m_arr2poly(const unsigned int p[], BIGNUM *a); 

int BN_nist_mod_192(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_224(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_256(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_384(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_521(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);

const BIGNUM *BN_get0_nist_prime_192(void);
const BIGNUM *BN_get0_nist_prime_224(void);
const BIGNUM *BN_get0_nist_prime_256(void);
const BIGNUM *BN_get0_nist_prime_384(void);
const BIGNUM *BN_get0_nist_prime_521(void);  

#define bn_expand(a,bits) ((((((bits+BN_BITS2-1))/BN_BITS2)) <= (a)->dmax)?\
	(a):bn_expand2((a),(bits+BN_BITS2-1)/BN_BITS2))
#define bn_wexpand(a,words) (((words) <= (a)->dmax)?(a):bn_expand2((a),(words)))
BIGNUM *bn_expand2(BIGNUM *a, int words);
#ifndef OPENSSL_NO_DEPRECATED
BIGNUM *bn_dup_expand(const BIGNUM *a, int words); 
#endif  

#ifdef BN_DEBUG 

#include <assert.h>

#ifdef BN_DEBUG_RAND

#ifndef RAND_pseudo_bytes
int RAND_pseudo_bytes(unsigned char *buf,int num);
#define BN_DEBUG_TRIX
#endif
#define bn_pollute(a) \
	do { \
		const BIGNUM *_bnum1 = (a); \
		if(_bnum1->top < _bnum1->dmax) { \
			unsigned char _tmp_char; \
			 \
			BN_ULONG *_not_const; \
			memcpy(&_not_const, &_bnum1->d, sizeof(BN_ULONG*)); \
			RAND_pseudo_bytes(&_tmp_char, 1); \
			memset((unsigned char *)(_not_const + _bnum1->top), _tmp_char, \
				(_bnum1->dmax - _bnum1->top) * sizeof(BN_ULONG)); \
		} \
	} while(0)
#ifdef BN_DEBUG_TRIX
#undef RAND_pseudo_bytes
#endif
#else
#define bn_pollute(a)
#endif
#define bn_check_top(a) \
	do { \
		const BIGNUM *_bnum2 = (a); \
		if (_bnum2 != NULL) { \
			assert((_bnum2->top == 0) || \
				(_bnum2->d[_bnum2->top - 1] != 0)); \
			bn_pollute(_bnum2); \
		} \
	} while(0)

#define bn_fix_top(a)		bn_check_top(a)

#else 

#define bn_pollute(a)
#define bn_check_top(a)
#define bn_fix_top(a)		bn_correct_top(a)

#endif

#define bn_correct_top(a) \
        { \
        BN_ULONG *ftl; \
	if ((a)->top > 0) \
		{ \
		for (ftl= &((a)->d[(a)->top-1]); (a)->top > 0; (a)->top--) \
		if (*(ftl--)) break; \
		} \
	bn_pollute(a); \
	}

BN_ULONG bn_mul_add_words(BN_ULONG *rp, const BN_ULONG *ap, int num, BN_ULONG w);
BN_ULONG bn_mul_words(BN_ULONG *rp, const BN_ULONG *ap, int num, BN_ULONG w);
void     bn_sqr_words(BN_ULONG *rp, const BN_ULONG *ap, int num);
BN_ULONG bn_div_words(BN_ULONG h, BN_ULONG l, BN_ULONG d);
BN_ULONG bn_add_words(BN_ULONG *rp, const BN_ULONG *ap, const BN_ULONG *bp,int num);
BN_ULONG bn_sub_words(BN_ULONG *rp, const BN_ULONG *ap, const BN_ULONG *bp,int num); 

BIGNUM *get_rfc2409_prime_768(BIGNUM *bn);
BIGNUM *get_rfc2409_prime_1024(BIGNUM *bn); 

BIGNUM *get_rfc3526_prime_1536(BIGNUM *bn);
BIGNUM *get_rfc3526_prime_2048(BIGNUM *bn);
BIGNUM *get_rfc3526_prime_3072(BIGNUM *bn);
BIGNUM *get_rfc3526_prime_4096(BIGNUM *bn);
BIGNUM *get_rfc3526_prime_6144(BIGNUM *bn);
BIGNUM *get_rfc3526_prime_8192(BIGNUM *bn);

int BN_bntest_rand(BIGNUM *rnd, int bits, int top,int bottom);  

void ERR_load_BN_strings(void);   

#define BN_F_BNRAND					 127
#define BN_F_BN_BLINDING_CONVERT_EX			 100
#define BN_F_BN_BLINDING_CREATE_PARAM			 128
#define BN_F_BN_BLINDING_INVERT_EX			 101
#define BN_F_BN_BLINDING_NEW				 102
#define BN_F_BN_BLINDING_UPDATE				 103
#define BN_F_BN_BN2DEC					 104
#define BN_F_BN_BN2HEX					 105
#define BN_F_BN_CTX_GET					 116
#define BN_F_BN_CTX_NEW					 106
#define BN_F_BN_CTX_START				 129
#define BN_F_BN_DIV					 107
#define BN_F_BN_DIV_NO_BRANCH				 138
#define BN_F_BN_DIV_RECP				 130
#define BN_F_BN_EXP					 123
#define BN_F_BN_EXPAND2					 108
#define BN_F_BN_EXPAND_INTERNAL				 120
#define BN_F_BN_GF2M_MOD				 131
#define BN_F_BN_GF2M_MOD_EXP				 132
#define BN_F_BN_GF2M_MOD_MUL				 133
#define BN_F_BN_GF2M_MOD_SOLVE_QUAD			 134
#define BN_F_BN_GF2M_MOD_SOLVE_QUAD_ARR			 135
#define BN_F_BN_GF2M_MOD_SQR				 136
#define BN_F_BN_GF2M_MOD_SQRT				 137
#define BN_F_BN_MOD_EXP2_MONT				 118
#define BN_F_BN_MOD_EXP_MONT				 109
#define BN_F_BN_MOD_EXP_MONT_CONSTTIME			 124
#define BN_F_BN_MOD_EXP_MONT_WORD			 117
#define BN_F_BN_MOD_EXP_RECP				 125
#define BN_F_BN_MOD_EXP_SIMPLE				 126
#define BN_F_BN_MOD_INVERSE				 110
#define BN_F_BN_MOD_INVERSE_NO_BRANCH			 139
#define BN_F_BN_MOD_LSHIFT_QUICK			 119
#define BN_F_BN_MOD_MUL_RECIPROCAL			 111
#define BN_F_BN_MOD_SQRT				 121
#define BN_F_BN_MPI2BN					 112
#define BN_F_BN_NEW					 113
#define BN_F_BN_RAND					 114
#define BN_F_BN_RAND_RANGE				 122
#define BN_F_BN_USUB					 115 

#define BN_R_ARG2_LT_ARG3				 100
#define BN_R_BAD_RECIPROCAL				 101
#define BN_R_BIGNUM_TOO_LONG				 114
#define BN_R_CALLED_WITH_EVEN_MODULUS			 102
#define BN_R_DIV_BY_ZERO				 103
#define BN_R_ENCODING_ERROR				 104
#define BN_R_EXPAND_ON_STATIC_BIGNUM_DATA		 105
#define BN_R_INPUT_NOT_REDUCED				 110
#define BN_R_INVALID_LENGTH				 106
#define BN_R_INVALID_RANGE				 115
#define BN_R_NOT_A_SQUARE				 111
#define BN_R_NOT_INITIALIZED				 107
#define BN_R_NO_INVERSE					 108
#define BN_R_NO_SOLUTION				 116
#define BN_R_P_IS_NOT_PRIME				 112
#define BN_R_TOO_MANY_ITERATIONS			 113
#define BN_R_TOO_MANY_TEMPORARY_VARIABLES		 109

#ifdef  __cplusplus
}
#endif
#endif
