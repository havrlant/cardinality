#include "hyperloglog.spec.h"

Hyperloglog *mock_hll(uint b) {
    Hyperloglog *hll = create_hll(b, 0);
    hll->M[0] = 5;
    hll->M[1] = 2;
    hll->M[2] = 0;
    hll->M[3] = 6;
    return hll;
}

Hyperloglog *mock_sparse_hll(uint b) {
    Hyperloglog *hll = create_hll(b, 1);
    update_sparse_list(hll, 0, 5);
    update_sparse_list(hll, 1, 2);
    update_sparse_list(hll, 3, 4);
    update_sparse_list(hll, 2, 0);
    update_sparse_list(hll, 3, 6);
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
    Hyperloglog *hll = mock_hll(8);
    sassert(185 == hyperloglog_cardinality(hll, compute_alpha(hll->m)));
    free(hll);
}

void test_sparse_hyperloglog_cardinality() {
    Hyperloglog *hll = mock_sparse_hll(8);
    sassert(185 == hyperloglog_cardinality(hll, compute_alpha(hll->m)));
    free(hll);
}

void test_sum_hll() {
    uint b = 10;
    Hyperloglog *hll = mock_hll(b);
    sassert(1021 == (int)sum_hll(hll));
    free(hll);
}

void test_sparse_sum_hll() {
    uint b = 10;
    Hyperloglog *hll = mock_sparse_hll(b);
    sassert(1021 == (int)sum_hll(hll));
    free(hll);
}

void test_apply_corrections() {
    Hyperloglog *hll = mock_hll(8);
    uint cardinality = hyperloglog_cardinality(hll, compute_alpha(hll->m));
    sassert(3 == apply_corrections(hll, cardinality));
    free(hll);
}

void test_sparse_apply_corrections() {
    Hyperloglog *hll = mock_sparse_hll(8);
    uint cardinality = hyperloglog_cardinality(hll, compute_alpha(hll->m));
    sassert(3 == apply_corrections(hll, cardinality));
    free(hll);
}

void test_estimate_cardinality() {
    uint b = 10;
    Hyperloglog *hll = mock_hll(b);
    sassert(3 == estimate_cardinality(hll));
    hll->M[54] = 20;
    sassert(4 == estimate_cardinality(hll));

    for (int i = 100; i < 150; i++) {
        hll->M[i] = i % 20;
    }

    sassert(52 == estimate_cardinality(hll));
    free(hll);
}

void test_sparse_estimate_cardinality() {
    uint b = 10;
    Hyperloglog *hll = mock_sparse_hll(b);
    sassert(3 == estimate_cardinality(hll));
    update_sparse_list(hll, 54, 20);
    sassert(4 == estimate_cardinality(hll));

    for (int i = 100; i < 150; i++) {
        update_sparse_list(hll, i, i % 20);
    }

    sassert(52 == estimate_cardinality(hll));
    free(hll);
}

void test_updateM() {
    byte results[] = {3, 4, 3, 3};
    uint b = 2;
    Hyperloglog *hll = create_hll(b, 0);
    uint64_t digest;

    for (uint32_t i = 0; i < 10; i++) {
        digest = MurmurHash64A(&i, 4, 42);
        updateM(hll, digest);
    }

    assert_array_eq(results, hll->M, hll->m, byte);

    free(hll);
}

void test_sparse_used() {
    uint b = 14;
    Hyperloglog *hll = create_hll(b, 0);
    sassert(hll->sparsed_used == 0);
    free(hll);
    hll = create_hll(b, 1);
    sassert(1 == hll->sparsed_used);
    sassert(0 == hll->last_index);
    sassert(1024 == hll->max_values);
    free(hll);
}

void test_size_of_sparse_pair() {
    sassert(3 == sizeof(SparsePair));
    sassert(4 == sizeof(_SparsePairForTestOnly));
}

void run_all_hyperloglog() {
    test_rho();
    test_bucket_index();
    test_hyperloglog_cardinality();
    test_estimate_cardinality();
    test_updateM();
    test_size_of_sparse_pair();
    test_sparse_used();
    test_sum_hll();
    test_sparse_sum_hll();
    test_sparse_estimate_cardinality();
    test_sparse_hyperloglog_cardinality();
    test_apply_corrections();
    test_sparse_apply_corrections();
}
