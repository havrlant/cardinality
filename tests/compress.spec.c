#include "compress.spec.h"

void test_comress_hll() {
    uint b = 10;
    Hyperloglog *hll = create_hll(b);
    byte *compressed = (byte*) malloc(1 << b);
    uint64_t size = compress_hll(hll, compressed);
    sassert(17 == size);
    
    for (int i = 0; i < 100; i++) {
        hll->M[i] = i % (1 << 6);
    }
    
    size = compress_hll(hll, compressed);
    sassert(83 == size);
}

void run_all_compress() {
    test_comress_hll();
}
