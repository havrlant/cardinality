#include "simpleCSV.h"

void init_parser(SimpleCSVParser *parser, FILE *fd, int max_length_line, int max_fields, char delimiter) {
    parser->fd = fd;
    parser->max_fields = max_fields;
    parser->max_length_line = max_length_line;
    parser->buffer = (char*) malloc(max_length_line + 2);
    parser->fields = (char**) malloc(sizeof(char*) * max_fields);
    parser->first_line = 1;
    parser->delimiter = delimiter;
}

char *singlestrtok (char *str, char delimiter) {
    static char *cache;
    static char *last;
    static int i;
    static int done;
    
    if (str != NULL) {
        i = 0;
        cache = last = str;
        done = 0;
    } else {
        i++;
        last = &cache[i];
    }
    
    if (done) {
        return NULL;
    }
    
    for (; cache[i] != '\0'; i++) {
        if (cache[i] == delimiter) {
            cache[i] = '\0';
            return last;
        }
    }
    done = 1;
    return last;
}

int next_line(SimpleCSVParser *parser) {
    char *pch;
    int counter = 0;
    if ((fgets(parser->buffer, parser->max_length_line, parser->fd)) != NULL) {
        pch = singlestrtok(parser->buffer, parser->delimiter);
        while (pch != NULL && *pch != '\n') {
            parser->fields[counter] = pch;
            counter++;
            pch = singlestrtok(NULL, parser->delimiter);
        }
        parser->fields_read = counter;
    } else {
        return 0;
    }
    return 1;
}

void free_parser(SimpleCSVParser *parser) {
    free(parser->buffer);
    free(parser->fields);
    fclose(parser->fd);
}
