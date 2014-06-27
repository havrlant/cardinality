#include "sparse.spec.h"

SparsePair* get_sparse_arr(uint length) {
    SparsePair* arr = (SparsePair*) malloc(sizeof(SparsePair) * length);

    for (int i = 0; i < length; i++) {
        arr[i].index = (uint16_t)i;
        arr[i].value = i;
    }

    return arr;
}

void test_search_sparse_pair() {
    uint length = 20;
    SparsePair* arr = get_sparse_arr(length);
    for (int i = 0; i < length; i++) {
        sassert(i == search_sparse_pair(arr, length, i)->index);
    }
    sassert(NULL == search_sparse_pair(arr, length, 42));
    sassert(NULL == search_sparse_pair(arr, length - 10, 15));
    sassert(0 == search_sparse_pair(arr, 1, 0)->index);
}

void test_search_sparse_pair_zero_array() {
    uint length = 0;
    SparsePair* arr = get_sparse_arr(length);
    sassert(NULL == search_sparse_pair(arr, length, 5));
}

void test_update_sparse_list() {
    uint b = 10;
    Hyperloglog *hll = create_hll(b, 1);
    update_sparse_list(hll, 5, 4);
    sassert(4 == hll->pairs[0].value);
    sassert(1 == hll->last_index);
    update_sparse_list(hll, 5, 6);
    sassert(6 == hll->pairs[0].value);
    sassert(1 == hll->last_index);
    update_sparse_list(hll, 18, 2);
    sassert(2 == hll->pairs[1].value);
    sassert(2 == hll->last_index);
    free(hll);
}

void test_merge_sparse() {
    uint b = 6;
    Hyperloglog *hll = create_hll(b, 1);
    sassert(4 == hll->max_values);
    for (int i = 0; i < 4; i++) {
        update_sparse_list(hll, i, i);
        sassert(i == hll->pairs[i].value);
        sassert(i == hll->pairs[i].index);
    }
    update_sparse_list(hll, 2, 1);
    sassert(1 == hll->sparsed_used);
    update_sparse_list(hll, 10, 2);
    sassert(0 == hll->sparsed_used);
    for (int i = 0; i < 4; i++) {
        sassert(hll->M[i] == i);
    }
    sassert(2 == hll->M[10]);
    free(hll);
}

void test_big_merge_sparse() {
    uint b = 8;
    Hyperloglog *hll = create_hll(b, 1);
    for (int i = 0; i <= hll->max_values; i++) {
        sassert(1 == hll->sparsed_used);
        update_sparse_list(hll, i, i % 20);
    }
    sassert(0 == hll->sparsed_used);
    for (int i = 0; i <= hll->max_values; i++) {
        sassert(hll->M[i] == i % 20);
    }
}

void run_all_sparse() {
    test_search_sparse_pair();
    test_search_sparse_pair_zero_array();
    test_update_sparse_list();
    test_merge_sparse();
    test_big_merge_sparse();
}
