/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
   

#ifndef HEADER_UI_H
#define HEADER_UI_H

#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/crypto.h>
#endif
#include <openssl/safestack.h>
#include <openssl/ossl_typ.h>

#ifdef  __cplusplus
extern "C" {
#endif        

UI *UI_new(void);
UI *UI_new_method(const UI_METHOD *method);
void UI_free(UI *ui); 

int UI_add_input_string(UI *ui, const char *prompt, int flags,
	char *result_buf, int minsize, int maxsize);
int UI_dup_input_string(UI *ui, const char *prompt, int flags,
	char *result_buf, int minsize, int maxsize);
int UI_add_verify_string(UI *ui, const char *prompt, int flags,
	char *result_buf, int minsize, int maxsize, const char *test_buf);
int UI_dup_verify_string(UI *ui, const char *prompt, int flags,
	char *result_buf, int minsize, int maxsize, const char *test_buf);
int UI_add_input_boolean(UI *ui, const char *prompt, const char *action_desc,
	const char *ok_chars, const char *cancel_chars,
	int flags, char *result_buf);
int UI_dup_input_boolean(UI *ui, const char *prompt, const char *action_desc,
	const char *ok_chars, const char *cancel_chars,
	int flags, char *result_buf);
int UI_add_info_string(UI *ui, const char *text);
int UI_dup_info_string(UI *ui, const char *text);
int UI_add_error_string(UI *ui, const char *text);
int UI_dup_error_string(UI *ui, const char *text);  

#define UI_INPUT_FLAG_ECHO		0x01

#define UI_INPUT_FLAG_DEFAULT_PWD	0x02 

#define UI_INPUT_FLAG_USER_BASE	16  

char *UI_construct_prompt(UI *ui_method,
	const char *object_desc, const char *object_name);  

void *UI_add_user_data(UI *ui, void *user_data);

void *UI_get0_user_data(UI *ui); 

const char *UI_get0_result(UI *ui, int i); 

int UI_process(UI *ui); 

int UI_ctrl(UI *ui, int cmd, long i, void *p, void (*f)(void));  

#define UI_CTRL_PRINT_ERRORS		1

#define UI_CTRL_IS_REDOABLE		2  

#define UI_set_app_data(s,arg)         UI_set_ex_data(s,0,arg)
#define UI_get_app_data(s)             UI_get_ex_data(s,0)
int UI_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int UI_set_ex_data(UI *r,int idx,void *arg);
void *UI_get_ex_data(UI *r, int idx); 

void UI_set_default_method(const UI_METHOD *meth);
const UI_METHOD *UI_get_default_method(void);
const UI_METHOD *UI_get_method(UI *ui);
const UI_METHOD *UI_set_method(UI *ui, const UI_METHOD *meth); 

UI_METHOD *UI_OpenSSL(void);     

DECLARE_STACK_OF(UI_STRING)
typedef struct ui_string_st UI_STRING; 

enum UI_string_types
	{
	UIT_NONE=0,
	UIT_PROMPT,		
	UIT_VERIFY,		
	UIT_BOOLEAN,		
	UIT_INFO,		
	UIT_ERROR		
	}; 

UI_METHOD *UI_create_method(char *name);
void UI_destroy_method(UI_METHOD *ui_method);
int UI_method_set_opener(UI_METHOD *method, int (*opener)(UI *ui));
int UI_method_set_writer(UI_METHOD *method, int (*writer)(UI *ui, UI_STRING *uis));
int UI_method_set_flusher(UI_METHOD *method, int (*flusher)(UI *ui));
int UI_method_set_reader(UI_METHOD *method, int (*reader)(UI *ui, UI_STRING *uis));
int UI_method_set_closer(UI_METHOD *method, int (*closer)(UI *ui));
int (*UI_method_get_opener(UI_METHOD *method))(UI*);
int (*UI_method_get_writer(UI_METHOD *method))(UI*,UI_STRING*);
int (*UI_method_get_flusher(UI_METHOD *method))(UI*);
int (*UI_method_get_reader(UI_METHOD *method))(UI*,UI_STRING*);
int (*UI_method_get_closer(UI_METHOD *method))(UI*);   

enum UI_string_types UI_get_string_type(UI_STRING *uis);

int UI_get_input_flags(UI_STRING *uis);

const char *UI_get0_output_string(UI_STRING *uis);

const char *UI_get0_action_string(UI_STRING *uis);

const char *UI_get0_result_string(UI_STRING *uis);

const char *UI_get0_test_string(UI_STRING *uis);

int UI_get_result_minsize(UI_STRING *uis);

int UI_get_result_maxsize(UI_STRING *uis);

int UI_set_result(UI *ui, UI_STRING *uis, const char *result);  

int UI_UTIL_read_pw_string(char *buf,int length,const char *prompt,int verify);
int UI_UTIL_read_pw(char *buf,char *buff,int size,const char *prompt,int verify);   

void ERR_load_UI_strings(void);   

#define UI_F_GENERAL_ALLOCATE_BOOLEAN			 108
#define UI_F_GENERAL_ALLOCATE_PROMPT			 109
#define UI_F_GENERAL_ALLOCATE_STRING			 100
#define UI_F_UI_CTRL					 111
#define UI_F_UI_DUP_ERROR_STRING			 101
#define UI_F_UI_DUP_INFO_STRING				 102
#define UI_F_UI_DUP_INPUT_BOOLEAN			 110
#define UI_F_UI_DUP_INPUT_STRING			 103
#define UI_F_UI_DUP_VERIFY_STRING			 106
#define UI_F_UI_GET0_RESULT				 107
#define UI_F_UI_NEW_METHOD				 104
#define UI_F_UI_SET_RESULT				 105 

#define UI_R_COMMON_OK_AND_CANCEL_CHARACTERS		 104
#define UI_R_INDEX_TOO_LARGE				 102
#define UI_R_INDEX_TOO_SMALL				 103
#define UI_R_NO_RESULT_BUFFER				 105
#define UI_R_RESULT_TOO_LARGE				 100
#define UI_R_RESULT_TOO_SMALL				 101
#define UI_R_UNKNOWN_CONTROL_COMMAND			 106

#ifdef  __cplusplus
}
#endif
#endif
