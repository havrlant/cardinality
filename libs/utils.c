#include "utils.h"

void substr(const char *source, int from, int length, char *destination) {
    memcpy(destination, &source[from], length);
    destination[length] = 0;
}

long int str_to_long_int(const char *str) {
    return strtol(str, NULL, 10);
}

int get_hour_from_timestamp(time_t timestamp) {
    struct tm *date_struct = gmtime(&timestamp);
    return date_struct->tm_hour;
}

