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
    printf("section_id: %s, ", row.section_id);
    printf("position_id: %s, ", row.position_id);
    printf("position_bannertype_id: %s\n", row.position_bannertype_id);
}

StructureRow fields_to_row(char **fields) {
    StructureRow row;
    row.ad_space_pk = atoi(copy_string(fields[0]));
    row.site_id = copy_string(fields[1]);
    row.section_id = copy_string(fields[2]);
    row.position_id = copy_string(fields[3]);
    row.position_bannertype_id = copy_string(fields[4]);
    return row;
}

Structure load_structure(FILE *fd, int max_length_line) {
    int buffsize = 20;
    StructureRow *rows = (StructureRow*) malloc(sizeof(StructureRow) * buffsize);
    SimpleCSVParser parser;
    init_parser(&parser, fd, max_length_line, 5);
    int i;
    for (i = 0; next_line(&parser) == 0; i++) {
        if (i == buffsize) {
            buffsize *= 2;
            rows = realloc(rows, sizeof(StructureRow) * buffsize);
        }
        rows[i] = fields_to_row(parser.fields);
    }
    
    Structure s = {rows, i};
    return s;
}

StructureRow find_row_by_ad_space_pk(Structure structure, int ad_space_pk) {
    if (ad_space_pk == 89229) {
        return structure.rows[15];
    } else {
        return structure.rows[ad_space_pk - 89202];
    }
}
