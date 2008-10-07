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
#define PORT_LDAPS 636
#define PORT_TFTP 69
#define PORT_SSH 22

#define DICT_MATCH "/MATCH:"
#define DICT_MATCH2 "/M:"
#define DICT_MATCH3 "/FIND:"
#define DICT_DEFINE "/DEFINE:"
#define DICT_DEFINE2 "/D:"
#define DICT_DEFINE3 "/LOOKUP:"

#define CURL_DEFAULT_USER "anonymous"
#define CURL_DEFAULT_PASSWORD "ftp@example.com"

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
#ifdef USE_GNUTLS
#error Configuration error; cannot use GnuTLS *and* OpenSSL.
#endif
#endif 

#ifdef USE_GNUTLS
#include <gnutls/gnutls.h>
#endif

#ifdef USE_NSS
#include <nspr.h>
#endif

#ifdef USE_QSOSSL
#include <qsossl.h>
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
#include "splay.h"

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

#ifdef HAVE_LIBSSH2_H
#include <libssh2.h>
#include <libssh2_sftp.h>
#endif  

#undef BUFSIZE
#define BUFSIZE CURL_MAX_WRITE_SIZE 

#define HEADERSIZE 256

#define CURLEASY_MAGIC_NUMBER 0xc0dedbadU 

#define CURLMAX(x,y) ((x)>(y)?(x):(y))

#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)

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
  prot_private,
  prot_cmd
};
#endif 

typedef enum {
  ssl_connect_1,
  ssl_connect_2,
  ssl_connect_2_reading,
  ssl_connect_2_writing,
  ssl_connect_3,
  ssl_connect_done
} ssl_connect_state;

typedef enum {
  ssl_connection_none,
  ssl_connection_negotiating,
  ssl_connection_complete
} ssl_connection_state; 

struct ssl_connect_data {
  
  bool use;
  ssl_connection_state state;
#ifdef USE_SSLEAY
  
  SSL_CTX* ctx;
  SSL*     handle;
  X509*    server_cert;
  ssl_connect_state connecting_state;
#endif 
#ifdef USE_GNUTLS
  gnutls_session session;
  gnutls_certificate_credentials cred;
#endif 
#ifdef USE_NSS
  PRFileDesc *handle;
  char *client_nickname;
#endif 
#ifdef USE_QSOSSL
  SSLHandle *handle;
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
  bool sessionid;        
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

#if defined(CURL_DOES_CONVERSIONS) && defined(HAVE_ICONV)
#include <iconv.h>
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
  unsigned int flags;
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

  struct back {
    curl_read_callback fread_func; 
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
  FTP_CCC,
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

typedef enum {
  FTPTRANSFER_BODY, 
  FTPTRANSFER_INFO, 
  FTPTRANSFER_NONE, 
  FTPTRANSFER_LAST  
} curl_ftptransfer; 

struct FTP {
  curl_off_t *bytecountp;
  char *user;    
  char *passwd;  

  
  curl_ftptransfer transfer;
  curl_off_t downloadsize;
}; 

struct ftp_conn {
  char *entrypath; 
  char **dirs;   
  int dirdepth;  
  int diralloc;  
  char *file;    
  char *cache;       
  curl_off_t cache_size; 
  bool dont_check;  
  long response_time; 
  bool ctl_valid;   
  bool cwddone;     
  bool cwdfail;     
  char *prevpath;   
  char transfertype; 
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
}; 

typedef enum {
  SSH_NO_STATE = -1,  
  SSH_STOP = 0,       

  SSH_S_STARTUP,      
  SSH_AUTHLIST,
  SSH_AUTH_PKEY_INIT,
  SSH_AUTH_PKEY,
  SSH_AUTH_PASS_INIT,
  SSH_AUTH_PASS,
  SSH_AUTH_HOST_INIT,
  SSH_AUTH_HOST,
  SSH_AUTH_KEY_INIT,
  SSH_AUTH_KEY,
  SSH_AUTH_DONE,
  SSH_SFTP_INIT,
  SSH_SFTP_REALPATH,   

  SSH_SFTP_QUOTE_INIT, 
  SSH_SFTP_POSTQUOTE_INIT, 
  SSH_SFTP_QUOTE,
  SSH_SFTP_NEXT_QUOTE,
  SSH_SFTP_QUOTE_STAT,
  SSH_SFTP_QUOTE_SETSTAT,
  SSH_SFTP_QUOTE_SYMLINK,
  SSH_SFTP_QUOTE_MKDIR,
  SSH_SFTP_QUOTE_RENAME,
  SSH_SFTP_QUOTE_RMDIR,
  SSH_SFTP_QUOTE_UNLINK,
  SSH_SFTP_TRANS_INIT,
  SSH_SFTP_UPLOAD_INIT,
  SSH_SFTP_CREATE_DIRS_INIT,
  SSH_SFTP_CREATE_DIRS,
  SSH_SFTP_CREATE_DIRS_MKDIR,
  SSH_SFTP_READDIR_INIT,
  SSH_SFTP_READDIR,
  SSH_SFTP_READDIR_LINK,
  SSH_SFTP_READDIR_BOTTOM,
  SSH_SFTP_READDIR_DONE,
  SSH_SFTP_DOWNLOAD_INIT,
  SSH_SFTP_DOWNLOAD_STAT, 
  SSH_SFTP_CLOSE,    
  SSH_SFTP_SHUTDOWN, 
  SSH_SCP_TRANS_INIT, 
  SSH_SCP_UPLOAD_INIT,
  SSH_SCP_DOWNLOAD_INIT,
  SSH_SCP_DONE,
  SSH_SCP_SEND_EOF,
  SSH_SCP_WAIT_EOF,
  SSH_SCP_WAIT_CLOSE,
  SSH_SCP_CHANNEL_FREE,   
  SSH_SESSION_DISCONNECT, 
  SSH_SESSION_FREE,       
  SSH_QUIT,
  SSH_LAST  
} sshstate; 

struct SSHPROTO {
  char *path;                   
}; 

struct ssh_conn {
  const char *authlist;       
#ifdef USE_LIBSSH2
  const char *passphrase;     
  char *rsa_pub;              
  char *rsa;                  
  bool authed;                
  sshstate state;             
  sshstate nextstate;         
  CURLcode actualcode;        
  struct curl_slist *quote_item; 
  char *quote_path1;          
  char *quote_path2;
  LIBSSH2_SFTP_ATTRIBUTES quote_attrs; 
  char *homedir;              

  
  LIBSSH2_SFTP_ATTRIBUTES readdir_attrs;
  char *readdir_filename;
  char *readdir_longentry;
  int readdir_len, readdir_totalLen, readdir_currLen;
  char *readdir_line;
  char *readdir_linkPath;
  

  int secondCreateDirs;         
  char *slash_pos;              
  LIBSSH2_SESSION *ssh_session; 
  LIBSSH2_CHANNEL *ssh_channel; 
  LIBSSH2_SFTP *sftp_session;   
  LIBSSH2_SFTP_HANDLE *sftp_handle;
#endif 
};  

struct FILEPROTO {
  char *path; 
  char *freepath; 
  int fd;     
}; 

struct ConnectBits {
  bool close; 
  bool reuse; 
  bool proxy; 
  bool httpproxy;    
  bool user_passwd;    
  bool proxy_user_passwd; 
  bool ipv6_ip; 
  bool ipv6;    

  bool do_more; 

  bool tcpconnect;    
  bool protoconnstart;

  bool retry;         
  bool tunnel_proxy;  
  bool tunnel_connecting; 
  bool authneg;       
  bool rewindaftersend;
  bool ftp_use_epsv;  

  bool ftp_use_eprt;  
  bool netrc;         

  bool done;          
  bool stream_was_rewound; 
  bool proxy_connect_closed; 
  bool bound; 
};

struct hostname {
  char *rawalloc; 
  char *encalloc; 
  char *name;     
  char *dispname; 
};  

#define KEEP_NONE  0
#define KEEP_READ  (1<<0)     
#define KEEP_WRITE (1<<1)     
#define KEEP_READ_HOLD (1<<2) 
#define KEEP_WRITE_HOLD (1<<3) 
#define KEEP_READ_PAUSE (1<<4) 
#define KEEP_WRITE_PAUSE (1<<5) 

#define KEEP_READBITS (KEEP_READ | KEEP_READ_HOLD | KEEP_READ_PAUSE)
#define KEEP_WRITEBITS (KEEP_WRITE | KEEP_WRITE_HOLD | KEEP_WRITE_PAUSE) 

#ifdef HAVE_LIBZ
typedef enum {
  ZLIB_UNINIT,          
  ZLIB_INIT,            
  ZLIB_GZIP_HEADER,     
  ZLIB_GZIP_INFLATING,  
  ZLIB_INIT_GZIP        
} zlibInitState;
#endif

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

typedef CURLcode (*Curl_do_more_func)(struct connectdata *);
typedef CURLcode (*Curl_done_func)(struct connectdata *, CURLcode, bool); 

enum expect100 {
  EXP100_SEND_DATA,           
  EXP100_AWAITING_CONTINUE,   
  EXP100_SENDING_REQUEST,     
  EXP100_FAILED               
}; 

struct SingleRequest {
  curl_off_t size;        
  curl_off_t *bytecountp; 

  curl_off_t maxdownload; 
  curl_off_t *writebytecountp; 

  curl_off_t bytecount;         
  curl_off_t writebytecount;    

  long headerbytecount;         
  long deductheadercount; 

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
  enum expect100 exp100;        

  int content_encoding;         

#define IDENTITY 0              
#define DEFLATE 1               
#define GZIP 2                  
#define COMPRESS 3              

#ifdef HAVE_LIBZ
  zlibInitState zlib_init;      
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

  char *location;   
  char *newurl;     

  
  ssize_t upload_present;

   
  char *upload_fromhere;

  bool chunk; 
  bool upload_chunky; 
  bool getheader;     

  bool forbidchunk;   
  bool trailerhdrpresent; 
};  

struct Curl_handler {
  const char * scheme;        

  
  CURLcode (*setup_connection)(struct connectdata *);

  
  CURLcode (*do_it)(struct connectdata *, bool *done);
  Curl_done_func done;

  
  Curl_do_more_func do_more;

  
  CURLcode (*connect_it)(struct connectdata *, bool *done);

  
  CURLcode (*connecting)(struct connectdata *, bool *done);
  CURLcode (*doing)(struct connectdata *, bool *done);

  
  int (*proto_getsock)(struct connectdata *conn,
                       curl_socket_t *socks,
                       int numsocks);

  
  int (*doing_getsock)(struct connectdata *conn,
                       curl_socket_t *socks,
                       int numsocks);

  
  CURLcode (*disconnect)(struct connectdata *);

  long defport;       
  long protocol;      
}; 

struct connectdata {
  
  struct SessionHandle *data;

  
  struct Curl_chunker chunk;

  bool inuse; 

  
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
#define PROT_FTPS    (1<<9)
#define PROT_SSL     (1<<10) 
#define PROT_TFTP    (1<<11)
#define PROT_SCP     (1<<12)
#define PROT_SFTP    (1<<13)

#define PROT_CLOSEACTION PROT_FTP 

  
  struct Curl_dns_entry *dns_entry;

  
  Curl_addrinfo *ip_addr;

  
  char *ip_addr_str;

  char protostr[16];  
  int socktype;  

  struct hostname host;
  struct hostname proxy;

  long port;       
  unsigned short remote_port; 

  char *user;    
  char *passwd;  

  char *proxyuser;    
  char *proxypasswd;  
  curl_proxytype proxytype; 

  struct timeval now;     
  struct timeval created; 
  curl_socket_t sock[2]; 

  struct ssl_connect_data ssl[2]; 
  struct ssl_config_data ssl_config;

  struct ConnectBits bits;    

  const struct Curl_handler * handler;  

  

  curl_socket_t sockfd;   
  curl_socket_t writesockfd; 

  
  
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

  int sec_complete; 
#if defined(HAVE_KRB4) || defined(HAVE_GSSAPI)
  enum protection_level command_prot;
  enum protection_level data_prot;
  enum protection_level request_data_prot;
  size_t buffer_size;
  struct krb4buffer in_buffer, out_buffer;
  void *app_data;
  const struct Curl_sec_client_mech *mech;
  struct sockaddr_in local_addr;
#endif

  bool readchannel_inuse;  
  bool writechannel_inuse; 
  bool server_supports_pipelining; 

  struct curl_llist *send_pipe; 
  struct curl_llist *recv_pipe; 
  struct curl_llist *pend_pipe; 
#define MAX_PIPELINE_LENGTH 5

  char* master_buffer; 
  size_t read_pos; 
  size_t buf_len;  

  curl_seek_callback seek_func; 
  void *seek_client;            

  

  
  curl_read_callback fread_func; 
  void *fread_in;           

  struct ntlmdata ntlm;     
  struct ntlmdata proxyntlm; 

  char syserr_buf [256]; 

#if defined(USE_ARES) || defined(USE_THREADING_GETHOSTBYNAME) || \
    defined(USE_THREADING_GETADDRINFO)
  
  struct Curl_async async;
#endif

  
  char *trailer; 
  int trlMax;    
  int trlPos;    

  union {
    struct ftp_conn ftpc;
    struct ssh_conn sshc;
  } proto;

  int cselect_bits; 
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
  char *wouldredirect; 
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

struct conncache {
  
  struct connectdata **connects;
  long num;           
  enum {
    CONNCACHE_PRIVATE, 
    CONNCACHE_MULTI    
  } type;
}; 

struct UrlState {
  enum {
    Curl_if_none,
    Curl_if_easy,
    Curl_if_multi
  } used_interface;

  struct conncache *connc; 

  
  struct timeval keeps_speed; 

  long lastconnect;  

  char *headerbuff; 
  size_t headersize;   

  char buffer[BUFSIZE+1]; 
  char uploadbuffer[BUFSIZE+1]; 
  curl_off_t current_speed;  
  bool this_is_a_follow; 

  char *first_host; 
  struct curl_ssl_session *session; 
  long sessionage;                  
  char *tempwrite;      
  size_t tempwritesize; 
  int tempwritetype;    
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
  struct negotiatedata proxyneg; 
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
  struct timeval expiretime; 
  struct Curl_tree timenode; 

  
  char *most_recent_ftp_entrypath;

  
  bool ftp_trying_alternative;

  bool expect100header;  

  bool pipe_broke; 

#ifndef WIN32

#define CURL_DO_LINEEND_CONV
  
  bool prev_block_had_trailing_cr;
  
  curl_off_t crlf_conversions;
#endif
  
  void *shared_conn;
  bool closed; 
  char *pathbuffer;
  char *path;      

  bool use_range;
  bool rangestringalloc; 

  char *range; 
  curl_off_t resume_from; 

  
  union {
    struct HTTP *http;
    struct HTTP *https;  
    struct FTP *ftp;
    void *tftp;        
    struct FILEPROTO *file;
    void *telnet;        
    void *generic;
    struct SSHPROTO *ssh;
  } proto;
  
  struct connectdata *current_conn;
};   

struct DynamicStatic {
  char *url;        
  bool url_alloc;   
  char *referer;    
  bool referer_alloc; 
  struct curl_slist *cookielist; 
}; 

struct Curl_one_easy; 
struct Curl_multi;    

enum dupstring {
  STRING_CERT,            
  STRING_CERT_TYPE,       
  STRING_COOKIE,          
  STRING_COOKIEJAR,       
  STRING_CUSTOMREQUEST,   
  STRING_DEVICE,          
  STRING_ENCODING,        
  STRING_FTP_ACCOUNT,     
  STRING_FTP_ALTERNATIVE_TO_USER, 
  STRING_FTPPORT,         
  STRING_KEY,             
  STRING_KEY_PASSWD,      
  STRING_KEY_TYPE,        
  STRING_KRB_LEVEL,       
  STRING_NETRC_FILE,      
  STRING_COPYPOSTFIELDS,  
  STRING_PROXY,           
  STRING_PROXYUSERPWD,    
  STRING_SET_RANGE,       
  STRING_SET_REFERER,     
  STRING_SET_URL,         
  STRING_SSH_PRIVATE_KEY, 
  STRING_SSH_PUBLIC_KEY,  
  STRING_SSL_CAPATH,      
  STRING_SSL_CAFILE,      
  STRING_SSL_CIPHER_LIST, 
  STRING_SSL_EGDSOCKET,   
  STRING_SSL_RANDOM_FILE, 
  STRING_USERAGENT,       
  STRING_USERPWD,         
  STRING_SSH_HOST_PUBLIC_KEY_MD5, 

  
  STRING_LAST 
};

struct UserDefined {
  FILE *err;         
  void *debugdata;   
  char *errorbuffer; 
  long proxyport; 
  void *out;         
  void *in;          
  void *writeheader; 
  long use_port;     
  long httpauth;     
  long proxyauth;    
  long followlocation; 
  long maxredirs;    
  bool post301;      
  bool free_referer; 
  void *postfields;  
  curl_seek_callback seek_func;      
  curl_off_t postfieldsize; 
  unsigned short localport; 
  int localportrange; 
  curl_write_callback fwrite_func;   
  curl_write_callback fwrite_header; 
  curl_read_callback fread_func;     
  curl_progress_callback fprogress;  
  curl_debug_callback fdebug;      
  curl_ioctl_callback ioctl_func;  
  curl_sockopt_callback fsockopt;  
  void *sockopt_client; 
  curl_opensocket_callback fopensocket; 
  void* opensocket_client;

  void *seek_client;    
  
  
  curl_conv_callback convfromnetwork;
  
  curl_conv_callback convtonetwork;
  
  curl_conv_callback convfromutf8;

  void *progress_client; 
  void *ioctl_client;   
  long timeout;         
  long connecttimeout;  
  long ftp_response_timeout; 
  curl_off_t infilesize;      
  long low_speed_limit; 
  long low_speed_time;  
  curl_off_t max_send_speed; 
  curl_off_t max_recv_speed; 
  curl_off_t set_resume_from;  
  struct curl_slist *headers; 
  struct curl_httppost *httppost;  
  bool cookiesession;   
  bool crlf;            
  struct curl_slist *quote;     
  struct curl_slist *postquote; 
  struct curl_slist *prequote; 
  struct curl_slist *source_quote;  
  struct curl_slist *source_prequote;  
  struct curl_slist *source_postquote; 
  struct curl_slist *telnet_options; 
  curl_TimeCond timecondition; 
  time_t timevalue;       
  Curl_HttpReq httpreq;   
  long httpversion; 
  struct ssl_config_data ssl;  
  curl_proxytype proxytype; 
  long dns_cache_timeout; 
  long buffer_size;      
  void *private_data; 

  struct Curl_one_easy *one_easy; 

  struct curl_slist *http200aliases; 

  long ip_version; 

  curl_off_t max_filesize; 

  curl_ftpfile ftp_filemethod;   

  bool printhost;        
  bool get_filetime;     
  bool tunnel_thru_httpproxy; 
  bool prefer_ascii;     
  bool ftp_append;       
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
  bool verbose;          
  bool krb;              
  bool reuse_forbid;     
  bool reuse_fresh;      
  bool ftp_use_epsv;     
  bool ftp_use_eprt;     

  curl_usessl ftp_ssl;   
  curl_ftpauth ftpsslauth; 
  curl_ftpccc ftp_ccc;   
  bool no_signal;        
  bool global_dns_cache; 
  bool tcp_nodelay;      
  bool ignorecl;         
  bool ftp_skip_ip;      
  bool connect_only;     
  long ssh_auth_types;   
  bool http_te_skip;     
  bool http_ce_skip;     
  long new_file_perms;    
  long new_directory_perms; 
  bool proxy_transfer_mode; 
  char *str[STRING_LAST]; 
};

struct Names {
  struct curl_hash *hostcache;
  enum {
    HCACHE_NONE,    
    HCACHE_PRIVATE, 
    HCACHE_GLOBAL,  
    HCACHE_MULTI,   
    HCACHE_SHARED   
  } hostcachetype;
};  

struct SessionHandle {
  struct Names dns;
  struct Curl_multi *multi;    
  struct Curl_one_easy *multi_pos; 
  struct Curl_share *share;    
  struct SingleRequest req;    
  struct UserDefined set;      
  struct DynamicStatic change; 
  struct CookieInfo *cookies;  
  struct Progress progress;    
  struct UrlState state;       
  struct PureInfo info;        
#if defined(CURL_DOES_CONVERSIONS) && defined(HAVE_ICONV)
  iconv_t outbound_cd;         
  iconv_t inbound_cd;          
  iconv_t utf8_cd;             
#endif 
  unsigned int magic;          
};

#define LIBCURL_NAME "libcurl"

#endif
