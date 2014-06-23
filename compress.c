#include "compress.h"

ulong compress_bytes(byte *array, byte *compressed, uint size_input, uint size_output) {
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)size_input; // size of input, string + terminator
    defstream.next_in = (Bytef *)array; // input char array
    defstream.avail_out = (uInt)size_output; // size of output
    defstream.next_out = (Bytef *)compressed; // output char array
    
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
    
    return defstream.total_out;
}

ulong compress_hll(Hyperloglog *hll, byte *compressed) {
    return (uint)compress_bytes(hll->M, compressed, hll->m, hll->m);
}


ulong compress_sparse(Hyperloglog *hll, byte *compressed, SparsePair *temp) {
    double V = (double)count_zero_buckets(hll);
    uint temp_index = 0;
    if ((V / (double)hll->m) >= 2.0 / 3.0) {
        for (uint16_t i = 0; i < hll->m; i++) {
            if (hll->M[i] > 0) {
                temp[temp_index].index = i;
                temp[temp_index].value = hll->M[i];
                temp_index++;
            }
        }
        uint raw_size = temp_index * 3;
        uint compressed_size = (uint) compress_bytes((byte*)temp, compressed, raw_size, hll->m);
        return min_ulong(compressed_size, raw_size);
    } else {
        return ULONG_MAX;
    }
}
