//
//  avl_tree.h
//  cardinality
//
//  Created by Lukáš Havrlant on 18.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_avl_tree_h
#define cardinality_avl_tree_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


typedef uint64_t NODE_TYPE;

typedef struct node {
    NODE_TYPE payload;
    int height;
    struct node * kid[2];
} node;

typedef struct {
    node **root;
    NODE_TYPE max;
    int counter;
    int max_values;
} AVLTree;

AVLTree *create_empty_tree(int max_values);
void AVL_insert(AVLTree *tree, NODE_TYPE value);

#endif
