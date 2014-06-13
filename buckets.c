#include "buckets.h"

uint compute_length_in_bytes(uint length, uint size_in_bits) {
    return (uint)ceil((length * size_in_bits) / (double)BITS_IN_BYTE);
}

Buckets *create_buckets(uint length, uint size_in_bits) {
    Buckets *bucks = (Buckets*) malloc(sizeof(Buckets));
    bucks->length = length;
    bucks->bucket_size_in_bites = size_in_bits;
    uint length_bytes = compute_length_in_bytes(length, size_in_bits);
    byte *buckets = calloc(length_bytes, 1);
    bucks->values = buckets;
    return bucks;
}

void set_value_to_bucket(uint index, uint size_in_bits, Buckets* buckets, uint value) {
    uint bits = size_in_bits * index;
    uint byte_index;
    uint bit_index;
    uint bit_pos;
    for (uint i = 0; i < size_in_bits; i++) {
        byte_index = (bits + i) / BITS_IN_BYTE;
        bit_index = (bits + i) % BITS_IN_BYTE;
        bit_pos = BIT_POS(bit_index);
        uint getmflag = (GET_MFLAG(value, BIT_POS(i)) << (bits % BITS_IN_BYTE));
        if (getmflag > 128) {
            getmflag >>= BITS_IN_BYTE;
        }
        buckets->values[byte_index] = buckets->values[byte_index] & (~bit_pos);
        SET_FLAG(buckets->values[byte_index], getmflag);
    }
}

uint get_value_from_bucket(uint index, uint size_in_bits, Buckets* buckets) {
    uint bits = size_in_bits * index;
    uint byte_index;
    uint bit_index;
    uint bit_pos;
    uint value = 0U;
    
    for (uint i = 0; i < size_in_bits; i++) {
        byte_index = (bits + i) / BITS_IN_BYTE;
        bit_index = (bits + i) % BITS_IN_BYTE;
        bit_pos = BIT_POS(bit_index);
        uint getmflag = GET_MFLAG(buckets->values[byte_index], bit_pos);
        if (getmflag > 0) {
            value += (1U << i);
        }
    }
    return value;
}
