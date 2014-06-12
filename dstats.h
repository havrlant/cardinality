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
#define ID_CHANNEL 4
#define ID_CAMPAIGN 5
#define ID_PLAN_CAMPAIGN 6
#define ID_BANNER 9
#define UUID_INDEX 27

#include "declarations.h"
#include <stdlib.h>

typedef struct {
    char *id_server;
    char *id_section;
    char *id_placement;
    char *banner_type;
    char *uuid;
} Dstats;

typedef struct {
    uint *fields_indices;
    uint fields_count;
} View;

#define VIEWS_COUNT 3
View views[VIEWS_COUNT];

void parse_line(char **fields, Dstats *dstats);


#endif
