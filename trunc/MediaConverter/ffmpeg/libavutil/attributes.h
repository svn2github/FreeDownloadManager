/*
  Free Download Manager Copyright (c) 2003-2011 FreeDownloadManager.ORG
*/





#ifndef AVUTIL_ATTRIBUTES_H
#define AVUTIL_ATTRIBUTES_H

#ifdef __GNUC__
#    define AV_GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > x || __GNUC__ == x && __GNUC_MINOR__ >= y)
#else
#    define AV_GCC_VERSION_AT_LEAST(x,y) 0
#endif

#ifndef av_always_inline
#if AV_GCC_VERSION_AT_LEAST(3,1)
#    define av_always_inline __attribute__((always_inline)) inline
#else
#    define av_always_inline inline
#endif
#endif

#ifndef av_noinline
#if AV_GCC_VERSION_AT_LEAST(3,1)
#    define av_noinline __attribute__((noinline))
#else
#    define av_noinline
#endif
#endif

#ifndef av_pure
#if AV_GCC_VERSION_AT_LEAST(3,1)
#    define av_pure __attribute__((pure))
#else
#    define av_pure
#endif
#endif

#ifndef av_const
#if AV_GCC_VERSION_AT_LEAST(2,6)
#    define av_const __attribute__((const))
#else
#    define av_const
#endif
#endif

#ifndef av_cold
#if (!defined(__ICC) || __ICC > 1110) && AV_GCC_VERSION_AT_LEAST(4,3)
#    define av_cold __attribute__((cold))
#else
#    define av_cold
#endif
#endif

#ifndef av_flatten
#if (!defined(__ICC) || __ICC > 1110) && AV_GCC_VERSION_AT_LEAST(4,1)
#    define av_flatten __attribute__((flatten))
#else
#    define av_flatten
#endif
#endif

#ifndef attribute_deprecated
#if AV_GCC_VERSION_AT_LEAST(3,1)
#    define attribute_deprecated __attribute__((deprecated))
#else
#    define attribute_deprecated
#endif
#endif

#ifndef av_unused
#if defined(__GNUC__)
#    define av_unused __attribute__((unused))
#else
#    define av_unused
#endif
#endif

#ifndef av_uninit
#if defined(__GNUC__) && !defined(__ICC)
#    define av_uninit(x) x=x
#else
#    define av_uninit(x) x
#endif
#endif

#ifdef __GNUC__
#    define av_builtin_constant_p __builtin_constant_p
#else
#    define av_builtin_constant_p(x) 0
#endif

#endif 
