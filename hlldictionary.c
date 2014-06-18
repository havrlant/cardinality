#include "hlldictionary.h"

HllDictionary *create_empty_hll_dict() {
    HllDictionary *hll = NULL;
    return hll;
}

void add_hll_to_dict(char *id, Hyperloglog *hll, HllDictionary **dict) {
    HllDictionary *s;
    s = (HllDictionary*) malloc(sizeof(HllDictionary));
    s->hash_id = id;
    s->hll = hll;
    HASH_ADD_KEYPTR(hh, *dict, s->hash_id, strlen(s->hash_id), s);
}

HllDictionary *find_hll(char *id, HllDictionary **dict) {
    HllDictionary *s;
    HASH_FIND_STR(*dict, id, s);
    return s;
}

