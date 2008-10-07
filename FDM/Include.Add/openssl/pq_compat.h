/*
  Free Download Manager Copyright (c) 2003-2007 FreeDownloadManager.ORG
*/
   

#include <openssl/opensslconf.h>
#include <openssl/bn.h>  

#if (defined(THIRTY_TWO_BIT) && !defined(BN_LLONG)) || defined(SIXTEEN_BIT) || defined(EIGHT_BIT)

#define PQ_64BIT_IS_INTEGER 0
#define PQ_64BIT_IS_BIGNUM 1

#define PQ_64BIT     BIGNUM
#define PQ_64BIT_CTX BN_CTX

#define pq_64bit_init(x)           BN_init(x)
#define pq_64bit_free(x)           BN_free(x)

#define pq_64bit_ctx_new(ctx)      BN_CTX_new()
#define pq_64bit_ctx_free(x)       BN_CTX_free(x)

#define pq_64bit_assign(x, y)      BN_copy(x, y)
#define pq_64bit_assign_word(x, y) BN_set_word(x, y)
#define pq_64bit_gt(x, y)          BN_ucmp(x, y) >= 1 ? 1 : 0
#define pq_64bit_eq(x, y)          BN_ucmp(x, y) == 0 ? 1 : 0
#define pq_64bit_add_word(x, w)    BN_add_word(x, w)
#define pq_64bit_sub(r, x, y)      BN_sub(r, x, y)
#define pq_64bit_sub_word(x, w)    BN_sub_word(x, w)
#define pq_64bit_mod(r, x, n, ctx) BN_mod(r, x, n, ctx)

#define pq_64bit_bin2num(bn, bytes, len)   BN_bin2bn(bytes, len, bn)
#define pq_64bit_num2bin(bn, bytes)        BN_bn2bin(bn, bytes)
#define pq_64bit_get_word(x)               BN_get_word(x)
#define pq_64bit_is_bit_set(x, offset)     BN_is_bit_set(x, offset)
#define pq_64bit_lshift(r, x, shift)       BN_lshift(r, x, shift)
#define pq_64bit_set_bit(x, num)           BN_set_bit(x, num)
#define pq_64bit_get_length(x)             BN_num_bits((x))

#else

#define PQ_64BIT_IS_INTEGER 1
#define PQ_64BIT_IS_BIGNUM 0

#if defined(SIXTY_FOUR_BIT)
#define PQ_64BIT BN_ULONG
#define PQ_64BIT_PRINT "%lld"
#elif defined(SIXTY_FOUR_BIT_LONG)
#define PQ_64BIT BN_ULONG
#define PQ_64BIT_PRINT "%ld"
#elif defined(THIRTY_TWO_BIT)
#define PQ_64BIT BN_ULLONG
#define PQ_64BIT_PRINT "%lld"
#endif

#define PQ_64BIT_CTX      void

#define pq_64bit_init(x)
#define pq_64bit_free(x)
#define pq_64bit_ctx_new(ctx)        (ctx)
#define pq_64bit_ctx_free(x)

#define pq_64bit_assign(x, y)        (*(x) = *(y))
#define pq_64bit_assign_word(x, y)   (*(x) = y)
#define pq_64bit_gt(x, y)	         (*(x) > *(y))
#define pq_64bit_eq(x, y)            (*(x) == *(y))
#define pq_64bit_add_word(x, w)      (*(x) = (*(x) + (w)))
#define pq_64bit_sub(r, x, y)        (*(r) = (*(x) - *(y)))
#define pq_64bit_sub_word(x, w)      (*(x) = (*(x) - (w)))
#define pq_64bit_mod(r, x, n, ctx)

#define pq_64bit_bin2num(num, bytes, len) bytes_to_long_long(bytes, num)
#define pq_64bit_num2bin(num, bytes)      long_long_to_bytes(num, bytes)
#define pq_64bit_get_word(x)              *(x)
#define pq_64bit_lshift(r, x, shift)      (*(r) = (*(x) << (shift)))
#define pq_64bit_set_bit(x, num)          do { \
                                              PQ_64BIT mask = 1; \
                                              mask = mask << (num); \
                                              *(x) |= mask; \
                                          } while(0)
#endif 
