#ifndef cardinality_hyperloglog_h
#define cardinality_hyperloglog_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "declarations.h"
#include "md5.h"
#include "structure.h"
#include "simpleCSV.h"
#include "assert.h"
#include "hlldictionary.h"
#include "dstats.h"
#include "setdictionary.h"
#include "set.h"
#include "tinydir.h"

typedef struct{
    Hyperloglog *website;
    Hyperloglog **sections;
    Hyperloglog **positions;
} SiteLoglog;

void hyperloglog(uint b, const char *path);
double linear_counting(uint m, uint V);

#endif
