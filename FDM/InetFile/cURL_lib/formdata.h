/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/

#ifndef __FORMDATA_H
#define __FORMDATA_H  

enum formtype {
  FORM_DATA,    
  FORM_CONTENT, 
  FORM_FILE     
}; 

struct FormData {
  struct FormData *next;
  enum formtype type;
  char *line;
  size_t length;
};

struct Form {
  struct FormData *data; 
  size_t sent;           
  FILE *fp;              
}; 

typedef struct FormInfo {
  char *name;
  bool name_alloc;
  size_t namelength;
  char *value;
  bool value_alloc;
  size_t contentslength;
  char *contenttype;
  bool contenttype_alloc;
  long flags;
  char *buffer;      
  size_t bufferlength;
  char *showfilename; 
  bool showfilename_alloc;
  struct curl_slist* contentheader;
  struct FormInfo *more;
} FormInfo;

int Curl_FormInit(struct Form *form, struct FormData *formdata );

CURLcode
Curl_getFormData(struct FormData **,
                 struct curl_httppost *post,
                 const char *custom_contenttype,
                 curl_off_t *size); 

size_t Curl_FormReader(char *buffer,
                       size_t size,
                       size_t nitems,
                       FILE *mydata); 

char *Curl_formpostheader(void *formp, size_t *len);

char *Curl_FormBoundary(void);

void Curl_formclean(struct FormData **);

CURLcode Curl_formconvert(struct SessionHandle *, struct FormData *);

#endif

