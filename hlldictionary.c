#include "hlldictionary.h"

HllDictionary *create_empty_hll_dict() {
    HllDictionary *hll = NULL;
    return hll;
}

void add_hll(uint id, Hyperloglog *hll, HllDictionary **dict) {
    HllDictionary *s;
    s = malloc(sizeof(HllDictionary));
    s->hash_id = id;
    s->hll = hll;
    HASH_ADD_INT(*dict, hash_id, s);
}

HllDictionary *find_hll(uint id, HllDictionary **dict) {
    HllDictionary *s;
    HASH_FIND_INT(*dict, &id, s);
    return s;
}

