/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVFILTER_PARSEUTILS_H
#define AVFILTER_PARSEUTILS_H

#include "libavcodec/opt.h"


char *av_get_token(const char **buf, const char *term);


int av_parse_color(uint8_t *rgba_color, const char *color_string, void *log_ctx);


int av_set_options_string(void *ctx, const char *opts,
                          const char *key_val_sep, const char *pairs_sep);

#endif  
