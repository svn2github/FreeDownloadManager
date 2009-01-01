/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef LIBCURL_AMIGAOS_H
#define LIBCURL_AMIGAOS_H 

#ifdef __AMIGA__ 

#ifndef __ixemul__

#include <exec/types.h>
#include <exec/execbase.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include <sys/socket.h>

#include "config-amigaos.h"

#ifndef select
# define select(args...) WaitSelect( args, NULL)
#endif
#ifndef ioctl
# define ioctl(a,b,c,d)  IoctlSocket( (LONG)a, (ULONG)b, (char*)c)
#endif
#define _AMIGASF        1

extern void amiga_cleanup();
extern BOOL amiga_init();

#else 

#warning compiling with ixemul...

#endif 
#endif 
#endif 

