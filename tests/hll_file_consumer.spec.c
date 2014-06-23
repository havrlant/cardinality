#include "hll_file_consumer.spec.h"

void test_compute_hash_length() {
    uint fields[] = {0, 2};
    char *values[] = {"one", "two", "three"};
    View view = (View){fields, 2};
    size_t hashlength = compute_hash_length(view, values);
    sassert(13 == hashlength);
}

void test_build_hash_id() {
    uint fields[] = {0, 2};
    char *values[] = {"one", "two", "three"};
    View view = (View){fields, 2};
    char *hash_id = build_hash_id(view, values);
    assert_str("1_one,3_three", hash_id);
}

void run_all_hll_file_consumer() {
    test_compute_hash_length();
    test_build_hash_id();
}
