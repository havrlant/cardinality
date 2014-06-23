#ifndef cardinality_hll_decl_h
#define cardinality_hll_decl_h

#include "declarations.h"

typedef struct {
    uint b;
    uint m;
    byte *M;
} Hyperloglog;

#pragma pack(1)

typedef struct {
    uint16_t index;
    byte value;
} SparsePair;

#endif
