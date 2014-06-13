#include "nbitvector.h"

uint min(uint a, uint b) {
    return a > b ? b : a;
}

uint compute_length_in_bytes(uint length, uint size_in_bits) {
    return (uint)ceil((length * size_in_bits) / (double)BITS_IN_BYTE);
}

NBitVector *create_buckets(uint length, uint size_in_bits) {
    NBitVector *bucks = (NBitVector*) malloc(sizeof(NBitVector));
    bucks->length = length;
    bucks->bucket_size_in_bites = size_in_bits;
    uint length_bytes = compute_length_in_bytes(length, size_in_bits);
    byte *buckets = calloc(length_bytes, 1);
    bucks->values = buckets;
    return bucks;
}

void set_value_to_nbitvector(uint index, uint size_in_bits, NBitVector* vector, uint value) {
    uint bits = size_in_bits * index;
    uint byte_index;
    uint bit_index;
    uint saved_count;
    byte_index = bits / BITS_IN_BYTE;
    bit_index = bits % BITS_IN_BYTE;
    uint a = bit_index, b = min(bit_index + size_in_bits - 1, BITS_IN_BYTE - 1);
    uint range = BIT_RANGE(a, b);
    vector->values[byte_index] &= ~range;
    vector->values[byte_index] |= range & (value << bit_index);
    if (a > BITS_IN_BYTE - size_in_bits) {
        saved_count = b - a + 1;
        b = size_in_bits - saved_count - 1;
        bit_index = a = 0;
        range = BIT_RANGE(a, b);
        byte_index++;
        vector->values[byte_index] &= ~range;
        value >>= saved_count;
        vector->values[byte_index] |= (range & value);
    }
}

uint get_value_from_bucket(uint index, uint size_in_bits, NBitVector* vector) {
    uint bits = size_in_bits * index;
    uint byte_index;
    uint bit_index;
    uint saved_count;
    uint value = 0U;
    byte_index = bits / BITS_IN_BYTE;
    bit_index = bits % BITS_IN_BYTE;
    uint a = bit_index, b = min(bit_index + size_in_bits - 1, BITS_IN_BYTE - 1);
    uint range = BIT_RANGE(a, b);
    value |= ((vector->values[byte_index] & range) >> bit_index);
    
    if (a > BITS_IN_BYTE - size_in_bits) {
        saved_count = b - a + 1;
        b = size_in_bits - saved_count - 1;
        bit_index = a = 0;
        range = BIT_RANGE(a, b);
        byte_index++;
        value |= (vector->values[byte_index] & range) << saved_count;
    }
    
    return value;
}
