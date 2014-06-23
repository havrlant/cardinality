#include "utils.h"

void substring(const char *source, int from, int length, char *destination) {
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

time_t get_timestamp_from_dstats(const char *path) {
    int timestamp_length = 10;
    char number[timestamp_length + 1];
    size_t pathlength = strlen(path);
    size_t timestamp_start = pathlength - strlen("1401947820_bmweb3.dstats");
    substring(path, (int)timestamp_start, timestamp_length, number);
    return (time_t)str_to_long_int(number);
}

uint get_hour_from_dstats(const char *path) {
    time_t timestamp = get_timestamp_from_dstats(path);
    return (uint)get_hour_from_timestamp(timestamp);
}

uint max(uint a, uint b) {
    return a > b ? a : b;
}

ulong min_ulong(ulong a, ulong b) {
    return a > b ? b : a;
}
