#ifndef cardinality_utils_h
#define cardinality_utils_h

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void substr(const char *source, int from, int length, char *destination);
long int str_to_long_int(const char *str);
int get_hour_from_timestamp(time_t timestamp);

#endif
