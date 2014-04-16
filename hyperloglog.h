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

typedef struct {
    uint b;
    uint m;
    byte *M;
    uint digestBitLength;
} Hyperloglog;

double hyperloglog(uint b, SimpleCSVParser *parser, Structure structure);

#endif
