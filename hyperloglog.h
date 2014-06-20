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
#include "vector_compressor.h"
#include "linear_counting.h"

typedef struct {
    uint16_t index;
    byte value;
} IndexPair;

uint bucket_index(uint64_t digest, uint b);
uint rho(uint64_t digest, uint bitfrom);
Hyperloglog *create_hll(uint b);
uint estimate_cardinality(Hyperloglog *hll);
void updateM(Hyperloglog *hll, uint64_t digest);
uint hyperloglog_cardinality(Hyperloglog *hll, double alpham);
double compute_alpha(unsigned int m);
uint apply_corrections(Hyperloglog *hll, uint hll_cardinality);

#endif
