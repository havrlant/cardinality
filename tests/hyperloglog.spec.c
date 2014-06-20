#include "hyperloglog.spec.h"

Hyperloglog *mock_hll() {
    uint b = 2;
    Hyperloglog *hll = create_hll(b);
    hll->M[0] = 5;
    hll->M[1] = 2;
    hll->M[2] = 0;
    hll->M[3] = 6;
    return hll;
}

// 11538222250332604846L ==
// 1010000000100000000000000000000100101010100101010100101001010010
void test_rho() {
    uint64_t digest = 11538222250332604846LLU;
    sassert(1 == rho(digest, 0));
    sassert(7 == rho(digest, 4));
    sassert(21 == rho(digest, 11));
}

void test_bucket_index() {
    uint64_t digest = 11538222250332604846LLU;
    sassert(5 == bucket_index(digest, 3));
    sassert(1 == bucket_index(digest, 1));
    sassert(160 == bucket_index(digest, 8));
}

void test_hyperloglog_cardinality() {
    Hyperloglog *hll = mock_hll();
    sassert(7 == hyperloglog_cardinality(hll, compute_alpha(hll->m)));
    free(hll);
}

void test_apply_corrections() {
    Hyperloglog *hll = mock_hll();
    uint cardinality = hyperloglog_cardinality(hll, compute_alpha(hll->m));
    sassert(5 == apply_corrections(hll, cardinality));
    free(hll);
}

void test_estimate_cardinality() {
    Hyperloglog *hll = mock_hll();
    sassert(5 == estimate_cardinality(hll));
    free(hll);
}

void run_all_hyperloglog() {
    test_rho();
    test_bucket_index();
    test_hyperloglog_cardinality();
    test_apply_corrections();
    test_estimate_cardinality();
}
