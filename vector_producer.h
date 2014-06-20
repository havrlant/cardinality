#ifndef cardinality_vector_producer_h
#define cardinality_vector_producer_h

#include <stdio.h>
#include <string.h>
#include "hll_decl.h"
#include "linear_counting.h"

void save_vector(Hyperloglog *hll, char *filename);
void save_sparse(Hyperloglog *hll, char *filename);

#endif
