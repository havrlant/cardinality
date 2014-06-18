#ifndef cardinality_avl_dictionary_h
#define cardinality_avl_dictionary_h

#include "uthash.h"
#include "avl_tree.h"
#include "declarations.h"

typedef struct {
    char *hash_id;
    AVLTree *avl;
    UT_hash_handle hh;
} AvlDictionary;

AvlDictionary *find_avl(char *id, AvlDictionary **dict);
void add_avl_to_dict(char *id, AVLTree *avl, AvlDictionary **dict);
AVLTree *create_empty_avl_dict();

#endif
