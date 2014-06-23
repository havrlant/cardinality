#include "integration.h"

ulong process_results(HllDictionary *table, uint b) {
    HllDictionary *h, *tmp;
    uint card;
    ulong bytes_sum = 0;
    uint i = 0;
    uint m = 1 << b;
    byte *compressed = (byte*) malloc(m);
    SparsePair *pairs = (SparsePair*) malloc(sizeof(SparsePair) * m); // ToDo dve tretiny m
    ulong hll_compressed_size;
    ulong sparse_size;
    ulong sum_cardinality = 0;
    HASH_ITER(hh, table, h, tmp) {
        i++;
        card = estimate_cardinality(h->hll);
        // printf("%s:%u\n", h->hash_id, card);
        sum_cardinality += card;
        hll_compressed_size = compress_hll(h->hll, compressed);
        sparse_size = compress_sparse(h->hll, compressed, pairs);
        bytes_sum += min_ulong(hll_compressed_size, sparse_size);
        free(h->hll->M);
        free(h->hll);
        free(h->hash_id);
    }
    HASH_CLEAR(hh, table);
    free(compressed);
    printf("Soucet vsech kardinalit:   %lu\n", sum_cardinality);
    printf("Celkova velikost vektoru:  %g MB\n", bytes_sum / (1024*1024.0));
    printf("Prumerna velikost vektoru: %g B\n", (bytes_sum / (double)i));
    return bytes_sum;
}

void run_integration_test(const char *path) {
    uint b = 4;
    HllDictionary *table;
    uint64_t bytes_sum = 0;
    tinydir_dir dir;
    for (uint hour = 0; hour < HOURS_IN_DAY; hour++) {
        table = NULL;
        tinydir_open(&dir, path); // ToDo: error handling
        process_all_files(&dir, &table, b, hour);
        if (table != NULL) {
            bytes_sum += process_results(table, b);
        }
        tinydir_close(&dir);
    }
}