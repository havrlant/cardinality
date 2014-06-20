#include "utils.spec.h"

void test_substr() {
    const char *str = "Hello World!";
    char destination[20];
    substring(str, 0, 5, destination);
    assert_str("Hello", destination);
    substring(str, 3, 6, destination);
    assert_str("lo Wor", destination);
    substring(str, 3, 0, destination);
    assert_str("", destination);
}

void test_str_to_long_int() {
    sassert(1234L == str_to_long_int("1234"));
    sassert(1099511627776L == str_to_long_int("1099511627776"));
}

void test_get_hour_from_timestamp() {
    sassert(10 == get_hour_from_timestamp((time_t)1403261453L));
}

void test_get_hour_from_dstats() {
    sassert(17 == get_hour_from_dstats("dstats_14019477880_bmweb2.dstats"));
}

void test_max() {
    sassert(5 == max(5, 3));
}

void run_all_utils() {
    test_substr();
    test_str_to_long_int();
    test_get_hour_from_timestamp();
    test_get_hour_from_dstats();
    test_max();
}
