#include "sparse.h"

void merge_sparse(Hyperloglog *hll, uint16_t index, byte first1) {
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
    for (int i = 0; i < hll->last_index; i++) {
        hll->M[hll->pairs[i].index] = hll->pairs[i].value;
    }
    hll->M[index] = max(hll->M[index], first1);
    free(hll->pairs);
}

int compare_sparse_pair(const void *a, const void *b) {
    if ( ((SparsePair*)a)->index <  ((SparsePair*)b)->index ) return -1;
    if ( ((SparsePair*)a)->index == ((SparsePair*)b)->index ) return 0;
    if ( ((SparsePair*)a)->index >  ((SparsePair*)b)->index ) return 1;
    return 0;
}

SparsePair *search_sparse_pair(SparsePair *pairs, uint length, uint16_t index) {
    //SparsePair pair = {index, 0};
    //return (SparsePair*) bsearch(&pair, pairs, length, sizeof(SparsePair), compare_sparse_pair);
    for (int i = 0; i < length; i++) {
        if (pairs[i].index == index) {
            return &pairs[i];
        }
    }
    return NULL;
}

void update_sparse_list(Hyperloglog *hll, uint16_t index, byte first1) {
    SparsePair *pair = search_sparse_pair(hll->pairs, hll->last_index, index);
    if (pair == NULL) {
        if (hll->last_index == hll->max_values) {
            hll->sparsed_used = 0;
            merge_sparse(hll, index, first1);
        } else {
            for (int j = 0; j < hll->last_index; j++) {
                if (hll->pairs[j].index == index) {
                    printf("Chyba!!! [%u, %u, %i, %u]\n", index, first1, j, hll->last_index);
                }
            }


            hll->pairs[hll->last_index].index = index;
            hll->pairs[hll->last_index].value = first1;
            hll->last_index++;
        }
    } else {
        pair->value = max(first1, pair->value);
    }
}
