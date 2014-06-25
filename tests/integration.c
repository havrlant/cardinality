#include "integration.h"

uint results[3][3] = {
    {103109, 103, 7},
    {2317403, 501, 10},
    {1173615, 400, 9}
};

ulong process_results(HllDictionary *table, uint b, uint hour) {
    static uint res_index = 0;
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
    assert(results[res_index][0] == sum_cardinality);
    assert(results[res_index][1] == (bytes_sum / 1024));
    assert(results[res_index][2] == (bytes_sum / i));
    res_index++;
    return bytes_sum;
}

void run_integration_test(const char *path) {
    View test_views[] = {
        {(uint[]){ID_SERVER, ID_SECTION, ID_PLACEMENT, BANNER_TYPE, ID_CAMPAIGN, ID_PLAN_CAMPAIGN, ID_BANNER, ID_CHANNEL, BANNER_VERSION}, 8}
    };

    ViewFilter test_vFilter = { test_views, 1 };
    uint b = 4;
    HllDictionary *table;
    ulong bytes_sum = 0;
    tinydir_dir dir;
    for (uint hour = 0; hour < HOURS_IN_DAY; hour++) {
        table = NULL;
        tinydir_open(&dir, path); // ToDo: error handling
        process_all_files(&dir, &table, b, hour, &test_vFilter, 0);
        if (table != NULL) {
            bytes_sum += process_results(table, b, hour);
        }
        tinydir_close(&dir);
    }
    assert(1030015 == bytes_sum);
    printf("Integration test passed! Relax.\n");
}
