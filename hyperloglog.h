#ifndef cardinality_hyperloglog_h
#define cardinality_hyperloglog_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "declarations.h"
#include "structure.h"
#include "simpleCSV.h"
#include "assert.h"
#include "hlldictionary.h"
#include "dstats.h"
#include "setdictionary.h"
#include "set.h"
#include "tinydir.h"
#include "murmurhash.h"
#include "io.h"
#include "hll_data.h"

typedef struct{
    Hyperloglog *website;
    Hyperloglog **sections;
    Hyperloglog **positions;
} SiteLoglog;

void hyperloglog(uint b, const char *path);
double linear_counting(uint m, uint V);
uint bucket_index(uint64_t digest, uint b);
uint rho(uint64_t digest, uint bitfrom);
char *create_hash_id(View view, char** fields);
size_t compute_hash_length(View view, char** fields);

#endif
