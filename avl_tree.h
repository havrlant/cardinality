#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


typedef uint64_t NODE_TYPE;



typedef struct {
    struct node **root;
    NODE_TYPE max;
    int counter;
    int max_values;
} AVLTree;
