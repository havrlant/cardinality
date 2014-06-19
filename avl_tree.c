#include "avl_tree.h"

node dummy = { 0, 0, {&dummy, &dummy} };
node *nnil = &dummy;

node *new_node(NODE_TYPE value)
{
    node *n = calloc(1, sizeof *n);
    *n = (struct node) { value, 1, {nnil, nnil} };
    return n;
}

NODE_TYPE getmax(NODE_TYPE a, NODE_TYPE b) { return a > b ? a : b; }

void set_height(node *n) {
    n->height = 1 + (int)getmax(n->kid[0]->height, n->kid[1]->height);
}

int ballance(node *n) {
    return n->kid[0]->height - n->kid[1]->height;
}

// rotate a subtree according to dir; if new root is nil, old root is freed
node * rotate(node **rootp, int dir) {
    node *old_r = *rootp, *new_r = old_r->kid[dir];
    
    if (nnil == (*rootp = new_r))
        free(old_r);
    else {
        old_r->kid[dir] = new_r->kid[!dir];
        set_height(old_r);
        new_r->kid[!dir] = old_r;
    }
    return new_r;
}

void adjust_balance(node **rootp) {
    node *root = *rootp;
    int b = ballance(root)/2;
    if (b) {
        int dir = (1 - b)/2;
        if (ballance(root->kid[dir]) == -b)
            rotate(&root->kid[dir], !dir);
        root = rotate(rootp, dir);
    }
    if (root != nnil) set_height(root);
}

// find the node that contains value as payload; or returns 0
node *query(node *root, NODE_TYPE value)
{
    return root == nnil
    ? 0
    : root->payload == value
    ? root
    : query(root->kid[value > root->payload], value);
}

int insert(node **rootp, NODE_TYPE value) {
    node *root = *rootp;
    
    if (root == nnil) {
        *rootp = new_node(value);
        return 1;
    }
    else if (value != root->payload) { // don't allow dup keys
        insert(&root->kid[value > root->payload], value);
        adjust_balance(rootp);
        return 1;
    } else {
        return 0;
    }
}

void delete(node **rootp, NODE_TYPE value) {
    node *root = *rootp;
    if (root == nnil) return; // not found
    
    // if this is the node we want, rotate until off the tree
    if (root->payload == value)
        if (nnil == (root = rotate(rootp, ballance(root) < 0)))
            return;
    
    delete(&root->kid[value > root->payload], value);
    adjust_balance(rootp);
}

// aux display and verification routines, helpful but not essential
NODE_TYPE max_value(node *root) {
    if (root == nnil) {
        return -1;
    }
    if (root->kid[1] != nnil) {
        return max_value(root->kid[1]);
    }
    return root->payload;
}

void AVL_ForEach(AVLTree *tree, void(*action)(NODE_TYPE value)) {
    
}

void AVL_delete(AVLTree *tree, NODE_TYPE value) {
    delete(tree->root, value);
}

NODE_TYPE AVL_max_value(AVLTree *tree) {
    return max_value(*(tree->root));
}

void init_AVL_Tree(AVLTree *tree, node **root, int max_values) {
    tree->root = root;
    tree->max = -1;
    tree->counter = 0;
    tree->max_values = max_values;
}

void AVL_insert(AVLTree *tree, NODE_TYPE value) {
    if (tree->counter < tree->max_values) {
        if (insert(tree->root, value)) {
            tree->counter++;
            tree->max = getmax(value, tree->max);
        }
    } else {
        if (value < tree->max) {
            if (insert(tree->root, value)) {
                AVL_delete(tree, AVL_max_value(tree));
                tree->max = getmax(value, AVL_max_value(tree));
            }
        }
    }
}

AVLTree *create_empty_tree(int max_values) {
    node **root = &nnil;
    AVLTree *tree = (AVLTree*) malloc(sizeof(AVLTree));
    init_AVL_Tree(tree, root, max_values);
    return tree;
}
