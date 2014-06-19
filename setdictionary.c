#include "setdictionary.h"

SetDictionary *create_empty_set_dict() {
    SetDictionary *hll = NULL;
    return hll;
}

void add_set_to_dict(char *id, Set set, SetDictionary **dict) {
    SetDictionary *s;
    s = (SetDictionary*) malloc(sizeof(SetDictionary));
    s->hash_id = id;
    s->set = set;
    // HASH_ADD_KEYPTR(hh, *dict, hash_id, s);
    HASH_ADD_KEYPTR(hh, *dict, s->hash_id, strlen(s->hash_id), s);
}

SetDictionary *find_set_in_dict(char *id, SetDictionary **dict) {
    SetDictionary *s;
    HASH_FIND_STR(*dict, id, s);
    return s;
}
