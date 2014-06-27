#ifndef cardinality_hll_decl_h
#define cardinality_hll_decl_h

#include "declarations.h"

#pragma pack(1)

typedef struct {
    uint16_t index;
    byte value;
} SparsePair;

// Ostatni struktury chceme ukladat normalne
// Napr. zlib bez tohoto nefunguje a hlasi chybu Z_VERSION_ERROR
#pragma pack()

typedef struct {
    uint16_t index;
    byte value;
} _SparsePairForTestOnly;

typedef struct {
    uint b;
    uint m;
    byte *M;
    SparsePair *pairs;
    byte sparsed_used;
    uint last_index;
    uint max_values;
    uint sparse_b_difference;
} Hyperloglog;

// pack(1) znamena, ze se struktura bude ukladat pametove efektivne
// bez tohoto by se struktura s uint16_t a byte ulozila do 4 bytu.
// S timto se ulozi do 3. Je to zase pomalejsi. 



#endif
