#include <stdio.h>
#include "simpleCSV.h"
#include "structure.h"
#include "hyperloglog.h"
#include "declarations.h"

FILE* try_fopen(const char *path) {
    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        printf("Nepodarilo se otevrit soubor %s", path);
        exit(EXIT_FAILURE);
    }
    return fd;
}

int main(int argc, const char * argv[])
{
    if (argc < 3) {
        printf("Malo parametru. Pouziti:\n");
        printf("cardinality <structure_file> <data_file> [<b>]\n");
        exit(EXIT_FAILURE);
    }
    const uint default_b = 10;
    uint b = default_b;
    
    if (argc >= 4) {
        b = atoi(argv[3]);
        if (b == 0) {
            b = default_b;
        }
    }
    
    FILE *structure_file = try_fopen(argv[1]);
    FILE *data_file = try_fopen(argv[2]);    
    
    Structure structure = load_structure(structure_file, 200);
    SimpleCSVParser parser;
    init_parser(&parser, data_file, 200, 10);
    hyperloglog(b, &parser, &structure);
    free_structure(&structure);
    fclose(structure_file);
    fclose(data_file);
    return 0;
}
