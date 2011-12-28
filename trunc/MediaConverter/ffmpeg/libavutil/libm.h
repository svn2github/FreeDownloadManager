/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_LIBM_H
#define AVUTIL_LIBM_H

#include <math.h>
#include "config.h"
#include "attributes.h"

#if !HAVE_EXP2
#undef exp2
#define exp2(x) exp((x) * 0.693147180559945)
#endif 

#if !HAVE_EXP2F
#undef exp2f
#define exp2f(x) ((float)exp2(x))
#endif 

#if !HAVE_LLRINT
#undef llrint
#define llrint(x) ((long long)rint(x))
#endif 

#if !HAVE_LOG2
#undef log2
#define log2(x) (log(x) * 1.44269504088896340736)
#endif 

#if !HAVE_LOG2F
#undef log2f
#define log2f(x) ((float)log2(x))
#endif 

#if !HAVE_LRINT
static av_always_inline av_const long int lrint(double x)
{
    return rint(x);
}
#endif 

#if !HAVE_LRINTF
static av_always_inline av_const long int lrintf(float x)
{
    return (int)(rint(x));
}
#endif 

#if !HAVE_ROUND
static av_always_inline av_const double round(double x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}
#endif 

#if !HAVE_ROUNDF
static av_always_inline av_const float roundf(float x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}
#endif 

#if !HAVE_TRUNCF
static av_always_inline av_const float truncf(float x)
{
    return (x > 0) ? floor(x) : ceil(x);
}
#endif 

#endif 
