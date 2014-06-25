#include "sparse.h"

void merge_sparse(Hyperloglog *hll, uint16_t index, byte first1) {
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
    for (int i = 0; i < hll->last_index; i++) {
        hll->M[hll->pairs[i].index] = hll->pairs[i].value;
    }
    hll->M[index] = max(hll->M[index], first1);
}

SparsePair *search_sparse_pair(SparsePair *pairs, uint length, uint16_t index) {
    uint first = 0, last = length - 1;
    uint middle = (first + last) / 2;

    do {
        middle = (first + last) / 2;
        if (pairs[middle].index < index)
            first = middle + 1;
        else if (pairs[middle].index == index) {
            return &pairs[middle];
        } else {
            last = middle - 1;
        }
    } while (first <= last);

    return NULL;
}

void update_sparse_list(Hyperloglog *hll, uint16_t index, byte first1) {
    SparsePair *pair = search_sparse_pair(hll->pairs, hll->last_index, index);
    if (pair == NULL) {
        if (hll->last_index == hll->max_values) {
            hll->sparsed_used = 0;
            free(hll->pairs);
            merge_sparse(hll, index, first1);
        } else {
            hll->pairs[hll->last_index].index = index;
            hll->pairs[hll->last_index].value = first1;
            hll->last_index++;
        }
    } else {
        pair->index = max(first1, pair->value);
    }
}
