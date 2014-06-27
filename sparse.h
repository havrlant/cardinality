#ifndef cardinality_sparse_h
#define cardinality_sparse_h

#include <stdlib.h>
#include "hll_decl.h"
#include "libs/utils.h"

SparsePair *search_sparse_pair(SparsePair *pairs, uint length, uint16_t index);
int compare_sparse_pair(const void *a, const void *b);
void update_sparse_list(Hyperloglog *hll, uint16_t index, byte first1);

#endif
