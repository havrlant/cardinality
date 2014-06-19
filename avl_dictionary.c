#include "avl_dictionary.h"

AvlDictionary *create_empty_avl_dict() {
    AvlDictionary *tree = NULL;
    return tree;
}

void himlhergot(char *id, AVLTree *tree, AvlDictionary **dict) {
    AvlDictionary *s;
    s = (AvlDictionary*) malloc(sizeof(AvlDictionary));
    s->hash_id = id;
    s->avl = tree;
    HASH_ADD_KEYPTR(hh, *dict, s->hash_id, strlen(s->hash_id), s);
}

void add_avl_to_dict(char *id, AVLTree *tree, AvlDictionary **dict) {
    AvlDictionary *s;
    s = (AvlDictionary*) malloc(sizeof(AvlDictionary));
    s->hash_id = id;
    s->avl = tree;
    HASH_ADD_KEYPTR(hh, *dict, s->hash_id, strlen(s->hash_id), s);
}

AvlDictionary *find_avl(char *id, AvlDictionary **dict) {
    AvlDictionary *s;
    HASH_FIND_STR(*dict, id, s);
    return s;
}

