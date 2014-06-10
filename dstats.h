//
//  dstats.h
//  cardinality
//
//  Created by Lukáš Havrlant on 10.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_dstats_h
#define cardinality_dstats_h

#define ID_SERVER 0
#define ID_SECTION 1
#define ID_PLACEMENT 2
#define BANNER_TYPE 3
#define UUID_INDEX 27

#include "declarations.h"
#include <stdlib.h>

typedef struct {
    uint id_server;
    uint id_section;
    uint id_placement;
    uint banner_type;
    char *uuid;
} Dstats;

void parse_line(char **fields, Dstats *dstats);

#endif
