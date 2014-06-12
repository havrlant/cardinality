#ifndef cardinality_buckets_h
#define cardinality_buckets_h

#include "declarations.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
    void *values;
    uint bucket_size_in_bites;
    uint length;
} Buckets;

#endif
