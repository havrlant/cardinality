#ifndef cardinality_hyperloglog_h
#define cardinality_hyperloglog_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "declarations.h"
#include "libs/simpleCSV.h"
#include "assert.h"
#include "hlldictionary.h"
#include "dstats.h"
#include "libs/tinydir.h"
#include "libs/murmurhash.h"
#include "io.h"
#include "hll_decl.h"
#include "compress.h"
#include "libs/utils.h"
#include "vector_producer.h"
#include "linear_counting.h"

typedef struct {
    uint16_t index;
    byte value;
} IndexPair;

void hyperloglog(uint b, const char *path);
double linear_counting(uint m, uint V);
uint bucket_index(uint64_t digest, uint b);
uint rho(uint64_t digest, uint bitfrom);
char *build_hash_id(View view, char** fields);
size_t compute_hash_length(View view, char** fields);
Hyperloglog *create_hll(uint b);
uint estimate_cardinality(Hyperloglog *hll);
void updateM(Hyperloglog *hll, uint64_t digest);

#endif
