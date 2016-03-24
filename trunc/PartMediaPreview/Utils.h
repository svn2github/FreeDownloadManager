/*
  Free Download Manager Copyright (c) 2003-2016 FreeDownloadManager.ORG
*/

#ifndef UTILS_INCLUDED_FILES
#define UTILS_INCLUDED_FILES

#ifdef UNICODE
#define _T(x) L##x
#define _tcsicmp _wcsicmp
#define _tcslen wcslen
#define _tstoi64 _wtoi64
#define _tstoi _wtoi
#define _stscanf_s swscanf_s
#define _tcscat_s wcscat_s
#else
#define _T(x) x
#define _tcsicmp _stricmp
#define _tcslen strlen
#define _tstoi64 _atoi64
#define _tstoi atoi
#define _stscanf_s sscanf_s
#define _tcscat_s strcat_s
#endif

#endif
