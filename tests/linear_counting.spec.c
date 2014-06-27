#include "linear_counting.spec.h"
#include "../linear_counting.h"

void test_count_zero_buckets() {
    Hyperloglog *hll = create_hll(5, 0);
    sassert(hll->m == count_zero_buckets(hll));
    hll->M[3] = 1;
    hll->M[8] = 2;
    sassert((hll->m - 2) == count_zero_buckets(hll));
    free(hll->M);
    free(hll);
}

void test_sparse_count_zero_buckets() {
    int test_length = 10;
    Hyperloglog *hll = create_hll(12, 1);
    sassert(hll->m == count_zero_buckets(hll));
    for (int i = 0; i < 10; i++) {
        update_sparse_list(hll, i, i + 5);
    }
    sassert(hll->m - test_length == count_zero_buckets(hll));
    free(hll);
}

void run_all_linear_counting() {
    test_count_zero_buckets();
    test_sparse_count_zero_buckets();
}
