#include "linear_counting.h"

double linear_counting(uint m, uint V) {
    return m * log((double)m / (double)V);
}

uint count_zero_buckets(Hyperloglog *hll) {
    uint count = 0;
    for (uint i = 0; i < hll->m; i++) {
        if (hll->M[i] == 0) {
            count++;
        }
    }
    return count;
}
