/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __CURL_CURL_H
#define __CURL_CURL_H   

#include "curlver.h" 

#include <stdio.h>
#include <limits.h> 

#ifdef vms
# include <types.h>
# include <time.h>
#else
# include <sys/types.h>
# include <time.h>
#endif 

typedef void CURL;

#ifdef  __cplusplus
extern "C" {
#endif 

#if (defined(WIN32) || defined(_WIN32)) && !defined(CURL_STATICLIB)
#if defined(BUILDING_LIBCURL)
#define CURL_EXTERN  __declspec(dllexport)
#else
#define CURL_EXTERN  __declspec(dllimport)
#endif
#else
#define CURL_EXTERN
#endif  

#if (defined(_MSC_VER) && !defined(__POCC__)) || (defined(__LCC__) && defined(WIN32))

#ifdef _WIN32_WCE
  typedef long curl_off_t;
#define CURL_FORMAT_OFF_T "%ld"
#else
  typedef signed __int64 curl_off_t;
#define CURL_FORMAT_OFF_T "%I64d"
#endif
#else 
#if (defined(__GNUC__) && defined(WIN32)) || defined(__WATCOMC__)

  typedef long long curl_off_t;
#define CURL_FORMAT_OFF_T "%I64d"
#else  

  typedef off_t curl_off_t; 

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 0 
#define UNDEF_FILE_OFFSET_BITS
#endif
#ifndef FILESIZEBITS
#define FILESIZEBITS 0 
#define UNDEF_FILESIZEBITS
#endif

#if defined(_LARGE_FILES) || (_FILE_OFFSET_BITS > 32) || (FILESIZEBITS > 32) \
   || defined(_LARGEFILE_SOURCE) || defined(_LARGEFILE64_SOURCE)
  
#define CURL_FORMAT_OFF_T "%lld"
#else 
#define CURL_FORMAT_OFF_T "%ld"
#endif
#endif 
#endif 

#ifdef UNDEF_FILE_OFFSET_BITS

#undef _FILE_OFFSET_BITS
#endif

#ifdef UNDEF_FILESIZEBITS

#undef FILESIZEBITS
#endif

struct curl_httppost {
  struct curl_httppost *next;       
  char *name;                       
  long namelength;                  
  char *contents;                   
  long contentslength;              
  char *buffer;                     
  long bufferlength;                
  char *contenttype;                
  struct curl_slist* contentheader; 
  struct curl_httppost *more;       
  long flags;                       
#define HTTPPOST_FILENAME (1<<0)    
#define HTTPPOST_READFILE (1<<1)    
#define HTTPPOST_PTRNAME (1<<2)     
#define HTTPPOST_PTRCONTENTS (1<<3) 
#define HTTPPOST_BUFFER (1<<4)      
#define HTTPPOST_PTRBUFFER (1<<5)   

  char *showfilename;               
};

typedef int (*curl_progress_callback)(void *clientp,
                                      double dltotal,
                                      double dlnow,
                                      double ultotal,
                                      double ulnow);

  
#define CURL_MAX_WRITE_SIZE 16384

typedef size_t (*curl_write_callback)(char *buffer,
                                      size_t size,
                                      size_t nitems,
                                      void *outstream); 

#define CURL_READFUNC_ABORT 0x10000000
typedef size_t (*curl_read_callback)(char *buffer,
                                      size_t size,
                                      size_t nitems,
                                      void *instream); 

#ifndef CURL_NO_OLDIES
  
typedef int (*curl_passwd_callback)(void *clientp,
                                    const char *prompt,
                                    char *buffer,
                                    int buflen);
#endif

typedef enum {
  CURLIOE_OK,            
  CURLIOE_UNKNOWNCMD,    
  CURLIOE_FAILRESTART,   
  CURLIOE_LAST           
} curlioerr;

typedef enum  {
  CURLIOCMD_NOP,         
  CURLIOCMD_RESTARTREAD, 
  CURLIOCMD_LAST         
} curliocmd;

typedef curlioerr (*curl_ioctl_callback)(CURL *handle,
                                         int cmd,
                                         void *clientp); 

typedef void *(*curl_malloc_callback)(size_t size);
typedef void (*curl_free_callback)(void *ptr);
typedef void *(*curl_realloc_callback)(void *ptr, size_t size);
typedef char *(*curl_strdup_callback)(const char *str);
typedef void *(*curl_calloc_callback)(size_t nmemb, size_t size); 

typedef enum {
  CURLINFO_TEXT = 0,
  CURLINFO_HEADER_IN,    
  CURLINFO_HEADER_OUT,   
  CURLINFO_DATA_IN,      
  CURLINFO_DATA_OUT,     
  CURLINFO_SSL_DATA_IN,  
  CURLINFO_SSL_DATA_OUT, 
  CURLINFO_END
} curl_infotype;

typedef int (*curl_debug_callback)
       (CURL *handle,      
        curl_infotype type, 
        char *data,        
        size_t size,       
        void *userptr);      

typedef enum {
  CURLE_OK = 0,
  CURLE_UNSUPPORTED_PROTOCOL,    
  CURLE_FAILED_INIT,             
  CURLE_URL_MALFORMAT,           
  CURLE_URL_MALFORMAT_USER,      
  CURLE_COULDNT_RESOLVE_PROXY,   
  CURLE_COULDNT_RESOLVE_HOST,    
  CURLE_COULDNT_CONNECT,         
  CURLE_FTP_WEIRD_SERVER_REPLY,  
  CURLE_FTP_ACCESS_DENIED,       
  CURLE_FTP_USER_PASSWORD_INCORRECT, 
  CURLE_FTP_WEIRD_PASS_REPLY,    
  CURLE_FTP_WEIRD_USER_REPLY,    
  CURLE_FTP_WEIRD_PASV_REPLY,    
  CURLE_FTP_WEIRD_227_FORMAT,    
  CURLE_FTP_CANT_GET_HOST,       
  CURLE_FTP_CANT_RECONNECT,      
  CURLE_FTP_COULDNT_SET_BINARY,  
  CURLE_PARTIAL_FILE,            
  CURLE_FTP_COULDNT_RETR_FILE,   
  CURLE_FTP_WRITE_ERROR,         
  CURLE_FTP_QUOTE_ERROR,         
  CURLE_HTTP_RETURNED_ERROR,     
  CURLE_WRITE_ERROR,             
  CURLE_MALFORMAT_USER,          
  CURLE_FTP_COULDNT_STOR_FILE,   
  CURLE_READ_ERROR,              
  CURLE_OUT_OF_MEMORY,           
  CURLE_OPERATION_TIMEOUTED,     
  CURLE_FTP_COULDNT_SET_ASCII,   
  CURLE_FTP_PORT_FAILED,         
  CURLE_FTP_COULDNT_USE_REST,    
  CURLE_FTP_COULDNT_GET_SIZE,    
  CURLE_HTTP_RANGE_ERROR,        
  CURLE_HTTP_POST_ERROR,         
  CURLE_SSL_CONNECT_ERROR,       
  CURLE_BAD_DOWNLOAD_RESUME,     
  CURLE_FILE_COULDNT_READ_FILE,  
  CURLE_LDAP_CANNOT_BIND,        
  CURLE_LDAP_SEARCH_FAILED,      
  CURLE_LIBRARY_NOT_FOUND,       
  CURLE_FUNCTION_NOT_FOUND,      
  CURLE_ABORTED_BY_CALLBACK,     
  CURLE_BAD_FUNCTION_ARGUMENT,   
  CURLE_BAD_CALLING_ORDER,       
  CURLE_INTERFACE_FAILED,        
  CURLE_BAD_PASSWORD_ENTERED,    
  CURLE_TOO_MANY_REDIRECTS ,     
  CURLE_UNKNOWN_TELNET_OPTION,   
  CURLE_TELNET_OPTION_SYNTAX ,   
  CURLE_OBSOLETE,                
  CURLE_SSL_PEER_CERTIFICATE,    
  CURLE_GOT_NOTHING,             
  CURLE_SSL_ENGINE_NOTFOUND,     
  CURLE_SSL_ENGINE_SETFAILED,    
  CURLE_SEND_ERROR,              
  CURLE_RECV_ERROR,              
  CURLE_SHARE_IN_USE,            
  CURLE_SSL_CERTPROBLEM,         
  CURLE_SSL_CIPHER,              
  CURLE_SSL_CACERT,              
  CURLE_BAD_CONTENT_ENCODING,    
  CURLE_LDAP_INVALID_URL,        
  CURLE_FILESIZE_EXCEEDED,       
  CURLE_FTP_SSL_FAILED,          
  CURLE_SEND_FAIL_REWIND,        
  CURLE_SSL_ENGINE_INITFAILED,   
  CURLE_LOGIN_DENIED,            
  CURLE_TFTP_NOTFOUND,           
  CURLE_TFTP_PERM,               
  CURLE_TFTP_DISKFULL,           
  CURLE_TFTP_ILLEGAL,            
  CURLE_TFTP_UNKNOWNID,          
  CURLE_TFTP_EXISTS,             
  CURLE_TFTP_NOSUCHUSER,         
  CURL_LAST 
} CURLcode;

typedef CURLcode (*curl_ssl_ctx_callback)(CURL *curl,    
                                          void *ssl_ctx, 
                                          void *userptr); 

#define CURLE_OPERATION_TIMEDOUT CURLE_OPERATION_TIMEOUTED

#ifndef CURL_NO_OLDIES 

#define CURLE_HTTP_NOT_FOUND CURLE_HTTP_RETURNED_ERROR
#define CURLE_HTTP_PORT_FAILED CURLE_INTERFACE_FAILED
#endif

typedef enum {
  CURLPROXY_HTTP = 0,
  CURLPROXY_SOCKS4 = 4,
  CURLPROXY_SOCKS5 = 5
} curl_proxytype;

#define CURLAUTH_NONE         0       
#define CURLAUTH_BASIC        (1<<0)  
#define CURLAUTH_DIGEST       (1<<1)  
#define CURLAUTH_GSSNEGOTIATE (1<<2)  
#define CURLAUTH_NTLM         (1<<3)  
#define CURLAUTH_ANY ~0               
#define CURLAUTH_ANYSAFE (~CURLAUTH_BASIC)

#ifndef CURL_NO_OLDIES 

#define CURLE_ALREADY_COMPLETE 99999 

#define CURLE_FTP_PARTIAL_FILE CURLE_PARTIAL_FILE
#define CURLE_FTP_BAD_DOWNLOAD_RESUME CURLE_BAD_DOWNLOAD_RESUME
#endif

#define CURL_ERROR_SIZE 256 

typedef enum {
  CURLFTPSSL_NONE,    
  CURLFTPSSL_TRY,     
  CURLFTPSSL_CONTROL, 
  CURLFTPSSL_ALL,     
  CURLFTPSSL_LAST     
} curl_ftpssl; 

typedef enum {
  CURLFTPAUTH_DEFAULT, 
  CURLFTPAUTH_SSL,     
  CURLFTPAUTH_TLS,     
  CURLFTPAUTH_LAST 
} curl_ftpauth; 

typedef enum {
  CURLFTPMETHOD_DEFAULT,   
  CURLFTPMETHOD_MULTICWD,  
  CURLFTPMETHOD_NOCWD,     
  CURLFTPMETHOD_SINGLECWD, 
  CURLFTPMETHOD_LAST       
} curl_ftpmethod; 

#define CURLOPTTYPE_LONG          0
#define CURLOPTTYPE_OBJECTPOINT   10000
#define CURLOPTTYPE_FUNCTIONPOINT 20000
#define CURLOPTTYPE_OFF_T         30000 

#ifdef CINIT
#undef CINIT
#endif

#if defined(__STDC__) || defined(_MSC_VER) || defined(__cplusplus) || \
  defined(__HP_aCC) || defined(__BORLANDC__) || defined(__LCC__) || \
  defined(__POCC__)
  
#define CURL_ISOCPP
#else
  
#undef CURL_ISOCPP
#endif

#ifdef CURL_ISOCPP
#define CINIT(name,type,number) CURLOPT_ ## name = CURLOPTTYPE_ ## type + number
#else

#define LONG          CURLOPTTYPE_LONG
#define OBJECTPOINT   CURLOPTTYPE_OBJECTPOINT
#define FUNCTIONPOINT CURLOPTTYPE_FUNCTIONPOINT
#define OFF_T         CURLOPTTYPE_OFF_T
#define CINIT(name,type,number) CURLOPT_name = type + number
#endif  

typedef enum {
  
  CINIT(FILE, OBJECTPOINT, 1),

  
  CINIT(URL,  OBJECTPOINT, 2),

  
  CINIT(PORT, LONG, 3),

  
  CINIT(PROXY, OBJECTPOINT, 4),

  
  CINIT(USERPWD, OBJECTPOINT, 5),

  
  CINIT(PROXYUSERPWD, OBJECTPOINT, 6),

  
  CINIT(RANGE, OBJECTPOINT, 7),

  

  
  CINIT(INFILE, OBJECTPOINT, 9),

  
  CINIT(ERRORBUFFER, OBJECTPOINT, 10),

  
  CINIT(WRITEFUNCTION, FUNCTIONPOINT, 11),

  
  CINIT(READFUNCTION, FUNCTIONPOINT, 12),

  
  CINIT(TIMEOUT, LONG, 13),

  
  CINIT(INFILESIZE, LONG, 14),

  
  CINIT(POSTFIELDS, OBJECTPOINT, 15),

  
  CINIT(REFERER, OBJECTPOINT, 16),

  
  CINIT(FTPPORT, OBJECTPOINT, 17),

  
  CINIT(USERAGENT, OBJECTPOINT, 18),

  

  
  CINIT(LOW_SPEED_LIMIT, LONG , 19),

  
  CINIT(LOW_SPEED_TIME, LONG, 20),

  
  CINIT(RESUME_FROM, LONG, 21),

  
  CINIT(COOKIE, OBJECTPOINT, 22),

  
  CINIT(HTTPHEADER, OBJECTPOINT, 23),

  
  CINIT(HTTPPOST, OBJECTPOINT, 24),

  
  CINIT(SSLCERT, OBJECTPOINT, 25),

  
  CINIT(SSLCERTPASSWD, OBJECTPOINT, 26),
  
  CINIT(SSLKEYPASSWD, OBJECTPOINT, 26),

  
  CINIT(CRLF, LONG, 27),

  
  CINIT(QUOTE, OBJECTPOINT, 28),

  
  CINIT(WRITEHEADER, OBJECTPOINT, 29),

  
  CINIT(COOKIEFILE, OBJECTPOINT, 31),

  
  CINIT(SSLVERSION, LONG, 32),

  
  CINIT(TIMECONDITION, LONG, 33),

  
  CINIT(TIMEVALUE, LONG, 34),

  

  
  CINIT(CUSTOMREQUEST, OBJECTPOINT, 36),

  
  CINIT(STDERR, OBJECTPOINT, 37),

  

  
  CINIT(POSTQUOTE, OBJECTPOINT, 39),

  
  CINIT(WRITEINFO, OBJECTPOINT, 40),

  CINIT(VERBOSE, LONG, 41),      
  CINIT(HEADER, LONG, 42),       
  CINIT(NOPROGRESS, LONG, 43),   
  CINIT(NOBODY, LONG, 44),       
  CINIT(FAILONERROR, LONG, 45),  
  CINIT(UPLOAD, LONG, 46),       
  CINIT(POST, LONG, 47),         
  CINIT(FTPLISTONLY, LONG, 48),  

  CINIT(FTPAPPEND, LONG, 50),    

  
  CINIT(NETRC, LONG, 51),

  CINIT(FOLLOWLOCATION, LONG, 52),  

  CINIT(TRANSFERTEXT, LONG, 53), 
  CINIT(PUT, LONG, 54),          

  

  
  CINIT(PROGRESSFUNCTION, FUNCTIONPOINT, 56),

  
  CINIT(PROGRESSDATA, OBJECTPOINT, 57),

  
  CINIT(AUTOREFERER, LONG, 58),

  
  CINIT(PROXYPORT, LONG, 59),

  
  CINIT(POSTFIELDSIZE, LONG, 60),

  
  CINIT(HTTPPROXYTUNNEL, LONG, 61),

  
  CINIT(INTERFACE, OBJECTPOINT, 62),

  
  CINIT(KRB4LEVEL, OBJECTPOINT, 63),

  
  CINIT(SSL_VERIFYPEER, LONG, 64),

  
  CINIT(CAINFO, OBJECTPOINT, 65),

  
  

  
  CINIT(MAXREDIRS, LONG, 68),

  
  CINIT(FILETIME, LONG, 69),

  
  CINIT(TELNETOPTIONS, OBJECTPOINT, 70),

  
  CINIT(MAXCONNECTS, LONG, 71),

  
  CINIT(CLOSEPOLICY, LONG, 72),

  

  
  CINIT(FRESH_CONNECT, LONG, 74),

  
  CINIT(FORBID_REUSE, LONG, 75),

  
  CINIT(RANDOM_FILE, OBJECTPOINT, 76),

  
  CINIT(EGDSOCKET, OBJECTPOINT, 77),

  
  CINIT(CONNECTTIMEOUT, LONG, 78),

  
  CINIT(HEADERFUNCTION, FUNCTIONPOINT, 79),

  
  CINIT(HTTPGET, LONG, 80),

  
  CINIT(SSL_VERIFYHOST, LONG, 81),

  
  CINIT(COOKIEJAR, OBJECTPOINT, 82),

  
  CINIT(SSL_CIPHER_LIST, OBJECTPOINT, 83),

  
  CINIT(HTTP_VERSION, LONG, 84),

  
  CINIT(FTP_USE_EPSV, LONG, 85),

  
  CINIT(SSLCERTTYPE, OBJECTPOINT, 86),

  
  CINIT(SSLKEY, OBJECTPOINT, 87),

  
  CINIT(SSLKEYTYPE, OBJECTPOINT, 88),

  
  CINIT(SSLENGINE, OBJECTPOINT, 89),

  
  CINIT(SSLENGINE_DEFAULT, LONG, 90),

  
  CINIT(DNS_USE_GLOBAL_CACHE, LONG, 91), 

  
  CINIT(DNS_CACHE_TIMEOUT, LONG, 92),

  
  CINIT(PREQUOTE, OBJECTPOINT, 93),

  
  CINIT(DEBUGFUNCTION, FUNCTIONPOINT, 94),

  
  CINIT(DEBUGDATA, OBJECTPOINT, 95),

  
  CINIT(COOKIESESSION, LONG, 96),

  
  CINIT(CAPATH, OBJECTPOINT, 97),

  
  CINIT(BUFFERSIZE, LONG, 98),

  
  CINIT(NOSIGNAL, LONG, 99),

  
  CINIT(SHARE, OBJECTPOINT, 100),

  
  CINIT(PROXYTYPE, LONG, 101),

  
  CINIT(ENCODING, OBJECTPOINT, 102),

  
  CINIT(PRIVATE, OBJECTPOINT, 103),

  
  CINIT(HTTP200ALIASES, OBJECTPOINT, 104),

  
  CINIT(UNRESTRICTED_AUTH, LONG, 105),

  
  CINIT(FTP_USE_EPRT, LONG, 106),

  
  CINIT(HTTPAUTH, LONG, 107),

  
  CINIT(SSL_CTX_FUNCTION, FUNCTIONPOINT, 108),

  
  CINIT(SSL_CTX_DATA, OBJECTPOINT, 109),

  
  CINIT(FTP_CREATE_MISSING_DIRS, LONG, 110),

  
  CINIT(PROXYAUTH, LONG, 111),

  
  CINIT(FTP_RESPONSE_TIMEOUT, LONG , 112),

  
  CINIT(IPRESOLVE, LONG, 113),

  
  CINIT(MAXFILESIZE, LONG, 114),

  
  CINIT(INFILESIZE_LARGE, OFF_T, 115),

  
  CINIT(RESUME_FROM_LARGE, OFF_T, 116),

  
  CINIT(MAXFILESIZE_LARGE, OFF_T, 117),

  
  CINIT(NETRC_FILE, OBJECTPOINT, 118),

  
  CINIT(FTP_SSL, LONG, 119),

  
  CINIT(POSTFIELDSIZE_LARGE, OFF_T, 120),

  
  CINIT(TCP_NODELAY, LONG, 121),

  

  
  CINIT(SOURCE_USERPWD, OBJECTPOINT, 123),

  
  
  

  
  CINIT(SOURCE_PREQUOTE, OBJECTPOINT, 127),

  
  CINIT(SOURCE_POSTQUOTE, OBJECTPOINT, 128),

  
  CINIT(FTPSSLAUTH, LONG, 129),

  CINIT(IOCTLFUNCTION, FUNCTIONPOINT, 130),
  CINIT(IOCTLDATA, OBJECTPOINT, 131),

  
  CINIT(SOURCE_URL, OBJECTPOINT, 132),

  
  CINIT(SOURCE_QUOTE, OBJECTPOINT, 133),

  
  CINIT(FTP_ACCOUNT, OBJECTPOINT, 134),

  
  CINIT(COOKIELIST, OBJECTPOINT, 135),

  
  CINIT(IGNORE_CONTENT_LENGTH, LONG, 136),

  
  CINIT(FTP_SKIP_PASV_IP, LONG, 137),

  
  CINIT(FTP_FILEMETHOD, LONG, 138),

  
  CINIT(LOCALPORT, LONG, 139),

  
  CINIT(LOCALPORTRANGE, LONG, 140),

  
  CINIT(CONNECT_ONLY, LONG, 141),

  CURLOPT_LASTENTRY 
} CURLoption;

  
#define CURL_IPRESOLVE_WHATEVER 0 
#define CURL_IPRESOLVE_V4       1 
#define CURL_IPRESOLVE_V6       2 

  
#define CURLOPT_WRITEDATA CURLOPT_FILE
#define CURLOPT_READDATA  CURLOPT_INFILE
#define CURLOPT_HEADERDATA CURLOPT_WRITEHEADER

#ifndef CURL_NO_OLDIES 
#define CURLOPT_HTTPREQUEST    -1
#define CURLOPT_FTPASCII       CURLOPT_TRANSFERTEXT
#define CURLOPT_MUTE           -2
#define CURLOPT_PASSWDFUNCTION -3
#define CURLOPT_PASSWDDATA     -4
#define CURLOPT_CLOSEFUNCTION  -5

#define CURLOPT_SOURCE_HOST    -6
#define CURLOPT_SOURCE_PATH    -7
#define CURLOPT_SOURCE_PORT    -8
#define CURLOPT_PASV_HOST      -9

#else

#undef CURLOPT_DNS_USE_GLOBAL_CACHE 
#endif 

  
enum {
  CURL_HTTP_VERSION_NONE, 
  CURL_HTTP_VERSION_1_0,  
  CURL_HTTP_VERSION_1_1,  

  CURL_HTTP_VERSION_LAST 
};

  
enum CURL_NETRC_OPTION {
  CURL_NETRC_IGNORED,     
  CURL_NETRC_OPTIONAL,    
  CURL_NETRC_REQUIRED,    
  CURL_NETRC_LAST
};

enum {
  CURL_SSLVERSION_DEFAULT,
  CURL_SSLVERSION_TLSv1,
  CURL_SSLVERSION_SSLv2,
  CURL_SSLVERSION_SSLv3,

  CURL_SSLVERSION_LAST 
}; 

typedef enum {
  CURL_TIMECOND_NONE,

  CURL_TIMECOND_IFMODSINCE,
  CURL_TIMECOND_IFUNMODSINCE,
  CURL_TIMECOND_LASTMOD,

  CURL_TIMECOND_LAST
} curl_TimeCond;

#ifdef __BEOS__
#include <support/SupportDefs.h>
#endif  

CURL_EXTERN int (curl_strequal)(const char *s1, const char *s2);
CURL_EXTERN int (curl_strnequal)(const char *s1, const char *s2, size_t n); 

#ifdef CFINIT
#undef CFINIT
#endif

#ifdef CURL_ISOCPP
#define CFINIT(name) CURLFORM_ ## name
#else

#define CFINIT(name) CURLFORM_name
#endif

typedef enum {
  CFINIT(NOTHING),        

  
  CFINIT(COPYNAME),
  CFINIT(PTRNAME),
  CFINIT(NAMELENGTH),
  CFINIT(COPYCONTENTS),
  CFINIT(PTRCONTENTS),
  CFINIT(CONTENTSLENGTH),
  CFINIT(FILECONTENT),
  CFINIT(ARRAY),
  CFINIT(OBSOLETE),
  CFINIT(FILE),

  CFINIT(BUFFER),
  CFINIT(BUFFERPTR),
  CFINIT(BUFFERLENGTH),

  CFINIT(CONTENTTYPE),
  CFINIT(CONTENTHEADER),
  CFINIT(FILENAME),
  CFINIT(END),
  CFINIT(OBSOLETE2),

  CURLFORM_LASTENTRY 
} CURLformoption;

#undef CFINIT  

struct curl_forms {
  CURLformoption option;
  const char     *value;
};  

typedef enum {
  CURL_FORMADD_OK, 

  CURL_FORMADD_MEMORY,
  CURL_FORMADD_OPTION_TWICE,
  CURL_FORMADD_NULL,
  CURL_FORMADD_UNKNOWN_OPTION,
  CURL_FORMADD_INCOMPLETE,
  CURL_FORMADD_ILLEGAL_ARRAY,
  CURL_FORMADD_DISABLED, 

  CURL_FORMADD_LAST 
} CURLFORMcode; 

CURL_EXTERN CURLFORMcode curl_formadd(struct curl_httppost **httppost,
                                      struct curl_httppost **last_post,
                                      ...); 

CURL_EXTERN void curl_formfree(struct curl_httppost *form); 

CURL_EXTERN char *curl_getenv(const char *variable); 

CURL_EXTERN char *curl_version(void); 

CURL_EXTERN char *curl_escape(const char *string, int length); 

CURL_EXTERN char *curl_unescape(const char *string, int length); 

CURL_EXTERN void curl_free(void *p); 

CURL_EXTERN CURLcode curl_global_init(long flags); 

CURL_EXTERN CURLcode curl_global_init_mem(long flags,
                                          curl_malloc_callback m,
                                          curl_free_callback f,
                                          curl_realloc_callback r,
                                          curl_strdup_callback s,
                                          curl_calloc_callback c); 

CURL_EXTERN void curl_global_cleanup(void); 

struct curl_slist {
  char *data;
  struct curl_slist *next;
}; 

CURL_EXTERN struct curl_slist *curl_slist_append(struct curl_slist *,
                                                 const char *); 

CURL_EXTERN void curl_slist_free_all(struct curl_slist *); 

CURL_EXTERN time_t curl_getdate(const char *p, const time_t *unused);

#define CURLINFO_STRING   0x100000
#define CURLINFO_LONG     0x200000
#define CURLINFO_DOUBLE   0x300000
#define CURLINFO_SLIST    0x400000
#define CURLINFO_MASK     0x0fffff
#define CURLINFO_TYPEMASK 0xf00000

typedef enum {
  CURLINFO_NONE, 
  CURLINFO_EFFECTIVE_URL    = CURLINFO_STRING + 1,
  CURLINFO_RESPONSE_CODE    = CURLINFO_LONG   + 2,
  CURLINFO_TOTAL_TIME       = CURLINFO_DOUBLE + 3,
  CURLINFO_NAMELOOKUP_TIME  = CURLINFO_DOUBLE + 4,
  CURLINFO_CONNECT_TIME     = CURLINFO_DOUBLE + 5,
  CURLINFO_PRETRANSFER_TIME = CURLINFO_DOUBLE + 6,
  CURLINFO_SIZE_UPLOAD      = CURLINFO_DOUBLE + 7,
  CURLINFO_SIZE_DOWNLOAD    = CURLINFO_DOUBLE + 8,
  CURLINFO_SPEED_DOWNLOAD   = CURLINFO_DOUBLE + 9,
  CURLINFO_SPEED_UPLOAD     = CURLINFO_DOUBLE + 10,
  CURLINFO_HEADER_SIZE      = CURLINFO_LONG   + 11,
  CURLINFO_REQUEST_SIZE     = CURLINFO_LONG   + 12,
  CURLINFO_SSL_VERIFYRESULT = CURLINFO_LONG   + 13,
  CURLINFO_FILETIME         = CURLINFO_LONG   + 14,
  CURLINFO_CONTENT_LENGTH_DOWNLOAD   = CURLINFO_DOUBLE + 15,
  CURLINFO_CONTENT_LENGTH_UPLOAD     = CURLINFO_DOUBLE + 16,
  CURLINFO_STARTTRANSFER_TIME = CURLINFO_DOUBLE + 17,
  CURLINFO_CONTENT_TYPE     = CURLINFO_STRING + 18,
  CURLINFO_REDIRECT_TIME    = CURLINFO_DOUBLE + 19,
  CURLINFO_REDIRECT_COUNT   = CURLINFO_LONG   + 20,
  CURLINFO_PRIVATE          = CURLINFO_STRING + 21,
  CURLINFO_HTTP_CONNECTCODE = CURLINFO_LONG   + 22,
  CURLINFO_HTTPAUTH_AVAIL   = CURLINFO_LONG   + 23,
  CURLINFO_PROXYAUTH_AVAIL  = CURLINFO_LONG   + 24,
  CURLINFO_OS_ERRNO         = CURLINFO_LONG   + 25,
  CURLINFO_NUM_CONNECTS     = CURLINFO_LONG   + 26,
  CURLINFO_SSL_ENGINES      = CURLINFO_SLIST  + 27,
  CURLINFO_COOKIELIST       = CURLINFO_SLIST  + 28,
  CURLINFO_LASTSOCKET       = CURLINFO_LONG   + 29,
  

  CURLINFO_LASTONE          = 29
} CURLINFO; 

#define CURLINFO_HTTP_CODE CURLINFO_RESPONSE_CODE

typedef enum {
  CURLCLOSEPOLICY_NONE, 

  CURLCLOSEPOLICY_OLDEST,
  CURLCLOSEPOLICY_LEAST_RECENTLY_USED,
  CURLCLOSEPOLICY_LEAST_TRAFFIC,
  CURLCLOSEPOLICY_SLOWEST,
  CURLCLOSEPOLICY_CALLBACK,

  CURLCLOSEPOLICY_LAST 
} curl_closepolicy;

#define CURL_GLOBAL_SSL (1<<0)
#define CURL_GLOBAL_WIN32 (1<<1)
#define CURL_GLOBAL_ALL (CURL_GLOBAL_SSL|CURL_GLOBAL_WIN32)
#define CURL_GLOBAL_NOTHING 0
#define CURL_GLOBAL_DEFAULT CURL_GLOBAL_ALL    

typedef enum {
  CURL_LOCK_DATA_NONE = 0,
  
  CURL_LOCK_DATA_SHARE,
  CURL_LOCK_DATA_COOKIE,
  CURL_LOCK_DATA_DNS,
  CURL_LOCK_DATA_SSL_SESSION,
  CURL_LOCK_DATA_CONNECT,
  CURL_LOCK_DATA_LAST
} curl_lock_data; 

typedef enum {
  CURL_LOCK_ACCESS_NONE = 0,   
  CURL_LOCK_ACCESS_SHARED = 1, 
  CURL_LOCK_ACCESS_SINGLE = 2, 
  CURL_LOCK_ACCESS_LAST        
} curl_lock_access;

typedef void (*curl_lock_function)(CURL *handle,
                                   curl_lock_data data,
                                   curl_lock_access locktype,
                                   void *userptr);
typedef void (*curl_unlock_function)(CURL *handle,
                                     curl_lock_data data,
                                     void *userptr);

typedef void CURLSH;

typedef enum {
  CURLSHE_OK,  
  CURLSHE_BAD_OPTION, 
  CURLSHE_IN_USE,     
  CURLSHE_INVALID,    
  CURLSHE_NOMEM,      
  CURLSHE_LAST 
} CURLSHcode;

typedef enum {
  CURLSHOPT_NONE,  
  CURLSHOPT_SHARE,   
  CURLSHOPT_UNSHARE, 
  CURLSHOPT_LOCKFUNC,   
  CURLSHOPT_UNLOCKFUNC, 
  CURLSHOPT_USERDATA,   
  CURLSHOPT_LAST  
} CURLSHoption;

CURL_EXTERN CURLSH *curl_share_init(void);
CURL_EXTERN CURLSHcode curl_share_setopt(CURLSH *, CURLSHoption option, ...);
CURL_EXTERN CURLSHcode curl_share_cleanup(CURLSH *);  

typedef enum {
  CURLVERSION_FIRST,
  CURLVERSION_SECOND,
  CURLVERSION_THIRD,
  CURLVERSION_LAST 
} CURLversion; 

#define CURLVERSION_NOW CURLVERSION_THIRD

typedef struct {
  CURLversion age;          
  const char *version;      
  unsigned int version_num; 
  const char *host;         
  int features;             
  const char *ssl_version;  
  long ssl_version_num;     
  const char *libz_version; 
  
  const char * const *protocols;

  
  const char *ares;
  int ares_num;

  
  const char *libidn;
} curl_version_info_data;

#define CURL_VERSION_IPV6      (1<<0)  
#define CURL_VERSION_KERBEROS4 (1<<1)  
#define CURL_VERSION_SSL       (1<<2)  
#define CURL_VERSION_LIBZ      (1<<3)  
#define CURL_VERSION_NTLM      (1<<4)  
#define CURL_VERSION_GSSNEGOTIATE (1<<5) 
#define CURL_VERSION_DEBUG     (1<<6)  
#define CURL_VERSION_ASYNCHDNS (1<<7)  
#define CURL_VERSION_SPNEGO    (1<<8)  
#define CURL_VERSION_LARGEFILE (1<<9)  
#define CURL_VERSION_IDN       (1<<10) 
#define CURL_VERSION_SSPI      (1<<11)  

CURL_EXTERN curl_version_info_data *curl_version_info(CURLversion); 

CURL_EXTERN const char *curl_easy_strerror(CURLcode); 

CURL_EXTERN const char *curl_share_strerror(CURLSHcode);

#ifdef  __cplusplus
}
#endif 

#include "easy.h" 
#include "multi.h"

#endif 
