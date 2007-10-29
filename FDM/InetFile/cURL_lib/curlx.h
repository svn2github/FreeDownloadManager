/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURLX_H
#define __CURLX_H   

#include <curl/mprintf.h> 

#include "strequal.h" 

#include "strtoofft.h" 

#include "timeval.h"   

#define curlx_getenv curl_getenv
#define curlx_strequal curl_strequal
#define curlx_strnequal curl_strnequal
#define curlx_mvsnprintf curl_mvsnprintf
#define curlx_msnprintf curl_msnprintf
#define curlx_maprintf curl_maprintf
#define curlx_mvaprintf curl_mvaprintf
#define curlx_msprintf curl_msprintf
#define curlx_mprintf curl_mprintf
#define curlx_mfprintf curl_mfprintf
#define curlx_mvsprintf curl_mvsprintf
#define curlx_mvprintf curl_mvprintf
#define curlx_mvfprintf curl_mvfprintf

#ifdef ENABLE_CURLX_PRINTF

# undef printf
# undef fprintf
# undef sprintf
# undef snprintf
# undef vprintf
# undef vfprintf
# undef vsprintf
# undef vsnprintf
# undef aprintf
# undef vaprintf

# define printf curlx_mprintf
# define fprintf curlx_mfprintf
# define sprintf curlx_msprintf
# define snprintf curlx_msnprintf
# define vprintf curlx_mvprintf
# define vfprintf curlx_mvfprintf
# define vsprintf curlx_mvsprintf
# define vsnprintf curlx_mvsnprintf
# define aprintf curlx_maprintf
# define vaprintf curlx_mvaprintf
#endif 

#endif 
