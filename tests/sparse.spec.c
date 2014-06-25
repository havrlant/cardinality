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
}

void run_all_sparse() {
    test_search_sparse_pair();
}
