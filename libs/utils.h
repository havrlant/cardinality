#ifndef cardinality_utils_h
#define cardinality_utils_h

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "../declarations.h"

void substring(const char *source, int from, int length, char *destination);
long int str_to_long_int(const char *str);
int get_hour_from_timestamp(time_t timestamp);
uint get_hour_from_dstats(const char *path);
uint max(uint a, uint b);
ulong min_ulong(ulong a, ulong b);

#endif
