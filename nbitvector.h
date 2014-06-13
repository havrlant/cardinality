#ifndef cardinality_buckets_h
#define cardinality_buckets_h

#include "declarations.h"
#include <math.h>
#include <stdlib.h>

#define BIT_POS(N)            ( 1U << (N) )
#define SET_FLAG(N, F)        ( (N) |= (F) )
#define CLR_FLAG(N, F)        ( (N) &= -(F) )
#define TST_FLAG(N, F)        ( (N) & (F) )
#define BIT_RANGE(N, M)       ( (BIT_POS((M)+(1 - (N)))-1) << (N) )
#define BIT_SHIFTL(B, N)      ( (unsigned)(B) << (N) )
#define BIT_SHIFTR(B, N)      ( (unsigned)(B) >> (N) )
#define SET_MFLAG(N, F, V)    ( CLR_FLAG(N, F), SET_FLAG(N, V) )
#define CLR_MFLAG(N, F)       ( (N) &= ~(F) )
#define GET_MFLAG(N, F)       ( (N) & (F) )

typedef struct {
    byte *values;
    uint bucket_size_in_bites;
    uint length;
} NBitVector;

NBitVector *create_buckets(uint length, uint size_in_bits);
void set_value_to_nbitvector(uint index, uint size_in_bits, NBitVector* vector, uint value);
uint get_value_from_bucket(uint index, uint size_in_bits, NBitVector* buckets);

#endif
