#include <stdio.h>
#include "simpleCSV.h"
#include "structure.h"
#include "hyperloglog.h"
#include "declarations.h"

void print_line(SimpleCSVParser parser) {
    for (int i = 0; i < parser.fields_read; i++) {
        printf("Nacetl jsem: '%s'\n", parser.fields[i]);
    }
    printf("\n");
}

void check_file(const FILE *fd, const char *path) {
    if (fd == NULL) {
        printf("Nepodarilo se otevrit soubor %s", path);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, const char * argv[])
{
    if (argc < 3) {
        printf("Malo parametru. Pouziti:\n");
        printf("cardinality <structure_file> <data_file> [<b>]\n");
        exit(EXIT_FAILURE);
    }
    const int default_b = 10;
    uint b = default_b;
    
    if (argc >= 4) {
        b = atoi(argv[3]);
        if (b == 0) {
            b = default_b;
        }
    }
    
    FILE *structure_file = fopen(argv[1], "r");
    check_file(structure_file, argv[1]);
    
    FILE *data_file = fopen(argv[2], "r");
    check_file(data_file, argv[2]);
    
    
    
    Structure structure = load_structure(structure_file, 200);
    SimpleCSVParser parser;
    init_parser(&parser, data_file, 100, 10);
    hyperloglog(b, &parser, structure);
    return 0;
}

