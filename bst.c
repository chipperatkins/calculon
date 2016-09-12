/*****
 * BST source file
 * Author: Chipper Atkins
 *****/
#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "bst.h"
#include "string.h"

/***** Public Methods *****/

bst *newBst() {
    bst *b = (bst *) malloc(sizeof(bst));
    if (b == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return b;
}

node *insert(node *n, node *d) { //inserts into bst
    if (n == 0) {
        node *t;
        t = d;
        return t;
    }
    if (strcmp(d->key,n->key)>0)
        n->right = insert(n->right,d);
    else if (strcmp(d->key,n->key)<0)
        n->left = insert(n->left,d);
    return n;
}

node *findMin(node *n) {
    if (n == 0)
        return 0;
    if (n->left != 0)
        return findMin(n->left);
    else
        return n;
}

node *findMax(node *n) {
    if (n == 0)
        return 0;
    if (n->right != 0)
        return findMax(n->right);
    else
        return n;
}

node *search(node *r, char *key)
{
    if (r == 0)
        return 0;
    if (strcmp(key,r->key)>0)
        return search(r->right,key);
    else if (strcmp(key,r->key)<0)
        return search(r->left,key);
    else
    {
        return r;
    }
}