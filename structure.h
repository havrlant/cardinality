#ifndef cardinality_structure_h
#define cardinality_structure_h

#include <stdio.h>
#include <stdlib.h>
#include "simpleCSV.h"

typedef struct {
    int ad_space_pk;
    char *site_id;
    int section_id;
    char *position_id;
    char *position_bannertype_id;
} StructureRow;

typedef struct {
    StructureRow *rows;
    int length;
} Structure;

Structure load_structure(FILE *fd, int max_length_line);
void print_row(StructureRow row);
StructureRow *find_row_by_ad_space_pk(Structure structure, int ad_space_pk);

#endif
