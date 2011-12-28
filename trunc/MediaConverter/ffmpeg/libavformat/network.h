/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_NETWORK_H
#define AVFORMAT_NETWORK_H

#include "config.h"

#if HAVE_WINSOCK2_H
#include <winsock2.h>
#include <ws2tcpip.h>

#define ff_neterrno() (-WSAGetLastError())
#define FF_NETERROR(err) (-WSA##err)
#define WSAEAGAIN WSAEWOULDBLOCK
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define ff_neterrno() AVERROR(errno)
#define FF_NETERROR(err) AVERROR(err)
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

int ff_socket_nonblock(int socket, int enable);

static inline int ff_network_init(void)
{
#if HAVE_WINSOCK2_H
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1,1), &wsaData))
        return 0;
#endif
    return 1;
}

static inline void ff_network_close(void)
{
#if HAVE_WINSOCK2_H
    WSACleanup();
#endif
}

int ff_inet_aton (const char * str, struct in_addr * add);

#if !HAVE_STRUCT_SOCKADDR_STORAGE
struct sockaddr_storage {
#if HAVE_STRUCT_SOCKADDR_SA_LEN
    uint8_t ss_len;
    uint8_t ss_family;
#else
    uint16_t ss_family;
#endif
    char ss_pad1[6];
    int64_t ss_align;
    char ss_pad2[112];
};
#endif

#if !HAVE_STRUCT_ADDRINFO
struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    int ai_addrlen;
    struct sockaddr *ai_addr;
    char *ai_canonname;
    struct addrinfo *ai_next;
};
#endif


#ifndef EAI_FAIL
#define EAI_FAIL 4
#endif

#ifndef EAI_FAMILY
#define EAI_FAMILY 5
#endif

#ifndef EAI_NONAME
#define EAI_NONAME 8
#endif

#ifndef AI_PASSIVE
#define AI_PASSIVE 1
#endif

#ifndef AI_CANONNAME
#define AI_CANONNAME 2
#endif

#ifndef AI_NUMERICHOST
#define AI_NUMERICHOST 4
#endif

#ifndef NI_NOFQDN
#define NI_NOFQDN 1
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 2
#endif

#ifndef NI_NAMERQD
#define NI_NAMERQD 4
#endif

#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV 8
#endif

#ifndef NI_DGRAM
#define NI_DGRAM 16
#endif

#if !HAVE_GETADDRINFO
int ff_getaddrinfo(const char *node, const char *service,
                   const struct addrinfo *hints, struct addrinfo **res);
void ff_freeaddrinfo(struct addrinfo *res);
int ff_getnameinfo(const struct sockaddr *sa, int salen,
                   char *host, int hostlen,
                   char *serv, int servlen, int flags);
const char *ff_gai_strerror(int ecode);
#define getaddrinfo ff_getaddrinfo
#define freeaddrinfo ff_freeaddrinfo
#define getnameinfo ff_getnameinfo
#define gai_strerror ff_gai_strerror
#endif

#endif 
