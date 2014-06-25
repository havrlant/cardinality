#include "compress.spec.h"

void test_compress_hll() {
    uint b = 10;
    Hyperloglog *hll = create_hll(b, 0);
    byte *compressed = (byte*) malloc(1 << b);
    ulong size = compress_hll(hll, compressed);
    sassert(17 == size);

    for (int i = 0; i < 100; i++) {
        hll->M[i] = i % (1 << 6);
    }

    size = compress_hll(hll, compressed);
    sassert(83 == size);
    free(compressed);
}

void test_compress_bytes() {
    byte arr[100] = {3, 5, 3, 5, 7, 8, 12, 12, 12};
    byte compressed[200];
    ulong compressed_length = compress_bytes(arr, compressed, 100, 200);
    sassert(20 == compressed_length);
    sassert(compressed[0] == 120);
    sassert(compressed[1] == 218);
    sassert(compressed[2] == 99);
    sassert(compressed[3] == 102);
}

void run_all_compress() {
    test_compress_hll();
    test_compress_bytes();
}
