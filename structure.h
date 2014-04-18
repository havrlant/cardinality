#ifndef cardinality_structure_h
#define cardinality_structure_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simpleCSV.h"
#include "declarations.h"

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
    int section_count;
} Structure;

Structure load_structure(FILE *fd, int max_length_line);
void print_row(StructureRow row);
StructureRow *find_row_by_ad_space_pk(Structure *structure, int ad_space_pk);
int find_index_by_ad_space_pk(Structure *structure, int ad_space_pk);
void free_structure(Structure *structure);

#endif
