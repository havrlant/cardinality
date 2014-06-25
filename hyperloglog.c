#include "hyperloglog.h"

const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;
const double LINEAR_COUNTING_LIMIT = 5;

/*
 Vrati pozici nejlevejsiho bitu, ktery je roven 1.
 Zacina hledat od index bitfrom. Indexuje se od 1.
 rho(10100000001, 4) = 6
 ^
 */
uint rho(uint64_t digest, uint bitfrom) {
    uint64_t base_mask = ((uint64_t) 1) << 63;
    uint64_t mask;
    for (uint64_t i = bitfrom; i < 64; i++) {
        mask = base_mask >> i;
        if ((mask & digest) == mask) {
            return (uint)i - bitfrom + (uint)1;
        }
    }
    return 0;
}


/*
 Prevede prvnich b bitu na cislo
 bucketIndex(1001000010, 4) = 1001 = 9
 */
uint bucket_index(uint64_t digest, uint b) {
    return (uint)(digest >> (uint64_t)(64 - b));
}

void updateM(Hyperloglog *hll, uint64_t digest) {
    uint j, first1;
    j = bucket_index(digest, hll->b);
    first1 = rho(digest, hll->b);
    if (hll->sparsed_used) {
        // printf("adasd");
    } else {
        hll->M[j] = max(hll->M[j], first1);
    }
}

uint hyperloglog_cardinality(Hyperloglog *hll, double alpham) {
    uint j;
    double E = 0;
    double sum = 0, harmonicMean;
    for (j = 0; j < hll->m; j++) {
        sum += pow(2, -(hll->M[j]));
    }
    harmonicMean = hll->m / sum;
    E = alpham * hll->m * harmonicMean;
    return (uint)E;
}

double compute_alpha(unsigned int m) {
    return 0.7213 / (1 + 1.079 / m);
}

void init_hll(Hyperloglog *hll, uint b, byte use_sparse) {
    uint max_sparse_pairs = 1 << (b - 4);
    hll->b = b;
    hll->m = 1 << b; // 2^b
    hll->sparsed_used = use_sparse;
    if (!use_sparse) {
        hll->M = (byte*) calloc(hll->m, sizeof(byte));
    } else {
        hll->pairs = (SparsePair*) malloc(sizeof(SparsePair) * max_sparse_pairs); // 2^b-4
        hll->last_index = 0;
        hll->max_values = max_sparse_pairs;
    }
}

Hyperloglog *create_hll(uint b, byte use_sparse) {
    Hyperloglog *hll = (Hyperloglog*) malloc(sizeof(Hyperloglog));
    init_hll(hll, b, use_sparse);
    return hll;
}

uint apply_corrections(Hyperloglog *hll, uint hll_cardinality) {
    uint lc_cardinality;
    if (hll_cardinality <= (LINEAR_COUNTING_LIMIT * hll->m)) {
        uint V = count_zero_buckets(hll);
        if (V != 0) {
            lc_cardinality = linear_counting(hll->m, V);
            return lc_cardinality;
        }
    }
    return hll_cardinality;
}

uint estimate_cardinality(Hyperloglog *hll) {
    uint cardinality = hyperloglog_cardinality(hll, compute_alpha(hll->m)); // cachovat alpha
    cardinality = apply_corrections(hll, cardinality);
    return cardinality;
}
