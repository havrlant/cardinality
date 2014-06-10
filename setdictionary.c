#include "setdictionary.h"

HllDictionary *create_empty_set_dict() {
    HllDictionary *hll = NULL;
    return hll;
}

void add_set_to_dict(uint id, Set *set, HllDictionary **dict) {
    HllDictionary *s;
    s = malloc(sizeof(HllDictionary));
    s->hash_id = id;
    s->set = set;
    HASH_ADD_INT(*dict, hash_id, s);
}

HllDictionary *find_set_in_dict(uint id, HllDictionary **dict) {
    HllDictionary *s;
    HASH_FIND_INT(*dict, &id, s);
    return s;
}

