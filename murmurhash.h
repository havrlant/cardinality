#ifndef cardinality_murmurhash_h
#define cardinality_murmurhash_h

#include <stdint.h>
uint64_t MurmurHash64A (const void *key, int len, uint64_t seed);
#endif
