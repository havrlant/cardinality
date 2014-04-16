#include <stdio.h>
#include "simpleCSV.h"
#include "structure.h"
#include "hyperloglog.h"

void print_line(SimpleCSVParser parser) {
    for (int i = 0; i < parser.fields_read; i++) {
        printf("Nacetl jsem: '%s'\n", parser.fields[i]);
    }
    printf("\n");
}

int main(int argc, const char * argv[])
{
    //SimpleCSVParser parser;
    FILE *sfd = fopen("/Users/lukashavrlant/Projects/C/hyperloglog/testhll/struktura.csv", "r");
    Structure structure = load_structure(sfd, 200);
    
    FILE *fd = fopen("/Users/lukashavrlant/Projects/C/hyperloglog/testhll/data.csv", "r");
    SimpleCSVParser parser;
    init_parser(&parser, fd, 100, 10);
    double card = hyperloglog(12, &parser, structure);
    // printf("Kardinalita: %g\n", card);
    return 0;
}

