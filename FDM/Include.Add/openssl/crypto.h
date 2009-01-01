/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

    

#ifndef HEADER_CRYPTO_H
#define HEADER_CRYPTO_H

#include <stdlib.h>

#include <openssl/e_os2.h>

#ifndef OPENSSL_NO_FP_API
#include <stdio.h>
#endif

#include <openssl/stack.h>
#include <openssl/safestack.h>
#include <openssl/opensslv.h>
#include <openssl/ossl_typ.h>

#ifdef CHARSET_EBCDIC
#include <openssl/ebcdic.h>
#endif 

#include <openssl/symhacks.h>

#ifdef  __cplusplus
extern "C" {
#endif  

#define SSLEAY_VERSION_NUMBER	OPENSSL_VERSION_NUMBER
#define SSLEAY_VERSION		0

#define SSLEAY_CFLAGS		2
#define SSLEAY_BUILT_ON		3
#define SSLEAY_PLATFORM		4
#define SSLEAY_DIR		5 

#if 0
typedef struct crypto_ex_data_st CRYPTO_EX_DATA;

typedef int CRYPTO_EX_new(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
					int idx, long argl, void *argp);

typedef void CRYPTO_EX_free(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
					int idx, long argl, void *argp);

typedef int CRYPTO_EX_dup(CRYPTO_EX_DATA *to, CRYPTO_EX_DATA *from, void *from_d, 
					int idx, long argl, void *argp);
#endif 

typedef struct openssl_item_st
	{
	int code;
	void *value;		
	size_t value_size;	
	size_t *value_length;	
	} OPENSSL_ITEM;   

#define	CRYPTO_LOCK_ERR			1
#define	CRYPTO_LOCK_EX_DATA		2
#define	CRYPTO_LOCK_X509		3
#define	CRYPTO_LOCK_X509_INFO		4
#define	CRYPTO_LOCK_X509_PKEY		5
#define CRYPTO_LOCK_X509_CRL		6
#define CRYPTO_LOCK_X509_REQ		7
#define CRYPTO_LOCK_DSA			8
#define CRYPTO_LOCK_RSA			9
#define CRYPTO_LOCK_EVP_PKEY		10
#define CRYPTO_LOCK_X509_STORE		11
#define CRYPTO_LOCK_SSL_CTX		12
#define CRYPTO_LOCK_SSL_CERT		13
#define CRYPTO_LOCK_SSL_SESSION		14
#define CRYPTO_LOCK_SSL_SESS_CERT	15
#define CRYPTO_LOCK_SSL			16
#define CRYPTO_LOCK_SSL_METHOD		17
#define CRYPTO_LOCK_RAND		18
#define CRYPTO_LOCK_RAND2		19
#define CRYPTO_LOCK_MALLOC		20
#define CRYPTO_LOCK_BIO			21
#define CRYPTO_LOCK_GETHOSTBYNAME	22
#define CRYPTO_LOCK_GETSERVBYNAME	23
#define CRYPTO_LOCK_READDIR		24
#define CRYPTO_LOCK_RSA_BLINDING	25
#define CRYPTO_LOCK_DH			26
#define CRYPTO_LOCK_MALLOC2		27
#define CRYPTO_LOCK_DSO			28
#define CRYPTO_LOCK_DYNLOCK		29
#define CRYPTO_LOCK_ENGINE		30
#define CRYPTO_LOCK_UI			31
#define CRYPTO_LOCK_ECDSA               32
#define CRYPTO_LOCK_EC			33
#define CRYPTO_LOCK_ECDH		34
#define CRYPTO_LOCK_BN  		35
#define CRYPTO_LOCK_EC_PRE_COMP		36
#define CRYPTO_LOCK_STORE		37
#define CRYPTO_LOCK_COMP		38
#define CRYPTO_NUM_LOCKS		39

#define CRYPTO_LOCK		1
#define CRYPTO_UNLOCK		2
#define CRYPTO_READ		4
#define CRYPTO_WRITE		8

#ifndef OPENSSL_NO_LOCKING
#ifndef CRYPTO_w_lock
#define CRYPTO_w_lock(type)	\
	CRYPTO_lock(CRYPTO_LOCK|CRYPTO_WRITE,type,__FILE__,__LINE__)
#define CRYPTO_w_unlock(type)	\
	CRYPTO_lock(CRYPTO_UNLOCK|CRYPTO_WRITE,type,__FILE__,__LINE__)
#define CRYPTO_r_lock(type)	\
	CRYPTO_lock(CRYPTO_LOCK|CRYPTO_READ,type,__FILE__,__LINE__)
#define CRYPTO_r_unlock(type)	\
	CRYPTO_lock(CRYPTO_UNLOCK|CRYPTO_READ,type,__FILE__,__LINE__)
#define CRYPTO_add(addr,amount,type)	\
	CRYPTO_add_lock(addr,amount,type,__FILE__,__LINE__)
#endif
#else
#define CRYPTO_w_lock(a)
#define CRYPTO_w_unlock(a)
#define CRYPTO_r_lock(a)
#define CRYPTO_r_unlock(a)
#define CRYPTO_add(a,b,c)	((*(a))+=(b))
#endif  

typedef struct
	{
	int references;
	struct CRYPTO_dynlock_value *data;
	} CRYPTO_dynlock;   

#define CRYPTO_MEM_CHECK_OFF	0x0	
#define CRYPTO_MEM_CHECK_ON	0x1	
#define CRYPTO_MEM_CHECK_ENABLE	0x2	
#define CRYPTO_MEM_CHECK_DISABLE 0x3	   

#define V_CRYPTO_MDEBUG_TIME	0x1 

#define V_CRYPTO_MDEBUG_THREAD	0x2 

#define V_CRYPTO_MDEBUG_ALL (V_CRYPTO_MDEBUG_TIME | V_CRYPTO_MDEBUG_THREAD)  

typedef struct bio_st BIO_dummy;

struct crypto_ex_data_st
	{
	STACK *sk;
	int dummy; 
	};  

typedef struct crypto_ex_data_func_st
	{
	long argl;	
	void *argp;	
	CRYPTO_EX_new *new_func;
	CRYPTO_EX_free *free_func;
	CRYPTO_EX_dup *dup_func;
	} CRYPTO_EX_DATA_FUNCS;

DECLARE_STACK_OF(CRYPTO_EX_DATA_FUNCS)  

#define CRYPTO_EX_INDEX_BIO		0
#define CRYPTO_EX_INDEX_SSL		1
#define CRYPTO_EX_INDEX_SSL_CTX		2
#define CRYPTO_EX_INDEX_SSL_SESSION	3
#define CRYPTO_EX_INDEX_X509_STORE	4
#define CRYPTO_EX_INDEX_X509_STORE_CTX	5
#define CRYPTO_EX_INDEX_RSA		6
#define CRYPTO_EX_INDEX_DSA		7
#define CRYPTO_EX_INDEX_DH		8
#define CRYPTO_EX_INDEX_ENGINE		9
#define CRYPTO_EX_INDEX_X509		10
#define CRYPTO_EX_INDEX_UI		11
#define CRYPTO_EX_INDEX_ECDSA		12
#define CRYPTO_EX_INDEX_ECDH		13
#define CRYPTO_EX_INDEX_COMP		14
#define CRYPTO_EX_INDEX_STORE		15 

#define CRYPTO_EX_INDEX_USER		100  

#define CRYPTO_malloc_init()	CRYPTO_set_mem_functions(\
	malloc, realloc, free)

#if defined CRYPTO_MDEBUG_ALL || defined CRYPTO_MDEBUG_TIME || defined CRYPTO_MDEBUG_THREAD
# ifndef CRYPTO_MDEBUG 
#  define CRYPTO_MDEBUG
# endif
#endif 

#define CRYPTO_malloc_debug_init()	do {\
	CRYPTO_set_mem_debug_functions(\
		CRYPTO_dbg_malloc,\
		CRYPTO_dbg_realloc,\
		CRYPTO_dbg_free,\
		CRYPTO_dbg_set_options,\
		CRYPTO_dbg_get_options);\
	} while(0)

int CRYPTO_mem_ctrl(int mode);
int CRYPTO_is_mem_check_on(void); 

#define MemCheck_start() CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON)
#define MemCheck_stop()	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_OFF) 

#define MemCheck_on()	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ENABLE)
#define MemCheck_off()	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_DISABLE)
#define is_MemCheck_on() CRYPTO_is_mem_check_on()

#define OPENSSL_malloc(num)	CRYPTO_malloc((int)num,__FILE__,__LINE__)
#define OPENSSL_realloc(addr,num) \
	CRYPTO_realloc((char *)addr,(int)num,__FILE__,__LINE__)
#define OPENSSL_realloc_clean(addr,old_num,num) \
	CRYPTO_realloc_clean(addr,old_num,num,__FILE__,__LINE__)
#define OPENSSL_remalloc(addr,num) \
	CRYPTO_remalloc((char **)addr,(int)num,__FILE__,__LINE__)
#define OPENSSL_freeFunc	CRYPTO_free
#define OPENSSL_free(addr)	CRYPTO_free(addr)

#define OPENSSL_malloc_locked(num) \
	CRYPTO_malloc_locked((int)num,__FILE__,__LINE__)
#define OPENSSL_free_locked(addr) CRYPTO_free_locked(addr) 

const char *SSLeay_version(int type);
unsigned long SSLeay(void);

int OPENSSL_issetugid(void); 

typedef struct st_CRYPTO_EX_DATA_IMPL	CRYPTO_EX_DATA_IMPL;

const CRYPTO_EX_DATA_IMPL *CRYPTO_get_ex_data_implementation(void);

int CRYPTO_set_ex_data_implementation(const CRYPTO_EX_DATA_IMPL *i);

int CRYPTO_ex_data_new_class(void);

int CRYPTO_get_ex_new_index(int class_index, long argl, void *argp,
		CRYPTO_EX_new *new_func, CRYPTO_EX_dup *dup_func,
		CRYPTO_EX_free *free_func);

int CRYPTO_new_ex_data(int class_index, void *obj, CRYPTO_EX_DATA *ad);
int CRYPTO_dup_ex_data(int class_index, CRYPTO_EX_DATA *to,
		CRYPTO_EX_DATA *from);
void CRYPTO_free_ex_data(int class_index, void *obj, CRYPTO_EX_DATA *ad);

int CRYPTO_set_ex_data(CRYPTO_EX_DATA *ad, int idx, void *val);
void *CRYPTO_get_ex_data(const CRYPTO_EX_DATA *ad,int idx);

void CRYPTO_cleanup_all_ex_data(void);

int CRYPTO_get_new_lockid(char *name);

int CRYPTO_num_locks(void); 
void CRYPTO_lock(int mode, int type,const char *file,int line);
void CRYPTO_set_locking_callback(void (*func)(int mode,int type,
					      const char *file,int line));
void (*CRYPTO_get_locking_callback(void))(int mode,int type,const char *file,
		int line);
void CRYPTO_set_add_lock_callback(int (*func)(int *num,int mount,int type,
					      const char *file, int line));
int (*CRYPTO_get_add_lock_callback(void))(int *num,int mount,int type,
					  const char *file,int line);
void CRYPTO_set_id_callback(unsigned long (*func)(void));
unsigned long (*CRYPTO_get_id_callback(void))(void);
unsigned long CRYPTO_thread_id(void);
const char *CRYPTO_get_lock_name(int type);
int CRYPTO_add_lock(int *pointer,int amount,int type, const char *file,
		    int line);

int CRYPTO_get_new_dynlockid(void);
void CRYPTO_destroy_dynlockid(int i);
struct CRYPTO_dynlock_value *CRYPTO_get_dynlock_value(int i);
void CRYPTO_set_dynlock_create_callback(struct CRYPTO_dynlock_value *(*dyn_create_function)(const char *file, int line));
void CRYPTO_set_dynlock_lock_callback(void (*dyn_lock_function)(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line));
void CRYPTO_set_dynlock_destroy_callback(void (*dyn_destroy_function)(struct CRYPTO_dynlock_value *l, const char *file, int line));
struct CRYPTO_dynlock_value *(*CRYPTO_get_dynlock_create_callback(void))(const char *file,int line);
void (*CRYPTO_get_dynlock_lock_callback(void))(int mode, struct CRYPTO_dynlock_value *l, const char *file,int line);
void (*CRYPTO_get_dynlock_destroy_callback(void))(struct CRYPTO_dynlock_value *l, const char *file,int line); 

int CRYPTO_set_mem_functions(void *(*m)(size_t),void *(*r)(void *,size_t), void (*f)(void *));
int CRYPTO_set_locked_mem_functions(void *(*m)(size_t), void (*free_func)(void *));
int CRYPTO_set_mem_ex_functions(void *(*m)(size_t,const char *,int),
                                void *(*r)(void *,size_t,const char *,int),
                                void (*f)(void *));
int CRYPTO_set_locked_mem_ex_functions(void *(*m)(size_t,const char *,int),
                                       void (*free_func)(void *));
int CRYPTO_set_mem_debug_functions(void (*m)(void *,int,const char *,int,int),
				   void (*r)(void *,void *,int,const char *,int,int),
				   void (*f)(void *,int),
				   void (*so)(long),
				   long (*go)(void));
void CRYPTO_get_mem_functions(void *(**m)(size_t),void *(**r)(void *, size_t), void (**f)(void *));
void CRYPTO_get_locked_mem_functions(void *(**m)(size_t), void (**f)(void *));
void CRYPTO_get_mem_ex_functions(void *(**m)(size_t,const char *,int),
                                 void *(**r)(void *, size_t,const char *,int),
                                 void (**f)(void *));
void CRYPTO_get_locked_mem_ex_functions(void *(**m)(size_t,const char *,int),
                                        void (**f)(void *));
void CRYPTO_get_mem_debug_functions(void (**m)(void *,int,const char *,int,int),
				    void (**r)(void *,void *,int,const char *,int,int),
				    void (**f)(void *,int),
				    void (**so)(long),
				    long (**go)(void));

void *CRYPTO_malloc_locked(int num, const char *file, int line);
void CRYPTO_free_locked(void *);
void *CRYPTO_malloc(int num, const char *file, int line);
void CRYPTO_free(void *);
void *CRYPTO_realloc(void *addr,int num, const char *file, int line);
void *CRYPTO_realloc_clean(void *addr,int old_num,int num,const char *file,
			   int line);
void *CRYPTO_remalloc(void *addr,int num, const char *file, int line);

void OPENSSL_cleanse(void *ptr, size_t len);

void CRYPTO_set_mem_debug_options(long bits);
long CRYPTO_get_mem_debug_options(void);

#define CRYPTO_push_info(info) \
        CRYPTO_push_info_(info, __FILE__, __LINE__);
int CRYPTO_push_info_(const char *info, const char *file, int line);
int CRYPTO_pop_info(void);
int CRYPTO_remove_all_info(void);   

void CRYPTO_dbg_malloc(void *addr,int num,const char *file,int line,int before_p);
void CRYPTO_dbg_realloc(void *addr1,void *addr2,int num,const char *file,int line,int before_p);
void CRYPTO_dbg_free(void *addr,int before_p);

void CRYPTO_dbg_set_options(long bits);
long CRYPTO_dbg_get_options(void); 

#ifndef OPENSSL_NO_FP_API
void CRYPTO_mem_leaks_fp(FILE *);
#endif
void CRYPTO_mem_leaks(struct bio_st *bio);

typedef void *CRYPTO_MEM_LEAK_CB(unsigned long, const char *, int, int, void *);
void CRYPTO_mem_leaks_cb(CRYPTO_MEM_LEAK_CB *cb); 

void OpenSSLDie(const char *file,int line,const char *assertion);
#define OPENSSL_assert(e)       (void)((e) ? 0 : (OpenSSLDie(__FILE__, __LINE__, #e),1))

unsigned long *OPENSSL_ia32cap_loc(void);
#define OPENSSL_ia32cap (*(OPENSSL_ia32cap_loc()))  

void ERR_load_CRYPTO_strings(void);   

#define CRYPTO_F_CRYPTO_GET_EX_NEW_INDEX		 100
#define CRYPTO_F_CRYPTO_GET_NEW_DYNLOCKID		 103
#define CRYPTO_F_CRYPTO_GET_NEW_LOCKID			 101
#define CRYPTO_F_CRYPTO_SET_EX_DATA			 102
#define CRYPTO_F_DEF_ADD_INDEX				 104
#define CRYPTO_F_DEF_GET_CLASS				 105
#define CRYPTO_F_INT_DUP_EX_DATA			 106
#define CRYPTO_F_INT_FREE_EX_DATA			 107
#define CRYPTO_F_INT_NEW_EX_DATA			 108 

#define CRYPTO_R_NO_DYNLOCK_CREATE_CALLBACK		 100

#ifdef  __cplusplus
}
#endif
#endif
