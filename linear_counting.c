#include "linear_counting.h"

double linear_counting(uint m, uint V) {
    return m * log((double)m / (double)V);
}

uint count_zero_buckets(Hyperloglog *hll) {
    uint count = 0;
    if (hll->sparsed_used) {
        count = (hll->max_values << hll->sparse_b_difference) - hll->last_index;
        for (int i = 0; i < hll->last_index; i++) {
            if (hll->pairs[i].value == 0) {
                count++;
            }
        }
    } else {
        for (uint i = 0; i < hll->m; i++) {
            if (hll->M[i] == 0) {
                count++;
            }
        }
    }

    return count;
}
