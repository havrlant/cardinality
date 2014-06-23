#include "linear_counting.spec.h"
#include "../linear_counting.h"

void test_count_zero_buckets() {
    Hyperloglog *hll = create_hll(5);
    sassert(hll->m == count_zero_buckets(hll));
    hll->M[3] = 1;
    hll->M[8] = 2;
    sassert((hll->m - 2) == count_zero_buckets(hll));
    free(hll);
}

void run_all_linear_counting() {
    test_count_zero_buckets();
}
