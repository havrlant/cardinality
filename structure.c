#include "structure.h"

char *copy_string(char *string) {
    size_t size = strlen(string) + 1;
    char *new_string = (char*) malloc(size);
    memcpy(new_string, string, size);
    return new_string;
}

void print_row(StructureRow row) {
    printf("ad_space_pk: %i, ", row.ad_space_pk);
    printf("site_id: %s, ", row.site_id);
    printf("section_id: %i, ", row.section_id);
    printf("position_id: %s, ", row.position_id);
    printf("position_bannertype_id: %s\n", row.position_bannertype_id);
}

StructureRow fields_to_row(char **fields) {
    char *temp;
    StructureRow row;
    temp = copy_string(fields[0]);
    row.ad_space_pk = atoi(temp);
    free(temp);
    row.site_id = copy_string(fields[1]);
    row.section_id = atoi(fields[2]);
    row.position_id = copy_string(fields[3]);
    row.position_bannertype_id = copy_string(fields[4]);
    return row;
}

int compare_structure_row (const void *a, const void *b) {
    if (((StructureRow*)a)->ad_space_pk < ((StructureRow*)b)->ad_space_pk) {
        return -1;
    }
    if (((StructureRow*)a)->ad_space_pk > ((StructureRow*)b)->ad_space_pk) {
        return 1;
    }
    return 0;
}

Structure load_structure(FILE *fd, int max_length_line) {
    int buffsize = 20;
    StructureRow *rows = (StructureRow*) malloc(sizeof(StructureRow) * buffsize);
    SimpleCSVParser parser;
    init_parser(&parser, fd, max_length_line, 5);
    int i;
    for (i = 0; next_line(&parser); i++) {
        if (i == buffsize) {
            buffsize *= 2;
            rows = realloc(rows, sizeof(StructureRow) * buffsize);
        }
        rows[i] = fields_to_row(parser.fields);
    }
    qsort(rows, i, sizeof(StructureRow), compare_structure_row);
    
    int sections_count = 0;
    for (int j = 0; j < i; j++) {
        sections_count = fmax(sections_count, rows[j].section_id);
    }
    
    return (Structure){rows, i, sections_count};
}

void free_structure(Structure *structure) {
    for (int i = 0; i < structure->length; i++) {
        free(structure->rows[i].site_id);
        free(structure->rows[i].position_id);
        free(structure->rows[i].position_bannertype_id);
    }
    free(structure->rows);
}

StructureRow *find_row_by_ad_space_pk(Structure *structure, int ad_space_pk) {
    int index = find_index_by_ad_space_pk(structure, ad_space_pk);
    return &structure->rows[index];
}

int find_index_by_ad_space_pk(Structure *structure, int ad_space_pk) {
    uint left = 0, right = structure->length - 1;
    uint middle;
    int temp_ad_space_pk;
    while (left < right) {
        middle = (left + right) / 2;
        temp_ad_space_pk = structure->rows[middle].ad_space_pk;
        if (temp_ad_space_pk < ad_space_pk) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return right;
}
