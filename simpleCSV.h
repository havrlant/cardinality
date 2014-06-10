#ifndef cardinality_simpleCSV_h
#define cardinality_simpleCSV_h

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct {
    FILE *fd;
    int max_length_line;
    int max_fields;
    char *buffer;
    char **fields;
    size_t fields_read;
    int first_line;
    char delimiter;
} SimpleCSVParser;

void init_parser(SimpleCSVParser *parser, FILE *fd, int max_length_line, int max_fields, char delimiter);
int next_line(SimpleCSVParser *);

#endif
