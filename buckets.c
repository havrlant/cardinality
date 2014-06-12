#include "buckets.h"

uint compute_length_in_bytes(uint length, uint size_in_bits) {
    return (uint)ceil((length * size_in_bits) / (double)BITS_IN_BYTE);
}

Buckets *create_buckets(uint length, uint size_in_bits) {
    Buckets *bucks = (Buckets*) malloc(sizeof(Buckets));
    bucks->length = length;
    bucks->bucket_size_in_bites = size_in_bits;
    uint length_bytes = compute_length_in_bytes(length, size_in_bits);
    void *buckets = calloc(length_bytes, 1);
    bucks->values = buckets;
    return bucks;
}

/*void set_value_to_bucket(uint index, uint value, uint size_in_bits, void* buckets) {
    uint bits = size_in_bits * index;
    uint real_index = bits / BITS_IN_BYTE;
    uint from_bit = bits % BITS_IN_BYTE;
    for (uint i = 0; i < size_in_bits; i++) {
        buckets[real_index] = (buckets[real_index] | (128 >> (from_bit + i))) & 
    }
    
}*/
