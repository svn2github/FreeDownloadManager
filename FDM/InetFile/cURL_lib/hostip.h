/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __HOSTIP_H
#define __HOSTIP_H 

#include "setup.h"
#include "hash.h"

#ifdef NETWARE
#undef in_addr_t
#define in_addr_t unsigned long
#endif  

#ifdef USE_ARES
#include <ares_version.h>
#endif

#ifdef USE_ARES
#define CURLRES_ASYNCH
#define CURLRES_ARES
#endif

#ifdef USE_THREADING_GETHOSTBYNAME
#define CURLRES_ASYNCH
#define CURLRES_THREADED
#endif

#ifdef USE_THREADING_GETADDRINFO
#define CURLRES_ASYNCH
#define CURLRES_THREADED
#endif

#ifdef ENABLE_IPV6
#define CURLRES_IPV6
#else
#define CURLRES_IPV4
#endif

#if defined(CURLRES_IPV4) || defined(CURLRES_ARES)
#if !defined(HAVE_GETHOSTBYNAME_R) || defined(CURLRES_ASYNCH)

#define CURLRES_ADDRINFO_COPY
#endif
#endif 

#ifndef CURLRES_ASYNCH
#define CURLRES_SYNCH
#endif

#ifndef USE_LIBIDN
#define CURLRES_IDN
#endif 

#define CURL_HOSTENT_SIZE 9000

#define CURL_TIMEOUT_RESOLVE 300 

#ifdef CURLRES_ARES
#define CURL_ASYNC_SUCCESS ARES_SUCCESS
#if ARES_VERSION >= 0x010500

#define HAVE_CARES_CALLBACK_TIMEOUTS 1
#endif
#else
#define CURL_ASYNC_SUCCESS CURLE_OK
#define ares_cancel(x) do {} while(0)
#define ares_destroy(x) do {} while(0)
#endif 

#ifdef CURLRES_IPV6
typedef struct addrinfo Curl_addrinfo;
#else

struct Curl_addrinfo {
  int                   ai_flags;
  int                   ai_family;
  int                   ai_socktype;
  int                   ai_protocol;
  socklen_t             ai_addrlen;   
  char                 *ai_canonname;
  struct sockaddr      *ai_addr;
  struct Curl_addrinfo *ai_next;
};
typedef struct Curl_addrinfo Curl_addrinfo;
#endif

struct addrinfo;
struct hostent;
struct SessionHandle;
struct connectdata; 

struct curl_hash *Curl_global_host_cache_init(void);
void Curl_global_host_cache_dtor(void);

struct Curl_dns_entry {
  Curl_addrinfo *addr;
  time_t timestamp;
  long inuse;      
};  

#define CURLRESOLV_ERROR    -1
#define CURLRESOLV_RESOLVED  0
#define CURLRESOLV_PENDING   1
int Curl_resolv(struct connectdata *conn, const char *hostname,
                int port, struct Curl_dns_entry **dnsentry); 

bool Curl_ipvalid(struct SessionHandle *data); 

Curl_addrinfo *Curl_getaddrinfo(struct connectdata *conn,
                                const char *hostname,
                                int port,
                                int *waitp);

CURLcode Curl_is_resolved(struct connectdata *conn,
                          struct Curl_dns_entry **dns);
CURLcode Curl_wait_for_resolv(struct connectdata *conn,
                              struct Curl_dns_entry **dnsentry); 

int Curl_resolv_getsock(struct connectdata *conn, curl_socket_t *sock,
                        int numsocks); 

void Curl_resolv_unlock(struct SessionHandle *data,
                        struct Curl_dns_entry *dns); 

void Curl_scan_cache_used(void *user, void *ptr); 

void Curl_freeaddrinfo(Curl_addrinfo *freeaddr); 

struct curl_hash *Curl_mk_dnscache(void); 

void Curl_hostcache_prune(struct SessionHandle *data); 

int Curl_num_addresses (const Curl_addrinfo *addr);

#ifdef CURLDEBUG
void curl_dofreeaddrinfo(struct addrinfo *freethis,
                         int line, const char *source);
int curl_dogetaddrinfo(const char *hostname, const char *service,
                       struct addrinfo *hints,
                       struct addrinfo **result,
                       int line, const char *source);
#ifdef HAVE_GETNAMEINFO
int curl_dogetnameinfo(GETNAMEINFO_QUAL_ARG1 GETNAMEINFO_TYPE_ARG1 sa,
                       GETNAMEINFO_TYPE_ARG2 salen,
                       char *host, GETNAMEINFO_TYPE_ARG46 hostlen,
                       char *serv, GETNAMEINFO_TYPE_ARG46 servlen,
                       GETNAMEINFO_TYPE_ARG7 flags,
                       int line, const char *source);
#endif
#endif 

CURLcode Curl_addrinfo4_callback(void *arg,
                                 int status,
#ifdef HAVE_CARES_CALLBACK_TIMEOUTS
                                 int timeouts,
#endif
                                 struct hostent *hostent);

CURLcode Curl_addrinfo6_callback(void *arg,
                                 int status,
#ifdef HAVE_CARES_CALLBACK_TIMEOUTS
                                 int timeouts,
#endif
                                 struct addrinfo *ai);  

Curl_addrinfo *Curl_ip2addr(in_addr_t num, const char *hostname, int port); 

Curl_addrinfo *Curl_he2ai(const struct hostent *, int port); 

Curl_addrinfo *Curl_addrinfo_copy(const void *orig, int port); 

const char *Curl_printable_address(const Curl_addrinfo *ip,
                                   char *buf, size_t bufsize); 

struct Curl_dns_entry *
Curl_cache_addr(struct SessionHandle *data, Curl_addrinfo *addr,
                const char *hostname, int port); 

struct Curl_async; 
void Curl_destroy_thread_data(struct Curl_async *async);

#ifndef INADDR_NONE
#define CURL_INADDR_NONE (in_addr_t) ~0
#else
#define CURL_INADDR_NONE INADDR_NONE
#endif   

#endif
