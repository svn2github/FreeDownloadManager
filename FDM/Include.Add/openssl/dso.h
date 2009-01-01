/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

   

#ifndef HEADER_DSO_H
#define HEADER_DSO_H

#include <openssl/crypto.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define DSO_CTRL_GET_FLAGS	1
#define DSO_CTRL_SET_FLAGS	2
#define DSO_CTRL_OR_FLAGS	3 

#define DSO_FLAG_NO_NAME_TRANSLATION		0x01

#define DSO_FLAG_NAME_TRANSLATION_EXT_ONLY	0x02 

#define DSO_FLAG_UPCASE_SYMBOL			0x10 

#define DSO_FLAG_GLOBAL_SYMBOLS			0x20 

typedef void (*DSO_FUNC_TYPE)(void);

typedef struct dso_st DSO; 

typedef char* (*DSO_NAME_CONVERTER_FUNC)(DSO *, const char *);

typedef char* (*DSO_MERGER_FUNC)(DSO *, const char *, const char *);

typedef struct dso_meth_st
	{
	const char *name;
	
	int (*dso_load)(DSO *dso);
	
	int (*dso_unload)(DSO *dso);
	
	void *(*dso_bind_var)(DSO *dso, const char *symname);
	
	DSO_FUNC_TYPE (*dso_bind_func)(DSO *dso, const char *symname); 

#if 0
	
	int (*dso_unbind_var)(DSO *dso, char *symname, void *symptr);
	
	int (*dso_unbind_func)(DSO *dso, char *symname, DSO_FUNC_TYPE symptr);
#endif
	
	long (*dso_ctrl)(DSO *dso, int cmd, long larg, void *parg);
	
	DSO_NAME_CONVERTER_FUNC dso_name_converter;
	
	DSO_MERGER_FUNC dso_merger;

	
	int (*init)(DSO *dso);
	int (*finish)(DSO *dso);
	} DSO_METHOD;   

struct dso_st
	{
	DSO_METHOD *meth;
	
	STACK *meth_data;
	int references;
	int flags;
	
	CRYPTO_EX_DATA ex_data;
	
	DSO_NAME_CONVERTER_FUNC name_converter;
	
	DSO_MERGER_FUNC merger;
	
	char *filename;
	
	char *loaded_filename;
	}; 

DSO *	DSO_new(void);
DSO *	DSO_new_method(DSO_METHOD *method);
int	DSO_free(DSO *dso);
int	DSO_flags(DSO *dso);
int	DSO_up_ref(DSO *dso);
long	DSO_ctrl(DSO *dso, int cmd, long larg, void *parg); 

int	DSO_set_name_converter(DSO *dso, DSO_NAME_CONVERTER_FUNC cb,
				DSO_NAME_CONVERTER_FUNC *oldcb);

const char *DSO_get_filename(DSO *dso);
int	DSO_set_filename(DSO *dso, const char *filename);

char	*DSO_convert_filename(DSO *dso, const char *filename);

char	*DSO_merge(DSO *dso, const char *filespec1, const char *filespec2);

const char *DSO_get_loaded_filename(DSO *dso);

void	DSO_set_default_method(DSO_METHOD *meth);
DSO_METHOD *DSO_get_default_method(void);
DSO_METHOD *DSO_get_method(DSO *dso);
DSO_METHOD *DSO_set_method(DSO *dso, DSO_METHOD *meth); 

DSO *DSO_load(DSO *dso, const char *filename, DSO_METHOD *meth, int flags); 

void *DSO_bind_var(DSO *dso, const char *symname); 

DSO_FUNC_TYPE DSO_bind_func(DSO *dso, const char *symname); 

DSO_METHOD *DSO_METHOD_openssl(void); 

DSO_METHOD *DSO_METHOD_null(void); 

DSO_METHOD *DSO_METHOD_dlfcn(void); 

DSO_METHOD *DSO_METHOD_dl(void); 

DSO_METHOD *DSO_METHOD_win32(void); 

DSO_METHOD *DSO_METHOD_vms(void);  

void ERR_load_DSO_strings(void);   

#define DSO_F_DLFCN_BIND_FUNC				 100
#define DSO_F_DLFCN_BIND_VAR				 101
#define DSO_F_DLFCN_LOAD				 102
#define DSO_F_DLFCN_MERGER				 130
#define DSO_F_DLFCN_NAME_CONVERTER			 123
#define DSO_F_DLFCN_UNLOAD				 103
#define DSO_F_DL_BIND_FUNC				 104
#define DSO_F_DL_BIND_VAR				 105
#define DSO_F_DL_LOAD					 106
#define DSO_F_DL_MERGER					 131
#define DSO_F_DL_NAME_CONVERTER				 124
#define DSO_F_DL_UNLOAD					 107
#define DSO_F_DSO_BIND_FUNC				 108
#define DSO_F_DSO_BIND_VAR				 109
#define DSO_F_DSO_CONVERT_FILENAME			 126
#define DSO_F_DSO_CTRL					 110
#define DSO_F_DSO_FREE					 111
#define DSO_F_DSO_GET_FILENAME				 127
#define DSO_F_DSO_GET_LOADED_FILENAME			 128
#define DSO_F_DSO_LOAD					 112
#define DSO_F_DSO_MERGE					 132
#define DSO_F_DSO_NEW_METHOD				 113
#define DSO_F_DSO_SET_FILENAME				 129
#define DSO_F_DSO_SET_NAME_CONVERTER			 122
#define DSO_F_DSO_UP_REF				 114
#define DSO_F_VMS_BIND_SYM				 115
#define DSO_F_VMS_LOAD					 116
#define DSO_F_VMS_MERGER				 133
#define DSO_F_VMS_UNLOAD				 117
#define DSO_F_WIN32_BIND_FUNC				 118
#define DSO_F_WIN32_BIND_VAR				 119
#define DSO_F_WIN32_JOINER				 135
#define DSO_F_WIN32_LOAD				 120
#define DSO_F_WIN32_MERGER				 134
#define DSO_F_WIN32_NAME_CONVERTER			 125
#define DSO_F_WIN32_SPLITTER				 136
#define DSO_F_WIN32_UNLOAD				 121 

#define DSO_R_CTRL_FAILED				 100
#define DSO_R_DSO_ALREADY_LOADED			 110
#define DSO_R_EMPTY_FILE_STRUCTURE			 113
#define DSO_R_FAILURE					 114
#define DSO_R_FILENAME_TOO_BIG				 101
#define DSO_R_FINISH_FAILED				 102
#define DSO_R_INCORRECT_FILE_SYNTAX			 115
#define DSO_R_LOAD_FAILED				 103
#define DSO_R_NAME_TRANSLATION_FAILED			 109
#define DSO_R_NO_FILENAME				 111
#define DSO_R_NO_FILE_SPECIFICATION			 116
#define DSO_R_NULL_HANDLE				 104
#define DSO_R_SET_FILENAME_FAILED			 112
#define DSO_R_STACK_ERROR				 105
#define DSO_R_SYM_FAILURE				 106
#define DSO_R_UNLOAD_FAILED				 107
#define DSO_R_UNSUPPORTED				 108

#ifdef  __cplusplus
}
#endif
#endif
