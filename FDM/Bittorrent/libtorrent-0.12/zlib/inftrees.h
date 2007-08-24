/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
    

typedef struct {
    unsigned char op;           
    unsigned char bits;         
    unsigned short val;         
} code;   

#define ENOUGH 2048
#define MAXD 592 

typedef enum {
    CODES,
    LENS,
    DISTS
} codetype;

extern int inflate_table OF((codetype type, unsigned short FAR *lens,
                             unsigned codes, code FAR * FAR *table,
                             unsigned FAR *bits, unsigned short FAR *work));
