#include "simpleCSV.h"

void init_parser(SimpleCSVParser *parser, FILE *fd, int max_length_line, int max_fields) {
    parser->fd = fd;
    parser->max_fields = max_fields;
    parser->max_length_line = max_length_line;
    parser->buffer = (char*) malloc(max_length_line + 2);
    parser->fields = (char**) malloc(sizeof(char*) * max_fields);
    parser->first_line = 1;
}

int next_line(SimpleCSVParser *parser) {
    char *pch;
    int counter = 0;
    if ((fgets(parser->buffer, parser->max_length_line, parser->fd)) != NULL) {
        if (parser->first_line) {
            parser->first_line = 0;
            return next_line(parser);
        }
        pch = strtok(parser->buffer, "\",\n\r");
        while (pch != NULL && *pch != '\n') {
            parser->fields[counter] = pch;
            counter++;
            pch = strtok(NULL, "\",\n\r");
        }
        parser->fields_read = counter;
    } else {
        return 1;
    }
    return 0;
}