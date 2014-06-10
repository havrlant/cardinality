#include "hyperloglog.spec.h"

void test_linear_counting() {
    assert((int)linear_counting(1024, 250) == 2083);
}

void test_hyperloglog() {
    test_linear_counting();
}