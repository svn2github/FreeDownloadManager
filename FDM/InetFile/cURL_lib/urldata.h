/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __URLDATA_H
#define __URLDATA_H   

#include "setup.h"

#define PORT_FTP 21
#define PORT_FTPS 990
#define PORT_TELNET 23
#define PORT_HTTP 80
#define PORT_HTTPS 443
#define PORT_DICT 2628
#define PORT_LDAP 389
#define PORT_TFTP 69

#define DICT_MATCH "/MATCH:"
#define DICT_MATCH2 "/M:"
#define DICT_MATCH3 "/FIND:"
#define DICT_DEFINE "/DEFINE:"
#define DICT_DEFINE2 "/D:"
#define DICT_DEFINE3 "/LOOKUP:"

#define CURL_DEFAULT_USER "anonymous"
#define CURL_DEFAULT_PASSWORD "curl_by_daniel@haxx.se"

#include "cookie.h"
#include "formdata.h"

#ifdef USE_SSLEAY
#ifdef USE_OPENSSL
#include "openssl/rsa.h"
#include "openssl/crypto.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#ifdef HAVE_OPENSSL_ENGINE_H
#include <openssl/engine.h>
#endif
#ifdef HAVE_OPENSSL_PKCS12_H
#include <openssl/pkcs12.h>
#endif
#else 
#include "rsa.h"
#include "crypto.h"
#include "x509.h"
#include "pem.h"
#include "ssl.h"
#include "err.h"
#endif 
#endif 

#ifdef USE_GNUTLS
#include <gnutls/gnutls.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include "timeval.h"

#ifdef HAVE_ZLIB_H
#include <zlib.h>               
#endif

#ifdef USE_ARES
#include <ares.h>
#endif

#include <curl/curl.h>

#include "http_chunks.h" 
#include "hostip.h"
#include "hash.h"

#ifdef HAVE_GSSAPI
# ifdef HAVE_GSSGNU
#  include <gss.h>
# elif defined HAVE_GSSMIT
#  include <gssapi/gssapi.h>
#  include <gssapi/gssapi_generic.h>
# else
#  include <gssapi.h>
# endif
#endif 

#define BUFSIZE CURL_MAX_WRITE_SIZE 

#define HEADERSIZE 256 

#define CURLMAX(x,y) ((x)>(y)?(x):(y))

#ifdef HAVE_KRB4

struct krb4buffer {
  void *data;
  size_t size;
  size_t index;
  int eof_flag;
};
enum protection_level {
    prot_clear,
    prot_safe,
    prot_confidential,
    prot_private
};
#endif 

struct ssl_connect_data {
  bool use;        
#ifdef USE_SSLEAY
  
  SSL_CTX* ctx;
  SSL*     handle;
  X509*    server_cert;
#endif 
#ifdef USE_GNUTLS
  gnutls_session session;
  gnutls_anon_client_credentials cred;
#endif 
};

struct ssl_config_data {
  long version;          
  long certverifyresult; 
  long verifypeer;       
  long verifyhost;       
  char *CApath;          
  char *CAfile;          
  char *random_file;     
  char *egdsocket;       
  char *cipher_list;     
  long numsessions;      
  curl_ssl_ctx_callback fsslctx;        
  void *fsslctxp;       
}; 

struct curl_ssl_session {
  char *name;       
  void *sessionid;  
  size_t idsize;    
  long age;         
  unsigned short remote_port; 
  struct ssl_config_data ssl_config; 
}; 

struct digestdata {
  char *nonce;
  char *cnonce;
  char *realm;
  int algo;
  bool stale; 
  char *opaque;
  char *qop;
  char *algorithm;
  int nc; 
};

typedef enum {
  NTLMSTATE_NONE,
  NTLMSTATE_TYPE1,
  NTLMSTATE_TYPE2,
  NTLMSTATE_TYPE3,
  NTLMSTATE_LAST
} curlntlm;

#ifdef USE_WINDOWS_SSPI

#define SECURITY_WIN32 1
#include <security.h>
#include <sspi.h>
#include <rpc.h>
#endif 

struct ntlmdata {
  curlntlm state;
#ifdef USE_WINDOWS_SSPI
  CredHandle handle;
  CtxtHandle c_handle;
  SEC_WINNT_AUTH_IDENTITY identity;
  SEC_WINNT_AUTH_IDENTITY *p_identity;
  int has_handles;
  void *type_2;
  int n_type_2;
#else
  unsigned char nonce[8];
#endif
};

#ifdef HAVE_GSSAPI
struct negotiatedata {
  bool gss; 
  const char* protocol; 
  OM_uint32 status;
  gss_ctx_id_t context;
  gss_name_t server_name;
  gss_buffer_desc output_token;
};
#endif 

struct HTTP {
  struct FormData *sendit;
  curl_off_t postsize; 
  char *postdata;

  const char *p_pragma;      
  const char *p_accept;      
  curl_off_t readbytecount;
  curl_off_t writebytecount;

  
  struct Form form;
  struct Curl_chunker chunk;

  struct back {
    curl_read_callback fread; 
    void *fread_in;           
    char *postdata;
    curl_off_t postsize;
  } backup;

  enum {
    HTTPSEND_NADA,    
    HTTPSEND_REQUEST, 
    HTTPSEND_BODY,    
    HTTPSEND_LAST     
  } sending;

  void *send_buffer; 
}; 

typedef enum {
  FTP_STOP,    
  FTP_WAIT220, 
  FTP_AUTH,
  FTP_USER,
  FTP_PASS,
  FTP_ACCT,
  FTP_PBSZ,
  FTP_PROT,
  FTP_PWD,
  FTP_QUOTE, 
  FTP_RETR_PREQUOTE,
  FTP_STOR_PREQUOTE,
  FTP_POSTQUOTE,
  FTP_CWD,  
  FTP_MKD,  
  FTP_MDTM, 
  FTP_TYPE, 
  FTP_LIST_TYPE, 
  FTP_RETR_TYPE, 
  FTP_STOR_TYPE, 
  FTP_SIZE, 
  FTP_RETR_SIZE, 
  FTP_STOR_SIZE, 
  FTP_REST, 
  FTP_RETR_REST, 
  FTP_PORT, 
  FTP_PASV, 
  FTP_LIST, 
  FTP_RETR,
  FTP_STOR, 
  FTP_QUIT,
  FTP_LAST  
} ftpstate;

typedef enum {
  FTPFILE_MULTICWD  = 1, 
  FTPFILE_NOCWD     = 2, 
  FTPFILE_SINGLECWD = 3  
} curl_ftpfile;

struct FTP {
  curl_off_t *bytecountp;
  char *user;    
  char *passwd;  
  char *urlpath; 
  char **dirs;   
  int dirdepth;  
  int diralloc;  
  char *file;    

  char *entrypath; 

  char *cache;       
  curl_off_t cache_size; 
  bool dont_check;  
  bool no_transfer; 
  long response_time; 
  bool ctl_valid;   
  bool cwddone;     
  bool cwdfail;     
  char *prevpath;   

  size_t nread_resp; 
  char *linestart_resp; 

  int count1; 
  int count2; 
  int count3; 
  char *sendthis; 
  size_t sendleft; 
  size_t sendsize; 
  struct timeval response; 
  ftpstate state; 
  curl_off_t downloadsize;
}; 

struct FILEPROTO {
  char *path; 
  char *freepath; 
  int fd;     
}; 

struct ConnectBits {
  bool close; 
  bool reuse; 
  bool chunk; 
  bool httpproxy;    
  bool user_passwd;    
  bool proxy_user_passwd; 
  bool ipv6_ip; 
  bool ipv6;    
  bool use_range;
  bool rangestringalloc; 

  bool do_more; 

  bool upload_chunky; 
  bool getheader;     

  bool forbidchunk;   

  bool tcpconnect;    
  bool protoconnstart;

  bool retry;         
  bool no_body;       
  bool tunnel_proxy;  
  bool authneg;       
  bool rewindaftersend;
  bool ftp_use_epsv;  

  bool ftp_use_eprt;  
  bool netrc;         

  bool trailerHdrPresent; 
  bool done;          
};

struct hostname {
  char *rawalloc; 
  char *encalloc; 
  char *name;     
  char *dispname; 
};  

struct Curl_transfer_keeper {
  curl_off_t bytecount;         
  curl_off_t writebytecount;    
  struct timeval start;         
  struct timeval now;           
  bool header;                  
  enum {
    HEADER_NORMAL,              
    HEADER_PARTHEADER,          
    HEADER_ALLBAD               
  } badheader;                  
  int headerline;               
  char *hbufp;                  
  size_t hbuflen;
  char *str;                    
  char *str_start;              
  char *end_ptr;                
  char *p;                      
  bool content_range;           
  curl_off_t offset;            
  int httpcode;                 
  int httpversion;              
  struct timeval start100;      
  bool write_after_100_header;  
  bool wait100_after_headers;   
  int content_encoding;         

#define IDENTITY 0              
#define DEFLATE 1               
#define GZIP 2                  
#define COMPRESS 3              

#ifdef HAVE_LIBZ
  bool zlib_init;               
  z_stream z;                   
#endif

  time_t timeofdoc;
  long bodywrites;

  char *buf;
  char *uploadbuf;
  curl_socket_t maxfd;

  int keepon;

  bool upload_done; 

  bool ignorebody;  
  bool ignorecl;    
};

#if defined(USE_ARES) || defined(USE_THREADING_GETHOSTBYNAME) || \
    defined(USE_THREADING_GETADDRINFO)
struct Curl_async {
  char *hostname;
  int port;
  struct Curl_dns_entry *dns;
  bool done;  
  int status; 
  void *os_specific;  
};
#endif

#define FIRSTSOCKET     0
#define SECONDARYSOCKET 1 

struct connectdata {
  
  struct SessionHandle *data; 
  long connectindex; 

  long protocol; 
#define PROT_MISSING (1<<0)
#define PROT_HTTP    (1<<2)
#define PROT_HTTPS   (1<<3)
#define PROT_FTP     (1<<4)
#define PROT_TELNET  (1<<5)
#define PROT_DICT    (1<<6)
#define PROT_LDAP    (1<<7)
#define PROT_FILE    (1<<8)
#define PROT_TFTP    (1<<11)
#define PROT_FTPS    (1<<9)
#define PROT_SSL     (1<<10) 

  
  struct Curl_dns_entry *dns_entry;

  
  Curl_addrinfo *ip_addr;

  
  char *ip_addr_str;

  char protostr[16];  
  int socktype;  

  struct hostname host;
  struct hostname proxy;

  char *pathbuffer;
  char *path;      
  long port;       
  unsigned short remote_port; 
  curl_off_t bytecount;
  long headerbytecount;  
  long deductheadercount; 

  char *range; 
  curl_off_t resume_from; 

  char *user;    
  char *passwd;  

  char *proxyuser;    
  char *proxypasswd;  

  struct timeval now;     
  struct timeval created; 
  curl_socket_t sock[2]; 
  curl_off_t maxdownload; 

  struct ssl_connect_data ssl[2]; 
  struct ssl_config_data ssl_config;

  struct ConnectBits bits;    

  
  CURLcode (*curl_do)(struct connectdata *, bool *done);
  CURLcode (*curl_done)(struct connectdata *, CURLcode);

  
  CURLcode (*curl_do_more)(struct connectdata *);

  
  CURLcode (*curl_connect)(struct connectdata *, bool *done);

  
  CURLcode (*curl_connecting)(struct connectdata *, bool *done);
  CURLcode (*curl_doing)(struct connectdata *, bool *done);

  
  CURLcode (*curl_proto_fdset)(struct connectdata *conn,
                               fd_set *read_fd_set,
                               fd_set *write_fd_set,
                               int *max_fdp);

  
  CURLcode (*curl_doing_fdset)(struct connectdata *conn,
                               fd_set *read_fd_set,
                               fd_set *write_fd_set,
                               int *max_fdp);

  
  CURLcode (*curl_disconnect)(struct connectdata *);

  
  CURLcode (*curl_close)(struct connectdata *);

  

  
  curl_socket_t sockfd;   
  curl_off_t size;        
  curl_off_t *bytecountp; 

  
  curl_socket_t writesockfd; 
  curl_off_t *writebytecountp; 

  
  
  struct dynamically_allocated_data {
    char *proxyuserpwd; 
    char *uagent; 
    char *accept_encoding; 
    char *userpwd; 
    char *rangeline; 
    char *ref; 
    char *host; 
    char *cookiehost; 
  } allocptr;

  char *newurl; 

  int sec_complete; 
#ifdef HAVE_KRB4
  enum protection_level command_prot;
  enum protection_level data_prot;
  enum protection_level request_data_prot;
  size_t buffer_size;
  struct krb4buffer in_buffer, out_buffer;
  void *app_data;
  const struct Curl_sec_client_mech *mech;
  struct sockaddr_in local_addr;
#endif

  
  
  union {
    struct HTTP *http;
    struct HTTP *https;  
    struct FTP *ftp;
    void *tftp;        
    struct FILEPROTO *file;
    void *telnet;        
    void *generic;
  } proto;

  
  struct Curl_transfer_keeper keep;

  
  ssize_t upload_present;

   
  char *upload_fromhere;

  curl_read_callback fread; 
  void *fread_in;           

  struct ntlmdata ntlm;     
  struct ntlmdata proxyntlm; 

  char syserr_buf [256]; 

#if defined(USE_ARES) || defined(USE_THREADING_GETHOSTBYNAME) || \
    defined(USE_THREADING_GETADDRINFO)
  
  struct Curl_async async;
#endif
  struct connectdata *sec_conn;   

  enum { NORMAL, SOURCE3RD, TARGET3RD } xfertype;

  
  char *trailer; 
  int trlMax;    
  int trlPos;    

};   

struct PureInfo {
  int httpcode;  
  int httpproxycode;
  int httpversion;
  long filetime; 
  long header_size;  
  long request_size; 

  long proxyauthavail;
  long httpauthavail;

  long numconnects; 

  char *contenttype; 
}; 

struct Progress {
  long lastshow; 
  curl_off_t size_dl; 
  curl_off_t size_ul; 
  curl_off_t downloaded; 
  curl_off_t uploaded; 

  curl_off_t current_speed; 

  bool callback;  
  int width; 
  int flags; 

  double timespent;

  curl_off_t dlspeed;
  curl_off_t ulspeed;

  double t_nslookup;
  double t_connect;
  double t_pretransfer;
  double t_starttransfer;
  double t_redirect;

  struct timeval start;
  struct timeval t_startsingle;
#define CURR_TIME (5+1) 

  curl_off_t speeder[ CURR_TIME ];
  struct timeval speeder_time[ CURR_TIME ];
  int speeder_c;
};

typedef enum {
  HTTPREQ_NONE, 
  HTTPREQ_GET,
  HTTPREQ_POST,
  HTTPREQ_POST_FORM, 
  HTTPREQ_PUT,
  HTTPREQ_HEAD,
  HTTPREQ_CUSTOM,
  HTTPREQ_LAST 
} Curl_HttpReq; 

#define MAX_CURL_USER_LENGTH 256
#define MAX_CURL_PASSWORD_LENGTH 256
#define MAX_CURL_USER_LENGTH_TXT "255"
#define MAX_CURL_PASSWORD_LENGTH_TXT "255"

struct auth {
  long want;  
  long picked;
  long avail; 
  bool done;  
  bool multi; 

};

struct UrlState {
  enum {
    Curl_if_none,
    Curl_if_easy,
    Curl_if_multi
  } used_interface;

  
  struct timeval keeps_speed; 

  
  struct connectdata **connects;
  long numconnects; 
  int lastconnect;  

  char *headerbuff; 
  size_t headersize;   

  char buffer[BUFSIZE+1]; 
  char uploadbuffer[BUFSIZE+1]; 
  curl_off_t current_speed;  
  bool this_is_a_follow; 

  char *first_host; 

  struct curl_ssl_session *session; 
  long sessionage;                  

  char *scratch; 
  bool errorbuf; 
  int os_errno;  
#ifdef HAVE_SIGNAL
  
  void (*prev_signal)(int sig);
#endif
  bool allow_port; 

  struct digestdata digest;
  struct digestdata proxydigest;

#ifdef HAVE_GSSAPI
  struct negotiatedata negotiate;
#endif

  struct auth authhost;
  struct auth authproxy;

  bool authproblem; 
#ifdef USE_ARES
  ares_channel areschannel; 
#endif

#if defined(USE_SSLEAY) && defined(HAVE_OPENSSL_ENGINE_H)
  ENGINE *engine;
#endif 
};   

struct DynamicStatic {
  char *url;        
  bool url_alloc;   
  bool url_changed; 
  char *proxy;      
  bool proxy_alloc; 
  char *referer;    
  bool referer_alloc; 
  struct curl_slist *cookielist; 
};  

struct UserDefined {
  FILE *err;         
  void *debugdata;   
  char *errorbuffer; 
  char *proxyuserpwd;  
  long proxyport; 
  void *out;         
  void *in;          
  void *writeheader; 
  char *set_url;     
  char *set_proxy;   
  long use_port;     
  char *userpwd;     
  long httpauth;     
  long proxyauth;    
  char *set_range;   
  long followlocation; 
  long maxredirs;    
  char *set_referer; 
  bool free_referer; 
  char *useragent;   
  char *encoding;    
  char *postfields;  
  curl_off_t postfieldsize; 
  char *ftpport;     
  char *device;      
  unsigned short localport; 
  int localportrange; 
  curl_write_callback fwrite;        
  curl_write_callback fwrite_header; 
  curl_read_callback fread;          
  curl_progress_callback fprogress;  
  curl_debug_callback fdebug;      
  curl_ioctl_callback ioctl;       
  void *progress_client; 
  void *ioctl_client;   
  long timeout;         
  long connecttimeout;  
  long ftp_response_timeout; 
  curl_off_t infilesize;      
  long low_speed_limit; 
  long low_speed_time;  
  curl_off_t set_resume_from;  
  char *cookie;         
  struct curl_slist *headers; 
  struct curl_httppost *httppost;  
  char *cert;           
  char *cert_type;      
  char *key;            
  char *key_type;       
  char *key_passwd;     
  char *cookiejar;      
  bool cookiesession;   
  bool crlf;            
  char *ftp_account;    
  struct curl_slist *quote;     
  struct curl_slist *postquote; 
  struct curl_slist *prequote; 
  struct curl_slist *source_quote;  
  struct curl_slist *source_prequote;  
  struct curl_slist *source_postquote; 
  struct curl_slist *telnet_options; 
  curl_TimeCond timecondition; 
  time_t timevalue;       
  curl_closepolicy closepolicy; 
  Curl_HttpReq httpreq;   
  char *customrequest;    
  long httpversion; 
  char *auth_host; 
  char *krb4_level; 
  struct ssl_config_data ssl;  

  curl_proxytype proxytype; 

  int dns_cache_timeout; 
  long buffer_size;      

  char *private_data; 

  struct curl_slist *http200aliases; 

  long ip_version;

  curl_off_t max_filesize; 

  char *source_url;     
  char *source_userpwd;  

  curl_ftpfile ftp_filemethod;   

  bool printhost;       
  bool get_filetime;
  bool tunnel_thru_httpproxy;
  bool ftp_append;
  bool ftp_ascii;
  bool ftp_list_only;
  bool ftp_create_missing_dirs;
  bool ftp_use_port;
  bool hide_progress;
  bool http_fail_on_error;
  bool http_follow_location;
  bool http_disable_hostname_check_before_authentication;
  bool include_header;   
  bool http_set_referer;
  bool http_auto_referer; 
  bool opt_no_body;      
  bool set_port;
  bool upload;
  enum CURL_NETRC_OPTION
       use_netrc;        
  char *netrc_file;      
  bool verbose;
  bool krb4;             
  bool reuse_forbid;     
  bool reuse_fresh;      
  bool expect100header;  
  bool ftp_use_epsv;     
  bool ftp_use_eprt;     
  curl_ftpssl ftp_ssl;   
  curl_ftpauth ftpsslauth; 
  bool no_signal;        
  bool global_dns_cache; 
  bool tcp_nodelay;      
  bool ignorecl;         
  bool ftp_skip_ip;      
  bool connect_only;     
};  

struct SessionHandle {
  struct curl_hash *hostcache;
  void *multi;                 
  struct Curl_share *share;    
  struct UserDefined set;      
  struct DynamicStatic change; 

  struct CookieInfo *cookies;  
  struct Progress progress;    
  struct UrlState state;       
  struct PureInfo info;        
};

#define LIBCURL_NAME "libcurl"

#endif
