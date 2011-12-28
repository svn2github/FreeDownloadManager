/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/



#ifndef AVFORMAT_HTTPAUTH_H
#define AVFORMAT_HTTPAUTH_H


typedef enum HTTPAuthType {
    HTTP_AUTH_NONE = 0,    
    HTTP_AUTH_BASIC,       
    HTTP_AUTH_DIGEST,      
} HTTPAuthType;

typedef struct {
    char nonce[300];       
    char algorithm[10];    
    char qop[30];          
    char opaque[300];      
    int nc;                
} DigestParams;


typedef struct {
    
    HTTPAuthType auth_type;
    
    char realm[200];
    
    DigestParams digest_params;
} HTTPAuthState;

void ff_http_auth_handle_header(HTTPAuthState *state, const char *key,
                                const char *value);
char *ff_http_auth_create_response(HTTPAuthState *state, const char *auth,
                                   const char *path, const char *method);

#endif 
