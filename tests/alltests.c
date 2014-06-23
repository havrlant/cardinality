#ifndef cardinality_init_h
#define cardinality_init_h

#include "alltests.h"

void run_all_tests() {
    run_all_utils();
    run_all_compress();
    run_all_hyperloglog();
    run_all_linear_counting();
    run_all_hll_file_consumer();

    if (_total_errors == 0) {
        printf("ALL %i TESTS PASSED!\n", _total_tests);
    } else {
        printf("%i errors found :-(. Total tests: %i\n", _total_errors, _total_tests);
    }
}

#endif
