/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/  

     

#ifndef HEADER_TMDIFF_H
#define HEADER_TMDIFF_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct ms_tm MS_TM;

MS_TM *ms_time_new(void );
void ms_time_free(MS_TM *a);
void ms_time_get(MS_TM *a);
double ms_time_diff(MS_TM *start, MS_TM *end);
int ms_time_cmp(const MS_TM *ap, const MS_TM *bp);

#ifdef  __cplusplus
}
#endif

#endif

