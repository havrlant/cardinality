#include "integration.h"

uint results[3][3] = {
    {101812, 103, 7},
    {2351573, 501, 10},
    {1180344, 400, 9}
};

ulong process_results(HllDictionary *table, uint b, uint hour, uint res_index) {
    HllDictionary *h, *tmp;
    uint card;
    ulong bytes_sum = 0;
    uint i = 0;
    uint m = 1 << b;
    byte *compressed = (byte*) malloc(m);
    ulong sum_cardinality = 0;
    HASH_ITER(hh, table, h, tmp) {
        i++;
        card = estimate_cardinality(h->hll);
        // printf("%s:%u\n", h->hash_id, card);
        sum_cardinality += card;
        free(h->hll);
        free(h->hash_id);
    }
    HASH_CLEAR(hh, table);
    free(compressed);
    assert(results[res_index][0] == sum_cardinality);
    return bytes_sum;
}

ulong compute_sum_of_cardinalities(const char *path, tinydir_dir *dir, uint compute_all_day, uint use_sparse, ViewFilter *test_vFilter, uint b) {
    ulong bytes_sum = 0;
    HllDictionary *table;
    uint res_index = 0;
    for (uint hour = 0; hour < HOURS_IN_DAY; hour++) {
        table = NULL;
        tinydir_open(dir, path); // ToDo: error handling
        process_all_files(dir, &table, b, hour, test_vFilter, compute_all_day, use_sparse);
        if (table != NULL) {
            bytes_sum += process_results(table, b, hour, res_index++);
        }
        tinydir_close(dir);
    }
    return bytes_sum;
}

void run_integration_test(const char *path) {
    uint compute_all_day = 0;
    View test_views[] = {
        {(uint[]){ID_SERVER, ID_SECTION, ID_PLACEMENT, BANNER_TYPE, ID_CAMPAIGN, ID_PLAN_CAMPAIGN, ID_BANNER, ID_CHANNEL, BANNER_VERSION}, 8}
    };

    ViewFilter test_vFilter = { test_views, 1 };
    tinydir_dir dir;

    for (int i = 0; i < 2; i++) {
        compute_sum_of_cardinalities(path, &dir, compute_all_day, i, &test_vFilter, 8);
    }
    // assert(1030015 == bytes_sum);
    printf("Integration test passed! Relax.\n");
}
