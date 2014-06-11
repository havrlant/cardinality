#ifndef cardinality_setdictionary_h
#define cardinality_setdictionary_h

#include "uthash.h"
#include "declarations.h"
#include "set.h"
#include "string.h"

typedef struct {
    char *hash_id;
    Set set;
    UT_hash_handle hh;
} SetDictionary;

SetDictionary *create_empty_set_dict();
void add_set_to_dict(char *id, Set set, SetDictionary **dict);
SetDictionary *find_set_in_dict(char *id, SetDictionary **dict);

#endif
