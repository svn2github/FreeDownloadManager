/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/

#ifndef __ARPA_TELNET_H
#define __ARPA_TELNET_H

#ifndef CURL_DISABLE_TELNET

#define CURL_TELOPT_BINARY   0  
#define CURL_TELOPT_SGA      3  
#define CURL_TELOPT_EXOPL  255  
#define CURL_TELOPT_TTYPE   24  
#define CURL_TELOPT_XDISPLOC 35 

#define CURL_TELOPT_NEW_ENVIRON 39  
#define CURL_NEW_ENV_VAR   0
#define CURL_NEW_ENV_VALUE 1


static const char * const telnetoptions[]=
{
  "BINARY",      "ECHO",           "RCP",           "SUPPRESS GO AHEAD",
  "NAME",        "STATUS",         "TIMING MARK",   "RCTE",
  "NAOL",        "NAOP",           "NAOCRD",        "NAOHTS",
  "NAOHTD",      "NAOFFD",         "NAOVTS",        "NAOVTD",
  "NAOLFD",      "EXTEND ASCII",   "LOGOUT",        "BYTE MACRO",
  "DE TERMINAL", "SUPDUP",         "SUPDUP OUTPUT", "SEND LOCATION",
  "TERM TYPE",   "END OF RECORD",  "TACACS UID",    "OUTPUT MARKING",
  "TTYLOC",      "3270 REGIME",    "X3 PAD",        "NAWS",
  "TERM SPEED",  "LFLOW",          "LINEMODE",      "XDISPLOC",
  "OLD-ENVIRON", "AUTHENTICATION", "ENCRYPT",       "NEW-ENVIRON"
};

#define CURL_TELOPT_MAXIMUM CURL_TELOPT_NEW_ENVIRON

#define CURL_TELOPT_OK(x) ((x) <= CURL_TELOPT_MAXIMUM)
#define CURL_TELOPT(x)    telnetoptions[x]

#define CURL_NTELOPTS 40 


#define CURL_xEOF 236  
#define CURL_SE   240 
#define CURL_NOP  241 
#define CURL_DM   242 
#define CURL_GA   249 
#define CURL_SB   250 
#define CURL_WILL 251 
#define CURL_WONT 252 
#define CURL_DO   253 
#define CURL_DONT 254 
#define CURL_IAC  255 


static const char * const telnetcmds[]=
{
  "EOF",  "SUSP",  "ABORT", "EOR",  "SE",
  "NOP",  "DMARK", "BRK",   "IP",   "AO",
  "AYT",  "EC",    "EL",    "GA",   "SB",
  "WILL", "WONT",  "DO",    "DONT", "IAC"
};

#define CURL_TELCMD_MINIMUM CURL_xEOF 
#define CURL_TELCMD_MAXIMUM CURL_IAC  

#define CURL_TELQUAL_IS   0
#define CURL_TELQUAL_SEND 1
#define CURL_TELQUAL_INFO 2
#define CURL_TELQUAL_NAME 3

#define CURL_TELCMD_OK(x) ( ((unsigned int)(x) >= CURL_TELCMD_MINIMUM) && \
                       ((unsigned int)(x) <= CURL_TELCMD_MAXIMUM) )
#define CURL_TELCMD(x)    telnetcmds[(x)-CURL_TELCMD_MINIMUM]
#endif
#endif
