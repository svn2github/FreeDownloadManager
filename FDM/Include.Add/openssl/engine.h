/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

    

#ifndef HEADER_ENGINE_H
#define HEADER_ENGINE_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_ENGINE
#error ENGINE is disabled.
#endif

#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#ifndef OPENSSL_NO_RSA
#include <openssl/rsa.h>
#endif
#ifndef OPENSSL_NO_DSA
#include <openssl/dsa.h>
#endif
#ifndef OPENSSL_NO_DH
#include <openssl/dh.h>
#endif
#ifndef OPENSSL_NO_ECDH
#include <openssl/ecdh.h>
#endif
#ifndef OPENSSL_NO_ECDSA
#include <openssl/ecdsa.h>
#endif
#include <openssl/rand.h>
#include <openssl/store.h>
#include <openssl/ui.h>
#include <openssl/err.h>
#endif

#include <openssl/ossl_typ.h>
#include <openssl/symhacks.h>

#ifdef  __cplusplus
extern "C" {
#endif 

#define ENGINE_METHOD_RSA		(unsigned int)0x0001
#define ENGINE_METHOD_DSA		(unsigned int)0x0002
#define ENGINE_METHOD_DH		(unsigned int)0x0004
#define ENGINE_METHOD_RAND		(unsigned int)0x0008
#define ENGINE_METHOD_ECDH		(unsigned int)0x0010
#define ENGINE_METHOD_ECDSA		(unsigned int)0x0020
#define ENGINE_METHOD_CIPHERS		(unsigned int)0x0040
#define ENGINE_METHOD_DIGESTS		(unsigned int)0x0080
#define ENGINE_METHOD_STORE		(unsigned int)0x0100

#define ENGINE_METHOD_ALL		(unsigned int)0xFFFF
#define ENGINE_METHOD_NONE		(unsigned int)0x0000 

#define ENGINE_TABLE_FLAG_NOINIT	(unsigned int)0x0001 

  

#define ENGINE_FLAGS_MANUAL_CMD_CTRL	(int)0x0002 

#define ENGINE_FLAGS_BY_ID_COPY		(int)0x0004   

#define ENGINE_CMD_FLAG_NUMERIC		(unsigned int)0x0001

#define ENGINE_CMD_FLAG_STRING		(unsigned int)0x0002

#define ENGINE_CMD_FLAG_NO_INPUT	(unsigned int)0x0004

#define ENGINE_CMD_FLAG_INTERNAL	(unsigned int)0x0008   

#define ENGINE_CTRL_SET_LOGSTREAM		1
#define ENGINE_CTRL_SET_PASSWORD_CALLBACK	2
#define ENGINE_CTRL_HUP				3 
#define ENGINE_CTRL_SET_USER_INTERFACE          4 
#define ENGINE_CTRL_SET_CALLBACK_DATA           5 
#define ENGINE_CTRL_LOAD_CONFIGURATION		6 
#define ENGINE_CTRL_LOAD_SECTION		7    

#define ENGINE_CTRL_HAS_CTRL_FUNCTION		10

#define ENGINE_CTRL_GET_FIRST_CMD_TYPE		11

#define ENGINE_CTRL_GET_NEXT_CMD_TYPE		12

#define ENGINE_CTRL_GET_CMD_FROM_NAME		13

#define ENGINE_CTRL_GET_NAME_LEN_FROM_CMD	14
#define ENGINE_CTRL_GET_NAME_FROM_CMD		15

#define ENGINE_CTRL_GET_DESC_LEN_FROM_CMD	16
#define ENGINE_CTRL_GET_DESC_FROM_CMD		17

#define ENGINE_CTRL_GET_CMD_FLAGS		18 

#define ENGINE_CMD_BASE				200   

#define ENGINE_CTRL_CHIL_SET_FORKCHECK		100
	
#define ENGINE_CTRL_CHIL_NO_LOCKING		101
	 

typedef struct ENGINE_CMD_DEFN_st
	{
	unsigned int cmd_num; 
	const char *cmd_name; 
	const char *cmd_desc; 
	unsigned int cmd_flags; 
	} ENGINE_CMD_DEFN; 

typedef int (*ENGINE_GEN_FUNC_PTR)(void);

typedef int (*ENGINE_GEN_INT_FUNC_PTR)(ENGINE *);

typedef int (*ENGINE_CTRL_FUNC_PTR)(ENGINE *, int, long, void *, void (*f)(void));

typedef EVP_PKEY * (*ENGINE_LOAD_KEY_PTR)(ENGINE *, const char *,
	UI_METHOD *ui_method, void *callback_data); 

typedef int (*ENGINE_CIPHERS_PTR)(ENGINE *, const EVP_CIPHER **, const int **, int);
typedef int (*ENGINE_DIGESTS_PTR)(ENGINE *, const EVP_MD **, const int **, int);   

ENGINE *ENGINE_get_first(void);
ENGINE *ENGINE_get_last(void);

ENGINE *ENGINE_get_next(ENGINE *e);
ENGINE *ENGINE_get_prev(ENGINE *e);

int ENGINE_add(ENGINE *e);

int ENGINE_remove(ENGINE *e);

ENGINE *ENGINE_by_id(const char *id);

void ENGINE_load_openssl(void);
void ENGINE_load_dynamic(void);
#ifndef OPENSSL_NO_STATIC_ENGINE
void ENGINE_load_4758cca(void);
void ENGINE_load_aep(void);
void ENGINE_load_atalla(void);
void ENGINE_load_chil(void);
void ENGINE_load_cswift(void);
#ifndef OPENSSL_NO_GMP
void ENGINE_load_gmp(void);
#endif
void ENGINE_load_nuron(void);
void ENGINE_load_sureware(void);
void ENGINE_load_ubsec(void);
#endif
void ENGINE_load_cryptodev(void);
void ENGINE_load_padlock(void);
void ENGINE_load_builtin_engines(void); 

unsigned int ENGINE_get_table_flags(void);
void ENGINE_set_table_flags(unsigned int flags);  

int ENGINE_register_RSA(ENGINE *e);
void ENGINE_unregister_RSA(ENGINE *e);
void ENGINE_register_all_RSA(void);

int ENGINE_register_DSA(ENGINE *e);
void ENGINE_unregister_DSA(ENGINE *e);
void ENGINE_register_all_DSA(void);

int ENGINE_register_ECDH(ENGINE *e);
void ENGINE_unregister_ECDH(ENGINE *e);
void ENGINE_register_all_ECDH(void);

int ENGINE_register_ECDSA(ENGINE *e);
void ENGINE_unregister_ECDSA(ENGINE *e);
void ENGINE_register_all_ECDSA(void);

int ENGINE_register_DH(ENGINE *e);
void ENGINE_unregister_DH(ENGINE *e);
void ENGINE_register_all_DH(void);

int ENGINE_register_RAND(ENGINE *e);
void ENGINE_unregister_RAND(ENGINE *e);
void ENGINE_register_all_RAND(void);

int ENGINE_register_STORE(ENGINE *e);
void ENGINE_unregister_STORE(ENGINE *e);
void ENGINE_register_all_STORE(void);

int ENGINE_register_ciphers(ENGINE *e);
void ENGINE_unregister_ciphers(ENGINE *e);
void ENGINE_register_all_ciphers(void);

int ENGINE_register_digests(ENGINE *e);
void ENGINE_unregister_digests(ENGINE *e);
void ENGINE_register_all_digests(void); 

int ENGINE_register_complete(ENGINE *e);
int ENGINE_register_all_complete(void); 

int ENGINE_ctrl(ENGINE *e, int cmd, long i, void *p, void (*f)(void)); 

int ENGINE_cmd_is_executable(ENGINE *e, int cmd); 

int ENGINE_ctrl_cmd(ENGINE *e, const char *cmd_name,
        long i, void *p, void (*f)(void), int cmd_optional); 

int ENGINE_ctrl_cmd_string(ENGINE *e, const char *cmd_name, const char *arg,
				int cmd_optional); 

ENGINE *ENGINE_new(void);
int ENGINE_free(ENGINE *e);
int ENGINE_up_ref(ENGINE *e);
int ENGINE_set_id(ENGINE *e, const char *id);
int ENGINE_set_name(ENGINE *e, const char *name);
int ENGINE_set_RSA(ENGINE *e, const RSA_METHOD *rsa_meth);
int ENGINE_set_DSA(ENGINE *e, const DSA_METHOD *dsa_meth);
int ENGINE_set_ECDH(ENGINE *e, const ECDH_METHOD *ecdh_meth);
int ENGINE_set_ECDSA(ENGINE *e, const ECDSA_METHOD *ecdsa_meth);
int ENGINE_set_DH(ENGINE *e, const DH_METHOD *dh_meth);
int ENGINE_set_RAND(ENGINE *e, const RAND_METHOD *rand_meth);
int ENGINE_set_STORE(ENGINE *e, const STORE_METHOD *store_meth);
int ENGINE_set_destroy_function(ENGINE *e, ENGINE_GEN_INT_FUNC_PTR destroy_f);
int ENGINE_set_init_function(ENGINE *e, ENGINE_GEN_INT_FUNC_PTR init_f);
int ENGINE_set_finish_function(ENGINE *e, ENGINE_GEN_INT_FUNC_PTR finish_f);
int ENGINE_set_ctrl_function(ENGINE *e, ENGINE_CTRL_FUNC_PTR ctrl_f);
int ENGINE_set_load_privkey_function(ENGINE *e, ENGINE_LOAD_KEY_PTR loadpriv_f);
int ENGINE_set_load_pubkey_function(ENGINE *e, ENGINE_LOAD_KEY_PTR loadpub_f);
int ENGINE_set_ciphers(ENGINE *e, ENGINE_CIPHERS_PTR f);
int ENGINE_set_digests(ENGINE *e, ENGINE_DIGESTS_PTR f);
int ENGINE_set_flags(ENGINE *e, int flags);
int ENGINE_set_cmd_defns(ENGINE *e, const ENGINE_CMD_DEFN *defns);

int ENGINE_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
		CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int ENGINE_set_ex_data(ENGINE *e, int idx, void *arg);
void *ENGINE_get_ex_data(const ENGINE *e, int idx); 

void ENGINE_cleanup(void); 

const char *ENGINE_get_id(const ENGINE *e);
const char *ENGINE_get_name(const ENGINE *e);
const RSA_METHOD *ENGINE_get_RSA(const ENGINE *e);
const DSA_METHOD *ENGINE_get_DSA(const ENGINE *e);
const ECDH_METHOD *ENGINE_get_ECDH(const ENGINE *e);
const ECDSA_METHOD *ENGINE_get_ECDSA(const ENGINE *e);
const DH_METHOD *ENGINE_get_DH(const ENGINE *e);
const RAND_METHOD *ENGINE_get_RAND(const ENGINE *e);
const STORE_METHOD *ENGINE_get_STORE(const ENGINE *e);
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_destroy_function(const ENGINE *e);
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_init_function(const ENGINE *e);
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_finish_function(const ENGINE *e);
ENGINE_CTRL_FUNC_PTR ENGINE_get_ctrl_function(const ENGINE *e);
ENGINE_LOAD_KEY_PTR ENGINE_get_load_privkey_function(const ENGINE *e);
ENGINE_LOAD_KEY_PTR ENGINE_get_load_pubkey_function(const ENGINE *e);
ENGINE_CIPHERS_PTR ENGINE_get_ciphers(const ENGINE *e);
ENGINE_DIGESTS_PTR ENGINE_get_digests(const ENGINE *e);
const EVP_CIPHER *ENGINE_get_cipher(ENGINE *e, int nid);
const EVP_MD *ENGINE_get_digest(ENGINE *e, int nid);
const ENGINE_CMD_DEFN *ENGINE_get_cmd_defns(const ENGINE *e);
int ENGINE_get_flags(const ENGINE *e);   

int ENGINE_init(ENGINE *e);

int ENGINE_finish(ENGINE *e); 

EVP_PKEY *ENGINE_load_private_key(ENGINE *e, const char *key_id,
	UI_METHOD *ui_method, void *callback_data);
EVP_PKEY *ENGINE_load_public_key(ENGINE *e, const char *key_id,
	UI_METHOD *ui_method, void *callback_data); 

ENGINE *ENGINE_get_default_RSA(void);

ENGINE *ENGINE_get_default_DSA(void);
ENGINE *ENGINE_get_default_ECDH(void);
ENGINE *ENGINE_get_default_ECDSA(void);
ENGINE *ENGINE_get_default_DH(void);
ENGINE *ENGINE_get_default_RAND(void);

ENGINE *ENGINE_get_cipher_engine(int nid);
ENGINE *ENGINE_get_digest_engine(int nid); 

int ENGINE_set_default_RSA(ENGINE *e);
int ENGINE_set_default_string(ENGINE *e, const char *def_list);

int ENGINE_set_default_DSA(ENGINE *e);
int ENGINE_set_default_ECDH(ENGINE *e);
int ENGINE_set_default_ECDSA(ENGINE *e);
int ENGINE_set_default_DH(ENGINE *e);
int ENGINE_set_default_RAND(ENGINE *e);
int ENGINE_set_default_ciphers(ENGINE *e);
int ENGINE_set_default_digests(ENGINE *e); 

int ENGINE_set_default(ENGINE *e, unsigned int flags);

void ENGINE_add_conf_module(void);        

#define OSSL_DYNAMIC_VERSION		(unsigned long)0x00020000

#define OSSL_DYNAMIC_OLDEST		(unsigned long)0x00020000 

typedef void *(*dyn_MEM_malloc_cb)(size_t);
typedef void *(*dyn_MEM_realloc_cb)(void *, size_t);
typedef void (*dyn_MEM_free_cb)(void *);
typedef struct st_dynamic_MEM_fns {
	dyn_MEM_malloc_cb			malloc_cb;
	dyn_MEM_realloc_cb			realloc_cb;
	dyn_MEM_free_cb				free_cb;
	} dynamic_MEM_fns;

typedef void (*dyn_lock_locking_cb)(int,int,const char *,int);
typedef int (*dyn_lock_add_lock_cb)(int*,int,int,const char *,int);
typedef struct CRYPTO_dynlock_value *(*dyn_dynlock_create_cb)(
						const char *,int);
typedef void (*dyn_dynlock_lock_cb)(int,struct CRYPTO_dynlock_value *,
						const char *,int);
typedef void (*dyn_dynlock_destroy_cb)(struct CRYPTO_dynlock_value *,
						const char *,int);
typedef struct st_dynamic_LOCK_fns {
	dyn_lock_locking_cb			lock_locking_cb;
	dyn_lock_add_lock_cb			lock_add_lock_cb;
	dyn_dynlock_create_cb			dynlock_create_cb;
	dyn_dynlock_lock_cb			dynlock_lock_cb;
	dyn_dynlock_destroy_cb			dynlock_destroy_cb;
	} dynamic_LOCK_fns;

typedef struct st_dynamic_fns {
	void 					*static_state;
	const ERR_FNS				*err_fns;
	const CRYPTO_EX_DATA_IMPL		*ex_data_fns;
	dynamic_MEM_fns				mem_fns;
	dynamic_LOCK_fns			lock_fns;
	} dynamic_fns; 

typedef unsigned long (*dynamic_v_check_fn)(unsigned long ossl_version);
#define IMPLEMENT_DYNAMIC_CHECK_FN() \
	OPENSSL_EXPORT unsigned long v_check(unsigned long v) { \
		if(v >= OSSL_DYNAMIC_OLDEST) return OSSL_DYNAMIC_VERSION; \
		return 0; } 

typedef int (*dynamic_bind_engine)(ENGINE *e, const char *id,
				const dynamic_fns *fns);
#define IMPLEMENT_DYNAMIC_BIND_FN(fn) \
	OPENSSL_EXPORT \
	int bind_engine(ENGINE *e, const char *id, const dynamic_fns *fns) { \
		if(ENGINE_get_static_state() == fns->static_state) goto skip_cbs; \
		if(!CRYPTO_set_mem_functions(fns->mem_fns.malloc_cb, \
			fns->mem_fns.realloc_cb, fns->mem_fns.free_cb)) \
			return 0; \
		CRYPTO_set_locking_callback(fns->lock_fns.lock_locking_cb); \
		CRYPTO_set_add_lock_callback(fns->lock_fns.lock_add_lock_cb); \
		CRYPTO_set_dynlock_create_callback(fns->lock_fns.dynlock_create_cb); \
		CRYPTO_set_dynlock_lock_callback(fns->lock_fns.dynlock_lock_cb); \
		CRYPTO_set_dynlock_destroy_callback(fns->lock_fns.dynlock_destroy_cb); \
		if(!CRYPTO_set_ex_data_implementation(fns->ex_data_fns)) \
			return 0; \
		if(!ERR_set_implementation(fns->err_fns)) return 0; \
	skip_cbs: \
		if(!fn(e,id)) return 0; \
		return 1; } 

void *ENGINE_get_static_state(void);

#if defined(__OpenBSD__) || defined(__FreeBSD__)
void ENGINE_setup_bsd_cryptodev(void);
#endif  

void ERR_load_ENGINE_strings(void);   

#define ENGINE_F_DYNAMIC_CTRL				 180
#define ENGINE_F_DYNAMIC_GET_DATA_CTX			 181
#define ENGINE_F_DYNAMIC_LOAD				 182
#define ENGINE_F_DYNAMIC_SET_DATA_CTX			 183
#define ENGINE_F_ENGINE_ADD				 105
#define ENGINE_F_ENGINE_BY_ID				 106
#define ENGINE_F_ENGINE_CMD_IS_EXECUTABLE		 170
#define ENGINE_F_ENGINE_CTRL				 142
#define ENGINE_F_ENGINE_CTRL_CMD			 178
#define ENGINE_F_ENGINE_CTRL_CMD_STRING			 171
#define ENGINE_F_ENGINE_FINISH				 107
#define ENGINE_F_ENGINE_FREE_UTIL			 108
#define ENGINE_F_ENGINE_GET_CIPHER			 185
#define ENGINE_F_ENGINE_GET_DEFAULT_TYPE		 177
#define ENGINE_F_ENGINE_GET_DIGEST			 186
#define ENGINE_F_ENGINE_GET_NEXT			 115
#define ENGINE_F_ENGINE_GET_PREV			 116
#define ENGINE_F_ENGINE_INIT				 119
#define ENGINE_F_ENGINE_LIST_ADD			 120
#define ENGINE_F_ENGINE_LIST_REMOVE			 121
#define ENGINE_F_ENGINE_LOAD_PRIVATE_KEY		 150
#define ENGINE_F_ENGINE_LOAD_PUBLIC_KEY			 151
#define ENGINE_F_ENGINE_NEW				 122
#define ENGINE_F_ENGINE_REMOVE				 123
#define ENGINE_F_ENGINE_SET_DEFAULT_STRING		 189
#define ENGINE_F_ENGINE_SET_DEFAULT_TYPE		 126
#define ENGINE_F_ENGINE_SET_ID				 129
#define ENGINE_F_ENGINE_SET_NAME			 130
#define ENGINE_F_ENGINE_TABLE_REGISTER			 184
#define ENGINE_F_ENGINE_UNLOAD_KEY			 152
#define ENGINE_F_ENGINE_UNLOCKED_FINISH			 191
#define ENGINE_F_ENGINE_UP_REF				 190
#define ENGINE_F_INT_CTRL_HELPER			 172
#define ENGINE_F_INT_ENGINE_CONFIGURE			 188
#define ENGINE_F_INT_ENGINE_MODULE_INIT			 187
#define ENGINE_F_LOG_MESSAGE				 141 

#define ENGINE_R_ALREADY_LOADED				 100
#define ENGINE_R_ARGUMENT_IS_NOT_A_NUMBER		 133
#define ENGINE_R_CMD_NOT_EXECUTABLE			 134
#define ENGINE_R_COMMAND_TAKES_INPUT			 135
#define ENGINE_R_COMMAND_TAKES_NO_INPUT			 136
#define ENGINE_R_CONFLICTING_ENGINE_ID			 103
#define ENGINE_R_CTRL_COMMAND_NOT_IMPLEMENTED		 119
#define ENGINE_R_DH_NOT_IMPLEMENTED			 139
#define ENGINE_R_DSA_NOT_IMPLEMENTED			 140
#define ENGINE_R_DSO_FAILURE				 104
#define ENGINE_R_DSO_NOT_FOUND				 132
#define ENGINE_R_ENGINES_SECTION_ERROR			 148
#define ENGINE_R_ENGINE_IS_NOT_IN_LIST			 105
#define ENGINE_R_ENGINE_SECTION_ERROR			 149
#define ENGINE_R_FAILED_LOADING_PRIVATE_KEY		 128
#define ENGINE_R_FAILED_LOADING_PUBLIC_KEY		 129
#define ENGINE_R_FINISH_FAILED				 106
#define ENGINE_R_GET_HANDLE_FAILED			 107
#define ENGINE_R_ID_OR_NAME_MISSING			 108
#define ENGINE_R_INIT_FAILED				 109
#define ENGINE_R_INTERNAL_LIST_ERROR			 110
#define ENGINE_R_INVALID_ARGUMENT			 143
#define ENGINE_R_INVALID_CMD_NAME			 137
#define ENGINE_R_INVALID_CMD_NUMBER			 138
#define ENGINE_R_INVALID_INIT_VALUE			 151
#define ENGINE_R_INVALID_STRING				 150
#define ENGINE_R_NOT_INITIALISED			 117
#define ENGINE_R_NOT_LOADED				 112
#define ENGINE_R_NO_CONTROL_FUNCTION			 120
#define ENGINE_R_NO_INDEX				 144
#define ENGINE_R_NO_LOAD_FUNCTION			 125
#define ENGINE_R_NO_REFERENCE				 130
#define ENGINE_R_NO_SUCH_ENGINE				 116
#define ENGINE_R_NO_UNLOAD_FUNCTION			 126
#define ENGINE_R_PROVIDE_PARAMETERS			 113
#define ENGINE_R_RSA_NOT_IMPLEMENTED			 141
#define ENGINE_R_UNIMPLEMENTED_CIPHER			 146
#define ENGINE_R_UNIMPLEMENTED_DIGEST			 147
#define ENGINE_R_VERSION_INCOMPATIBILITY		 145

#ifdef  __cplusplus
}
#endif
#endif
