/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/




#ifndef HEADER_BUFFER_H
#define HEADER_BUFFER_H

#include <openssl/ossl_typ.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h>

#if !defined(NO_SYS_TYPES_H)
#include <sys/types.h>
#endif




struct buf_mem_st
	{
	int length;	
	char *data;
	int max;	
	};

BUF_MEM *BUF_MEM_new(void);
void	BUF_MEM_free(BUF_MEM *a);
int	BUF_MEM_grow(BUF_MEM *str, int len);
int	BUF_MEM_grow_clean(BUF_MEM *str, int len);
char *	BUF_strdup(const char *str);
char *	BUF_strndup(const char *str, size_t siz);
void *	BUF_memdup(const void *data, size_t siz);


size_t BUF_strlcpy(char *dst,const char *src,size_t siz);
size_t BUF_strlcat(char *dst,const char *src,size_t siz);




void ERR_load_BUF_strings(void);




#define BUF_F_BUF_MEMDUP				 103
#define BUF_F_BUF_MEM_GROW				 100
#define BUF_F_BUF_MEM_GROW_CLEAN			 105
#define BUF_F_BUF_MEM_NEW				 101
#define BUF_F_BUF_STRDUP				 102
#define BUF_F_BUF_STRNDUP				 104



#ifdef  __cplusplus
}
#endif
#endif
