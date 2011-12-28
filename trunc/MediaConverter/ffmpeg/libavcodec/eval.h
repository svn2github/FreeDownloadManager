/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVCODEC_EVAL_H
#define AVCODEC_EVAL_H

typedef struct AVExpr AVExpr;


double ff_parse_and_eval_expr(const char *s, const double *const_value, const char * const *const_name,
               double (* const *func1)(void *, double), const char * const *func1_name,
               double (* const *func2)(void *, double, double), const char * const *func2_name,
               void *opaque, const char **error);


AVExpr *ff_parse_expr(const char *s, const char * const *const_name,
               double (* const *func1)(void *, double), const char * const *func1_name,
               double (* const *func2)(void *, double, double), const char * const *func2_name,
               const char **error);


double ff_eval_expr(AVExpr * e, const double *const_value, void *opaque);


void ff_free_expr(AVExpr *e);


double av_strtod(const char *numstr, char **tail);

#endif 
