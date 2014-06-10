#include "setdictionary.h"

SetDictionary *create_empty_set_dict() {
    SetDictionary *hll = NULL;
    return hll;
}

void add_set_to_dict(uint id, Set set, SetDictionary **dict) {
    SetDictionary *s;
    s = malloc(sizeof(SetDictionary));
    s->hash_id = id;
    s->set = set;
    HASH_ADD_INT(*dict, hash_id, s);
}

SetDictionary *find_set_in_dict(uint id, SetDictionary **dict) {
    SetDictionary *s;
    HASH_FIND_INT(*dict, &id, s);
    return s;
}
