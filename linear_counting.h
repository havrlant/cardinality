#ifndef cardinality_linear_counting_h
#define cardinality_linear_counting_h

#include <math.h>
#include "declarations.h"
#include "hll_decl.h"

double linear_counting(uint m, uint V);
uint count_zero_buckets(Hyperloglog *hll);

#endif
