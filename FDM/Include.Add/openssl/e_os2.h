/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

  

#include <openssl/opensslconf.h>

#ifndef HEADER_E_OS2_H
#define HEADER_E_OS2_H

#ifdef  __cplusplus
extern "C" {
#endif  

#define OPENSSL_SYS_UNIX 

#if defined(__MWERKS__) && defined(macintosh) || defined(OPENSSL_SYSNAME_MAC)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_MACINTOSH_CLASSIC
#endif 

#if defined(NETWARE) || defined(OPENSSL_SYSNAME_NETWARE)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_NETWARE
#endif   

#if defined(OPENSSL_SYSNAME_MSDOS)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_MSDOS
#endif 

#if defined(OPENSSL_SYSNAME_UWIN)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_WIN32_UWIN
#else
# if defined(__CYGWIN32__) || defined(OPENSSL_SYSNAME_CYGWIN32)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_WIN32_CYGWIN
# else
#  if defined(_WIN32) || defined(OPENSSL_SYSNAME_WIN32)
#   undef OPENSSL_SYS_UNIX
#   define OPENSSL_SYS_WIN32
#  endif
#  if defined(OPENSSL_SYSNAME_WINNT)
#   undef OPENSSL_SYS_UNIX
#   define OPENSSL_SYS_WINNT
#  endif
#  if defined(OPENSSL_SYSNAME_WINCE)
#   undef OPENSSL_SYS_UNIX
#   define OPENSSL_SYS_WINCE
#  endif
# endif
#endif 

#if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_WINNT) || defined(OPENSSL_SYS_WINCE)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_WINDOWS
# ifndef OPENSSL_SYS_MSDOS
#  define OPENSSL_SYS_MSDOS
# endif
#endif 

#ifdef OPENSSL_SYS_WINDOWS
# ifndef OPENSSL_OPT_WINDLL
#  if defined(_WINDLL) 
#   define OPENSSL_OPT_WINDLL
#  endif
# endif
#endif 

#if defined(__VMS) || defined(VMS) || defined(OPENSSL_SYSNAME_VMS)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_VMS
# if defined(__DECC)
#  define OPENSSL_SYS_VMS_DECC
# elif defined(__DECCXX)
#  define OPENSSL_SYS_VMS_DECC
#  define OPENSSL_SYS_VMS_DECCXX
# else
#  define OPENSSL_SYS_VMS_NODECC
# endif
#endif 

#if defined(__EMX__) || defined(__OS2__)
# undef OPENSSL_SYS_UNIX
# define OPENSSL_SYS_OS2
#endif 

#ifdef OPENSSL_SYS_UNIX
# if defined(linux) || defined(__linux__) || defined(OPENSSL_SYSNAME_LINUX)
#  define OPENSSL_SYS_LINUX
# endif
# ifdef OPENSSL_SYSNAME_MPE
#  define OPENSSL_SYS_MPE
# endif
# ifdef OPENSSL_SYSNAME_SNI
#  define OPENSSL_SYS_SNI
# endif
# ifdef OPENSSL_SYSNAME_ULTRASPARC
#  define OPENSSL_SYS_ULTRASPARC
# endif
# ifdef OPENSSL_SYSNAME_NEWS4
#  define OPENSSL_SYS_NEWS4
# endif
# ifdef OPENSSL_SYSNAME_MACOSX
#  define OPENSSL_SYS_MACOSX
# endif
# ifdef OPENSSL_SYSNAME_MACOSX_RHAPSODY
#  define OPENSSL_SYS_MACOSX_RHAPSODY
#  define OPENSSL_SYS_MACOSX
# endif
# ifdef OPENSSL_SYSNAME_SUNOS
#  define OPENSSL_SYS_SUNOS
#endif
# if defined(_CRAY) || defined(OPENSSL_SYSNAME_CRAY)
#  define OPENSSL_SYS_CRAY
# endif
# if defined(_AIX) || defined(OPENSSL_SYSNAME_AIX)
#  define OPENSSL_SYS_AIX
# endif
#endif 

#ifdef OPENSSL_SYSNAME_VOS
# define OPENSSL_SYS_VOS
#endif 

#ifdef OPENSSL_SYSNAME_VXWORKS
# define OPENSSL_SYS_VXWORKS
#endif    

#ifdef OPENSSL_SYS_MSDOS
# define OPENSSL_UNISTD_IO <io.h>
# define OPENSSL_DECLARE_EXIT extern void exit(int);
#else
# define OPENSSL_UNISTD_IO OPENSSL_UNISTD
# define OPENSSL_DECLARE_EXIT 
#endif  

#if defined(OPENSSL_SYS_VMS_NODECC)
# define OPENSSL_EXPORT globalref
# define OPENSSL_IMPORT globalref
# define OPENSSL_GLOBAL globaldef
#elif defined(OPENSSL_SYS_WINDOWS) && defined(OPENSSL_OPT_WINDLL)
# define OPENSSL_EXPORT extern __declspec(dllexport)
# define OPENSSL_IMPORT extern __declspec(dllimport)
# define OPENSSL_GLOBAL
#else
# define OPENSSL_EXPORT extern
# define OPENSSL_IMPORT extern
# define OPENSSL_GLOBAL
#endif
#define OPENSSL_EXTERN OPENSSL_IMPORT 

#ifdef OPENSSL_EXPORT_VAR_AS_FUNCTION
# define OPENSSL_IMPLEMENT_GLOBAL(type,name)			     \
	extern type _hide_##name;				     \
	type *_shadow_##name(void) { return &_hide_##name; }	     \
	static type _hide_##name
# define OPENSSL_DECLARE_GLOBAL(type,name) type *_shadow_##name(void)
# define OPENSSL_GLOBAL_REF(name) (*(_shadow_##name()))
#else
# define OPENSSL_IMPLEMENT_GLOBAL(type,name) OPENSSL_GLOBAL type _shadow_##name
# define OPENSSL_DECLARE_GLOBAL(type,name) OPENSSL_EXPORT type _shadow_##name
# define OPENSSL_GLOBAL_REF(name) _shadow_##name
#endif

#ifdef  __cplusplus
}
#endif
#endif
