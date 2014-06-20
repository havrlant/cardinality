//
//  init.h
//  cardinality
//
//  Created by Lukáš Havrlant on 9.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_init_h
#define cardinality_init_h

#include "alltests.h"

void run_all_tests() {
    run_all_utils();
    run_all_compress();

    if (_total_errors == 0) {
        printf("ALL %i TESTS PASSED!\n", _total_tests);
    } else {
        printf("%i errors found :-(. Total tests: %i\n", _total_errors, _total_tests);
    }
}

#endif
