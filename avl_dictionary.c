#include "avl_dictionary.h"

AVLTree *create_empty_avl_dict() {
    AVLTree *hll = NULL;
    return hll;
}

void add_avl_to_dict(char *id, AVLTree *avl, AvlDictionary **dict) {
    AvlDictionary *s;
    s = (AvlDictionary*) malloc(sizeof(AvlDictionary));
    s->hash_id = id;
    s->avl = avl;
    HASH_ADD_KEYPTR(hh, *dict, s->hash_id, strlen(s->hash_id), s);
}

AvlDictionary *find_avl(char *id, AvlDictionary **dict) {
    AvlDictionary *s;
    HASH_FIND_STR(*dict, id, s);
    return s;
}

