/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __IF2IP_H
#define __IF2IP_H

#include "setup.h"

extern char *Curl_if2ip(int af, const char *interf, char *buf, int buf_size);

#ifdef __INTERIX
#include <sys/socket.h>


struct ifreq {
#define IFNAMSIZ 16
#define IFHWADDRLEN 6
  union {
    char ifrn_name[IFNAMSIZ]; 
  } ifr_ifrn;

 union {
   struct sockaddr ifru_addr;
   struct sockaddr ifru_broadaddr;
   struct sockaddr ifru_netmask;
   struct sockaddr ifru_hwaddr;
   short ifru_flags;
   int ifru_metric;
   int ifru_mtu;
 } ifr_ifru;
};


#define ifr_dstaddr ifr_addr

#define ifr_name ifr_ifrn.ifrn_name 
#define ifr_addr ifr_ifru.ifru_addr 
#define ifr_broadaddr ifr_ifru.ifru_broadaddr 
#define ifr_netmask ifr_ifru.ifru_netmask 
#define ifr_flags ifr_ifru.ifru_flags 
#define ifr_hwaddr ifr_ifru.ifru_hwaddr 
#define ifr_metric ifr_ifru.ifru_metric 
#define ifr_mtu ifr_ifru.ifru_mtu 

#define SIOCGIFADDR _IOW('s', 102, struct ifreq) 
#endif 

#endif
